//
// Created by nekosune on 04/07/19.
//
#include <data/crypto/sha256.hpp>
#include <data/encoding/hex.hpp>
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
    std::cout << "Test array is: " << "0,99,234,23,45,99,128,140" << std::endl;
    std::cout << "Test array as hex string: " << "0063EA172D63808C" << std::endl;
    data::encoding::hex::string hexString(std::string("0063EA172D63808C"));

    std::stringstream ss2;
    for(int i=0;i< static_cast<data::bytes>(hexString).size();i++)
    {
        ss2 << std::dec << (int)static_cast<data::bytes>(hexString)[i] << ", ";
    }
    std::string myArray = ss2.str();
    std::cout << "Hex output is : " << myArray.c_str() << std::endl;
    std::cout << "Test Exception handling " << std::endl;
    std::cout << "Test array as malformed hex string: " << "0063EA172D63808" << std::endl;
    try {
        data::encoding::hex::string malformedHexString(std::string("0063EA172D63808"));
    }
    catch (data::encoding::hex::invalid inv) {
        const char * exceptionError=inv.what();
        std::cout << "Exception: " <<  *exceptionError << std::endl;
    }


}
