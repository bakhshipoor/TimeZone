![TimeZone Library](https://github.com/bakhshipoor/TimeZone/blob/master/assets/time_zone_header_image.jpg?raw=true)

<br />

# Time zone database for embedded systems

## Time Zone Database Creator and Offset Calculator

### Introduction

This repository contains two C libraries:

* **TimeZoneDatabaseCreator:** This library extracts time zone data from the IANA time zone database and generates C code files for use in embedded systems.
* **TimeZoneOffsetCalculator:** This library calculates time zone offsets based on the generated data.

### Prerequisites

* **Microsoft Visual Studio 2022 (Version 17.12.0 or later)** is recommended for building the `TimeZoneDatabaseCreator`.
* **A C compiler** for the target platform of your embedded system.

### Getting Started

#### Obtaining the IANA Time Zone Database

1. Download the latest **Data Only Distribution** from https://www.iana.org/time-zones.
2. Extract the downloaded archive to a suitable location.

#### Building the TimeZoneDatabaseCreator

1. Open the `TimeZoneDatabaseCreator` solution in Visual Studio.
2. Configure the project to point to the extracted IANA time zone database.
3. Build the solution.

#### Using the TimeZoneOffsetCalculator

1. Include the generated C code files and the `TimeZoneOffsetCalculator` library in your project.
2. Link your project with the `TimeZoneOffsetCalculator` library.
3. Use the provided functions to calculate time zone offsets based on a given date and time.

### Additional Information

* **IANA Time Zone Database:** https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
* **tz database:** https://github.com/eggert/tz

**Disclaimer:** This library is under active development and may contain bugs or limitations.

