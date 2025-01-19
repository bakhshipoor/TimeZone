#include "../Inc/Parse_Rules.h"

Rule_Entry_t* Parse_Rules(COUNTER* rules_Count)
{
    // Initialize the Rules_List to NULL and set the rules count to 0
    Rule_Entry_t* Rules_List = NULL;
    *rules_Count = 0;
    CHAR line[2048]; // Buffer to store each line read from the file
    COUNTER dataFile_index = 0; // Index to iterate over data files

    // Loop through each data file
    for (dataFile_index = 3; dataFile_index < DATA_FILES_COUNT; dataFile_index++)
    {
        // Open the data file in read mode
        FILE* data_File = fopen(Data_File[dataFile_index], "r");
        if (!data_File)
        {
            continue; // If the file cannot be opened, move to the next file
        }

        // Read each line from the file
        while (fgets(line, sizeof(line), data_File))
        {
            // Skip lines that are too short or start with a '#' CHARacter
            if (strlen(line) < 5 || line[0] == '#')
            {
                continue;
            }

            // Parse the line into a rule_data structure
            Rule_Data_t rule_data = Parse_Rule_Data(line);

            // Skip if the rule name is empty
            if (strlen(rule_data.Name) <= 0)
            {
                continue;
            }

            COUNTER find_index = -1; // Variable to store the index if the rule exists

            // Check if the rule already exists in the Rules_List
            if (!Rule_isExist(Rules_List, rules_Count, rule_data.Name, &find_index))
            {
                // If the rule does not exist, create a new rule entry
                if (Rule_Create(&Rules_List, *rules_Count, rule_data.Name))
                {
                    find_index = (*rules_Count);
                    (*rules_Count)++; // Increment the rules count
                }
            }

            if (find_index != -1)
            {
                // Parse the year range for the rule
                Parse_Rule_Year_Range(Rules_List, find_index, rule_data);
            }
            
        }

        // Close the data file
        fclose(data_File);
    }

    // Parse the years for all rules
    Parse_Rule_Years(Rules_List, rules_Count);

    // Return the list of rules
    return Rules_List;
}

Rule_Data_t Parse_Rule_Data(CONST CHAR* line)
{
    Rule_Data_t rule_data; // Initialize a Rule_Data_t structure
    LENGHT scan_lenght = 0; // Variable to store the length of the scanned data

    // Initialize all fields of rule_data to null CHARacters
    memset(rule_data.Field, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.Name, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.From, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.To, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.Reserved, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.In, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.On, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.At, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.Save, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.Letter, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.Comment, '\0', MAX_LENGHT_DATA_FIELD);

    // Parse the input line and populate the fields of rule_data
    scan_lenght = sscanf(line, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%[^\r\n]",
        rule_data.Field,
        rule_data.Name,
        rule_data.From,
        rule_data.To,
        rule_data.Reserved,
        rule_data.In,
        rule_data.On,
        rule_data.At,
        rule_data.Save,
        rule_data.Letter,
        rule_data.Comment);

    // Copy the parsed fields to ensure they are null-terminated
    sprintf(rule_data.Field, "%s", rule_data.Field);
    sprintf(rule_data.Name, "%s", rule_data.Name);
    sprintf(rule_data.From, "%s", rule_data.From);
    sprintf(rule_data.To, "%s", rule_data.To);
    sprintf(rule_data.Reserved, "%s", rule_data.Reserved);
    sprintf(rule_data.In, "%s", rule_data.In);
    sprintf(rule_data.On, "%s", rule_data.On);
    sprintf(rule_data.At, "%s", rule_data.At);
    sprintf(rule_data.Save, "%s", rule_data.Save);
    sprintf(rule_data.Letter, "%s", rule_data.Letter);
    sprintf(rule_data.Comment, "%s", rule_data.Comment);

    // Check if the parsed data is valid
    if (strcmp("Rule", rule_data.Field) != 0 || scan_lenght < 10)
    {
        // If invalid, reset all fields to null CHARacters
        memset(rule_data.Field, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.Name, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.From, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.To, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.Reserved, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.In, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.On, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.At, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.Save, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.Letter, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.Comment, '\0', MAX_LENGHT_DATA_FIELD);
    }

    // Return the parsed rule data
    return rule_data;
}

