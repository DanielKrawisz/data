#include <data/crypto/pp/CryptoPP.hpp>
#include <cryptopp/sha.cpp>

namespace data {
    
    namespace crypto { 
        
        namespace pp {
        
            std::string SHA256::AlgorithmProvider() const
            {
                return CryptoPP::SHA256_AlgorithmProvider();
            }

            void SHA256::Transform(CryptoPP::word32 *state, const CryptoPP::word32 *data)
            {
                CRYPTOPP_ASSERT(state);
                CRYPTOPP_ASSERT(data);

            #if CRYPTOPP_SHANI_AVAILABLE
                if (CryptoPP::HasSHA())
                {
                    SHA256_HashMultipleBlocks_SHANI(state, data, SHA256::BLOCKSIZE, CryptoPP::LITTLE_ENDIAN_ORDER);
                    return;
                }
            #endif
            #if CRYPTOPP_ARM_SHA2_AVAILABLE
                if (HasSHA2())
                {
                    SHA256_HashMultipleBlocks_ARMV8(state, data, SHA256::BLOCKSIZE, LITTLE_ENDIAN_ORDER);
                    return;
                }
            #endif
            #if CRYPTOPP_POWER8_SHA_AVAILABLE
                if (HasSHA256())
                {
                    SHA256_HashMultipleBlocks_POWER8(state, data, SHA256::BLOCKSIZE, LITTLE_ENDIAN_ORDER);
                    return;
                }
            #endif

                CryptoPP::SHA256_HashBlock_CXX(state, data);
            }

            size_t SHA256::HashMultipleBlocks(const CryptoPP::word32 *input, size_t length)
            {
                CRYPTOPP_ASSERT(input);
                CRYPTOPP_ASSERT(length >= SHA256::BLOCKSIZE);

            #if CRYPTOPP_SHANI_AVAILABLE
                if (CryptoPP::HasSHA())
                {
                    SHA256_HashMultipleBlocks_SHANI(m_state, input, length, CryptoPP::BIG_ENDIAN_ORDER);
                    return length & (SHA256::BLOCKSIZE - 1);
                }
            #endif
            #if CRYPTOPP_SSE2_ASM_AVAILABLE || CRYPTOPP_X64_MASM_AVAILABLE
                if (CryptoPP::HasSSE2())
                {
                    const size_t res = length & (SHA256::BLOCKSIZE - 1);
                    CryptoPP::SHA256_HashMultipleBlocks_SSE2(m_state, input, length-res);
                    return res;
                }
            #endif
            #if CRYPTOPP_ARM_SHA2_AVAILABLE
                if (HasSHA2())
                {
                    SHA256_HashMultipleBlocks_ARMV8(m_state, input, length, BIG_ENDIAN_ORDER);
                    return length & (SHA256::BLOCKSIZE - 1);
                }
            #endif
            #if CRYPTOPP_POWER8_SHA_AVAILABLE
                if (HasSHA256())
                {
                    SHA256_HashMultipleBlocks_POWER8(m_state, input, length, BIG_ENDIAN_ORDER);
                    return length & (SHA256::BLOCKSIZE - 1);
                }
            #endif

                const bool noReverse = NativeByteOrderIs(this->GetByteOrder());
                CryptoPP::word32 *dataBuf = this->DataBuf();
                do
                {
                    if (noReverse)
                    {
                        CryptoPP::SHA256_HashBlock_CXX(m_state, input);
                    }
                    else
                    {
                        ByteReverse(dataBuf, input, SHA256::BLOCKSIZE);
                        CryptoPP::SHA256_HashBlock_CXX(m_state, dataBuf);
                    }

                    input += SHA256::BLOCKSIZE/sizeof(CryptoPP::word32);
                    length -= SHA256::BLOCKSIZE;
                }
                while (length >= SHA256::BLOCKSIZE);
                return length;
            }
            
        }
        
    }
    
}
