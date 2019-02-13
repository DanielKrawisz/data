#ifndef DATA_CRYPTO_CRYPTOPP_HPP
#define DATA_CRYPTO_CRYPTOPP_HPP

#include <cryptopp/drbg.h>
#include <data/types.hpp>
#include <data/crypto/sha256.hpp>

namespace data {
        
    namespace crypto {
        
        namespace pp {
            
            // modified from cryptopp sha.hp. It is now possible to initialize the
            // object from an intermediate state. This means that there isn't really
            // a point in using secure memory but we can save the state of the
            // random number generator. 

            /// \brief Fixed size stack-based SecBlock
            /// \tparam T class or type
            /// \tparam A AllocatorBase derived class for allocation and cleanup
            template <class T, class A>
            class IterableSecBlock : public CryptoPP::SecBlock<T, A>
            {
            public:
                /// \brief Construct a FixedSizeSecBlock
                explicit IterableSecBlock(const T *ptr, unsigned int s) : CryptoPP::SecBlock<T, A>(ptr, s) {}
                
                T* begin() {
                    return CryptoPP::SecBlock<T, A>::m_ptr;
                }
                
                T* end() {
                    return CryptoPP::SecBlock<T, A>::m_ptr + CryptoPP::SecBlock<T, A>::m_size * sizeof(T);
                }
            };

            /// \brief Fixed size stack-based SecBlock
            /// \tparam T class or type
            /// \tparam S fixed-size of the stack-based memory block, in elements
            /// \tparam A AllocatorBase derived class for allocation and cleanup
            template <class T, unsigned int S, class A = CryptoPP::FixedSizeAllocatorWithCleanup<T, S> >
            class FixedSizeSecBlock : public IterableSecBlock<T, A>
            {
            public:
                /// \brief Construct a FixedSizeSecBlock
                explicit FixedSizeSecBlock() : IterableSecBlock<T, A>(S) {}
                explicit FixedSizeSecBlock(const T *ptr) : IterableSecBlock<T, A>(ptr, S) {}
            };

            /// \brief Fixed size stack-based SecBlock with 16-byte alignment
            /// \tparam T class or type
            /// \tparam S fixed-size of the stack-based memory block, in elements
            /// \tparam T_Align16 boolean that determines whether allocations should be aligned on a 16-byte boundary
            template <class T, unsigned int S, bool T_Align16 = true>
            class FixedSizeAlignedSecBlock : public FixedSizeSecBlock<T, S, CryptoPP::FixedSizeAllocatorWithCleanup<T, S, CryptoPP::NullAllocator<T>, T_Align16> >
            {
            public:
                explicit FixedSizeAlignedSecBlock() {}
                explicit FixedSizeAlignedSecBlock(const T *ptr) : FixedSizeSecBlock<T, S, CryptoPP::FixedSizeAllocatorWithCleanup<T, S, CryptoPP::NullAllocator<T>, T_Align16> >(ptr) {}
            };
            
