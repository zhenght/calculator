//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include "vpfHeader1General5TimeDate.h"
#include "vpfHeader1General0_General.h"
#include "vpfHeader2Math0_General.h"

static ProjectInformationInstance ProjectInfoDateAndtimeWrappers(__FILE__, "Date and time wrapper class implementation. ");

TimeWrapper::TimeWrapper()
{ this->theTime.tm_gmtoff=0;
  this->theTime.tm_hour=0;
  this->theTime.tm_isdst=0;
  this->theTime.tm_mday=0;
  this->theTime.tm_min=0;
  this->theTime.tm_mon=0;
  this->theTime.tm_sec=0;
  this->theTime.tm_wday=0;
  this->theTime.tm_yday=0;
  this->theTime.tm_year=0;
  this->theTime.tm_zone=0;
}

bool TimeWrapper::AssignMonthDayYear(const std::string& input, std::stringstream& commentsOnFailure)
{ this->AssignLocalTime(); //<-hopefully this initialized things properly ...
  List<unsigned char> theDelimiters;
  theDelimiters.AddOnTop('/');
  theDelimiters.AddOnTop('-');
  theDelimiters.AddOnTop('.');
  List<std::string> output;
  MathRoutines::StringSplitExcludeDelimiters(input, theDelimiters, output);
  if (output.size<3)
  { commentsOnFailure << "Failed to extract a M/D/Y date from: " << input;
    return false;
  }
  int month= atoi(output[0].c_str());
  int day= atoi(output[1].c_str());
  int year= atoi(output[2].c_str());
  this->theTime.tm_sec=59;
  this->theTime.tm_min=59;
  this->theTime.tm_hour=23;
  this->theTime.tm_mday=day;
  this->theTime.tm_mon=month-1;
  this->theTime.tm_year=year-1900;
  mktime(&this->theTime);
  return true;
}

void TimeWrapper::ComputeTimeStringNonReadable()
{ std::stringstream out;
  out << this->theTime.tm_year << "-" << this->theTime.tm_mon << "-"
  << this->theTime.tm_mday
  << "-" << this->theTime.tm_hour << "-"
  << this->theTime.tm_min
  << "-"
  << this->theTime.tm_sec;
  this->theTimeStringNonReadable=out.str();
}

void TimeWrapper::AssignLocalTime()
{ std::time_t rawtime;
  time(&rawtime);
  this->theTime=*std::gmtime(&rawtime);
  mktime(&this->theTime);
  this->ComputeTimeStringNonReadable();
}

std::string TimeWrapper::ToStringSecondsToDaysHoursSecondsString(double input, bool includeSeconds)
{ std::stringstream out;
  bool isPositive=(input>0);
  if (!isPositive)
    input*=-1;
  int days= (int ) FloatingPoint::floor( input/(24*3600));
  input-=days*24*3600;
  if (!isPositive)
    out << "-(";
  out << "~";
  if (days>0)
    out << days << " day(s) ";
  out.precision(1);
  if (input>0)
    out << std::fixed << input/3600 << " hour(s)";
  if (includeSeconds)
    out << std::fixed << (((int)input) /60) << " minute(s) " << (((int)input) %60) << " second(s).";
  if (!isPositive)
    out << ")";
  return out.str();
}

double TimeWrapper::SubtractAnotherTimeFromMeAndGet_APPROXIMATE_ResultInHours(TimeWrapper& other)
{ return this->SubtractAnotherTimeFromMeInSeconds(other)/3600;
}

double TimeWrapper::SubtractAnotherTimeFromMeInSeconds(TimeWrapper& other)
{ return difftime(mktime(&this->theTime), mktime(&other.theTime));
}

std::string TimeWrapper::ToStringHumanReadable()
{ return std::asctime(&this->theTime);
}

void TimeWrapper::operator=(const std::string& input)
{ std::string inputNoDashes;
  inputNoDashes.reserve(input.size());
  for (unsigned int i=0; i<input.size(); i++)
    if (input[i]!='-')
      inputNoDashes.push_back(input[i]);
    else
      inputNoDashes.push_back(' ');
  std::stringstream inputStream;
  inputStream << inputNoDashes;
  inputStream.seekg(0);
  inputStream >> this->theTime.tm_year;
  inputStream >> this->theTime.tm_mon;
  inputStream >> this->theTime.tm_mday;
  inputStream >> this->theTime.tm_hour;
  inputStream >> this->theTime.tm_min;
  inputStream >> this->theTime.tm_sec;
  this->ComputeTimeStringNonReadable();
}