YEAR Parse_Rule_Data_From(CONST Rule_Data_t rule_data)
{
    // Convert the 'From' field of rule_data from a string to an integer and return it
    return atoi(rule_data.From);
}

YEAR Parse_Rule_Data_To(CONST Rule_Data_t rule_data, YEAR max_value)
{
    // Check if the 'To' field is "only". 
    // If it is, return the integer value of the 'From' field.
    if (strcmp(rule_data.To, "only") == 0)
    {
        return atoi(rule_data.From);
    }
    // Check if the 'To' field is "max". 
    // If it is, return the provided max_value.
    else if (strcmp(rule_data.To, "max") == 0)
    {
        return max_value;
    }
    // If the 'To' field is neither "only" nor "max", 
    // return the integer value of the 'To' field.
    else
    {
        return atoi(rule_data.To);
    }
    // Default return value, although this line is never reached due to the previous return statements.
    return 0;
}

MONTH Parse_Rule_Data_In(CONST Rule_Data_t rule_data)
{
    return Parse_Month(rule_data.In);
}

Rule_Year_Day_t Parse_Rule_Data_On(CONST Rule_Data_t rule_data)
{
    Rule_Year_Day_t day = { 0 }; // Initialize a Rule_Year_Day_t structure with default values
    Parse_Day_Of_Month(rule_data.On, &day.Day, &day.Weekday, &day.Weekday_isAfterOrEqual_Day);
    return day; // Return the day structure
}

Rule_Year_Hour_t Parse_Rule_Data_At(CONST Rule_Data_t rule_data)
{
    // Initialize a Rule_Year_Hour_t structure with default values
    Rule_Year_Hour_t hour = { 0 };
    CHAR* s = { 0 };
    hour.Hour = Parse_Hour(rule_data.At, &s);
    hour.Hour_isUTC = FALSE;
    if (s[0]== 'u')
    {
        hour.Hour_isUTC = TRUE;
    }
    // Return the parsed hour structure
    return hour;
}

HOUR Parse_Rule_Data_Save(CONST Rule_Data_t rule_data)
{
    CHAR* s = { 0 };
    return Parse_Hour(rule_data.Save, &s);
}

BOOL Rule_isExist(CONST Rule_Entry_t* rule_list, CONST COUNTER* rules_count, CONST CHAR* rule_name, COUNTER* find_Index)
{
    // Check if the rule_list is not NULL and rules_count is greater than 0
    if (rule_list != NULL && ((*rules_count) > 0))
    {
        // Iterate through the list of rules
        for (COUNTER rule_index = 0; rule_index < (*rules_count); rule_index++)
        {
            // Compare the name of the current rule with rule_name
            if (strcmp(rule_list[rule_index].Name, rule_name) == 0)
            {
                // If a match is found, set find_Index to the current index and return TRUE
                *find_Index = rule_index;
                return TRUE;
            }
        }
    }
    // If no match is found, return FALSE
    return FALSE;
}

BOOL Rule_Create(Rule_Entry_t** rule_list, CONST COUNTER rules_Count, CONST CHAR* rule_name)
{
    // Initialize a new Rule_Entry_t structure with default values
    Rule_Entry_t rule = { 0 };

    // Allocate memory for the rule name and assign it to rule.Name
    rule.Name = (CHAR*)malloc((strlen(rule_name) + 1) * sizeof(CHAR));
    if (rule.Name != NULL)
    {
        // Copy the rule name to the allocated memory
        sprintf(rule.Name, "%s", rule_name);
    }

    // Initialize the other fields of the rule
    rule.Years_Count = 0;
    rule.Year_Begin = 0;
    rule.Year_End = 0;

    // Reallocate memory for the rule list to accommodate the new rule
    if (rules_Count == 0)
    {
        Rule_Entry_t* rules = malloc(sizeof(Rule_Entry_t));
        if (rules == NULL)
        {
            return FALSE;
        }
        *rule_list = rules;
    }
    else
    {
        Rule_Entry_t* rules = realloc(*rule_list, (rules_Count + 1) * sizeof(Rule_Entry_t));
        if (rules == NULL)
        {
            return FALSE;
        }
        *rule_list = rules;
    }
    

    // Add the new rule to the rule list at the specified index
    (*rule_list)[rules_Count] = rule;

    // Return TRUE indicating that the rule was successfully created
    return TRUE;
}