            /// \brief Iterated hash with a static transformation function
            /// \tparam T_HashWordType Hash word type
            /// \tparam T_Endianness Endianness type of hash
            /// \tparam T_BlockSize Block size of the hash
            /// \tparam T_StateSize Internal state size of the hash
            /// \tparam T_Transform HashTransformation derived class
            /// \tparam T_DigestSize Digest size of the hash
            /// \tparam T_StateAligned Flag indicating if state is 16-byte aligned
            /// \sa HashTransformation, MessageAuthenticationCode
            template <class T_HashWordType, class T_Endianness, unsigned int T_BlockSize, unsigned int T_StateSize, class T_Transform, unsigned int T_DigestSize = 0, bool T_StateAligned = false>
            class CRYPTOPP_NO_VTABLE IteratedHashWithStaticTransform
                : public CryptoPP::ClonableImpl<T_Transform, CryptoPP::AlgorithmImpl<CryptoPP::IteratedHash<T_HashWordType, T_Endianness, T_BlockSize>, T_Transform> >
            {
            public:
                CRYPTOPP_CONSTANT(DIGESTSIZE = T_DigestSize ? T_DigestSize : T_StateSize)

                virtual ~IteratedHashWithStaticTransform() {}

                /// \brief Provides the digest size of the hash
                /// \return the digest size of the hash, in bytes
                /// \details DigestSize() returns <tt>DIGESTSIZE</tt>.
                unsigned int DigestSize() const {return DIGESTSIZE;}
                
            T_HashWordType* begin() {
                return CryptoPP::SecBlock<T, A>::m_ptr;
            }
                
            T_HashWordType* end() {
                return CryptoPP::SecBlock<T, A>::m_ptr + CryptoPP::SecBlock<T, A>::m_size * sizeof(T);
            }

            protected:
                IteratedHashWithStaticTransform() {this->Init();}
                IteratedHashWithStaticTransform(const T_HashWordType *data) : m_state{data, T_BlockSize/sizeof(T_HashWordType)} {}
                void HashEndianCorrectedBlock(const T_HashWordType *data) {T_Transform::Transform(this->m_state, data);}
                void Init() {T_Transform::InitState(this->m_state);}

                T_HashWordType* StateBuf() {return this->m_state;}
                FixedSizeAlignedSecBlock<T_HashWordType, T_BlockSize/sizeof(T_HashWordType), T_StateAligned> m_state;
            };
                
            /// \brief SHA-256 message digest
            class CRYPTOPP_DLL SHA256 : public IteratedHashWithStaticTransform<CryptoPP::word32, CryptoPP::BigEndian, 64, 32, SHA256, 32, true>
            {
            public:
                /// \brief Initialize state array
                /// \param state the state of the hash
                /// \details InitState sets a state array to SHA256 initial values
                /// \details Hashes which derive from IteratedHashWithStaticTransform provide static
                ///   member functions InitState and Transform. External classes, like SEAL and MDC,
                ///   can initialize state with a user provided key and operate the hash on the data
                ///   with the user supplied state.
                /// \note On Intel platforms the state array must be 16-byte aligned for SSE2.
                static void CRYPTOPP_API InitState(HashWordType *state);
                /// \brief Operate the hash
                /// \param digest the state of the hash
                /// \param data the data to be digested
                /// \details Transform operates the hash on <tt>data</tt>. When the call is invoked
                ///   <tt>digest</tt> holds initial state. Upon return <tt>digest</tt> holds the hash
                ///   or updated state.
                /// \details Hashes which derive from IteratedHashWithStaticTransform provide static
                ///   member functions InitState and Transform. External classes, like SEAL and MDC,
                ///   can initialize state with a user provided key and operate the hash on the data
                ///   with the user supplied state.
                /// \note On Intel platforms the state array and data must be 16-byte aligned for SSE2.
                static void CRYPTOPP_API Transform(HashWordType *digest, const HashWordType *data);
                /// \brief The algorithm name
                /// \returns C-style string "SHA-256"
                CRYPTOPP_STATIC_CONSTEXPR const char* CRYPTOPP_API StaticAlgorithmName() {return "SHA-256";}

                // Algorithm class
                std::string AlgorithmProvider() const;
                
                sha256::digest GetState() {
                    sha256::digest a;
                    std::copy(begin(), end(), a.begin());
                    return a;
                }
                    
                SHA256(sha256::digest& d) : IteratedHashWithStaticTransform<CryptoPP::word32, CryptoPP::BigEndian, 64, 32, SHA256, 32, true>(reinterpret_cast<uint32_t*>(d.data())) {}
                SHA256() {}

            protected:
                size_t HashMultipleBlocks(const HashWordType *input, size_t length);
            };

            /// \tparam STRENGTH security strength, in bytes
            /// \brief Hash_DRBG from SP 800-90A Rev 1 (June 2015)
            /// \details The NIST Hash DRBG is instantiated with a number of parameters. Two of the parameters,
            ///   Security Strength and Seed Length, depend on the hash and are specified as template parameters.
            ///   The remaining parameters are included in the class. The parameters and their values are listed
            ///   in NIST SP 800-90A Rev. 1, Table 2: Definitions for Hash-Based DRBG Mechanisms (p.38).
            /// \details Some parameters have been reduce to fit C++ datatypes. For example, NIST allows upto
            ///   2<sup>48</sup> requests before a reseed. However, Hash_DRBG limits it to <tt>INT_MAX</tt> due
            ///   to the limited data range of an int.
            /// \sa <A HREF="http://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-90Ar1.pdf">Recommendation
            ///   for Random Number Generation Using Deterministic Random Bit Generators, Rev 1 (June 2015)</A>
            /// \since Crypto++ 6.0
            template <typename HASH=SHA256, unsigned int STRENGTH=128/8, unsigned int SEEDLENGTH=440/8>
            class Hash_DRBG : public CryptoPP::NIST_DRBG
            {
            public:
                
