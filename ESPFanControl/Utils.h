#ifndef UTILS_H
#define UTILS_H

#include <NTPClient.h>
#include <Arduino.h>

class Utils {
public:
  // Constructor to initialize the Utils class
  Utils(NTPClient& timeClient);

  // Function to check if a year is a leap year
  static bool isLeapYear(int year);

  // Function to get the number of days in a month
  static int daysInMonth(int year, int month);

  // Function to get the current timestamp in ISO 8601 format
  String getISO8601Timestamp();

private:
  NTPClient& timeClient; // Reference to NTPClient for date/time operations
};

#endif
