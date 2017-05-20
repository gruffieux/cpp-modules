////////////////////////////////////
///	\file cdate.cpp
///	\brief Date class source
///
///	\date 08.10.2008
///	\author Gabriel Ruffieux
////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "cdate.h"

//////////////////////////
/// \brief Constructor
Date::Date()
{
  day = month = year = hour = minute = second = 0;
}

////////////////////////////////////////////////////////
/// \brief Overloaded constructor with parameters
Date::Date(char *format)
{
  parse(format);
}

////////////////////////////////////////////////////////
/// \brief Overloaded constructor with parameters
Date::Date(int day, int month, int year, int hour, int minute, int second)
{
  this->day = day;
  this->month = month;
  this->year = year;
  this->hour = hour;
  this->minute = minute;
  this->second = second;
}

////////////////////////////////////////////
/// \brief Overloaded constructor of copy
Date::Date(Date &model)
{
  day = model.getDay();
  month = model.getMonth();
  year = model.getYear();
  hour = model.getHour();
  minute = model.getMinute();
  second = model.getSecond();
}

//////////////////////////
/// \brief Destructor
Date::~Date()
{
}

Date Date::currentDate()
{
	SYSTEMTIME sysTime;

	GetLocalTime(&sysTime);

	return Date(sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
}

//////////////////////////
/// \brief Forward hour
void Date::fwdHour(int h)
{
  int h2, h3;

  if (hour + h >= 24)
  {
    h2 = 24 - hour;
    h3 = h - h2;
    hour = 0;
    fwdDay(1);
    fwdHour(h3);
  }
  else
    hour += h;
}

//////////////////////////
/// \brief Reward hour
void Date::rewHour(int h)
{
  int h2, h3;

  if (hour - h <= 0)
  {
    h2 = hour;
    h3 = h - h2;
    hour = 24;
    rewDay(1);
    if (h3)
      rewHour(h3);
    else
      hour = 0;
  }
  else
    hour -= h;
}

//////////////////////////
/// \brief Forward day
void Date::fwdDay(int d)
{
  int d2, d3, last;

  last = lastDayOfMonth();

  if (day + d > last)
  {
    d2 = last - day;
    d3 = d - d2;
    if (d3)
      day = 0;
    else
      day = 1;
    fwdMonth(1);
    fwdDay(d3);
  }
  else
    day += d;
}

//////////////////////////
/// \brief Reward day
void Date::rewDay(int d)
{
  int d2, d3;

  if (day - d < 1)
  {
    d2 = day;
    d3 = d - d2;
    rewMonth(1);
    day = lastDayOfMonth();
    rewDay(d3);
  }
  else
    day -= d;
}

//////////////////////////
/// \brief Forward month
void Date::fwdMonth(int m)
{
  int m2, m3;

  if (month + m > 12)
  {
    m2 = 12 - month;
    m3 = m - m2;
    if (m3)
      month = 0;
    else
      month = 1;
    fwdYear(1);
    fwdMonth(m3);
  }
  else
    month += m;
}

//////////////////////////
/// \brief Reward month
void Date::rewMonth(int m)
{
  int m2, m3;

  if (month - m < 1)
  {
    m2 = month;
    m3 = m - m2;
    month = 12;
    rewYear(1);
    rewMonth(m3);
  }
  else
    month -= m;
}

/////////////////////////////////////////////////////////
/// \brief Parse a string formatted as a date and time
bool Date::parse(char *format)
{
  bool dateIsValid, timeIsValid, dateWithTimeIsValid;
  char *dateFormat, *timeFormat, *nextToken;

  dateIsValid = format[2] == '.' && format[5] == '.';
  timeIsValid = format[2] == ':' && format[5] == ':';
  dateWithTimeIsValid = format[2] == '.' && format[5] == '.' && format[13] == ':' && format[16] == ':';

  // Parse date and time, or only date, or only time
  if (dateWithTimeIsValid)
  {
    dateFormat = strtok_s(format, " ", &nextToken);
    timeFormat = strtok_s(NULL, " ", &nextToken);
    if (parseDate(dateFormat))
      return parseTime(timeFormat);
  }
  else
    if (dateIsValid)
      return parseDate(format);
    else
      if (timeIsValid)
        return parseTime(format);

  return false;
}

////////////////////////////////////////////////
/// \brief Parse a string formatted as a date
bool Date::parseDate(char *format)
{
  char dayStr[3], monthStr[3], yearStr[5], *token, *nextToken;

  token = strtok_s(format, ".", &nextToken);

  if (token)
  {
    strcpy_s(dayStr, sizeof(dayStr), token);
    token = strtok_s(NULL, ".", &nextToken);
    if (token)
    {
      strcpy_s(monthStr, sizeof(monthStr), token);
      token = strtok_s(NULL, ".", &nextToken);
      if (token)
        strcpy_s(yearStr, sizeof(yearStr), token);
      else
        return false;
    }
    else
      return false;
   }
  else
    return false;

  day = atol(dayStr);
  month = atol(monthStr);
  year = atol(yearStr);

  return true;
}

////////////////////////////////////////////////
/// \brief Parse a string formatted as a time
bool Date::parseTime(char *format)
{
  char hourStr[3], minuteStr[3], secondStr[3], *token, *nextToken;

  token = strtok_s(format, ":", &nextToken);

  if (token)
  {
    strcpy_s(hourStr, sizeof(hourStr), token);
    token = strtok_s(NULL, ":", &nextToken);
    if (token)
    {
      strcpy_s(minuteStr, sizeof(minuteStr), token);
      token = strtok_s(NULL, ":", &nextToken);
      if (token)
        strcpy_s(secondStr, sizeof(secondStr), token);
      else
        return false;
    }
    else
      return false;
   }
  else
    return false;

  hour = atol(hourStr);
  minute = atol(minuteStr);
  second = atol(secondStr);

  return true;
}

///////////////////////////////////////////////////////
/// \brief Compare date and time with another Date
int Date::compare(Date date)
{
  int result;

  if (!dateIsEmpty())
    result = compareDate(date);
  else
    return compareTime(date);

  if (result == 0 && !timeIsEmpty())
    return compareTime(date);

  return result;
}

////////////////////////////////////////////////
/// \brief Compare date with another Date
int Date::compareDate(Date date)
{
  if (year > date.getYear())
    return 1;
  else
  {
    if (year == date.getYear())
    {
      if (month > date.getMonth())
        return 1;
      else
      {
        if (month == date.getMonth())
        {
          if (day > date.getDay())
            return 1;
          else
            if (day < date.getDay())
              return -1;
            else
              return 0;
        }
        else
          return -1;
      }
    }
    else
      return -1;
  }
}

////////////////////////////////////////////////
/// \brief Compare time with another Date
int Date::compareTime(Date time)
{
  int t1, t2;

  t1 = hour * 3600 + minute * 60 + second;
  t2 = time.getHour() * 3600 + time.getMinute() * 60 + time.getSecond();

  if (t1 == t2)
    return 0;

  if (t1 > t2)
    return 1;

  return -1;
}

//////////////////////////////////
/// \brief Leap year algorithm
bool Date::isLeapYear()
{
  // Leap year is divisible by 4 but not divisible by 100 or divisible by 400
  return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

///////////////////////////////////////////////////////////
/// \brief Search if current day is the last of the month
int Date::lastDayOfMonth()
{
  if (month == 2)
    if (isLeapYear())
        return 29;
      else
        return 28;

  if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    return 31;

  if (month == 4 || month == 6 || month == 9 || month == 11)
      return 30;

  return 0;
}
