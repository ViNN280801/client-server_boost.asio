#include <syncstream>
#include <iostream>
#include <thread>
#include <memory>
#include <atomic>

#include "StringExt.hpp"
#include "project_types.hpp"

static const std::string DEFAULT_IP_ADDRESS_V4{"127.0.0.1"};
static constexpr uint DEFAULT_PORT_NUMBER{8888U};
