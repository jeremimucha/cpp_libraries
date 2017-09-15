/* timer3.cpp -- Boost/ASIO Tutorial
 * Repeating timer which calls back once a second.
 */
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
// #include <functional>
#include <boost/date_time/posix_time/posix_time.hpp>


void print( const boost::system::error_code& /*e*/
          , boost::asio::deadline_timer* t  // so that we can change the timer
          , int* count)                      // so that we can stop the timer after n calls
{
    if( *count < 5 ){
        std::cout << *count << std::endl;
        ++*count;
    
    // Move the expiry time for the timer along by one second from the previous
    // expiry time by calculating the new expiry time relative to the old.
    t->expires_at(t->expires_at() + boost::posix_time::seconds(1));

    // Start a new asynchronous wait on the timer.
    t->async_wait( boost::bind(print, boost::asio::placeholders::error, t, count)
                 );
    }
}



int main()
{
    boost::asio::io_service io;
    int count{0};
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));

    // The deadline_timer::async_wait() function expects a handler function
    // with the signature void(const boost::system::error_code&)
    // we can use bind() to pass additional parameters to the callback
    t.async_wait( boost::bind(print,
                            boost::asio::placeholders::error, &t, &count) );
    io.run();

    std::cout << "Final count is " << count << std::endl;
}
