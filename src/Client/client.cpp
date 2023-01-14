#include "../../include/client.hpp"

int ClientErrorHandler::errHandle(ksys_err &e) const
{
    std::cerr << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
    return e.code().value();
}

int ClientErrorHandler::handleSocketCreation(shPtrSocketBA sock,
                                             endpoint &ep)
{
    try
    {
        sock->open(ep.protocol());
    }
    catch (ksys_err &e)
    {
        std::cerr << "Error creation socket. Client::handleSocketCreation()" << std::endl;
        return errHandle(e);
    }
    return 0;
}

int ClientErrorHandler::handleSocketConnection(shPtrSocketBA sock,
                                               endpoint &ep)
{
    while (true)
    {
        m_isServerReachable.test_and_set();
        while (m_isServerReachable.test())
        {
            // back:
            err_code err;
            sock->connect(ep, err);

            if (err)
            {
                std::cout << "Server is unreacheble now. Trying to reconnect...\n";
                std::this_thread::sleep_for(std::chrono::seconds(3));
                m_isServerReachable.clear();
                break;
            }
            else
            {
                m_isServerReachable.test_and_set();
                break;
            }
        }
        if (m_isServerReachable.test())
            break;
    }
    return 0;
}

int ClientErrorHandler::handleSocketClosure(shPtrSocketBA sock)
{
    try
    {
        sock->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        sock->close();
        sock.reset();
    }
    catch (ksys_err &e)
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
    boost::asio::read_until(*m_sock.get(), buf, "\n");

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

    // Idle msg (because server is waiting that both of threads will send msg)
    sendMessage("\n");

    m_mutex.unlock();
}

void Client::processingSharedBuffer()
{
    m_mutex.lock();

    // Get shared buffer
    StringExt str{m_clientMsg};

    // Clear shared buffer
    m_clientMsg.clear();

    std::osyncstream(std::cout) << "Data from shared buffer: " << str;

    std::string sum{std::to_string(str.sumOfDigits())};
    // Calculating sum of digits and then send it to server side
    sendMessage(sum);

    m_mutex.unlock();
}
