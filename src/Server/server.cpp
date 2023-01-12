#include "../../include/server.hpp"

void Service::handleClient(boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
{
    try
    {
        while (true)
        {
            boost::asio::streambuf request;
            boost::asio::read_until(*sock.get(), request, '\n');
            StringExt strExt;
            std::string str{strExt.asio_streambuf_to_str(request)};

            // Skipping the idle message
            if (str.at(0UL) not_eq '\n')
            {
                strExt = str;

                // Deleting endline symbol -> '\n' and symbol that specifies
                // that string is ending -> '\0' - nil terminator
                strExt.removeChars();
                strExt.removeChars('\0');

                std::osyncstream(std::cout) << "Response to previous clients message: ";
                (strExt.is_2moreDigits() and strExt.is_32multiple())
                    ? std::osyncstream(std::cout) << "\033[1;32mData recieved successfully!\033[0m\033[1m\n"
                    : std::osyncstream(std::cout) << "\033[1;31mError! Data is incorrect\033[0m\033[1m\n";
            }

            boost::asio::write(*sock.get(), boost::asio::buffer(str));
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
