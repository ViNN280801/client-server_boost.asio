#pragma once

#include <boost/asio.hpp>
#include <syncstream>
#include <iostream>
#include <concepts>

template <typename T>
concept String = std::is_convertible_v<T, std::string_view>;

static const String auto RAW_IP_ADDR{"127.0.0.1"};
static constexpr std::unsigned_integral auto PORT_NUM{8888U};
