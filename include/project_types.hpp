#include <boost/asio.hpp>

using sys_err = boost::system::system_error;
using ksys_err = const boost::system::system_error;

using err_code = boost::system::error_code;

// BA means Boost Asio
using socketBA = boost::asio::ip::tcp::socket;
// BA means Boost Asio
using shPtrSocketBA = std::shared_ptr<socketBA>;

using endpoint = boost::asio::ip::tcp::endpoint;
using io_service = boost::asio::io_service;
using acceptor = boost::asio::ip::tcp::acceptor;

using atomic_flag = std::atomic_flag;
using jthread = std::jthread;
