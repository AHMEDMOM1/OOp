#pragma once
#include "Date.h"
#include <string>

class clsString {
public:
  static std::string getCurrentDate(time_t date = time(0)) {
    tm ltm = Date::getDate(date);
    return std::to_string(ltm.tm_mday) + "/" + std::to_string(ltm.tm_mon + 1) +
           "/" + std::to_string(ltm.tm_year + 1900);
  }

  static std::string getCurrentTime(time_t date = time(0)) {
    tm ltm = Date::getDate(date);
    return std::to_string(ltm.tm_hour) + ":" + std::to_string(ltm.tm_min) +
           ":" + std::to_string(ltm.tm_sec);
  }

  static std::string getFullDate(time_t date = time(0)) {
    return getCurrentDate(date) + " - " + getCurrentTime(date);
  }
};