#pragma once

#include <mutex>
#include "ServerSettings.hpp"

class ClientErrorHandler
{
public:
    explicit ClientErrorHandler() {}

    int errHandle(const boost::system::system_error &) const;

    int handleSocketCreation(std::shared_ptr<boost::asio::ip::tcp::socket>, boost::asio::ip::tcp::endpoint &);
    int handleSocketConnection(std::shared_ptr<boost::asio::ip::tcp::socket>, boost::asio::ip::tcp::endpoint &);

    int handleWritingMessage(boost::asio::ip::tcp::socket &sock, const String auto &msg)
    {
        try
        {
            boost::asio::write(sock, boost::asio::buffer(msg));
        }
        catch (const boost::system::system_error &e)
        {
            std::cerr << "Error writing message to socket. Client::handleWritingMessage()" << std::endl;
            return errHandle(e);
        }
        return 0;
    }

    int handleSocketClosure(std::shared_ptr<boost::asio::ip::tcp::socket>);

    virtual ~ClientErrorHandler() {}
};

class Client
{
private:
    boost::asio::io_service m_ios;
    boost::asio::ip::tcp::endpoint m_ep;
    std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;

    ClientErrorHandler errorHandler;

    std::string m_clientMsg;
    std::timed_mutex m_mutex;

public:
    explicit Client()
        : m_ep(boost::asio::ip::address::from_string(DEFAULT_IP_ADDRESS_V4), DEFAULT_PORT_NUMBER),
          m_sock(new boost::asio::ip::tcp::socket(m_ios))
    {
        if (errorHandler.handleSocketCreation(m_sock, m_ep))
            exit(EXIT_FAILURE);
    }

    template <std::unsigned_integral T = uint>
    explicit Client(const String auto &rawIP, const T &port)
        : m_ep(boost::asio::ip::address::from_string(rawIP), port), m_sock(new boost::asio::ip::tcp::socket(m_ios))
    {
        if (errorHandler.handleSocketCreation(m_sock, m_ep))
            exit(EXIT_FAILURE);
    }

    inline void Connect()
    {
        if (errorHandler.handleSocketConnection(m_sock, m_ep))
            exit(EXIT_FAILURE);
    }

    inline void Close()
    {
        if (errorHandler.handleSocketClosure(m_sock))
            exit(EXIT_FAILURE);
    }

    inline void sendMessage(const String auto &msg)
    {
        if (errorHandler.handleWritingMessage(*m_sock.get(), msg))
            exit(EXIT_FAILURE);
    }

    const std::string receiveMessage() noexcept;

    void processingClientMessage();
    void processingSharedBuffer();
};