                CRYPTOPP_CONSTANT(SECURITY_STRENGTH=STRENGTH)
                CRYPTOPP_CONSTANT(SEED_LENGTH=SEEDLENGTH)
                CRYPTOPP_CONSTANT(MINIMUM_ENTROPY=STRENGTH)
                CRYPTOPP_CONSTANT(MINIMUM_NONCE=0)
                CRYPTOPP_CONSTANT(MINIMUM_ADDITIONAL=0)
                CRYPTOPP_CONSTANT(MINIMUM_PERSONALIZATION=0)
                CRYPTOPP_CONSTANT(MAXIMUM_ENTROPY=INT_MAX)
                CRYPTOPP_CONSTANT(MAXIMUM_NONCE=INT_MAX)
                CRYPTOPP_CONSTANT(MAXIMUM_ADDITIONAL=INT_MAX)
                CRYPTOPP_CONSTANT(MAXIMUM_PERSONALIZATION=INT_MAX)
                CRYPTOPP_CONSTANT(MAXIMUM_BYTES_PER_REQUEST=65536)
                CRYPTOPP_CONSTANT(MAXIMUM_REQUESTS_BEFORE_RESEED=INT_MAX)

                static std::string StaticAlgorithmName() { return std::string("Hash_DRBG(") + HASH::StaticAlgorithmName() + std::string(")"); }

                /// \brief Construct a Hash DRBG
                /// \param entropy the entropy to instantiate the generator
                /// \param entropyLength the size of the entropy buffer
                /// \param nonce additional input to instantiate the generator
                /// \param nonceLength the size of the nonce buffer
                /// \param personalization additional input to instantiate the generator
                /// \param personalizationLength the size of the personalization buffer
                /// \throws NIST_DRBG::Err if the generator is instantiated with insufficient entropy
                /// \details All NIST DRBGs must be instaniated with at least <tt>MINIMUM_ENTROPY</tt> bytes of entropy.
                ///   The byte array for <tt>entropy</tt> must meet <A HREF ="http://csrc.nist.gov/publications/PubsSPs.html">NIST
                ///   SP 800-90B or SP 800-90C</A> requirements.
                /// \details The <tt>nonce</tt> and <tt>personalization</tt> are optional byte arrays. If <tt>nonce</tt> is supplied,
                ///   then it should be at least <tt>MINIMUM_NONCE</tt> bytes of entropy.
                /// \details An example of instantiating a SHA256 generator is shown below.
                ///   The example provides more entropy than required for SHA256. The <tt>NonblockingRng</tt> meets the
                ///   requirements of <A HREF ="http://csrc.nist.gov/publications/PubsSPs.html">NIST SP 800-90B or SP 800-90C</A>.
                ///   RDRAND() and RDSEED() generators would work as well.
                /// <pre>
                ///    SecByteBlock entropy(48), result(128);
                ///    NonblockingRng prng;
                ///    RandomNumberSource rns(prng, entropy.size(), new ArraySink(entropy, entropy.size()));
                ///
                ///    Hash_DRBG<SHA256, 128/8, 440/8> drbg(entropy, 32, entropy+32, 16);
                ///    drbg.GenerateBlock(result, result.size());
                /// </pre>
                Hash_DRBG(const byte* entropy=NULLPTR, size_t entropyLength=STRENGTH, const byte* nonce=NULLPTR,
                    size_t nonceLength=0, const byte* personalization=NULLPTR, size_t personalizationLength=0)
                    : CryptoPP::NIST_DRBG(), m_c(SEEDLENGTH), m_v(SEEDLENGTH), m_reseed(0)
                {
                    if (entropy != NULLPTR && entropyLength != 0)
                        DRBG_Instantiate(entropy, entropyLength, nonce, nonceLength, personalization, personalizationLength);
                }
                
