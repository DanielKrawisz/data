// Copyright (c) 2019 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_HEX
#define DATA_ENCODING_HEX

#include <data/types.hpp>
#include <data/encoding/hex.hpp>
#include <data/encoding/invalid.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>

namespace data::encoding::hex {
            const std::string format = "hex";
            
            bool valid(const std::string&);
            std::string write(const bytes&);
            template<unsigned long size>
            std::string write(const std::array<data::byte, size>& a) {
                std::string output;
                boost::algorithm::hex(a.begin(), a.end(),std::back_inserter(output));
                return data::string(output);
            }
            bytes read(const std::string&);
            
            class string : public std::string {
                bytes Bytes;
                bytes *ToBytes;

            public:
                operator bytes() const {
                    if (ToBytes == nullptr) throw;
                    return *ToBytes;
                }

                bool valid() const {
                    return ToBytes != nullptr;
                }

                string(std::string);

                string(std::string *sourceString) : string(*sourceString) {};

                string(const string &s) :
                        std::string{static_cast<const std::string &>(s)},
                        Bytes{s.Bytes},
                        ToBytes{s.ToBytes == nullptr ? nullptr : &Bytes} {}

                string(string &&s) :
                        std::string{static_cast<std::string &&>(s)},
                        Bytes{s.Bytes},
                        ToBytes{s.ToBytes == nullptr ? nullptr : &Bytes} {}

                string(const bytes &input):
                        ToBytes{new std::vector<byte>(input.begin(),input.end())},
                        std::string{data::encoding::hex::write(input)} {}

                template<unsigned long s>
                string(const std::array<byte, s>& input):
                        ToBytes{new std::vector<byte>(input.begin(),input.end())},
                        std::string{data::encoding::hex::write(input)} {}
                ~string();

                string &operator=(string &);
            };
        }

#endif
