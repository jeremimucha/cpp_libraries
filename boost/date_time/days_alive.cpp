/* Short example that calculates the number of days since user was born.
 * Demonstrates comparisons of durations, use of the day_clock,
 * and parsing a date from a string.
 */
#include <iostream>
#include <string>
#include "boost/date_time/gregorian/gregorian.hpp"
using std::cout;
using std::endl;


int main()
{
    using namespace boost::gregorian;
    std::string s;
    cout << "Enter birth day YYYY-MM-DD (eg: 2002-02-01): ";
    std::cin >> s;
    try{
        date birthday(from_simple_string(s));
        date today = day_clock::local_day();
        days days_alive = today - birthday;
        days one_day(1);
        if(days_alive == one_day){
            cout << "Born yesterday, very funny" << endl;
        }
        else if(days_alive < days(0)){
            cout << "Not born yet, hmm: " << days_alive.days()
                 << " days" << endl;
        }
        else{
            cout << "Days alive: " << days_alive.days() << endl;
        }
    }
    catch(...){
        cout << "Bad date entered: " << s << std::endl;
    }
    return 0;
}
