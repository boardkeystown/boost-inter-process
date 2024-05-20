// message.hpp
#pragma once
#include <string>
#include <sstream>
#include <boost/serialization/vector.hpp>
namespace my_message {
struct Message {
    std::string queue_name  {""};
    std::size_t buffer_size {0};
    std::string to_string() {
        std::stringstream ss;
        ss << "queue_name =" << this->queue_name << "\n";
        ss << "buffer_size =" << this->buffer_size << "\n";
        return ss.str();
    }
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        (void)version; // Suppress unused parameter warning
        ar & queue_name;
        ar & buffer_size;
    }
};
} // namespace my_message
