#pragma once

#include <boost/asio.hpp>
#include <syncstream>
#include <iostream>
#include <concepts>

template <typename T>
concept String = std::is_convertible_v<T, std::string_view>;

static const String auto DEFAULT_IP_ADDRESS_V4{"127.0.0.1"};
static constexpr std::unsigned_integral auto DEFAULT_PORT_NUMBER{8888U};
