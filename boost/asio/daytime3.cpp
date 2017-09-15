/* daytime3.cpp -- Boost/ASIO Tutorial
 * Asynchronous TCP daytime server
 */
#include <iostream>
#include <string>
#include <ctime>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
using boost::asio::ip::tcp;


// create a string to be sent back to the client
std::string make_daytime_string()
{
    using namespace std;    // for time_t and ctime
    time_t now = time(0);
    return ctime(&now);
}


class tcp_server;
class tcp_connection;


// Use shared_ptr and enable_shared_from_this because we want to keep the
// tcp_connection object alive as long as there is an operation that refers to it
class tcp_connection : public std::enable_shared_from_this<tcp_connection>
{

public:
    typedef std::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_service& io_service)
    {
        return std::make_shared<tcp_connection>(io_service);
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        // Keep the data in a member variable to make sure it's valid
        // until the asynchronous operation is complete
        message_ = make_daytime_string();

        boost::asio::async_write(socket_, boost::asio::buffer(message_),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
    
    tcp_connection(boost::asio::io_service& io_service)
        : socket_(io_service)
        { }
private:
    
    void handle_write(const boost::system::error_code& /*error*/
                     ,std::size_t /*bytes_transferred*/)
    {
    }

    tcp::socket socket_;
    std::string message_;
};

class tcp_server
{
public:
    // initialize an acceptor to listen on TCP port 13
    tcp_server(boost::asio::io_service& io_service)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), 13))
        {
            start_accept();
        }

private:
    void start_accept()
    {   
        // Create a socket
        tcp_connection::pointer new_connection =
            tcp_connection::create(acceptor_.get_io_service());

        // initiate asynchronous accept operation to wait for a new connection
        acceptor_.async_accept( new_connection->socket(),
            boost::bind(&tcp_server::handle_accept, this, new_connection,
                boost::asio::placeholders::error) );
        std::cerr << "awaiting connection..." << std::endl;
    }

    // Called when the asynchronous accept operation initiated
    // by start_accept() finishes.
    void handle_accept(tcp_connection::pointer new_connection,
                       const boost::system::error_code& error)
    {
        // Service the client request
        if( !error ){
            new_connection->start();
            std::cerr << "new connection started" << std::endl;
        }
        // initiate next accept operation
        start_accept();
    }

    tcp::acceptor acceptor_;
};


int main()
{
    try{
        // Create a server object to accept incoming client connections.
        // The asio::io_service object provides I/O serviecs, such as sockets
        // that the server object will use.
        boost::asio::io_service io_service;
        tcp_server server(io_service);

        // run() the io_service object so that it will perform asynchronous 
        // operations on our behalf
        io_service.run();
    }
    catch( std::exception& e ){
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
