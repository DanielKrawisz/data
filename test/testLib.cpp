//
// Created by nekosune on 04/07/19.
//
#include <data/crypto/sha256.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <c++/6/cstring>

std::vector<uint8_t> convertString(const char* temp) {
    std::vector<uint8_t> ret;
    if(strlen(temp)==0)
        return ret;
    int i=0;
    uint8_t chr=temp[i];

    while (chr!=0)
    {
        ret.push_back(temp[i]);
        i++;
        chr=temp[i];
    }
}

int main(int argc, char *argv[]){
    data::sha256::digest temp=data::sha256::hash(convertString("I am a Poppy"));

    std::stringstream ss;
    for(int i=0; i<temp.size(); ++i)
        ss << std::hex << (int)temp.data()[i];
    std::string mystr = ss.str();
    std::cout << "Test String is: " << "I am a Poppy" << std::endl;
    std::cout << "SHA256 of test string is : " << mystr.c_str() << std::endl;
}
