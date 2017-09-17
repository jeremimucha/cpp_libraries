// example 7b -- networking asynchronously
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using boost::asio::ip::tcp;

boost::mutex global_stream_lock;



void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
              << "] Thread Start" << std::endl;
    global_stream_lock.unlock();

    while( true ){
    try{
        boost::system::error_code ec;
        io_service->run( ec );
    
        if( ec ){
            global_stream_lock.lock();
            std::cout << "[" << boost::this_thread::get_id()
                    << "] Exception: " << ec << std::endl;
            global_stream_lock.unlock();
        }

        break;  // break out of loop if no exceptions occured
    }
    catch( std::exception& e ){
        global_stream_lock.lock();
        std::cout << "[" << boost::this_thread::get_id()
                  << "] Exception: " << e.what()
                  << std::endl;
        global_stream_lock.unlock();
    }
    } // while(true)

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
              << "] Thread Finish" << std::endl;
    global_stream_lock.unlock();
}


void OnConnect( const boost::system::error_code& ec,
                boost::shared_ptr< tcp::socket > sock )
{
    if( ec ){
        global_stream_lock.lock();
        std::cout << "[" << boost::this_thread::get_id()
                  << "] Error: " << ec << std::endl;
        global_stream_lock.unlock();
    }
    else{
        global_stream_lock.lock();
        std::cout << "[" << boost::this_thread::get_id()
                  << "] Connected!" << std::endl;
        global_stream_lock.unlock();
    }
}


int main()
{
    boost::shared_ptr< boost::asio::io_service > io_service(
        new boost::asio::io_service
    );
    boost::shared_ptr< boost::asio::io_service::work > work(
        new boost::asio::io_service::work( *io_service )
    );
    boost::shared_ptr< boost::asio::io_service::strand > strand(
        new boost::asio::io_service::strand( *io_service )
    );

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
               << "] Press [return] to exit."
               << std::endl;
    global_stream_lock.unlock();

    boost::thread_group worker_threads;
    for( int x=0; x<2; ++x ){
        worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
    }

    boost::shared_ptr< tcp::socket > sock( new tcp::socket( *io_service ) );

    try{
        tcp::resolver resolver( *io_service );
        tcp::resolver::query query( "www.google.com",
                                    boost::lexical_cast< std::string >( 80 ) );
        tcp::resolver::iterator iterator = resolver.resolve( query );
        tcp::endpoint endpoint = *iterator;

        global_stream_lock.lock();
        std::cout << "Connecting to: " << endpoint << std::endl;
        global_stream_lock.unlock();

        sock->async_connect( endpoint, boost::bind( OnConnect,
                                            boost::asio::placeholders::error,
                                            sock ) 
                           );
    }
    catch( std::exception& e ){
        global_stream_lock.lock();
        std::cout << "[" << boost::this_thread::get_id()
                  << "] Exception: " << e.what() << std::endl;
        global_stream_lock.unlock();
    }
    
    std::cin.get();

    boost::system::error_code ec;
    sock->shutdown( tcp::socket::shutdown_both, ec );
    sock->close( ec );

    io_service->stop();

    worker_threads.join_all();

    return 0;
}