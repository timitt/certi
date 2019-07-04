#include <RTI/VariableLengthData.h>
#include <RTI/encoding/EncodingConfig.h>
#include <RTI/encoding/EncodingExceptions.h>
#include <vector>
#include <cstring>
#include <iostream>

#include <bitset>

#include "HLAtypesIEEE1516.hh"

#ifdef HOST_IS_BIG_ENDIAN
#define g_isBigEndian true
#else
#define g_isBigEndian false
#endif

#ifdef DEBUG_ENCODE_DECODE
#define g_showingDebugInfo true
#else
#define g_showingDebugInfo false
#endif

enum class Encode { encode_before_swap, encode_after_swap, decode_before_swap, decode_after_swap };
enum class Endian { little, big };



template<typename I>
class Swap
{

public:
    Swap(I &a_intValue, Endian a_endianRepresentation) : _intValue(a_intValue)
    {
        if((g_isBigEndian && a_endianRepresentation == Endian::little)
                || (!g_isBigEndian && a_endianRepresentation == Endian::big))
        {
            if(sizeof(I) == 2)
            {
                swap2Bytes();
            }
            else if(sizeof(I) == 4)
            {
                swap4Bytes();
            }
            else if(sizeof(I) == 8)
            {
                swap8Bytes();
            }
        }
    }
    virtual ~Swap(){}

private:

    void swap2Bytes()
    {
        _intValue = (_intValue << 8 | _intValue >> 8);
    }

    void swap4Bytes()
    {
        _intValue = (_intValue << 24 | _intValue >> 24 | (_intValue & 0x0000ff00UL) << 8 | (_intValue & 0x00ff0000UL) >> 8);
    }

    void swap8Bytes()
    {
        _intValue = (uint64_t(_intValue) << 56 | uint64_t(_intValue) >> 56
                        | (uint64_t(_intValue) & 0x000000000000ff00ULL) << 40
                        | (uint64_t(_intValue) & 0x0000000000ff0000ULL) << 24
                        | (uint64_t(_intValue) & 0x00000000ff000000ULL) << 8
                        | (uint64_t(_intValue) & 0x000000ff00000000ULL) >> 8
                        | (uint64_t(_intValue) & 0x0000ff0000000000ULL) >> 24
                        | (uint64_t(_intValue) & 0x00ff000000000000ULL) >> 40);
    }

private:
    I &_intValue;
};

template<class I, class V=I>
class PrintInfo
{
public:

    PrintInfo(const Encode &a_encode, const I &a_int, const char *a_octets, const uint &a_nbOctets)
        : _intValue(a_int), _value(a_int), _octets(a_octets), _nbOctets(a_nbOctets)
    {
        reshow(a_encode);
    }

    PrintInfo(const Encode &a_encode, const I &a_int, const char *a_octets, const uint &a_nbOctets, const V &a_v)
        : _intValue(a_int), _value(a_v), _octets(a_octets), _nbOctets(a_nbOctets)
    {
        reshow(a_encode);
    }

    void reshow(const Encode &a_encode) const
    {
        if(g_showingDebugInfo)
        {
            if(a_encode == Encode::encode_before_swap)
                std::cout << "-------------------------------------ENCODE BEFORE SWAP--------------------------------------" << std::endl;
            else if(a_encode == Encode::encode_after_swap)
                std::cout << "-------------------------------------ENCODE AFTER SWAP---------------------------------------" << std::endl;
            else if(a_encode == Encode::decode_before_swap)
                std::cout << "-------------------------------------DECODE BEFORE SWAP--------------------------------------" << std::endl;
            else if(a_encode == Encode::decode_after_swap)
                std::cout << "-------------------------------------DECODE AFTER SWAP---------------------------------------" << std::endl;

            std::cout << "value : " << std::dec << _value << std::endl;
            std::cout << "bits : ";
            for(int i=0; i < static_cast<int>(_nbOctets); i++)
                std::cout << std::bitset<8>(_octets[i]) << " ";

            std::cout << std::endl;
        }
    }

private:
    const I &_intValue;
    const V &_value;
    const char *_octets;
    const uint &_nbOctets;
};

