// main.cpp
#include <iostream>
#include <boost/process.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "message_info_constants.hpp"
#include "message.hpp"
#include "data.hpp"
#include "helpers.hpp"
int main() {
    my_message::Message message {"data_queue",sizeof(std::size_t) * 1488};
    std::string serialized = helpers::get_serialized<my_message::Message>(message);
    std::size_t serialized_size = helpers::get_serialized_size<my_message::Message>(message);
    try { 
        boost::interprocess::message_queue::remove(message_info_constants::MIQ_NAME);
        boost::interprocess::message_queue mq(
            boost::interprocess::create_only,
            message_info_constants::MIQ_NAME,
            message_info_constants::MAX_NUMBER_OF_MESSAGES,
            message_info_constants::MAX_BUFFER_SIZE
        );
        // send data
        mq.send(
            serialized.c_str(),
            serialized_size,
            0
        );
        std::cout << "(main) message sent \n";
        // open the client
        // std::system("client.exe");
        boost::process::child client("client.exe");
        if (client.running()) std::cout << "(main) child started\n";
        client.wait();
        std::cout << "(main) childs is done \n";
        // retrieve data
        my_data::Data data = helpers::retrieve_data<my_data::Data>(message);
        std::cout << "(main) retrieve data\n" 
                  << data.tostring();
    } catch (const boost::interprocess::interprocess_exception &ex) {
        std::cerr << "Error in main: " << ex.what() << "\n";
    }
    // do we need to close the one that is owned by CLIENT???
    boost::interprocess::message_queue::remove(message.queue_name.c_str());
    boost::interprocess::message_queue::remove(message_info_constants::MIQ_NAME);
    return 0;
}