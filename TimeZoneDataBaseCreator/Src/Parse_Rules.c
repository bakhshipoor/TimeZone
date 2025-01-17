#include "../Inc/Parse_Rules.h"

Rule_Entry_t* Parse_Rules(int32_t* rules_Count)
{
    // Initialize the Rules_List to NULL and set the rules count to 0
    Rule_Entry_t* Rules_List = NULL;
    *rules_Count = 0;
    char line[2048]; // Buffer to store each line read from the file
    uint16_t dataFile_index = 0; // Index to iterate over data files

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
            // Skip lines that are too short or start with a '#' character
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

            int32_t find_index = -1; // Variable to store the index if the rule exists

            // Check if the rule already exists in the Rules_List
            if (!Rule_isExist(Rules_List, rules_Count, rule_data.Name, &find_index))
            {
                // If the rule does not exist, create a new rule entry
                if (Rule_Create(&Rules_List, *rules_Count, rule_data.Name))
                {
                    (*rules_Count)++; // Increment the rules count
                }
            }

            // Parse the year range for the rule
            Parse_Rule_Year_Range(Rules_List, find_index, rule_data);
        }

        // Close the data file
        fclose(data_File);
    }

    // Parse the years for all rules
    Parse_Rule_Years(Rules_List, rules_Count);

    // Return the list of rules
    return Rules_List;
}

