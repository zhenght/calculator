//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeaderDateAndTimeWrappersAlreadyDefined
#define vpfHeaderDateAndTimeWrappersAlreadyDefined
#include "vpfMacros.h"
#include <ctime>


static ProjectInformationInstance ProjectInfoHeaderDateAndtimeWrappers(__FILE__, "Date and time wrapper classes. ");

struct TimeWrapper
{
public:
  tm theTime;
  std::string theTimeStringNonReadable;
//  std::string GetRFC1123time();
  void AssignLocalTime();
  void ComputeTimeStringNonReadable();
  std::string ToStringHumanReadable();
  std::string ToString();
  void operator=(const std::string& inputTime);
  double SubtractAnotherTimeFromMeInSeconds(TimeWrapper& other);
  double SubtractAnotherTimeFromMeAndGet_APPROXIMATE_ResultInHours(TimeWrapper& other);
  bool AssignMonthDayYear(const std::string& input, std::stringstream& commentsOnFailure);
  static std::string ToStringSecondsToDaysHoursSecondsString
  (double input, bool includeSeconds, bool beShort);
  TimeWrapper();
};

#endif
