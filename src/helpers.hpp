// helpers.hpp
#pragma once
#include <sstream>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "message.hpp"
namespace helpers {
template <class T>
std::string get_serialized(const T &o) {
    std::ostringstream oss;
    boost::archive::text_oarchive oa(oss);
    oa << o;
    return oss.str();
}
template <class T>
std::size_t get_serialized_size(const T &o) {
    return sizeof(std::size_t) * (get_serialized<T>(o).size() + 1); // + 1 for null terminator
}
template <class T>
T get_deserialized(const char *buffer, const std::size_t received_size) {
    std::string response_str(buffer,received_size);
    std::istringstream iss(response_str);
    boost::archive::text_iarchive ia(iss);
    T receive_data;
    ia >> receive_data;
    return receive_data;
}
template <class T>
T retrieve_data(const my_message::Message &message, bool close_after_read=false) {
    boost::interprocess::message_queue mq(
        boost::interprocess::open_only,
        message.queue_name.c_str()
    );
    char buffer[message.buffer_size];
    std::size_t received_size = 0;
    unsigned int priority = 0;
    mq.receive(
        buffer,
        message.buffer_size,
        received_size,
        priority
    );
    T data = helpers::get_deserialized<T>(
        buffer,
        received_size
    );
    if (close_after_read) 
        boost::interprocess::message_queue::remove(message.queue_name.c_str());
    return data;
}

} // namespace helpers