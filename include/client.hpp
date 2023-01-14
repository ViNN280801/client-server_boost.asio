#pragma once

#include <mutex>
#include "ServerSettings.hpp"

class ClientErrorHandler
{
private:
    atomic_flag m_isServerReachable;

public:
    explicit ClientErrorHandler() {}

    int errHandle(ksys_err &) const;

    int handleSocketCreation(shPtrSocketBA, endpoint &);
    int handleSocketConnection(shPtrSocketBA, endpoint &);
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
    atomic_flag m_isServerReachable;

protected:
    void reconnect(shPtrSocketBA, io_service &, endpoint &);

public:
    explicit Client()
        : m_ep(boost::asio::ip::address::from_string(DEFAULT_IP_ADDRESS_V4), DEFAULT_PORT_NUMBER),
          m_sock(new socketBA(m_ios))
    {
        if (errorHandler.handleSocketCreation(m_sock, m_ep))
            exit(EXIT_FAILURE);
    }

    template <std::unsigned_integral T = unsigned int>
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

    void sendMessage(const String auto &msg)
    {
        while (true)
        {
            m_isServerReachable.test_and_set();
            while (m_isServerReachable.test())
            {
                err_code err;
                boost::asio::write(*m_sock.get(), boost::asio::buffer(msg), err);

                if (err)
                {
                    std::osyncstream(std::cout) << "Something went wrong. Trying to reconnect...\n";
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

            // Following code designed for recconnection
            m_sock.reset(new socketBA(m_ios));
            errorHandler.handleSocketCreation(m_sock, m_ep);
            errorHandler.handleSocketConnection(m_sock, m_ep);
        }
    }

    const std::string receiveMessage() noexcept;

    void processingClientMessage();
    void processingSharedBuffer();
};
