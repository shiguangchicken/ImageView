#include <vector>
#include <iostream>
#include <sstream>
#include "date_time.h"
using namespace std;

DateTime::DateTime():
    year(0),month(0),day(0),hour(0),
    minute(0),second(0),millisecond(0)
{
    
}

DateTime::DateTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, \
                 uint32_t minute, uint32_t second, uint32_t millisecond)
{
    this->year          = year;
    this->month         = month;
    this->day           = day;
    this->hour          = hour;
    this->minute        = minute;
    this->second        = second;
    this->millisecond   = millisecond;
}

string DateTime::toString(DateTime &dateTime)
{
    stringstream ss;
    ss << dateTime.year << "-" << dateTime.month << "-" << dateTime.day \
        << "-" << dateTime.hour << "-" << dateTime.minute << "-" \
        << dateTime.second << "-" << dateTime.millisecond; 
    
    return ss.str();    
}

DateTime DateTime::fromString(std::string &str) /* yyyy-mm-dd-hh-mm-ss-mm */
{
    DateTime dt;
    uint32_t* p[DATE_TIME_MEMBER_NUM] = { &dt.year, &dt.month, &dt.day,\
                &dt.hour, &dt.minute, &dt.second, &dt.millisecond };
    vector<uint32_t *> variable(p, p + DATE_TIME_MEMBER_NUM) ;
    
    string tempStr;
    size_t index = 0;
    for (size_t i = 0; i < str.length(); i++)
    {
        if(str[i] == '-')
        {
            *p[index++] = stoi(tempStr);
            tempStr.clear();
        }
        else
        {
            tempStr.append(str.substr(i, 1));
        }
    }     
    return dt;
}

std::string DateTime::toString()
{
    return toString(*this);
}