//
// 2017 SYSU Software Engineering Training Project -- Agenda
//
// Date.cpp
// C++
//
// Created by Nino on 17/06/02.
// Copyright Nino Leo. All rights reserved.
//


#include "Date.hpp"
#include <cstdio>


#define DATESTRING_LEN 16
#define FORMATTED "%04d-%02d-%02d/%02d:%02d"


/**
* @brief default constructor
*/
Date::Date() : m_year(0), m_month(0), m_day(0), m_hour(0), m_minute(0) {}


/**
* @brief constructor with arguments
*/
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute)
  	: m_year(t_year),
  	m_month(t_month),
  	m_day(t_day),
  	m_hour(t_hour),
  	m_minute(t_minute) {}


/**
* @brief constructor with a string
*/
Date::Date(const std::string &dateString) { (*this) = stringToDate(dateString); }
  

/**
* @brief return the year of a Date
* @return   a integer indicate the year of a date
*/
int Date::getYear(void) const { return m_year; }


/**
* @brief set the year of a date
* @param a integer indicate the new year of a date
*/
void Date::setYear(const int t_year) { m_year = t_year; }


/**
* @brief return the month of a Date
* @return   a integer indicate the month of a date
*/
int Date::getMonth(void) const { return m_month; }


/**
* @brief set the month of a date
* @param a integer indicate the new month of a date
*/
void Date::setMonth(const int t_month) { m_month = t_month; }


/**
* @brief return the day of a Date
* @return   a integer indicate the day of a date
*/
int Date::getDay(void) const { return m_day; }


/**
* @brief set the day of a date
* @param a integer indicate the new day of a date
*/
void Date::setDay(const int t_day) { m_day = t_day; }


/**
* @brief return the hour of a Date
* @return   a integer indicate the hour of a date
*/
int Date::getHour(void) const { return m_hour; }


/**
* @brief set the hour of a date
* @param a integer indicate the new hour of a date
*/
void Date::setHour(const int t_hour) { m_hour = t_hour; }


/**
* @brief return the minute of a Date
* @return   a integer indicate the minute of a date
*/
int Date::getMinute(void) const { return m_minute; }


/**
* @brief set the minute of a date
* @param a integer indicate the new minute of a date
*/
void Date::setMinute(const int t_minute) { m_minute = t_minute; }


/**
* @brief check whether the date is valid or not
* @return the bool indicate valid or not
*/
bool Date::isValid(const Date &t_date) {
  // year valid 
  if(t_date.getYear() > 9999 || t_date.getYear() < 1000) 
  	return false;
  // month valid 
  if(t_date.m_month > 12 || t_date.m_month < 1) 
  	return false;
  // day of month and day valid
  bool leap = false;
 if(t_date.getYear() % 400 == 0 || (t_date.getYear() % 4 == 0 && t_date.getYear() % 100 != 0))
    leap = true;
  int dayofmonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if(leap) 
  	dayofmonth[1] = 29;
  if(t_date.getDay() < 1 || t_date.getDay() > dayofmonth[t_date.getMonth()-1]) return false;
  // hour valid
  if(t_date.getHour() < 0 || t_date.getHour() > 23) return false;
  // minute valid
  if(t_date.getMinute() < 0 || t_date.getMinute() > 59) return false;
  return true;
}


/**
* @brief convert a string to date, if the format is not correct return
* 0000-00-00/00:00
* @return a date
*/
Date Date::stringToDate(const std::string &t_dateString) {
  if(t_dateString.length() != DATESTRING_LEN) 
  	return Date(0, 0, 0, 0, 0);
  for(int i = 0; i < DATESTRING_LEN; i++) {
  	if(i == 4 || i == 7) {
  		if(t_dateString[i] != '-')
  			return Date(0, 0, 0, 0, 0);
    }
  	else if(i == 10) {
  		if(t_dateString[i] != '/')
  		  	return Date(0, 0, 0, 0, 0);
    }
  	else if(i == 13) {
  		if(t_dateString[i] != ':')
  		  	return Date(0, 0, 0, 0, 0);
    }
  	else {
  		if(t_dateString[i] < '0' || t_dateString[i] > '9')
  		  	return Date(0, 0, 0, 0, 0);
    }
  }
  char buff[DATESTRING_LEN+1];
  for(int i = 0; i < DATESTRING_LEN; i++) 
    buff[i] = t_dateString[i];
  int t_year, t_month, t_day, t_hour, t_minute;
  if(sscanf(buff, FORMATTED, &t_year, &t_month, &t_day, &t_hour, &t_minute) != 5) {
    t_year = 0;
    t_month = 0;
    t_day = 0;
    t_hour = 0;
    t_minute = 0;
  }
  Date temp(t_year, t_month, t_day, t_hour, t_minute);
  return temp;
}


/**
* @brief convert a date to string, if the format is not correct return
* 0000-00-00/00:00
*/
std::string Date::dateToString(const Date &t_date) {
  char buff[DATESTRING_LEN+1];
  sprintf(buff, FORMATTED, t_date.getYear(), t_date.getMonth(), t_date.getDay(), t_date.getHour(), t_date.getMinute());
  buff[DATESTRING_LEN] = 0;
  return buff;
}


/**
*  @brief overload the assign operator
*/
Date& Date::operator=(const Date &t_date) {
  m_year = t_date.getYear();
  m_month = t_date.getMonth();
  m_day = t_date.getDay();
  m_minute = t_date.getMinute();
  m_hour = t_date.getHour();
  return *this;
}


/**
* @brief check whether the CurrentDate is equal to the t_date
*/
bool Date::operator==(const Date &t_date) const {
  // return dateToString(*this) > dateToString(t_date);
  if(m_year != t_date.getYear()) return false;
  if(m_month != t_date.getMonth()) return false;
  if(m_day != t_date.getDay()) return false;
  if(m_minute != t_date.getMinute()) return false;
  if(m_hour != t_date.getHour()) return false;
  return true;
}


/**
* @brief check whether the CurrentDate is  greater than the t_date
*/
bool Date::operator>(const Date &t_date) const {
  if(m_year > t_date.getYear()) return true;
  if(m_year < t_date.getYear()) return false;
  if(m_month > t_date.getMonth()) return true;
  if(m_month < t_date.getMonth()) return false;
  if(m_day > t_date.getDay()) return true;
  if(m_day < t_date.getDay()) return false;
  if(m_hour > t_date.getHour()) return true;
  if(m_hour < t_date.getHour()) return false;
  if(m_minute > t_date.getMinute()) return true;
  return false;
}


/**
* @brief check whether the CurrentDate is  less than the t_date
*/
bool Date::operator<(const Date &t_date) const {
  if(*this >= t_date) return false;
  return true;
}


/**
* @brief check whether the CurrentDate is  greater or equal than the t_date
*/
bool Date::operator>=(const Date &t_date) const {
  if(*this > t_date || *this == t_date) return true;
  return false;
}


/**
* @brief check whether the CurrentDate is  less than or equal to the t_date
*/
bool Date::operator<=(const Date &t_date) const {
  if(*this < t_date || *this == t_date) return true;
  return false;
}