Rule_Data_t Parse_Rule_Data(const char* line)
{
    Rule_Data_t rule_data; // Initialize a Rule_Data_t structure
    int scan_lenght = 0; // Variable to store the length of the scanned data

    // Initialize all fields of rule_data to null characters
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
        // If invalid, reset all fields to null characters
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

int16_t Parse_Rule_Data_From(const Rule_Data_t rule_data)
{
    // Convert the 'From' field of rule_data from a string to an integer and return it
    return atoi(rule_data.From);
}

int16_t Parse_Rule_Data_To(const Rule_Data_t rule_data, int16_t max_value)
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

uint8_t Parse_Rule_Data_In(const Rule_Data_t rule_data)
{
    // Loop through all the months in the TZDB_MONTH_TOTAL
    for (int month_index = 0; month_index < TZDB_MONTH_TOTAL; month_index++)
    {
        // Check if the 'In' field matches either the abbreviated or full name of the month
        if (strcmp(rule_data.In, Month_Names[month_index].Abbr) == 0 || strcmp(rule_data.In, Month_Names[month_index].Full) == 0)
        {
            // Return the month number (1-based index, hence the +1)
            return Month_Names[month_index].Number + 1;
        }
    }
    // Return 0 if no match is found
    return 0;
}

Rule_Year_Day_t Parse_Rule_Data_On(const Rule_Data_t rule_data)
{
    Rule_Year_Day_t day = { 0 }; // Initialize a Rule_Year_Day_t structure with default values
    day.Day = 0;
    day.Weekday = (uint8_t)TZDB_WEEKDAY_NONE;
    day.Weekday_isAfterOrEqual_Day = false;

    // Check if the length of the 'On' field is less than or equal to 3
    if (strlen(rule_data.On) <= 3)
    {
        day.Day = atoi(rule_data.On); // Convert the 'On' field to an integer and assign to day.Day
        day.Weekday = (uint8_t)TZDB_WEEKDAY_NONE;
        day.Weekday_isAfterOrEqual_Day = false;
    }
    else
    {
        uint8_t dow[10], d[10]; // Buffers to store day of week (dow) and day (d)
        int sscanf_lenght = sscanf(rule_data.On, "%[^=]=%s", dow, d); // Parse the 'On' field into dow and d
        sprintf(dow, "%s", dow); // Ensure null-terminated strings
        sprintf(d, "%s", d);

        // Check if only day of week was found
        if (sscanf_lenght == 1)
        {
            // Loop through all weekdays to find a match
            for (int last_week_index = 0; last_week_index < TZDB_WEEKDAY_TOTAL; last_week_index++)
            {
                if (strcmp(dow, Last_Weekday_Names[last_week_index].Full) == 0)
                {
                    day.Day = 0; // Last week
                    day.Weekday = Last_Weekday_Names[last_week_index].Number;
                    day.Weekday_isAfterOrEqual_Day = false;
                    return day; // Return the day structure
                }
            }
        }
        else
        {
            int len = strlen(dow); // Get the length of dow

            // Check if dow ends with '>' or '<'
            if (dow[len - 1] == '>')
            {
                day.Weekday_isAfterOrEqual_Day = true; // Set to true if dow ends with '>'
            }
            else if (dow[len - 1] == '<')
            {
                day.Weekday_isAfterOrEqual_Day = false; // Set to false if dow ends with '<'
            }
            dow[len - 1] = '\0'; // Remove the last character ('>' or '<') from dow

            // Loop through all weekdays to find a match
            for (int week_index = 0; week_index < TZDB_WEEKDAY_TOTAL; week_index++)
            {
                if (strcmp(dow, Weekday_Names[week_index].Abbr) == 0 || strcmp(dow, Weekday_Names[week_index].Full) == 0)
                {
                    day.Day = atoi(d); // Convert d to an integer and assign to day.Day
                    day.Weekday = Weekday_Names[week_index].Number; // Assign the corresponding weekday number
                }
            }
        }
    }
    return day; // Return the day structure
}

Rule_Year_Hour_t Parse_Rule_Data_At(const Rule_Data_t rule_data)
{
    // Initialize a Rule_Year_Hour_t structure with default values
    Rule_Year_Hour_t hour = { 0 };
    hour.Hour = 0;
    hour.Hour_isUTC = false;

    uint8_t base = 0; // Variable to store the base character ('u' or 's')
    uint32_t h = 0, m = 0; // Variables to store hours and minutes

    // Parse the 'At' field in the format "h:m" or "h:mu" or "h:ms"
    if (sscanf(rule_data.At, "%d:%d%c", &h, &m, &base) == 3)
    {
        // Convert hours and minutes to seconds and assign to hour.Hour
        hour.Hour = (h * 3600) + (m * 60);

        // Check the base character to set the hour as UTC or standard time
        if (base == 'u')
        {
            hour.Hour_isUTC = true;
        }
        else if (base == 's')
        {
            hour.Hour_isUTC = false;
        }
    }
    else if (sscanf(rule_data.At, "%d:%d", &h, &m) == 2)
    {
        // Convert hours and minutes to seconds and assign to hour.Hour
        hour.Hour = (h * 3600) + (m * 60);
        hour.Hour_isUTC = false;
    }

    // Return the parsed hour structure
    return hour;
}

int32_t Parse_Rule_Data_Save(const Rule_Data_t rule_data)
{
    int32_t save = 0; // Variable to store the parsed save value in seconds
    uint8_t sign = 0; // Variable to store the sign ('+' or '-')
    uint32_t h = 0, m = 0; // Variables to store hours and minutes

    // Parse the 'Save' field in the format "±h:m"
    if (sscanf(rule_data.Save, "%c%d:%d", &sign, &h, &m) == 3)
    {
        // Convert hours and minutes to seconds and assign to save
        save = (h * 3600) + (m * 60);

        // Check the sign to determine if the value should be negative
        if (sign == '-')
        {
            save *= -1;
        }
    }
    // Parse the 'Save' field in the format "h:m"
    else if (sscanf(rule_data.Save, "%d:%d", &h, &m) == 2)
    {
        // Convert hours and minutes to seconds and assign to save
        save = (h * 3600) + (m * 60);
    }
    // Parse the 'Save' field in the format "h"
    else if (sscanf(rule_data.Save, "%d", &h) == 1)
    {
        // Convert hours to seconds and assign to save
        save = (h * 3600);
    }

    // Return the parsed save value in seconds
    return save;
}

bool Rule_isExist(const Rule_Entry_t* rule_list, const int32_t* rules_count, const char* rule_name, int32_t* find_Index)
{
    // Check if the rule_list is not NULL and rules_count is greater than 0
    if (rule_list != NULL && ((*rules_count) > 0))
    {
        // Iterate through the list of rules
        for (int rule_index = 0; rule_index < (*rules_count); rule_index++)
        {
            // Compare the name of the current rule with rule_name
            if (strcmp(rule_list[rule_index].Name, rule_name) == 0)
            {
                // If a match is found, set find_Index to the current index and return true
                *find_Index = rule_index;
                return true;
            }
        }
    }
    // If no match is found, return false
    return false;
}

bool Rule_Create(Rule_Entry_t** rule_list, const int32_t rules_Count, const char* rule_name)
{
    // Initialize a new Rule_Entry_t structure with default values
    Rule_Entry_t rule = { 0 };

    // Allocate memory for the rule name and assign it to rule.Name
    rule.Name = (uint8_t*)malloc((strlen(rule_name) + 1) * sizeof(uint8_t));
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
    Rule_Entry_t* rules = realloc(*rule_list, (rules_Count + 1) * sizeof(Rule_Entry_t));
    if (rules == NULL)
    {
        // If memory allocation fails, return false
        return false;
    }

    // Update the rule list pointer to point to the newly allocated memory
    *rule_list = rules;

    // Add the new rule to the rule list at the specified index
    (*rule_list)[rules_Count] = rule;

    // Return true indicating that the rule was successfully created
    return true;
}

void Parse_Rule_Year_Range(Rule_Entry_t* rule_list, const int32_t rule_index, const Rule_Data_t rule_data)
{
    // Parse the 'From' year from the rule data
    int16_t year_from = Parse_Rule_Data_From(rule_data);

    // Parse the 'To' year from the rule data, with a default maximum value of -1
    int16_t year_to = Parse_Rule_Data_To(rule_data, -1);

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

void Parse_Rule_Years(Rule_Entry_t* rule_list, int32_t* rules_Count)
{
    uint8_t line[2048]; // Buffer to store each line read from the data file
    uint16_t dataFile_index = 0; // Index to iterate through the data files

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

            int32_t rule_find_index = -1; // Index to store the found rule
            // Check if the rule exists in the rule list
            if (Rule_isExist(rule_list, rules_Count, rule_data.Name, &rule_find_index))
            {
                if (rule_find_index >= 0)
                {
                    // Parse the 'From' and 'To' years from the rule data
                    int16_t year_from = Parse_Rule_Data_From(rule_data);
                    int16_t year_to = Parse_Rule_Data_To(rule_data, rule_list[rule_find_index].Year_End);
                    int16_t years_count = 0; // Variable to store the number of years in the range
                    int32_t year_find_index = -1; // Index to store the found year

                    // Check if the range of years is valid
                    if (year_to > year_from)
                    {
                        years_count = (year_to - year_from) + 1;
                        // Loop through the range of years
                        for (int year_index = 0; year_index <= years_count; year_index++)
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
                                int32_t save_hour = Parse_Rule_Data_Save(rule_data);
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
                            int32_t save_hour = Parse_Rule_Data_Save(rule_data);
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

bool Rule_Year_isExist(const Rule_Year_t* year_list, const uint32_t years_count, const int16_t year_from, const int16_t year_to, int32_t* find_index)
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
                // If a match is found, set find_index to the current index and return true
                (*find_index) = year_index;
                return true;
            }
        }
    }
    // If no match is found, return false
    return false;
}

bool Rule_Year_Create(Rule_Year_t** year_list, const uint32_t years_count, const int16_t year_from, const int16_t year_to, const Rule_Data_t rule_data)
{
    // Initialize a new Rule_Year_t structure with default values
    Rule_Year_t rule_year = { 0 };

    // Set the 'From' and 'To' year fields of the rule year
    rule_year.From = year_from;
    rule_year.To = year_to;

    // Allocate memory for the 'Reserved' field and copy the data from rule_data
    rule_year.Reserved = (uint8_t*)malloc((strlen(rule_data.Reserved) + 1) * sizeof(uint8_t));
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
    int32_t save_hour = Parse_Rule_Data_Save(rule_data);

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
    Rule_Year_t* year = (Rule_Year_t*)realloc(*year_list, (years_count + 1) * sizeof(Rule_Year_t));
    if (year != NULL)
    {
        // Update the year list pointer to point to the newly allocated memory
        *year_list = year;
        // Add the new rule year to the list at the specified index
        (*year_list)[years_count] = rule_year;
        return true; // Return true indicating the rule year was successfully created
    }
    // If memory allocation fails, return false
    return false;
}

bool Rule_Year_Add_Data(Rule_Year_Data_t** year_data, const uint16_t count, const Rule_Data_t rule_data)
{
    // Initialize a new Rule_Year_Data_t structure with default values
    Rule_Year_Data_t rule_year_data = { 0 };

    // Parse the month, day, hour, and save hour from the rule data
    rule_year_data.Month = Parse_Rule_Data_In(rule_data);
    rule_year_data.Day = Parse_Rule_Data_On(rule_data);
    rule_year_data.Hour = Parse_Rule_Data_At(rule_data);
    rule_year_data.Save_Hour = Parse_Rule_Data_Save(rule_data);

    // Allocate memory for the 'Letter' field and copy the data from rule_data
    rule_year_data.Letter = (uint8_t*)malloc((strlen(rule_data.Letter) + 1) * sizeof(uint8_t));
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
    rule_year_data.Comment = (uint8_t*)malloc((strlen(rule_data.Comment) + 1) * sizeof(uint8_t));
    if (rule_year_data.Comment != NULL)
    {
        sprintf(rule_year_data.Comment, "%s", rule_data.Comment);
    }

    // Reallocate memory for the year data list to accommodate the new rule year data
    Rule_Year_Data_t* data = (Rule_Year_Data_t*)realloc(*year_data, (count + 1) * sizeof(Rule_Year_Data_t));
    if (data != NULL)
    {
        // Update the year data pointer to point to the newly allocated memory
        *year_data = data;
        // Add the new rule year data to the list at the specified index
        (*year_data)[count] = rule_year_data;
        return true; // Return true indicating the rule year data was successfully added
    }
    // If memory allocation fails, return false
    return false;
}
