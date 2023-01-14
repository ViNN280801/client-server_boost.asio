#pragma once

#include <mutex>
#include "ServerSettings.hpp"

class ClientErrorHandler
{
private:
    std::atomic_flag m_isServerReachable;

public:
    explicit ClientErrorHandler() {}

    int errHandle(const sys_err &) const;

    int handleSocketCreation(shPtrSocketBA, endpoint &);
    int handleSocketConnection(shPtrSocketBA, endpoint &);

    int handleWritingMessage(socketBA &sock, const String auto &msg)
    {
        try
        {
            boost::asio::write(sock, boost::asio::buffer(msg));
        }
        catch (const sys_err &e)
        {
            std::cerr << "Error writing message to socket. Client::handleWritingMessage()" << std::endl;
            return errHandle(e);
        }
        return 0;
    }

    int handleSocketClosure(shPtrSocketBA);

    virtual ~ClientErrorHandler() {}
};

class Client
{
private:
    io_service m_ios;
    endpoint m_ep;
    shPtrSocketBA m_sock;

    ClientErrorHandler errorHandler;

    std::string m_clientMsg;
    std::timed_mutex m_mutex;

public:
    explicit Client()
        : m_ep(boost::asio::ip::address::from_string(DEFAULT_IP_ADDRESS_V4), DEFAULT_PORT_NUMBER),
          m_sock(new socketBA(m_ios))
    {
        if (errorHandler.handleSocketCreation(m_sock, m_ep))
            exit(EXIT_FAILURE);
    }

    template <std::unsigned_integral T = uint>
    explicit Client(const String auto &rawIP, const T &port)
        : m_ep(boost::asio::ip::address::from_string(rawIP), port), m_sock(new socketBA(m_ios))
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
