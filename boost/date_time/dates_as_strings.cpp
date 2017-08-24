/* The following is a simple example that shows conversion of dates 
 * to and from a std::string.
 * 
 * Expected output:
 * 2001-Oct-09
 * 2001-10-09
 * Tuesday October 9, 2001
 * An expected exception is next: 
 * Exception: Month number is out of range 1..12
 */
#include <iostream>
#include "boost/date_time/gregorian/gregorian.hpp"
#include <string>


int main()
{
    using namespace boost::gregorian;

    try{
        std::string s("2001-10-9");
        date d(from_simple_string(s));
        std::cout << to_simple_string(d) << std::endl;

        std::string ud("20011009");
        date d1(from_undelimited_string(ud));
        std::cout << to_iso_extended_string(d1) << std::endl;

        // output the parts of the date - Tuesday October 9, 2001
        date::ymd_type ymd = d1.year_month_day();
        greg_weekday wd = d1.day_of_week();
        std::cout << wd.as_long_string() << " "
                  << ymd.month.as_long_string() << " "
                  << ymd.day << ", " << ymd.year
                  << std::endl;

        // month 25 - will cause an exception
        std::string bad_date("20012509");   // 2001-??-09
        std::cout << "An unexpected exception is next: " << std::endl;
        date wont_construct(from_undelimited_string(bad_date));
        // use won't_construct so compilder doesn't complain
        // but we won't get here
        std::cout << "this should not be reached"
                  << to_iso_string(wont_construct) << std::endl;
    }
    catch(std::exception& e){
        std::cout << "  Exception: " << e.what() << std::endl;
    }

    return 0;
}
