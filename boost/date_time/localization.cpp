/* Creation of a facet for the output of dates in Polish */
#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <algorithm>
#include <iterator>

/* Define a series of char arrays for short and long name strings
 * to be associated with Polish date output.
 * US names will be retrieved from the locale).
 */
const char* const pl_short_month_names[] =
{
    "Sty", "Lut", "Mar", "Kwi", "Maj" "Cze", "Lip",
    "Sie", "Wrz", "Paz", "Lis", "Gru", "NAM"
};

const char* const pl_long_month_names[] =
{
    "Styczen", "Luty", "Marzec", "Kwiecien", "Maj", "Czerwiec",
    "Lipiec", "Sierpien", "Wrzesien", "Pazdziernik", "Listopad", "Grudzien"
};

const char* const pl_long_weekday_names[] =
{
    "Niedziela", "Poniedzialek", "Wtorek", "Sroda", "Czwartek",
    "Piatek", "Sobota"
};

const char* const pl_short_weekday_names[] =
{
    "Nie", "Pon", "Wt", "Sr", "Czw", "Pi", "So"
};


int main()
{
    using namespace boost::gregorian;

    // create some gregorian objects to output
    date d1(2002, Oct, 1);
    greg_month m = d1.month();
    greg_weekday wd = d1.day_of_week();

    // create a facet and locale for Polish dates
    date_facet* pl_facet = new date_facet();
    std::cout.imbue(std::locale(std::locale::classic(), pl_facet));

    // create the Polish name collections
    date_facet::input_collection_type short_months, long_months,
                                      short_weekdays, long_weekdays;
    std::copy(std::begin(pl_short_month_names), std::end(pl_short_month_names),
              std::back_inserter(short_months));
    std::copy(std::begin(pl_long_month_names), std::end(pl_long_month_names),
              std::back_inserter(long_months));
    std::copy(std::begin(pl_short_weekday_names), std::end(pl_short_weekday_names),
              std::back_inserter(short_weekdays));
    std::copy(std::begin(pl_long_weekday_names), std::end(pl_long_weekday_names),
              std::back_inserter(long_weekdays));

    // replace the default names with ours
    // NOTE: date_generators and special_values were not replaced as
    // they are not used in this example
    pl_facet->short_month_names(short_months);
    pl_facet->long_month_names(long_months);
    pl_facet->short_weekday_names(short_weekdays);
    pl_facet->long_weekday_names(long_weekdays);

    // output the date in Polish using short months names
    pl_facet->format("%d.%m.%Y");
    std::cout << d1 << std::endl;   // 01.10.2002
    pl_facet->format("%d %B %Y");
    std::cout << d1 << std::endl;

    pl_facet->month_format("%B");
    std::cout << m << std::endl;
    
    pl_facet->weekday_format("%A");
    std::cout << wd << std::endl;

    // go back to US names
    date_facet* us_facet = new date_facet();
    std::cout.imbue(std::locale(std::locale::classic(), us_facet));

    us_facet->format("%m/%d/%Y");
    std::cout << d1 << std::endl;
    us_facet->format("%m %b %Y");
    std::cout << d1 << std::endl;
}
