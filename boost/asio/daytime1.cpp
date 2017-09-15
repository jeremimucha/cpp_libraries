/* daytime1.cpp -- Boost/ASIO Tutorial
 * a synchronous TCP daytime client
 */
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;


int main( int argc, char* argv[] )
{
    // The purpose of this application is to access a daytime service
    // so we need the user to specify the server
    try{
        if( argc != 2 ){
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }

        // boost::asio programs need at least one asio::io_service object
        boost::asio::io_service io_service;

        // Turn the server name that was specified as a parameter to the application
        // into a TCP endpoint.
        // we use an ip::tcp::resolver object for this purpose
        tcp::resolver resolver( io_service );
        
        // tcp::resolver takes a tck::resolver::query object
        // and turns it into a list of endpoints
        // Construct a query using the name of the server - specified in argv[1]
        // and the name of the service - here "daytime"
        tcp::resolver::query query( argv[1], "daytime" );

        // The list of endpoints is returned as an tcp::resolver::iterator
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        // create and connect to a socket
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        // Connection is open. We need to read the response from the daytime service
        // boost::asio::buffer automatically determines the size of the array
        // to help prevent buffer overruns.
        for(;;){
            boost::array<char,128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some( boost::asio::buffer(buf), error );

            // when the server closes the connection the ip::tcp::socket::read_some
            // will exit with boost::asio::error::eof - a sign to end the loop
            if( error == boost::asio::error::eof )
                break;  // Connection closed cleanly by peer
            else if( error )
                throw boost::system::system_error(error);

            std::cout.write(buf.data(), len);
        }
    }
    catch( std::exception& e ){
        std::cerr << e.what() << std::endl;
    }
}