VOID Parse_Rule_Year_Range(Rule_Entry_t* rule_list, CONST COUNTER rule_index, CONST Rule_Data_t rule_data)
{
    // Parse the 'From' year from the rule data
    YEAR year_from = Parse_Rule_Data_From(rule_data);

    // Parse the 'To' year from the rule data, with a default maximum value of -1
    YEAR year_to = Parse_Rule_Data_To(rule_data, -1);

    // If the Year_Begin field of the rule is not set (i.e., 0)
    if (rule_list[rule_index].Year_Begin == 0)
    {
        // Set the Year_Begin field to the 'From' year
        rule_list[rule_index].Year_Begin = year_from;
    }
    // If the Year_Begin field is greater than the 'From' year, update it
    else if (rule_list[rule_index].Year_Begin > year_from)
    {
        rule_list[rule_index].Year_Begin = year_from;
    }

    // If the Year_End field of the rule is not set (i.e., 0)
    if (rule_list[rule_index].Year_End == 0)
    {
        // Set the Year_End field to the 'To' year
        rule_list[rule_index].Year_End = year_to;
    }
    // If the 'To' year is -1, update the Year_End field to -1
    else if (year_to == -1)
    {
        rule_list[rule_index].Year_End = year_to;
    }
    // If the Year_End field is less than the 'To' year, update it
    else if (rule_list[rule_index].Year_End < year_to)
    {
        rule_list[rule_index].Year_End = year_to;
    }
}

VOID Parse_Rule_Years(Rule_Entry_t* rule_list, COUNTER* rules_Count)
{
    CHAR line[2048]; // Buffer to store each line read from the data file
    COUNTER dataFile_index = 0; // Index to iterate through the data files

    // Loop through all data files
    for (dataFile_index = 3; dataFile_index < DATA_FILES_COUNT; dataFile_index++)
    {
        // Open the current data file for reading
        FILE* data_File = fopen(Data_File[dataFile_index], "r");
        if (!data_File)
        {
            continue; // If the file cannot be opened, skip to the next file
        }

        // Read each line from the data file
        while (fgets(line, sizeof(line), data_File))
        {
            // Skip lines that are too short or are comments
            if (strlen(line) < 5 || line[0] == '#')
            {
                continue;
            }

            // Parse the line into rule_data
            Rule_Data_t rule_data = Parse_Rule_Data(line);
            if (strlen(rule_data.Name) <= 0)
            {
                continue; // Skip if the rule name is empty
            }

            COUNTER rule_find_index = -1; // Index to store the found rule
            // Check if the rule exists in the rule list
            if (Rule_isExist(rule_list, rules_Count, rule_data.Name, &rule_find_index))
            {
                if (rule_find_index >= 0)
                {
                    // Parse the 'From' and 'To' years from the rule data
                    YEAR year_from = Parse_Rule_Data_From(rule_data);
                    YEAR year_to = Parse_Rule_Data_To(rule_data, rule_list[rule_find_index].Year_End);
                    COUNTER years_count = 0; // Variable to store the number of years in the range
                    COUNTER year_find_index = -1; // Index to store the found year

                    // Check if the range of years is valid
                    if (year_to > year_from)
                    {
                        years_count = (year_to - year_from) + 1;
                        // Loop through the range of years
                        for (COUNTER year_index = 0; year_index <= years_count; year_index++)
                        {
                            year_find_index = -1;
                            // Check if the year already exists in the rule's year list
                            if (!Rule_Year_isExist(rule_list[rule_find_index].Years, rule_list[rule_find_index].Years_Count, year_from, year_from, &year_find_index))
                            {
                                // Create a new year entry if it does not exist
                                if (Rule_Year_Create(&rule_list[rule_find_index].Years, rule_list[rule_find_index].Years_Count, year_from, year_from, rule_data))
                                {
                                    rule_list[rule_find_index].Years_Count++;
                                }
                            }
                            else
                            {
                                // Parse the save hour from the rule data
                                HOUR save_hour = Parse_Rule_Data_Save(rule_data);
                                // Add the rule data to the appropriate list (Standard or DST)
                                if (save_hour == 0)
                                {
                                    if (Rule_Year_Add_Data(&(rule_list[rule_find_index].Years[year_find_index].Standard), rule_list[rule_find_index].Years[year_find_index].Standard_Count, rule_data))
                                    {
                                        rule_list[rule_find_index].Years[year_find_index].Standard_Count++;
                                    }
                                }
                                else
                                {
                                    if (Rule_Year_Add_Data(&(rule_list[rule_find_index].Years[year_find_index].DST), rule_list[rule_find_index].Years[year_find_index].DST_Count, rule_data))
                                    {
                                        rule_list[rule_find_index].Years[year_find_index].DST_Count++;
                                    }
                                }
                            }
                            year_from++; // Move to the next year in the range
                        }
                    }
                    else
                    {
                        // Single year entry
                        year_find_index = -1;
                        // Check if the year already exists in the rule's year list
                        if (!Rule_Year_isExist(rule_list[rule_find_index].Years, rule_list[rule_find_index].Years_Count, year_from, year_to, &year_find_index))
                        {
                            // Create a new year entry if it does not exist
                            if (Rule_Year_Create(&rule_list[rule_find_index].Years, rule_list[rule_find_index].Years_Count, year_from, year_to, rule_data))
                            {
                                rule_list[rule_find_index].Years_Count++;
                            }
                        }
                        else
                        {
                            // Parse the save hour from the rule data
                            HOUR save_hour = Parse_Rule_Data_Save(rule_data);
                            // Add the rule data to the appropriate list (Standard or DST)
                            if (save_hour == 0)
                            {
                                if (Rule_Year_Add_Data(&rule_list[rule_find_index].Years[year_find_index].Standard, rule_list[rule_find_index].Years[year_find_index].Standard_Count, rule_data))
                                {
                                    rule_list[rule_find_index].Years[year_find_index].Standard_Count++;
                                }
                            }
                            else
                            {
                                if (Rule_Year_Add_Data(&rule_list[rule_find_index].Years[year_find_index].DST, rule_list[rule_find_index].Years[year_find_index].DST_Count, rule_data))
                                {
                                    rule_list[rule_find_index].Years[year_find_index].DST_Count++;
                                }
                            }
                        }
                    }
                }
            }
        }
        // Close the current data file
        fclose(data_File);
    }
}

