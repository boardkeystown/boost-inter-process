// client.cpp
#include <iostream>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "message_info_constants.hpp"
#include "message.hpp"
#include "helpers.hpp"
#include "data.hpp"

int main() {
my_message::Message message;
 try {
        boost::interprocess::message_queue mq(
            boost::interprocess::open_only,
            message_info_constants::MIQ_NAME
        );
        char buffer[message_info_constants::MAX_BUFFER_SIZE];
        std::size_t received_size = 0;
        unsigned int priority = 0;
        mq.receive(
            buffer,
            message_info_constants::MAX_BUFFER_SIZE,
            received_size,
            priority
        );
        message = helpers::get_deserialized<my_message::Message>(
            buffer,
            received_size
        );
        // message received 
        std::cout << "(client) message received \n" 
                  << message.to_string()
                  << "\n";
        // send data
        my_data::Data data(0,0,0,{1,2,3,4,5});

        boost::interprocess::message_queue mq_sender(
            boost::interprocess::create_only,
            message.queue_name.c_str(),
            message_info_constants::MAX_NUMBER_OF_MESSAGES,
            message.buffer_size
        );
        // send data
        std::string serialized = helpers::get_serialized<my_data::Data>(data);
        std::size_t serialized_size = helpers::get_serialized_size<my_data::Data>(data);
        mq_sender.send(
            serialized.c_str(),
            serialized_size,
            0
        );

    } catch (const boost::interprocess::interprocess_exception &ex) {
        std::cerr << "(client.exe) Error: " << ex.what() << "\n";
    }
    // boost::interprocess::message_queue::remove(message.queue_name.c_str());
    // do we need to close the one that is owned by main???
    boost::interprocess::message_queue::remove(message_info_constants::MIQ_NAME);
    return 0;
}
