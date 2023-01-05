#pragma once

#include <boost/asio.hpp>
#include <syncstream>
#include <iostream>

#include "strmanip.hpp"

static const String auto DEFAULT_IP_ADDRESS_V4{"127.0.0.1"};
static constexpr std::unsigned_integral auto DEFAULT_PORT_NUMBER{8888U};
