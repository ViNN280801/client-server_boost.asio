#include "../../include/client.hpp"

int ClientErrorHandler::errHandle(const boost::system::system_error &e) const
{
    std::cerr << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
    return e.code().value();
}

int ClientErrorHandler::handleSocketCreation(boost::asio::ip::tcp::socket &sock, boost::asio::ip::tcp::endpoint &ep)
{
    try
    {
        sock.open(ep.protocol());
    }
    catch (const boost::system::system_error &e)
    {
        std::cerr << "Error creation socket. Client::handleSocketCreation()" << std::endl;
        return errHandle(e);
    }
    return 0;
}

int ClientErrorHandler::handleSocketConnection(boost::asio::ip::tcp::socket &sock, boost::asio::ip::tcp::endpoint &ep)
{
    try
    {
        sock.connect(ep);
    }
    catch (const boost::system::system_error &e)
    {
        std::cerr << "Error connecting to socket. Client::handleSocketConnection()" << std::endl;
        return errHandle(e);
    }
    return 0;
}

int ClientErrorHandler::handleSocketClosure(boost::asio::ip::tcp::socket &sock)
{
    try
    {
        sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        sock.close();
    }
    catch (const boost::system::system_error &e)
    {
        std::cerr << "Error socket closure. Client::handleSocketClosure()" << std::endl;
        return errHandle(e);
    }
    return 0;
}

const std::string Client::receiveMessage() noexcept
{
    boost::asio::streambuf buf;
    boost::asio::read_until(m_sock, buf, "\n");
    return std::string(boost::asio::buffers_begin(buf.data()),
                       boost::asio::buffers_begin(buf.data()) + buf.size());
}

const std::string Client::processingClientMessage()
{
    std::string msg("");
    std::osyncstream(std::cout) << "Enter the message: ";
    std::getline(std::cin, msg);
    msg += '\n';
    sendMessage(msg);
    return receiveMessage();
}