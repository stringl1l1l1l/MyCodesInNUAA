#include <iostream>
using namespace std;

typedef struct DateTime
{
    int year, month, day;
} DateTime;

int monthDay[13][2] =
    {{0, 0}, {31, 31}, {28, 29}, {31, 31}, {30, 30}, {31, 31}, {30, 30}, {31, 31}, {31, 31}, {30, 30}, {31, 31}, {30, 30}, {31, 31}};

bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isPlalindrome(DateTime dateTime)
{
    if (dateTime.year % 10 > 1)
        return false;
    int yu, yd, yh, yt;
    yu = dateTime.year % 10;
    yd = dateTime.year / 10 % 10;
    yh = dateTime.year / 100 % 10;
    yt = dateTime.year / 1000;
    int plaMonth = yu * 10 + yd,
        plaDay = yh * 10 + yt;
    if (plaDay > monthDay[plaMonth][isLeapYear(dateTime.year)])
        return false;
    if (plaDay == dateTime.day && plaMonth == dateTime.month)
        return true;
}

bool isSpecialPlalin(DateTime dateTime)
{
    int yu, yd, yh, yt;
    yu = dateTime.year % 10;
    yd = dateTime.year / 10 % 10;
    yh = dateTime.year / 100 % 10;
    yt = dateTime.year / 1000;
    if (yt == yd && yh == yu && isPlalindrome(dateTime))
        return true;
    else
        return false;
}

void addOneDay(DateTime &dateTime)
{
    if (dateTime.day == monthDay[dateTime.month][isLeapYear(dateTime.year)])
    {
        if (dateTime.month == 12)
        {
            dateTime.month = 1;
            ++dateTime.year;
        }
        else
            ++dateTime.month;
        dateTime.day = 1;
    }
    else
        dateTime.day++;
}

int main()
{
    DateTime dateTime;
    int time, year, month, day;
    cin >> time;
    year = time / 10000;
    day = time % 100;
    month = time / 100 % 100;
    dateTime.day = day;
    dateTime.month = month;
    dateTime.year = year;

    int flag1 = 0, flag2 = 0;
    while (true)
    {
        addOneDay(dateTime);
        if (flag1 == 0 && isPlalindrome(dateTime))
        {
            cout << dateTime.year * 10000 + dateTime.month * 100 + dateTime.day << endl;
            flag1 = 1;
        }
        if (flag2 == 0 && isSpecialPlalin(dateTime))
        {
            cout << dateTime.year * 10000 + dateTime.month * 100 + dateTime.day << endl;
            flag2 = 1;
        }
        if (flag1 == 1 && flag2 == 1)
            break;
    }
    return 0;
}
