// message_info_constants.hpp
#pragma once
#include <cstddef>
namespace message_info_constants {
const char *MIQ_NAME{"message_info_queue"};
const std::size_t MAX_NUMBER_OF_MESSAGES = 10;
const std::size_t MAX_BUFFER_SIZE = sizeof(std::size_t) * 1024;
}