typedef PrintInfo<uint16_t, wchar_t> PrintInfo16;
typedef PrintInfo<uint32_t, wchar_t> PrintInfo32;

#define DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(EncodableDataType, SimpleDataType, encoder)                     \
                                                                                                                    \
    class RTI_EXPORT EncodableDataType##Implementation                                                              \
    {                                                                                                               \
    public:                                                                                                         \
        EncodableDataType##Implementation();                                                                        \
        EncodableDataType##Implementation(const SimpleDataType& value);                                             \
        EncodableDataType##Implementation(const EncodableDataType##Implementation& rhs);                            \
        virtual ~EncodableDataType##Implementation();                                                               \
                                                                                                                    \
        EncodableDataType##Implementation& operator=(const EncodableDataType##Implementation& rhs);                 \
        EncodableDataType##Implementation& operator=(const SimpleDataType& data);                                   \
                                                                                                                    \
        void set(const SimpleDataType& inData);                                                                     \
                                                                                                                    \
        const SimpleDataType& get() const;                                                                          \
                                                                                                                    \
        encoder                                                                                                     \
                                                                                                                    \
    private:                                                                                                        \
        template<typename I>                                                                                        \
        size_t decode(std::vector<Octet> const & a_buffer, size_t a_index, Endian a_endianRepresentation            \
            , const uint a_nbOctet = sizeof(_data))                                                                 \
        {                                                                                                           \
            union {                                                                                                 \
                I intValue;                                                                                         \
                SimpleDataType value;                                                                               \
                char charValue[8];                                                                                  \
            } val;                                                                                                  \
                                                                                                                    \
            for(uint i=0; i<a_nbOctet; i++)                                                                         \
            {                                                                                                       \
                val.charValue[i] = a_buffer[a_index + i];                                                           \
            }                                                                                                       \
            typedef PrintInfo<I, SimpleDataType> PrintInfo;                                                         \
            PrintInfo info(Encode::decode_before_swap, val.intValue, val.charValue, a_nbOctet, val.value);          \
            Swap<I>(val.intValue, a_endianRepresentation);                                                          \
            info.reshow(Encode::decode_after_swap);                                                                 \
            _data = val.value;                                                                                      \
                                                                                                                    \
            return a_index + a_nbOctet;                                                                             \
        }                                                                                                           \
                                                                                                                    \
        template<typename I>                                                                                        \
        void encode(std::vector<Octet>& a_buffer, Endian a_endianRepresentation                                     \
                , const uint a_nbOctet = sizeof(_data)) const                                                       \
        {                                                                                                           \
            union {                                                                                                 \
                I intValue;                                                                                         \
                SimpleDataType value;                                                                               \
                char charValue[8];                                                                                  \
            } val;                                                                                                  \
            val.value = _data;                                                                                      \
            typedef PrintInfo<I, SimpleDataType> PrintInfo;                                                         \
            PrintInfo info(Encode::encode_before_swap, val.intValue, val.charValue, a_nbOctet, val.value);          \
            Swap<I>(val.intValue, a_endianRepresentation);                                                          \
            info.reshow(Encode::encode_after_swap);                                                                 \
            for(uint i=0; i < a_nbOctet; i++)                                                                       \
            {                                                                                                       \
                a_buffer.push_back(val.charValue[i]);                                                               \
            }                                                                                                       \
        }                                                                                                           \
                                                                                                                    \
                                                                                                                    \
    private:                                                                                                        \
        SimpleDataType _data;                                                                                       \
                                                                                                                    \
    };                                                                                                              \


