#ifndef PARSE_RULES_H
#define PARSE_RULES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"


    // Structure to hold rule data fields
    typedef struct
    {
        CHAR Field[MAX_LENGHT_DATA_FIELD];   // General field for rule data
        CHAR Name[MAX_LENGHT_DATA_FIELD];    // Name of the rule
        CHAR From[MAX_LENGHT_DATA_FIELD];    // Start year of the rule
        CHAR To[MAX_LENGHT_DATA_FIELD];      // End year of the rule
        CHAR Reserved[MAX_LENGHT_DATA_FIELD];// Reserved field for future use
        CHAR In[MAX_LENGHT_DATA_FIELD];      // Month when the rule applies
        CHAR On[MAX_LENGHT_DATA_FIELD];      // Day when the rule applies
        CHAR At[MAX_LENGHT_DATA_FIELD];      // Time when the rule applies
        CHAR Save[MAX_LENGHT_DATA_FIELD];    // Amount of time to save (e.g., daylight saving time offset)
        CHAR Letter[MAX_LENGHT_DATA_FIELD];  // Letter designation for the rule (e.g., 'S' for standard time, 'D' for daylight time)
        CHAR Comment[MAX_LENGHT_DATA_FIELD]; // Comment or description of the rule
    } Rule_Data_t;

    // Structure to hold rule year day data
    typedef struct
    {
        DAY Day;                      // The specific day of the month
        WEEKDAY Weekday;                  // The specific weekday (e.g., Monday, Tuesday, etc.)
        BOOL Weekday_isAfterOrEqual_Day;  // True if the specified weekday is on or after the reference day; FALSE if it is on or before the reference day
    } Rule_Year_Day_t;

    // Structure to hold rule year hour data
    typedef struct
    {
        HOUR Hour;          // The specific hour of the rule in seconds  (hour * 3600) + (minute * 60) + second
        BOOL Hour_isUTC;        // True if the time is set based on UTC, FALSE if based on local standard time
    } Rule_Year_Hour_t;

    // Structure to hold rule year data
    typedef struct
    {
        MONTH Month;                // The month when the rule applies (e.g., 1 for January, 12 for December)
        Rule_Year_Day_t Day;          // The specific day or weekday when the rule applies
        Rule_Year_Hour_t Hour;        // The specific hour when the rule applies, including whether it is in UTC or local time
        HOUR Save_Hour;            // The amount of time to save (e.g., daylight saving time offset in hours)
        CHAR* Letter;              // A letter designation for the rule (e.g., 'S' for standard time, 'D' for daylight time)
        CHAR* Comment;             // A comment or description of the rule
    } Rule_Year_Data_t;

    // Structure to hold rule year information
    typedef struct
    {
        YEAR From;                  // The start year for the rule
        YEAR To;                    // The end year for the rule
        CHAR* Reserved;             // Reserved field for future use, dynamically allocated
        //BOOL DST_Effect;             // (Commented out) Field to indicate if Daylight Saving Time (DST) is in effect
        COUNTER Standard_Count;       // The number of standard time entries
        Rule_Year_Data_t* Standard;    // Pointer to an array of Rule_Year_Data_t structures for standard time entries
        COUNTER DST_Count;            // The number of Daylight Saving Time (DST) entries
        Rule_Year_Data_t* DST;         // Pointer to an array of Rule_Year_Data_t structures for DST entries
    } Rule_Year_t;

    // Structure to hold rule entry information
    typedef struct
    {
        CHAR* Name;            // Pointer to the name of the rule entry (dynamically allocated string)
        COUNTER Years_Count;     // Number of years the rule entry spans
        Rule_Year_t* Years;       // Pointer to an array of Rule_Year_t structures representing the years
        YEAR Year_Begin;       // The beginning year of the rule entry
        YEAR Year_End;         // The ending year of the rule entry
    } Rule_Entry_t;

    // Function declarations for parsing rules and rule data
    /// <summary>
    /// Parses rules and returns a pointer to an array of rule entries.
    /// </summary>
    /// <param name="rules_Count">Pointer to an integer to store the number of parsed rules.</param>
    /// <returns>Pointer to an array of Rule_Entry_t structures.</returns>
    Rule_Entry_t* Parse_Rules(COUNTER* rules_Count);

    /// <summary>
    /// Parses a single line of rule data and returns a Rule_Data_t structure.
    /// </summary>
    /// <param name="line">A string representing a line of rule data.</param>
    /// <returns>A Rule_Data_t structure containing parsed rule data.</returns>
    Rule_Data_t Parse_Rule_Data(CONST CHAR* line);

    /// <summary>
    /// Extracts the 'From' year from the given rule data.
    /// </summary>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>An integer representing the 'From' year.</returns>
    YEAR Parse_Rule_Data_From(CONST Rule_Data_t rule_data);

    /// <summary>
    /// Extracts the 'To' year from the given rule data, with a specified maximum value.
    /// </summary>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <param name="max_value">An integer representing the maximum value for the 'To' year.</param>
    /// <returns>An integer representing the 'To' year.</returns>
    YEAR Parse_Rule_Data_To(CONST Rule_Data_t rule_data, YEAR max_value);

    /// <summary>
    /// Extracts the month ('In' field) from the given rule data.
    /// </summary>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>An unsigned integer representing the month.</returns>
    MONTH Parse_Rule_Data_In(CONST Rule_Data_t rule_data);

    /// <summary>
    /// Extracts the day and weekday ('On' field) from the given rule data and returns a Rule_Year_Day_t structure.
    /// </summary>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>A Rule_Year_Day_t structure containing the parsed day and weekday data.</returns>
    Rule_Year_Day_t Parse_Rule_Data_On(CONST Rule_Data_t rule_data);

    /// <summary>
    /// Extracts the hour ('At' field) from the given rule data and returns a Rule_Year_Hour_t structure.
    /// </summary>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>A Rule_Year_Hour_t structure containing the parsed hour data.</returns>
    Rule_Year_Hour_t Parse_Rule_Data_At(CONST Rule_Data_t rule_data);

    /// <summary>
    /// Extracts the save hour ('Save' field) from the given rule data.
    /// </summary>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>An integer representing the save hour.</returns>
    HOUR Parse_Rule_Data_Save(CONST Rule_Data_t rule_data);

    /// <summary>
    /// Checks if a rule with the specified name exists in the rule list.
    /// If the rule exists, the index is stored in the find_Index parameter.
    /// </summary>
    /// <param name="rule_list">Pointer to an array of Rule_Entry_t structures representing the rule list.</param>
    /// <param name="rules_count">Pointer to an integer representing the number of rules in the list.</param>
    /// <param name="rule_name">A string representing the name of the rule to check for.</param>
    /// <param name="find_Index">Pointer to an integer to store the index of the found rule.</param>
    /// <returns>A BOOLean value indicating whether the rule exists (TRUE) or not (FALSE).</returns>
    BOOL Rule_isExist(CONST Rule_Entry_t* rule_list, CONST COUNTER* rules_count, CONST CHAR* rule_name, COUNTER* find_Index);

    /// <summary>
    /// Creates a new rule with the specified name and adds it to the rule list.
    /// The number of rules in the list is specified by the rules_Count parameter.
    /// </summary>
    /// <param name="rule_list">Pointer to a pointer to an array of Rule_Entry_t structures representing the rule list.</param>
    /// <param name="rules_Count">An integer representing the number of rules in the list.</param>
    /// <param name="rule_name">A string representing the name of the new rule.</param>
    /// <returns>A BOOLean value indicating whether the rule was successfully created (TRUE) or not (FALSE).</returns>
    BOOL Rule_Create(Rule_Entry_t** rule_list, CONST COUNTER rules_Count, CONST CHAR* rule_name);

    /// <summary>
    /// Parses the year range for a rule and updates the rule list at the specified index.
    /// </summary>
    /// <param name="rule_list">Pointer to an array of Rule_Entry_t structures representing the rule list.</param>
    /// <param name="rule_index">An integer representing the index of the rule to update.</param>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    VOID Parse_Rule_Year_Range(Rule_Entry_t* rule_list, CONST COUNTER rule_index, CONST Rule_Data_t rule_data);

    /// <summary>
    /// Parses the years for all rules and updates the rule list.
    /// </summary>
    /// <param name="rule_list">Pointer to an array of Rule_Entry_t structures representing the rule list.</param>
    /// <param name="rules_Count">Pointer to an integer representing the number of rules in the list.</param>
    VOID Parse_Rule_Years(Rule_Entry_t* rule_list, COUNTER* rules_Count);

    /// <summary>
    /// Checks if a rule year with the specified range exists in the year list.
    /// If the rule year exists, the index is stored in the find_index parameter.
    /// </summary>
    /// <param name="year_list">Pointer to an array of Rule_Year_t structures representing the year list.</param>
    /// <param name="years_count">An unsigned integer representing the number of years in the list.</param>
    /// <param name="year_from">An integer representing the start year of the range.</param>
    /// <param name="year_to">An integer representing the end year of the range.</param>
    /// <param name="find_index">Pointer to an integer to store the index of the found year.</param>
    /// <returns>A BOOLean value indicating whether the rule year exists (TRUE) or not (FALSE).</returns>
    BOOL Rule_Year_isExist(CONST Rule_Year_t* year_list, CONST COUNTER years_count, CONST YEAR year_from, CONST YEAR year_to, COUNTER* find_index);

    /// <summary>
    /// Creates a new rule year with the specified range and adds it to the year list.
    /// The number of years in the list is specified by the years_count parameter.
    /// </summary>
    /// <param name="year_list">Pointer to a pointer to an array of Rule_Year_t structures representing the year list.</param>
    /// <param name="years_count">An unsigned integer representing the number of years in the list.</param>
    /// <param name="year_from">An integer representing the start year of the new rule year.</param>
    /// <param name="year_to">An integer representing the end year of the new rule year.</param>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>A BOOLean value indicating whether the rule year was successfully created (TRUE) or not (FALSE).</returns>
    BOOL Rule_Year_Create(Rule_Year_t** year_list, CONST COUNTER years_count, CONST YEAR year_from, CONST YEAR year_to, CONST Rule_Data_t rule_data);

    /// <summary>
    /// Adds rule year data to the specified year data list.
    /// The number of entries in the list is specified by the count parameter.
    /// </summary>
    /// <param name="year_data">Pointer to a pointer to an array of Rule_Year_Data_t structures representing the year data list.</param>
    /// <param name="count">An unsigned integer representing the number of entries in the list.</param>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>A BOOLean value indicating whether the rule year data was successfully added (TRUE) or not (FALSE).</returns>
    BOOL Rule_Year_Add_Data(Rule_Year_Data_t** year_data, CONST COUNTER count, CONST Rule_Data_t rule_data);

#ifdef __cplusplus
}
#endif

#endif