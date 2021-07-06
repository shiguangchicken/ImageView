#ifndef _DATE_TIME_H
#define _DATE_TIME_H

#include <string>

#define DATE_TIME_MEMBER_NUM 7
class DateTime
{
public:
    uint32_t year;
    uint32_t month;
    uint32_t day;
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
    uint32_t millisecond;

    DateTime();

    DateTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, \
                 uint32_t minute, uint32_t second, uint32_t millisecond);
    ~DateTime(){};

    static std::string toString(DateTime &dateTime); /* y-m-d-h-m-s-ms */
    
    static DateTime fromString(std::string &str);
    
    std::string toString();
    
};

#endif