BOOL Rule_Year_isExist(CONST Rule_Year_t* year_list, CONST COUNTER years_count, CONST YEAR year_from, CONST YEAR year_to, COUNTER* find_index)
{
    // Check if the year_list is not NULL and years_count is greater than 0
    if (year_list != NULL && (years_count > 0))
    {
        // Iterate through the list of years
        for (int year_index = 0; year_index < years_count; year_index++)
        {
            // Compare the 'From' and 'To' years with the given year_from and year_to
            if (year_list[year_index].From == year_from && year_list[year_index].To == year_to)
            {
                // If a match is found, set find_index to the current index and return TRUE
                (*find_index) = year_index;
                return TRUE;
            }
        }
    }
    // If no match is found, return FALSE
    return FALSE;
}

BOOL Rule_Year_Create(Rule_Year_t** year_list, CONST COUNTER years_count, CONST YEAR year_from, CONST YEAR year_to, CONST Rule_Data_t rule_data)
{
    // Initialize a new Rule_Year_t structure with default values
    Rule_Year_t rule_year = { 0 };

    // Set the 'From' and 'To' year fields of the rule year
    rule_year.From = year_from;
    rule_year.To = year_to;

    // Allocate memory for the 'Reserved' field and copy the data from rule_data
    rule_year.Reserved = (CHAR*)malloc((strlen(rule_data.Reserved) + 1) * sizeof(CHAR));
    if (rule_year.Reserved != NULL)
    {
        // If the 'Reserved' field in rule_data starts with '-', set it to an empty string
        if (rule_data.Reserved[0] == '-')
        {
            sprintf(rule_year.Reserved, "%c", '\0');
        }
        // Otherwise, copy the 'Reserved' field from rule_data
        else
        {
            sprintf(rule_year.Reserved, "%s", rule_data.Reserved);
        }
    }

    // Initialize the Standard_Count and DST_Count fields
    rule_year.Standard_Count = 0;
    rule_year.DST_Count = 0;

    // Parse the save hour from the rule data
    HOUR save_hour = Parse_Rule_Data_Save(rule_data);

    // Add the rule data to the appropriate list (Standard or DST) based on the save hour
    if (save_hour == 0)
    {
        if (Rule_Year_Add_Data(&rule_year.Standard, rule_year.Standard_Count, rule_data))
        {
            rule_year.Standard_Count++;
        }
    }
    else
    {
        if (Rule_Year_Add_Data(&rule_year.DST, rule_year.DST_Count, rule_data))
        {
            rule_year.DST_Count++;
        }
    }

    // Reallocate memory for the year list to accommodate the new rule year
    if (years_count == 0)
    {
        Rule_Year_t* year = (Rule_Year_t*)malloc(sizeof(Rule_Year_t));
        if (year != NULL)
        {
            *year_list = year;
            (*year_list)[years_count] = rule_year;
            return TRUE; // Return TRUE indicating the rule year was successfully created
        }
    }
    else
    {
        Rule_Year_t* year = (Rule_Year_t*)realloc(*year_list, (years_count + 1) * sizeof(Rule_Year_t));
        if (year != NULL)
        {
            *year_list = year;
            (*year_list)[years_count] = rule_year;
            return TRUE; // Return TRUE indicating the rule year was successfully created
        }
    }
    
    // If memory allocation fails, return FALSE
    return FALSE;
}

