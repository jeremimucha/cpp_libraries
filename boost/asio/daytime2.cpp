/* daytime2.cpp -- Boost/ASIO Tutorial
 * A synchronous daytime server
 */
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;


// create a string to be sent back to the client
std::string make_daytime_string()
{
    using namespace std;    // for time_t and ctime
    time_t now = time(0);
    return ctime(&now);
}


int main()
{
    try
    {
        boost::asio::io_service io_service;

        // ip::tcp::acceptor object needs to be created to listen for new connections
        // It is initialized to listen on TCP port 13, IPv4
        tcp::acceptor acceptor( io_service, tcp::endpoint(tcp::v4(), 13) );

        // This is an iterative server -- it will handle one connection at a time
        for(;;){
            // Create a socket that will represent the connection to the client
            tcp::socket socket(io_service);
            // and then wait for a connection
            acceptor.accept(socket);

            // A Client is accessing our service - determine the current time
            // and transfer this information to the client
            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }
    catch( std::exception& e ){
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