                Hash_DRBG(HASH hash, std::array<byte, 2*SEEDLENGTH> seed, CryptoPP::word64 reseed) : m_hash{hash}, m_c{seed[0], SEEDLENGTH}, m_v{seed[SEEDLENGTH], SEEDLENGTH}, m_reseed{reseed} {}

                unsigned int SecurityStrength() const {return SECURITY_STRENGTH;}
                unsigned int SeedLength() const {return SEED_LENGTH;}
                unsigned int MinEntropyLength() const {return MINIMUM_ENTROPY;}
                unsigned int MaxEntropyLength() const {return MAXIMUM_ENTROPY;}
                unsigned int MinNonceLength() const {return MINIMUM_NONCE;}
                unsigned int MaxNonceLength() const {return MAXIMUM_NONCE;}
                unsigned int MaxBytesPerRequest() const {return MAXIMUM_BYTES_PER_REQUEST;}
                unsigned int MaxRequestBeforeReseed() const {return MAXIMUM_REQUESTS_BEFORE_RESEED;}

                void IncorporateEntropy(const byte *input, size_t length)
                    {return DRBG_Reseed(input, length, NULLPTR, 0);}

                void IncorporateEntropy(const byte *entropy, size_t entropyLength, const byte* additional, size_t additionaLength)
                    {return DRBG_Reseed(entropy, entropyLength, additional, additionaLength);}

                void GenerateBlock(byte *output, size_t size)
                    {return Hash_Generate(NULLPTR, 0, output, size);}

                void GenerateBlock(const byte* additional, size_t additionaLength, byte *output, size_t size)
                    {return Hash_Generate(additional, additionaLength, output, size);}

                std::string AlgorithmProvider() const
                    {/*Hack*/HASH hash; return hash.AlgorithmProvider();}

            protected:
                // 10.1.1.2 Instantiation of Hash_DRBG (p.39)
                void DRBG_Instantiate(const byte* entropy, size_t entropyLength, const byte* nonce, size_t nonceLength,
                    const byte* personalization, size_t personalizationLength);

                // 10.1.1.3 Reseeding a Hash_DRBG Instantiation (p.40)
                void DRBG_Reseed(const byte* entropy, size_t entropyLength, const byte* additional, size_t additionaLength);

                // 10.1.1.4 Generating Pseudorandom Bits Using Hash_DRBG (p.41)
                void Hash_Generate(const byte* additional, size_t additionaLength, byte *output, size_t size);

                // 10.3.1 Derivation Function Using a Hash Function (Hash_df) (p.49)
                void Hash_Update(const byte* input1, size_t inlen1, const byte* input2, size_t inlen2,
                    const byte* input3, size_t inlen3, const byte* input4, size_t inlen4, byte* output, size_t outlen);

                HASH m_hash;
                CryptoPP::SecByteBlock m_c, m_v, m_temp;
                CryptoPP::word64 m_reseed;
            };
            
