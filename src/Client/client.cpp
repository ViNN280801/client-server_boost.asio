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
    m_mutex.lock();

    boost::asio::streambuf buf;
    boost::asio::read_until(m_sock, buf, "\n");

    m_mutex.unlock();

    return std::string(boost::asio::buffers_begin(buf.data()),
                       boost::asio::buffers_begin(buf.data()) + buf.size());
}

void Client::processingClientMessage()
{
    m_mutex.lock();

    std::string msg("");
    StringExt tmp;
    while (true)
    {
        std::osyncstream(std::cout) << "Enter the message: ";
        std::getline(std::cin, msg);

        if ((msg == "quit") or (msg == "q") or (msg == "exit"))
        {
            this->Close();
            exit(EXIT_SUCCESS);
        }

        tmp = msg;

        // If message contains only digits and doesn't exceed 64 symbols
        if (tmp.isDigits() and tmp.isLength_lteq_64())
            break;
        else
            std::osyncstream(std::cout) << "You entered wrong string. Try again.\n";
    }

    // After checking message will be sorted by descending order and all
    // elements which value is even, replacing with "KB" letters
    tmp.sortDescending();
    tmp.replaceEvenDigitsTo_KB();

    msg = tmp.getStr();

    msg += '\n';
    m_clientMsg = msg;

    m_mutex.unlock();
}

const std::string Client::processingSharedBuffer()
{
    m_mutex.lock();

    // Get shared buffer
    StringExt str{m_clientMsg};

    // Clear shared buffer
    m_clientMsg.clear();

    std::osyncstream(std::cout) << "Data from shared buffer: " << str;

    m_mutex.unlock();

    // Calculating sum of digits and then send it to server side
    sendMessage<std::string>(std::to_string(str.sumOfDigits<uint>()));
    return "this->receiveMessage()";
}