namespace rti1516e
{
    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAASCIIchar, char,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        _data = a_buffer[a_index];
        return a_index + 1;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        if(_data < 0 || _data > 127)
        {
            throw EncoderException(L"This is not a Standard ASCII character(see ANSI X3.4-1986).");
        }
        a_buffer.push_back(_data);
    }

    size_t getEncodedLength() const
    {
        return 1;
    }

    unsigned int getOctetBoundary() const
    {
        return 1;
    }

    Integer64 hash() const
    {
        return Integer64(_data);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAboolean, bool,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        ::libhla::HLAdata<::libhla::HLAboolean> value;
        *value = a_buffer[a_index];
        if(*value == ::libhla::HLAtrue)
            _data = true;
        else if(*value == ::libhla::HLAfalse)
            _data = false;
        else
        {
            throw EncoderException(L"This is not a Standard Boolean type.");
        }
        return a_index + 1;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        ::libhla::HLAdata<::libhla::HLAboolean> value;
        if(*value != ::libhla::HLAtrue && *value != ::libhla::HLAfalse)
        {
            throw EncoderException(L"This is not a Standard Boolean type.");
        }
        a_buffer.push_back(_data);
    }

    size_t getEncodedLength() const
    {
        return 1;
    }

    unsigned int getOctetBoundary() const
    {
        return 1;
    }

    Integer64 hash() const
    {
        return Integer64(_data);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAbyte, Octet,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        _data = a_buffer[a_index];
        return a_index + 1;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        if(_data < -128 || _data > 127)
        {
            throw EncoderException(L"This is not a Standard ASCII character(see ANSI X3.4-1986).");
        }
        a_buffer.push_back(_data);
    }

    size_t getEncodedLength() const
    {
        return 1;
    }

    unsigned int getOctetBoundary() const
    {
        return 1;
    }

    Integer64 hash() const
    {
        return Integer64(_data);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAfloat32BE, float,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        return decode<uint32_t>(a_buffer, a_index, Endian::big);
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        encode<uint32_t>(a_buffer, Endian::big);
    }

    size_t getEncodedLength() const
    {
        return 4;
    }

    unsigned int getOctetBoundary() const
    {
        return 4;
    }

    Integer64 hash() const
    {
        return Integer64(_data);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAfloat32LE, float,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        return decode<uint32_t>(a_buffer, a_index, Endian::little);
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        encode<uint32_t>(a_buffer, Endian::little);
    }

    size_t getEncodedLength() const
    {
        return 4;
    }

    unsigned int getOctetBoundary() const
    {
        return 4;
    }

    Integer64 hash() const
    {
        union {
            uint32_t intValue;
            float floatValue;
        } val;
        val.floatValue = _data;
        return Integer64(val.intValue);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAfloat64BE, double,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        return decode<uint64_t>(a_buffer, a_index, Endian::big);
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        encode<uint64_t>(a_buffer, Endian::big);
    }

    size_t getEncodedLength() const
    {
        return 8;
    }

    unsigned int getOctetBoundary() const
    {
        return 8;
    }

    Integer64 hash() const
    {
        union {
            uint64_t intValue;
            double doubleValue;
        } val;
        val.doubleValue = _data;
        return Integer64(val.intValue);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAfloat64LE, double,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        return decode<uint64_t>(a_buffer, a_index, Endian::little);
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        encode<uint64_t>(a_buffer, Endian::little);
    }

    size_t getEncodedLength() const
    {
        return 8;
    }

    unsigned int getOctetBoundary() const
    {
        return 8;
    }

    Integer64 hash() const
    {
        union {
            uint64_t intValue;
            double doubleValue;
        } val;
        val.doubleValue = _data;
        return Integer64(val.intValue);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAinteger16LE, Integer16,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        return decode<uint16_t>(a_buffer, a_index, Endian::little);
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        encode<uint16_t>(a_buffer, Endian::little);
    }

    size_t getEncodedLength() const
    {
        return 2;
    }

    unsigned int getOctetBoundary() const
    {
        return 2;
    }

    Integer64 hash() const
    {
        return Integer64(_data);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAinteger16BE, Integer16,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        return decode<uint16_t>(a_buffer, a_index, Endian::big);
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        encode<uint16_t>(a_buffer, Endian::big);
    }

    size_t getEncodedLength() const
    {
        return 2;
    }

    unsigned int getOctetBoundary() const
    {
        return 2;
    }

    Integer64 hash() const
    {
        return Integer64(_data);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAinteger32LE, Integer32,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        return decode<uint32_t>(a_buffer, a_index, Endian::little);
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        encode<uint32_t>(a_buffer, Endian::little);
    }

    size_t getEncodedLength() const
    {
        return 4;
    }

    unsigned int getOctetBoundary() const
    {
        return 4;
    }

    Integer64 hash() const
    {
        return Integer64(_data);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAinteger32BE, Integer32,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        return decode<uint32_t>(a_buffer, a_index, Endian::big);
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        encode<uint32_t>(a_buffer, Endian::big);
    }

    size_t getEncodedLength() const
    {
        return 4;
    }

    unsigned int getOctetBoundary() const
    {
        return 4;
    }

    Integer64 hash() const
    {
        return Integer64(_data);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAinteger64LE, Integer64,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        return decode<uint64_t>(a_buffer, a_index, Endian::little);
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        encode<uint64_t>(a_buffer, Endian::little);
    }

    size_t getEncodedLength() const
    {
        return 8;
    }

    unsigned int getOctetBoundary() const
    {
        return 8;
    }

    Integer64 hash() const
    {
        return Integer64(_data);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAinteger64BE, Integer64,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        return decode<uint64_t>(a_buffer, a_index, Endian::big);
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        encode<uint64_t>(a_buffer, Endian::big);
    }

    size_t getEncodedLength() const
    {
        return 8;
    }

    unsigned int getOctetBoundary() const
    {
        return 8;
    }

    Integer64 hash() const
    {
        return Integer64(_data);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAoctetPairLE, OctetPair,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        const uint16_t nbOctet = 2;
        union {
            uint16_t intValue;
            char charValue[nbOctet];
        } val;

        for(uint i=0; i<nbOctet; i++)
        {
            val.charValue[i] = a_buffer[a_index + i];
        }
        typedef PrintInfo<uint16_t> PrintInfo;
        PrintInfo info(Encode::decode_before_swap, val.intValue, val.charValue, nbOctet);
        Swap<uint16_t>(val.intValue, Endian::little);
        info.reshow(Encode::decode_after_swap);
        _data.first = val.charValue[0];
        _data.second = val.charValue[1];

        return a_index + nbOctet;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        const uint nbOctet = 2;
        union {
            uint16_t intValue;
            char charValue[nbOctet];
        } val;
        val.charValue[0] = _data.first;
        val.charValue[1] = _data.second;
        typedef PrintInfo<uint16_t> PrintInfo;
        PrintInfo info(Encode::encode_before_swap, val.intValue, val.charValue, nbOctet);
        Swap<uint16_t>(val.intValue, Endian::little);
        info.reshow(Encode::encode_after_swap);
        for(uint i=0; i < nbOctet; i++)
        {
            a_buffer.push_back(val.charValue[i]);
        }
    }

    size_t getEncodedLength() const
    {
        return 2;
    }

    unsigned int getOctetBoundary() const
    {
        return 2;
    }

    Integer64 hash() const
    {
        union {
            Integer64 intValue;
            char charValue[2];
        } val;
        val.charValue[0] = _data.first;
        val.charValue[1] = _data.second;
        return val.intValue;
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAoctetPairBE, OctetPair,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        const uint nbOctet = 2;
        union {
            uint16_t intValue;
            char charValue[nbOctet];
        } val;

        for(uint i=0; i<nbOctet; i++)
        {
            val.charValue[i] = a_buffer[a_index + i];
        }
        typedef PrintInfo<uint16_t> PrintInfo;
        PrintInfo info(Encode::decode_before_swap, val.intValue, val.charValue, nbOctet);
        Swap<uint16_t>(val.intValue, Endian::big);
        info.reshow(Encode::decode_after_swap);
        _data.first = val.charValue[0];
        _data.second = val.charValue[1];

        return a_index + nbOctet;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        const uint nbOctet = 2;
        union {
            uint16_t intValue;
            char charValue[nbOctet];
        } val;
        val.charValue[0] = _data.first;
        val.charValue[1] = _data.second;
        typedef PrintInfo<uint16_t> PrintInfo;
        PrintInfo info(Encode::encode_before_swap, val.intValue, val.charValue, nbOctet);
        Swap<uint16_t>(val.intValue, Endian::big);
        info.reshow(Encode::encode_after_swap);
        for(uint i=0; i < nbOctet; i++)
        {
            a_buffer.push_back(val.charValue[i]);
        }
    }

    size_t getEncodedLength() const
    {
        return 2;
    }

    unsigned int getOctetBoundary() const
    {
        return 2;
    }

    Integer64 hash() const
    {
        union {
            Integer64 intValue;
            char charValue[2];
        } val;
        val.charValue[0] = _data.first;
        val.charValue[1] = _data.second;
        return val.intValue;
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAunicodeChar, wchar_t,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        union {
            uint16_t intValue;
            uint32_t int32Value;
            wchar_t value;
            char charValue[2];
            char char4Value[4];
        } val;

        for(uint i=0; i<2; i++)
        {
            val.charValue[i] = a_buffer[a_index + i];
        }
        PrintInfo16 info16(Encode::decode_before_swap, val.intValue, val.charValue, 2, val.value);
        PrintInfo32 info32(Encode::decode_before_swap, val.int32Value, val.char4Value, 4, val.value);
        Swap<uint16_t>(val.intValue, Endian::big);
        info16.reshow(Encode::decode_after_swap);
        info32.reshow(Encode::decode_after_swap);
        if(sizeof(wchar_t) == 4)
        {
            if(!g_isBigEndian)
                val.int32Value = val.int32Value & 0x0000ffff;
            else
                val.int32Value = val.int32Value & 0xffff0000;
        }
        info16.reshow(Encode::decode_after_swap);
        info32.reshow(Encode::decode_after_swap);
        _data = val.value;

        return a_index + 2;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        union {
            uint16_t intValue;
            uint32_t int32Value;
            wchar_t value;
            char charValue[2];
            char char4Value[4];
        } val;
        val.value = _data;
        if(sizeof(wchar_t) == 4)
        {
            if(!g_isBigEndian)
                val.int32Value = val.int32Value & 0x0000ffff;
            else {
                val.int32Value = val.int32Value & 0xffff0000;
                val.int32Value = val.int32Value >> 16;
            }
        }
        else if(sizeof(wchar_t) != 2)
        {
            throw EncoderException(L"SimpleDataType have to be on 16 bits or on 32 bits");
        }

        PrintInfo16 info16(Encode::encode_before_swap, val.intValue, val.charValue, 2, val.value);
        PrintInfo32 info32(Encode::encode_before_swap, val.int32Value, val.char4Value, 4, val.value);
        Swap<uint16_t>(val.intValue, Endian::big);
        info16.reshow(Encode::encode_after_swap);
        info32.reshow(Encode::encode_after_swap);
        for(uint i=0; i < 2; i++)
        {
            a_buffer.push_back(val.charValue[i]);
        }
    }

    size_t getEncodedLength() const
    {
        return 2;
    }

    unsigned int getOctetBoundary() const
    {
        return 2;
    }

    Integer64 hash() const
    {
        union {
            Integer64 intValue;
            wchar_t value;
        } val;
        val.value = _data;
        return Integer64(val.intValue);
    }
    )
}