            // 10.1.1.2 Instantiation of Hash_DRBG (p.39)
            template <typename HASH, unsigned int STRENGTH, unsigned int SEEDLENGTH>
            void Hash_DRBG<HASH, STRENGTH, SEEDLENGTH>::DRBG_Instantiate(const byte* entropy, size_t entropyLength, const byte* nonce, size_t nonceLength,
                const byte* personalization, size_t personalizationLength)
            {
                //  SP 800-90A, 8.6.3: The entropy input shall have entropy that is equal to or greater than the security
                //  strength of the instantiation. Additional entropy may be provided in the nonce or the optional
                //  personalization string during instantiation, or in the additional input during reseeding and generation,
                //  but this is not required and does not increase the "official" security strength of the DRBG
                //  instantiation that is recorded in the internal state.
                CRYPTOPP_ASSERT(entropyLength >= MINIMUM_ENTROPY);
                if (entropyLength < MINIMUM_ENTROPY)
                    throw NIST_DRBG::Err("Hash_DRBG", "Insufficient entropy during instantiate");

                // SP 800-90A, Section 9, says we should throw if we have too much entropy, too large a nonce,
                // or too large a persoanlization string. We warn in Debug builds, but do nothing in Release builds.
                CRYPTOPP_ASSERT(entropyLength <= MAXIMUM_ENTROPY);
                CRYPTOPP_ASSERT(nonceLength <= MAXIMUM_NONCE);
                CRYPTOPP_ASSERT(personalizationLength <= MAXIMUM_PERSONALIZATION);

                const byte zero = 0;
                CryptoPP::SecByteBlock t1(SEEDLENGTH), t2(SEEDLENGTH);
                Hash_Update(entropy, entropyLength, nonce, nonceLength, personalization, personalizationLength, NULLPTR, 0, t1, t1.size());
                Hash_Update(&zero, 1, t1, t1.size(), NULLPTR, 0, NULLPTR, 0, t2, t2.size());

                m_v.swap(t1); m_c.swap(t2);
                m_reseed = 1;
            }

            // 10.1.1.3 Reseeding a Hash_DRBG Instantiation (p.40)
            template <typename HASH, unsigned int STRENGTH, unsigned int SEEDLENGTH>
            void Hash_DRBG<HASH, STRENGTH, SEEDLENGTH>::DRBG_Reseed(const byte* entropy, size_t entropyLength, const byte* additional, size_t additionaLength)
            {
                //  SP 800-90A, 8.6.3: The entropy input shall have entropy that is equal to or greater than the security
                //  strength of the instantiation. Additional entropy may be provided in the nonce or the optional
                //  personalization string during instantiation, or in the additional input during reseeding and generation,
                //  but this is not required and does not increase the "official" security strength of the DRBG
                //  instantiation that is recorded in the internal state..
                CRYPTOPP_ASSERT(entropyLength >= MINIMUM_ENTROPY);
                if (entropyLength < MINIMUM_ENTROPY)
                    throw NIST_DRBG::Err("Hash_DRBG", "Insufficient entropy during reseed");

                // SP 800-90A, Section 9, says we should throw if we have too much entropy, too large a nonce,
                // or too large a persoanlization string. We warn in Debug builds, but do nothing in Release builds.
                CRYPTOPP_ASSERT(entropyLength <= MAXIMUM_ENTROPY);
                CRYPTOPP_ASSERT(additionaLength <= MAXIMUM_ADDITIONAL);

                const byte zero = 0, one = 1;
                CryptoPP::SecByteBlock t1(SEEDLENGTH), t2(SEEDLENGTH);
                Hash_Update(&one, 1, m_v, m_v.size(), entropy, entropyLength, additional, additionaLength, t1, t1.size());
                Hash_Update(&zero, 1, t1, t1.size(), NULLPTR, 0, NULLPTR, 0, t2, t2.size());

                m_v.swap(t1); m_c.swap(t2);
                m_reseed = 1;
            }