BOOL Rule_Year_Add_Data(Rule_Year_Data_t** year_data, CONST COUNTER count, CONST Rule_Data_t rule_data)
{
    // Initialize a new Rule_Year_Data_t structure with default values
    Rule_Year_Data_t rule_year_data = { 0 };

    // Parse the month, day, hour, and save hour from the rule data
    rule_year_data.Month = Parse_Rule_Data_In(rule_data);
    rule_year_data.Day = Parse_Rule_Data_On(rule_data);
    rule_year_data.Hour = Parse_Rule_Data_At(rule_data);
    rule_year_data.Save_Hour = Parse_Rule_Data_Save(rule_data);

    // Allocate memory for the 'Letter' field and copy the data from rule_data
    rule_year_data.Letter = (CHAR*)malloc((strlen(rule_data.Letter) + 1) * sizeof(CHAR));
    if (rule_data.Letter != NULL)
    {
        // If the 'Letter' field in rule_data starts with '-', set it to an empty string
        if (rule_data.Letter[0] == '-')
        {
            sprintf(rule_year_data.Letter, "%c", '\0');
        }
        // Otherwise, copy the 'Letter' field from rule_data
        else
        {
            sprintf(rule_year_data.Letter, "%s", rule_data.Letter);
        }
    }

    // Allocate memory for the 'Comment' field and copy the data from rule_data
    rule_year_data.Comment = (CHAR*)malloc((strlen(rule_data.Comment) + 1) * sizeof(CHAR));
    if (rule_year_data.Comment != NULL)
    {
        sprintf(rule_year_data.Comment, "%s", rule_data.Comment);
    }

    // Reallocate memory for the year data list to accommodate the new rule year data
    if (count==0)
    {
        Rule_Year_Data_t* data = (Rule_Year_Data_t*)malloc(sizeof(Rule_Year_Data_t));
        if (data != NULL)
        {
            // Update the year data pointer to point to the newly allocated memory
            *year_data = data;
            // Add the new rule year data to the list at the specified index
            (*year_data)[count] = rule_year_data;
            return TRUE; // Return TRUE indicating the rule year data was successfully added
        }
    }
    else
    {
        Rule_Year_Data_t* data = (Rule_Year_Data_t*)realloc(*year_data, (count + 1) * sizeof(Rule_Year_Data_t));
        if (data != NULL)
        {
            // Update the year data pointer to point to the newly allocated memory
            *year_data = data;
            // Add the new rule year data to the list at the specified index
            (*year_data)[count] = rule_year_data;
            return TRUE; // Return TRUE indicating the rule year data was successfully added
        }
    }
   
    // If memory allocation fails, return FALSE
    return FALSE;
}
