/* timer2.cpp -- Boost/ASIO Tutorial
 * Using a timer asynchronously
 */
#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


// Callback function called when an asynchronous operation completes.
void print(const boost::system::error_code& /*e*/)
{
    std::cout << "Hello, world!" << std::endl;
}


int main()
{
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));

    // call deadline_timer::async_wait() - perform asynchronous wait
    t.async_wait(&print);

    // call the io_service::run() member
    // The callback handlers will be called only from threads that are currently
    // calling io_service::run
    io.run();

    return 0;
}
