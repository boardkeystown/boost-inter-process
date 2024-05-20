// data.hpp
#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <boost/serialization/vector.hpp>
namespace my_data {
struct Data {
    int i_value;
    float f_value;
    double d_value;
    std::vector<int> i_vec;
    Data() : 
        i_value(0),
        f_value(0.f),
        d_value(0.0),
        i_vec({})
    {}
    Data(
        int i_value,
        float f_value,
        double d_value,
        std::vector<int> i_vec
    ) : 
        i_value(i_value),
        f_value(f_value),
        d_value(d_value),
        i_vec(i_vec)
    {}
    ~Data() {}
    std::string tostring() {
        std::stringstream ss;
        ss << "i_value =" << this->i_value << "\n";
        ss << "f_value =" << this->f_value << "\n";
        ss << "f_value =" << this->d_value << "\n";
        ss << "f_value = { ";
        if (this->i_vec.size())
            ss << this->i_vec.at(1);
        for (std::size_t i = 1; i < this->i_vec.size(); ++i) {
            ss << ", " << this->i_vec.at(i);
        }
        ss << "}\n";
        return ss.str();
    }
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & i_value;
        ar & f_value;
        ar & d_value;
        ar & i_vec;
    }
};
}