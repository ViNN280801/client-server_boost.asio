#include "../../include/server.hpp"

void Service::handleClient(boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
{
    try
    {
        while (true)
        {
            boost::asio::streambuf request;
            boost::asio::read_until(*sock.get(), request, '\n');
            std::string str{asio_streambuf_to_str(request)};

            // Skipping the idle message
            if (str.at(0UL) not_eq '\n')
                std::osyncstream(std::cout) << str;

            std::string response{asio_streambuf_to_str(request)};
            boost::asio::write(*sock.get(), boost::asio::buffer(response));
        }
    }
    catch (const boost::system::system_error &e)
    {
        std::cerr << "Error while handling client message. Service::handleClient()" << std::endl;
        std::cerr << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Clean-up
    delete this;
}

void Acceptor::accept() noexcept
{
    boost::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(m_ios));
    m_acceptor.accept(*sock.get());
    (new Service)->startHandlingClient(sock);
}
