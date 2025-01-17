#ifndef PARSE_RULES_H
#define PARSE_RULES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TimeZoneDBCreator.h"
#include "Parse_Common.h"

// Define the maximum length for data fields
#define MAX_LENGHT_DATA_FIELD 256

    // Structure to hold rule data fields
    typedef struct
    {
        uint8_t Field[MAX_LENGHT_DATA_FIELD];   // General field for rule data
        uint8_t Name[MAX_LENGHT_DATA_FIELD];    // Name of the rule
        uint8_t From[MAX_LENGHT_DATA_FIELD];    // Start year of the rule
        uint8_t To[MAX_LENGHT_DATA_FIELD];      // End year of the rule
        uint8_t Reserved[MAX_LENGHT_DATA_FIELD];// Reserved field for future use
        uint8_t In[MAX_LENGHT_DATA_FIELD];      // Month when the rule applies
        uint8_t On[MAX_LENGHT_DATA_FIELD];      // Day when the rule applies
        uint8_t At[MAX_LENGHT_DATA_FIELD];      // Time when the rule applies
        uint8_t Save[MAX_LENGHT_DATA_FIELD];    // Amount of time to save (e.g., daylight saving time offset)
        uint8_t Letter[MAX_LENGHT_DATA_FIELD];  // Letter designation for the rule (e.g., 'S' for standard time, 'D' for daylight time)
        uint8_t Comment[MAX_LENGHT_DATA_FIELD]; // Comment or description of the rule
    } Rule_Data_t;

    // Structure to hold rule year day data
    typedef struct
    {
        uint8_t Day;                      // The specific day of the month
        uint8_t Weekday;                  // The specific weekday (e.g., Monday, Tuesday, etc.)
        bool Weekday_isAfterOrEqual_Day;  // True if the specified weekday is on or after the reference day; false if it is on or before the reference day
    } Rule_Year_Day_t;

    // Structure to hold rule year hour data
    typedef struct
    {
        int32_t Hour;          // The specific hour of the rule in seconds  (hour * 3600) + (minute * 60) + second
        bool Hour_isUTC;        // True if the time is set based on UTC, false if based on local standard time
    } Rule_Year_Hour_t;

    // Structure to hold rule year data
    typedef struct
    {
        uint8_t Month;                // The month when the rule applies (e.g., 1 for January, 12 for December)
        Rule_Year_Day_t Day;          // The specific day or weekday when the rule applies
        Rule_Year_Hour_t Hour;        // The specific hour when the rule applies, including whether it is in UTC or local time
        int32_t Save_Hour;            // The amount of time to save (e.g., daylight saving time offset in hours)
        uint8_t* Letter;              // A letter designation for the rule (e.g., 'S' for standard time, 'D' for daylight time)
        uint8_t* Comment;             // A comment or description of the rule
    } Rule_Year_Data_t;

    // Structure to hold rule year information
    typedef struct
    {
        int16_t From;                  // The start year for the rule
        int16_t To;                    // The end year for the rule
        uint8_t* Reserved;             // Reserved field for future use, dynamically allocated
        //bool DST_Effect;             // (Commented out) Field to indicate if Daylight Saving Time (DST) is in effect
        uint16_t Standard_Count;       // The number of standard time entries
        Rule_Year_Data_t* Standard;    // Pointer to an array of Rule_Year_Data_t structures for standard time entries
        uint16_t DST_Count;            // The number of Daylight Saving Time (DST) entries
        Rule_Year_Data_t* DST;         // Pointer to an array of Rule_Year_Data_t structures for DST entries
    } Rule_Year_t;

    // Structure to hold rule entry information
    typedef struct
    {
        uint8_t* Name;            // Pointer to the name of the rule entry (dynamically allocated string)
        uint32_t Years_Count;     // Number of years the rule entry spans
        Rule_Year_t* Years;       // Pointer to an array of Rule_Year_t structures representing the years
        int16_t Year_Begin;       // The beginning year of the rule entry
        int16_t Year_End;         // The ending year of the rule entry
    } Rule_Entry_t;

    // Function declarations for parsing rules and rule data
    /// <summary>
    /// Parses rules and returns a pointer to an array of rule entries.
    /// </summary>
    /// <param name="rules_Count">Pointer to an integer to store the number of parsed rules.</param>
    /// <returns>Pointer to an array of Rule_Entry_t structures.</returns>
    Rule_Entry_t* Parse_Rules(int32_t* rules_Count);

    /// <summary>
    /// Parses a single line of rule data and returns a Rule_Data_t structure.
    /// </summary>
    /// <param name="line">A string representing a line of rule data.</param>
    /// <returns>A Rule_Data_t structure containing parsed rule data.</returns>
    Rule_Data_t Parse_Rule_Data(const char* line);

    /// <summary>
    /// Extracts the 'From' year from the given rule data.
    /// </summary>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>An integer representing the 'From' year.</returns>
    int16_t Parse_Rule_Data_From(const Rule_Data_t rule_data);

    /// <summary>
    /// Extracts the 'To' year from the given rule data, with a specified maximum value.
    /// </summary>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <param name="max_value">An integer representing the maximum value for the 'To' year.</param>
    /// <returns>An integer representing the 'To' year.</returns>
    int16_t Parse_Rule_Data_To(const Rule_Data_t rule_data, int16_t max_value);

    /// <summary>
    /// Extracts the month ('In' field) from the given rule data.
    /// </summary>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>An unsigned integer representing the month.</returns>
    uint8_t Parse_Rule_Data_In(const Rule_Data_t rule_data);

    /// <summary>
    /// Extracts the day and weekday ('On' field) from the given rule data and returns a Rule_Year_Day_t structure.
    /// </summary>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>A Rule_Year_Day_t structure containing the parsed day and weekday data.</returns>
    Rule_Year_Day_t Parse_Rule_Data_On(const Rule_Data_t rule_data);

    /// <summary>
    /// Extracts the hour ('At' field) from the given rule data and returns a Rule_Year_Hour_t structure.
    /// </summary>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>A Rule_Year_Hour_t structure containing the parsed hour data.</returns>
    Rule_Year_Hour_t Parse_Rule_Data_At(const Rule_Data_t rule_data);

    /// <summary>
    /// Extracts the save hour ('Save' field) from the given rule data.
    /// </summary>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>An integer representing the save hour.</returns>
    int32_t Parse_Rule_Data_Save(const Rule_Data_t rule_data);

    /// <summary>
    /// Checks if a rule with the specified name exists in the rule list.
    /// If the rule exists, the index is stored in the find_Index parameter.
    /// </summary>
    /// <param name="rule_list">Pointer to an array of Rule_Entry_t structures representing the rule list.</param>
    /// <param name="rules_count">Pointer to an integer representing the number of rules in the list.</param>
    /// <param name="rule_name">A string representing the name of the rule to check for.</param>
    /// <param name="find_Index">Pointer to an integer to store the index of the found rule.</param>
    /// <returns>A boolean value indicating whether the rule exists (true) or not (false).</returns>
    bool Rule_isExist(const Rule_Entry_t* rule_list, const int32_t* rules_count, const char* rule_name, int32_t* find_Index);

    /// <summary>
    /// Creates a new rule with the specified name and adds it to the rule list.
    /// The number of rules in the list is specified by the rules_Count parameter.
    /// </summary>
    /// <param name="rule_list">Pointer to a pointer to an array of Rule_Entry_t structures representing the rule list.</param>
    /// <param name="rules_Count">An integer representing the number of rules in the list.</param>
    /// <param name="rule_name">A string representing the name of the new rule.</param>
    /// <returns>A boolean value indicating whether the rule was successfully created (true) or not (false).</returns>
    bool Rule_Create(Rule_Entry_t** rule_list, const int32_t rules_Count, const char* rule_name);

    /// <summary>
    /// Parses the year range for a rule and updates the rule list at the specified index.
    /// </summary>
    /// <param name="rule_list">Pointer to an array of Rule_Entry_t structures representing the rule list.</param>
    /// <param name="rule_index">An integer representing the index of the rule to update.</param>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    void Parse_Rule_Year_Range(Rule_Entry_t* rule_list, const int32_t rule_index, const Rule_Data_t rule_data);

    /// <summary>
    /// Parses the years for all rules and updates the rule list.
    /// </summary>
    /// <param name="rule_list">Pointer to an array of Rule_Entry_t structures representing the rule list.</param>
    /// <param name="rules_Count">Pointer to an integer representing the number of rules in the list.</param>
    void Parse_Rule_Years(Rule_Entry_t* rule_list, int32_t* rules_Count);

    /// <summary>
    /// Checks if a rule year with the specified range exists in the year list.
    /// If the rule year exists, the index is stored in the find_index parameter.
    /// </summary>
    /// <param name="year_list">Pointer to an array of Rule_Year_t structures representing the year list.</param>
    /// <param name="years_count">An unsigned integer representing the number of years in the list.</param>
    /// <param name="year_from">An integer representing the start year of the range.</param>
    /// <param name="year_to">An integer representing the end year of the range.</param>
    /// <param name="find_index">Pointer to an integer to store the index of the found year.</param>
    /// <returns>A boolean value indicating whether the rule year exists (true) or not (false).</returns>
    bool Rule_Year_isExist(const Rule_Year_t* year_list, const uint32_t years_count, const int16_t year_from, const int16_t year_to, int32_t* find_index);

    /// <summary>
    /// Creates a new rule year with the specified range and adds it to the year list.
    /// The number of years in the list is specified by the years_count parameter.
    /// </summary>
    /// <param name="year_list">Pointer to a pointer to an array of Rule_Year_t structures representing the year list.</param>
    /// <param name="years_count">An unsigned integer representing the number of years in the list.</param>
    /// <param name="year_from">An integer representing the start year of the new rule year.</param>
    /// <param name="year_to">An integer representing the end year of the new rule year.</param>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>A boolean value indicating whether the rule year was successfully created (true) or not (false).</returns>
    bool Rule_Year_Create(Rule_Year_t** year_list, const uint32_t years_count, const int16_t year_from, const int16_t year_to, const Rule_Data_t rule_data);

    /// <summary>
    /// Adds rule year data to the specified year data list.
    /// The number of entries in the list is specified by the count parameter.
    /// </summary>
    /// <param name="year_data">Pointer to a pointer to an array of Rule_Year_Data_t structures representing the year data list.</param>
    /// <param name="count">An unsigned integer representing the number of entries in the list.</param>
    /// <param name="rule_data">A Rule_Data_t structure containing the rule data.</param>
    /// <returns>A boolean value indicating whether the rule year data was successfully added (true) or not (false).</returns>
    bool Rule_Year_Add_Data(Rule_Year_Data_t** year_data, const uint16_t count, const Rule_Data_t rule_data);

#ifdef __cplusplus
}
#endif

#endif