            // 10.1.1.4 Generating Pseudorandom Bits Using Hash_DRBG (p.41)
            template <typename HASH, unsigned int STRENGTH, unsigned int SEEDLENGTH>
            void Hash_DRBG<HASH, STRENGTH, SEEDLENGTH>::Hash_Generate(const byte* additional, size_t additionaLength, byte *output, size_t size)
            {
                // Step 1
                if (static_cast<CryptoPP::word64>(m_reseed) >= static_cast<CryptoPP::word64>(MaxRequestBeforeReseed()))
                    throw NIST_DRBG::Err("Hash_DRBG", "Reseed required");

                if (size > MaxBytesPerRequest())
                    throw NIST_DRBG::Err("Hash_DRBG", "Request size exceeds limit");

                // SP 800-90A, Section 9, says we should throw if we have too much entropy, too large a nonce,
                // or too large a persoanlization string. We warn in Debug builds, but do nothing in Release builds.
                CRYPTOPP_ASSERT(additionaLength <= MAXIMUM_ADDITIONAL);

                // Step 2
                if (additional && additionaLength)
                {
                    const byte two = 2;
                    m_temp.New(HASH::DIGESTSIZE);

                    m_hash.Update(&two, 1);
                    m_hash.Update(m_v, m_v.size());
                    m_hash.Update(additional, additionaLength);
                    m_hash.Final(m_temp);

                    CRYPTOPP_ASSERT(SEEDLENGTH >= HASH::DIGESTSIZE);
                    int carry=0, j=HASH::DIGESTSIZE-1, i=SEEDLENGTH-1;
                    while (j>=0)
                    {
                        carry = m_v[i] + m_temp[j] + carry;
                        m_v[i] = static_cast<byte>(carry);
                        i--; j--; carry >>= 8;
                    }
                    while (i>=0)
                    {
                        carry = m_v[i] + carry;
                        m_v[i] = static_cast<byte>(carry);
                        i--; carry >>= 8;
                    }
                }

                // Step 3
                {
                    m_temp.Assign(m_v);
                    while (size)
                    {
                        m_hash.Update(m_temp, m_temp.size());
                        size_t count = CryptoPP::STDMIN(size, (size_t)HASH::DIGESTSIZE);
                        m_hash.TruncatedFinal(output, count);

                        IncrementCounterByOne(m_temp, static_cast<unsigned int>(m_temp.size()));
                        size -= count; output += count;
                    }
                }

                // Steps 4-7
                {
                    const byte three = 3;
                    m_temp.New(HASH::DIGESTSIZE);

                    m_hash.Update(&three, 1);
                    m_hash.Update(m_v, m_v.size());
                    m_hash.Final(m_temp);

                    CRYPTOPP_ASSERT(SEEDLENGTH >= HASH::DIGESTSIZE);
                    CRYPTOPP_ASSERT(HASH::DIGESTSIZE >= sizeof(m_reseed));
                    int carry=0, k=sizeof(m_reseed)-1, j=HASH::DIGESTSIZE-1, i=SEEDLENGTH-1;

                    while (k>=0)
                    {
                        carry = m_v[i] + m_c[i] + m_temp[j] + CryptoPP::GetByte<CryptoPP::word64>(CryptoPP::BIG_ENDIAN_ORDER, m_reseed, k) + carry;
                        m_v[i] = static_cast<byte>(carry);
                        i--; j--; k--; carry >>= 8;
                    }

                    while (j>=0)
                    {
                        carry = m_v[i] + m_c[i] + m_temp[j] + carry;
                        m_v[i] = static_cast<byte>(carry);
                        i--; j--; carry >>= 8;
                    }

                    while (i>=0)
                    {
                        carry = m_v[i] + m_c[i] + carry;
                        m_v[i] = static_cast<byte>(carry);
                        i--; carry >>= 8;
                    }
                }

                m_reseed++;
            }

            // 10.3.1 Derivation Function Using a Hash Function (Hash_df) (p.49)
            template <typename HASH, unsigned int STRENGTH, unsigned int SEEDLENGTH>
            void Hash_DRBG<HASH, STRENGTH, SEEDLENGTH>::Hash_Update(const byte* input1, size_t inlen1, const byte* input2, size_t inlen2,
                const byte* input3, size_t inlen3, const byte* input4, size_t inlen4, byte* output, size_t outlen)
            {
                byte counter = 1;
                CryptoPP::word32 bits = ConditionalByteReverse(CryptoPP::BIG_ENDIAN_ORDER, static_cast<CryptoPP::word32>(outlen*8));

                while (outlen)
                {
                    m_hash.Update(&counter, 1);
                    m_hash.Update(reinterpret_cast<const byte*>(&bits), 4);

                    if (input1 && inlen1)
                        m_hash.Update(input1, inlen1);
                    if (input2 && inlen2)
                        m_hash.Update(input2, inlen2);
                    if (input3 && inlen3)
                        m_hash.Update(input3, inlen3);
                    if (input4 && inlen4)
                        m_hash.Update(input4, inlen4);

                    size_t count = CryptoPP::STDMIN(outlen, (size_t)HASH::DIGESTSIZE);
                    m_hash.TruncatedFinal(output, count);

                    output += count; outlen -= count;
                    counter++;
                }
            }
            
        }
            
    }

}

#endif
