/* timer5.cpp -- Boost/ASIO Tutorial
 * Synchronising handlers in multithreaded programs by using asio::strand
 */
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


class printer
{
public:
    printer( boost::asio::io_service& io )
        : strand_(io)   // use asio::strand to synchronise the threads
        , timer1_(io, boost::posix_time::seconds(1))
        , timer2_(io, boost::posix_time::seconds(2))
        , count_(0)
        {
            // wrap the asynchronous operations in an asio::strand object
            // this ensures that those handlers can not execute concurrently
            timer1_.async_wait( strand_.wrap(boost::bind(&printer::print1, this)) );
            timer2_.async_wait( strand_.wrap(boost::bind(&printer::print2, this)) );
        }

    ~printer() noexcept
        { std::cout << "Funal count is " << count_ << std::endl; }

    // In a multithreaded environment handlers for asynch operations should be
    // synchronized if they access shared resources.
    // Here shared resources are std::cout and count_
    void print1()
    {
        if( count_ < 10 ){
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;

            timer1_.expires_at( timer1_.expires_at() + boost::posix_time::seconds(1) );
            timer1_.async_wait( strand_.wrap(boost::bind(&printer::print1, this)) );
        }
    }

    void print2()
    {
        if( count_ < 10 ){
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;

            timer2_.expires_at( timer2_.expires_at() + boost::posix_time::seconds(2) );
            timer2_.async_wait( strand_.wrap(boost::bind(&printer::print2,this)) );
        }
    }

private:
    boost::asio::io_service::strand strand_;
    boost::asio::deadline_timer timer1_;
    boost::asio::deadline_timer timer2_;
    int count_;
};


class async_printer
{
public:
    async_printer( boost::asio::io_service& io )
        : timer1_(io, boost::posix_time::seconds(1))
        , timer2_(io, boost::posix_time::seconds(1))
        , count_(0)
        {
            // wrap the asynchronous operations in an asio::strand object
            // this ensures that those handlers can not execute concurrently
            timer1_.async_wait( boost::bind(&async_printer::print1, this) );
            timer2_.async_wait( boost::bind(&async_printer::print2, this) );
        }

    ~async_printer() noexcept
        { std::cout << "Funal count is " << count_ << std::endl; }

    // In a multithreaded environment handlers for asynch operations should be
    // synchronized if they access shared resources.
    // Here shared resources are std::cout and count_
    void print1()
    {
        if( count_ < 10 ){
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;

            timer1_.expires_at( timer1_.expires_at() + boost::posix_time::seconds(1) );
            timer1_.async_wait( boost::bind(&async_printer::print1, this) );
        }
    }

    void print2()
    {
        if( count_ < 10 ){
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;

            timer2_.expires_at( timer2_.expires_at() + boost::posix_time::seconds(1) );
            timer2_.async_wait( boost::bind(&async_printer::print2,this) );
        }
    }

private:
    boost::asio::deadline_timer timer1_;
    boost::asio::deadline_timer timer2_;
    int count_;
};

int main()
{
    boost::asio::io_service io;
    printer p(io);
    // Call io_service::run() from another thread by using boost::thread object
    // io_service::run() calls will continue while there is still "work" to do
    // the background thread won't exit until all asynchronous operations
    // have completed
    boost::thread t( boost::bind(&boost::asio::io_service::run, &io) );
    // Call io_service::run() from this thread
    io.run();
    t.join();

    return 0;
}
