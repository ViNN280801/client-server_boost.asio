#pragma once

#include <atomic>
#include "ServerSettings.hpp"

class Server
{
private:
    boost::asio::io_service m_ios;
    std::unique_ptr<std::jthread> m_jth;
    std::atomic_flag m_stop;
    std::atomic_flag m_isClientConnected;

protected:
    template <std::unsigned_integral T = uint>
    void handleClient(const T &port) noexcept
    {
        boost::asio::ip::tcp::acceptor acc(m_ios, boost::asio::ip::tcp::endpoint(
                                                      boost::asio::ip::address_v4::any(),
                                                      port));
        acc.listen();

        while (not m_stop.test())
        {
            while (true)
            {
                m_isClientConnected.test_and_set();
                boost::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(m_ios));
                acc.accept(*sock.get());
                while (m_isClientConnected.test())
                {
                    boost::asio::streambuf request;
                    boost::system::error_code err;
                    boost::asio::read_until(*sock.get(), request, '\n', err);

                    if (err)
                    {
                        std::cout << "Client disconnected or any error occurred on a client side. "
                                  << "Waiting for the next connection establishment..."
                                  << std::endl;
                        m_isClientConnected.clear();
                        break;
                    }

                    StringExt strExt;
                    std::string str{strExt.asio_streambuf_to_str(request)};

                    // Skipping the idle message
                    if (str.at(0UL) not_eq '\n')
                    {
                        strExt = str;

                        // Deleting endline symbol -> '\n' and symbol that specifies
                        // that string is ending -> '\0' - nil terminator
                        strExt.removeChar();
                        strExt.removeChar('\0');

                        std::osyncstream(std::cout) << "Response to previous clients message: ";
                        (strExt.is_2moreDigits() and strExt.is_32multiple())
                            ? std::osyncstream(std::cout) << "\033[1;32mData recieved successfully!\033[0m\033[1m\n"
                            : std::osyncstream(std::cout) << "\033[1;31mError! Data is incorrect\033[0m\033[1m\n";
                    }

                    boost::asio::write(*sock.get(), boost::asio::buffer(str));
                }
            }
            // Clean-up
            delete this;
        }
    }

public:
    explicit Server() : m_stop(false) {}

    template <std::unsigned_integral T = uint>
    constexpr void start(const T &port)
    {
        m_jth.reset(new std::jthread([this, port]()
                                     { handleClient(port); }));
    }

    inline void stop() noexcept
    {
        m_stop.test_and_set();
    }
};
