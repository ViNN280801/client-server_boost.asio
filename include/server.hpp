#pragma once

#include <thread>
#include <atomic>
#include <memory>

#include "ServerSettings.hpp"

class Service
{
public:
    explicit Service() {}

    inline const std::string asio_streambuf_to_str(const boost::asio::streambuf &buf) const noexcept
    {
        return std::string(boost::asio::buffers_begin(buf.data()),
                           boost::asio::buffers_begin(buf.data()) + buf.size());
    }

    void handleClient(boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
    void startHandlingClient(boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
};

class Acceptor
{
private:
    boost::asio::io_service &m_ios;
    boost::asio::ip::tcp::acceptor m_acceptor;

public:
    template <std::unsigned_integral T>
    explicit Acceptor(boost::asio::io_service &ios, const T &port)
        : m_ios(ios),
          m_acceptor(m_ios,
                     boost::asio::ip::tcp::endpoint(
                         boost::asio::ip::address_v4::any(),
                         port))
    {
        m_acceptor.listen();
    }

    void accept() noexcept;
};

class Server
{
private:
    boost::asio::io_service m_ios;
    std::unique_ptr<std::thread> m_th;
    std::atomic_flag m_stop;

protected:
    template <std::unsigned_integral T>
    void run(const T &port) noexcept
    {
        Acceptor acc(m_ios, port);
        while (not m_stop.test())
        {
            acc.accept();
        }
    }

public:
    explicit Server() : m_stop(false) {}

    template <std::unsigned_integral T>
    inline void start(const T &port) noexcept
    {
        m_th.reset(new std::thread([this, port]()
                                   { run(port); }));
    }

    void stop() noexcept;
};