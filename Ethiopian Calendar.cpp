/*
==========================================================================================

Program Title: Ethiopian and Gregorian Calendar System

Purpose:
---------
This C++ program is designed to perform multiple calendar-related operations, mainly focusing
on the Ethiopian calendar. It allows users to display both Ethiopian and Gregorian calendars,
convert dates between the two calendar systems, and identify major Ethiopian holidays.

Key Features:
--------------
1. Display a full Ethiopian calendar for a given Ethiopian year, including holidays.
2. Convert a Gregorian date to the equivalent Ethiopian date.
3. Convert an Ethiopian date to the equivalent Gregorian date.
4. Display a standard Gregorian calendar for any given Gregorian year.
5. Identify and display major Ethiopian holidays based on the date.

This program enhances understanding of date handling, calendar logic, array usage, leap year
calculations, and user input/output operations in C++.

==========================================================================================
*/


#include <iostream>
#include <iomanip>
#include <ctime> // for time and date calculations
using namespace std;

// Ethiopian calendar month names
string months[13] = {
    "Meskerem", "Tikimt", "Hidar", "Tahisas", "Tir", "Yekatit",
    "Megabit", "Miyazia", "Ginbot", "Sene", "Hamle", "Nehase", "Pagume"
};

// Weekday names starting from Monday
string weekdays[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

// Function to check if an Ethiopian year is a leap year
bool isLeapYear(int year) {
    // In the Ethiopian calendar, a year is a leap year if it leaves remainder 3 when divided by 4
    return year % 4 == 3;
}

// Calculate the Ethiopian "Amete Alem" (year since creation of the world)
int computeAmeteAlem(int year) {
    return 5500 + year;
}

// Calculate Metene Rabiet (used to determine starting weekday)
int computeMeteneRabiet(int amete_alem) {
    return amete_alem / 4;
}

// Determine the Evangelist name for the year
string getEvangelist(int amete_alem) {
    switch (amete_alem % 4) {
        case 1: return "Mathewos";
        case 2: return "Markos";
        case 3: return "Lukas";
        default: return "Yohannes";
    }
}

// Find the start day of the Ethiopian year (1 Meskerem)
// Returns day index: 0 = Monday, ..., 6 = Sunday
int computeNewYearStartDay(int year) {
    int amete_alem = computeAmeteAlem(year);
    int metene_rabiet = computeMeteneRabiet(amete_alem);
    return (amete_alem + metene_rabiet) % 7;
}

// Get Ethiopian holidays based on fixed dates
string getEthiopianHoliday(int year, int month, int day) {
    bool isLeap = isLeapYear(year);

    if (month == 1 && day == 1) return "Enkutatash (New Year)";
    if (month == 1 && day == 17) return "Meskel";
    if (month == 4 && ((!isLeap && day == 29) || (isLeap && day == 28))) return "Gena (Christmas)";
    if (month == 5 && day == 11) return "Timket (Epiphany)";
    if (month == 6 && day == 23) return "Adwa (Adwa Victory Day)";
    if (month == 8 && day == 23) return "Ye labaderoch Ken (Labour Day)";
    if (month == 8 && day == 27) return "Ye Arbegnoch Ken (Patriots' Victory Day)";

    return "";
}

// Print a calendar grid for a given Ethiopian month
void printMonthGrid(string monthName, int startDay, int numDays, int year, int monthIndex) {
    cout << "\n" << monthName << " " << year << "\n";
    cout << "Mon Tue Wed Thu Fri Sat Sun\n";

    int day = 1, weekDay = 0;
    bool printedHolidayInMonth = false;

    // Print spaces before the first day of the month
    while (weekDay < startDay) {
        cout << "    ";
        weekDay++;
    }

    // Print each day, marking holidays with '*'
    while (day <= numDays) {
        string holiday = getEthiopianHoliday(year, monthIndex, day);
        if (!holiday.empty()) {
            cout << setw(2) << day << "* ";
            printedHolidayInMonth = true;
        } else {
            cout << setw(3) << day << " ";
        }

        weekDay++;
        if (weekDay == 7) {
            cout << endl;
            weekDay = 0;
        }
        day++;
    }
    cout << endl;

    // If there were holidays, list them below the calendar
    if (printedHolidayInMonth) {
        cout << "Holidays this month:\n";
        for (int d = 1; d <= numDays; d++) {
            string holiday = getEthiopianHoliday(year, monthIndex, d);
            if (!holiday.empty()) {
                cout << d << " - " << holiday << endl;
            }
        }
    }
}

// Display the full Ethiopian calendar for a given year
void displayFullEthiopianCalendar(int year) {
    int amete_alem = computeAmeteAlem(year);
    bool leap = isLeapYear(year);
    int startDay = computeNewYearStartDay(year); // Starting weekday for Meskerem
    string evangelist = getEvangelist(amete_alem);

    cout << "\nYear: " << year << endl;
    cout << "Amete Alem: " << amete_alem << endl;
    cout << "Evangelist: " << evangelist << endl;
    cout << "First day of Meskerem: " << weekdays[startDay] << endl;

    // Loop through all 13 months
    for (int i = 0; i < 13; ++i) {
        int daysInMonth = (i == 12) ? (leap ? 6 : 5) : 30;
        printMonthGrid(months[i], startDay, daysInMonth, year, i + 1);
        startDay = (startDay + daysInMonth) % 7;
    }
}

// Convert Gregorian date to Ethiopian date
void convertGregorianToEthiopian(int gYear, int gMonth, int gDay) {
    struct tm gDate = {0}, newYearDate = {0};
    gDate.tm_year = gYear - 1900;
    gDate.tm_mon = gMonth - 1;
    gDate.tm_mday = gDay;

    // Validate the Gregorian date
    if (mktime(&gDate) == -1) {
        cout << "Invalid Gregorian date.\n";
        return;
    }

    // Determine New Year day in Gregorian calendar
    int newYearDay = 11;
    bool isGregorianLeap = ((gYear % 4 == 0 && gYear % 100 != 0) || (gYear % 400 == 0));
    if (isGregorianLeap) newYearDay = 12;

    // Estimate Ethiopian year
    int eYear = gYear - 8;
    if (gMonth > 9 || (gMonth == 9 && gDay >= newYearDay)) {
        eYear++;
    }

    newYearDate.tm_year = (eYear + 8) - 1900;
    newYearDate.tm_mon = 8;
    newYearDate.tm_mday = newYearDay;

    // Calculate date difference
    time_t gTime = mktime(&gDate);
    time_t newYearTime = mktime(&newYearDate);
    int dayDiff = static_cast<int>(difftime(gTime, newYearTime) / (60 * 60 * 24));

    // Adjust for previous Ethiopian year
    if (dayDiff < 0) {
        dayDiff += 365;
        if (((eYear + 7) % 4) == 3) {
            dayDiff += 1;
        }
    }

    int eMonth = dayDiff / 30 + 1;
    int eDay = dayDiff % 30 + 1;

    // Display result
    cout << "Gregorian Date: " << gYear << "-" << gMonth << "-" << gDay << endl;
    cout << "Ethiopian Date: " << eYear << "-" << eMonth << "-" << eDay << endl;
}

// Convert Ethiopian date to Gregorian date
void convertEthiopianToGregorian(int eYear, int eMonth, int eDay) {
    // Validate Ethiopian date
    if (eMonth < 1 || eMonth > 13 || eDay < 1 || eDay > 30 || (eMonth == 13 && eDay > (isLeapYear(eYear) ? 6 : 5))) {
        cout << "Invalid Ethiopian date.\n";
        return;
    }

    int gYear = eYear + 7;
    bool gLeap = ((gYear % 4 == 0 && gYear % 100 != 0) || (gYear % 400 == 0));
    int newYearDay = gLeap ? 12 : 11;

    // Calculate days passed from 1 Meskerem
    int daysFromNewYear = (eMonth - 1) * 30 + (eDay - 2);

    struct tm newYearDate = {0};
    newYearDate.tm_year = gYear - 1900;
    newYearDate.tm_mon = 8;
    newYearDate.tm_mday = newYearDay;

    // Add the days to New Year to get Gregorian date
    time_t newYearTime = mktime(&newYearDate);
    time_t targetTime = newYearTime + daysFromNewYear * 24 * 60 * 60;

    struct tm* result = localtime(&targetTime);
    cout << "Ethiopian Date: " << eYear << "-" << eMonth << "-" << eDay << endl;
    cout << "Gregorian Date: " << result->tm_year + 1900 << "-"
         << result->tm_mon + 1 << "-" << result->tm_mday << endl;
}

// Display Gregorian calendar for the whole year
void displayGregorianCalendar(int year) {
    cout << "\nGregorian Calendar for " << year << "\n";

    string months[12] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    int daysInMonth[12] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    // Adjust February for leap years
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth[1] = 29;
    }

    for (int month = 0; month < 12; ++month) {
        cout << "\n  " << months[month] << " " << year << "\n";
        cout << "Sun Mon Tue Wed Thu Fri Sat\n";

        struct tm firstDay = {0};
        firstDay.tm_year = year - 1900;
        firstDay.tm_mon = month;
        firstDay.tm_mday = 1;
        mktime(&firstDay);

        int startWeekday = firstDay.tm_wday; // 0=Sunday
        int day = 1;

        // Print empty spaces before the first day
        for (int i = 0; i < startWeekday; ++i) {
            cout << "    ";
        }

        // Print all days in the month
        while (day <= daysInMonth[month]) {
            cout << setw(3) << day << " ";
            if ((startWeekday + day) % 7 == 0) {
                cout << endl;
            }
            ++day;
        }
        cout << "\n";
    }
}

// Main menu-driven program
int main() {
    int choice;
    cout << "===== Calendar System =====\n";
    do {
        cout << "\nSelect an option:\n";
        cout << "1. Display Ethiopian Calendar\n";
        cout << "2. Convert Gregorian to Ethiopian Date\n";
        cout << "3. Convert Ethiopian to Gregorian Date\n";
        cout << "4. Display Gregorian Calendar\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int eYear;
            cout << "Enter Ethiopian year: ";
            cin >> eYear;
            displayFullEthiopianCalendar(eYear);
        }
        else if (choice == 2) {
            int gY, gM, gD;
            cout << "Enter Gregorian date (YYYY MM DD): ";
            cin >> gY >> gM >> gD;
            convertGregorianToEthiopian(gY, gM, gD);
        }
        else if (choice == 3) {
            int eY, eM, eD;
            cout << "Enter Ethiopian date (YYYY MM DD): ";
            cin >> eY >> eM >> eD;
            convertEthiopianToGregorian(eY, eM, eD);
        }
        else if (choice == 4) {
            int gYear;
            cout << "Enter Gregorian year: ";
            cin >> gYear;
            displayGregorianCalendar(gYear);
        }
    } while (choice != 5);

    return 0;
}
