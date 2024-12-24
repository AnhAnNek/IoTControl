#include "Utils.h"

// Constructor
Utils::Utils(NTPClient& timeClientRef) : timeClient(timeClientRef) {}

// Function to check if a year is a leap year
bool Utils::isLeapYear(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Function to get the number of days in a month
int Utils::daysInMonth(int year, int month) {
  switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
    case 4: case 6: case 9: case 11: return 30;
    case 2: return isLeapYear(year) ? 29 : 28;
  }
  return 0;
}

// Function to get the current timestamp in ISO 8601 format
String Utils::getISO8601Timestamp() {
  // Ensure the NTP client is updated
  if (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  unsigned long epochTime = timeClient.getEpochTime();

  // Calculate date and time components
  int year = 1970;
  while (epochTime >= 31556926) { // Seconds in an average year
    epochTime -= (isLeapYear(year) ? 31622400 : 31536000);
    year++;
  }

  int month = 1;
  while (true) {
    int daysInMonthValue = daysInMonth(year, month);
    if (epochTime < daysInMonthValue * 86400) break;
    epochTime -= daysInMonthValue * 86400;
    month++;
  }

  int day = epochTime / 86400 + 1;
  epochTime %= 86400;
  int hour = epochTime / 3600;
  epochTime %= 3600;
  int minute = epochTime / 60;
  int second = epochTime % 60;

  // Format as ISO 8601
  char buffer[25];
  sprintf(buffer, "%04d-%02d-%02dT%02d:%02d:%02dZ", year, month, day, hour, minute, second);
  return String(buffer);
}
