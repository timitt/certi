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
            if(sizeof(I) == 4)
            {
                swap4Bytes();
            }
            else if(sizeof(I) == 8)
            {
                swap8Bytes();
            }
        }
    }

private:
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

            std::cout << "value : " << _value << std::endl;
            std::cout << "hex : " << std::hex << _intValue << std::endl;
            std::cout << "bits : ";
            for(uint i=0; i < _nbOctets; i++)
            {
                std::cout << std::bitset<8>(_octets[i]) << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    const I &_intValue;
    const V &_value;
    const char *_octets;
    const uint &_nbOctets;
};

typedef PrintInfo<uint64_t, double> PrintUint64Double;
typedef PrintInfo<uint32_t, float> PrintUint32Float;
typedef PrintInfo<uint16_t, Integer16> PrintUint16Int;

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
                                                                                                                    \
        void set(const SimpleDataType& inData);                                                                     \
                                                                                                                    \
        const SimpleDataType& get() const;                                                                          \
                                                                                                                    \
        encoder                                                                                                     \
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
        if(int(a_buffer[a_index]) < 0 || int(a_buffer[a_index]) > 127)
        {
            throw EncoderException(L"This is not a Standard ASCII character(see ANSI X3.4-1986).");
        }
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
        if(a_buffer[a_index] < -128 || a_buffer[a_index] > 127)
        {
            throw EncoderException(L"This is not an uninterpreted 8-bit byte.");
        }
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
        const uint nbOctet = sizeof(_data);

        union {
            uint32_t intValue;
            float floatValue;
            char charValue[nbOctet];
        } val;

        for(uint i=0; i<nbOctet; i++)
        {
            val.charValue[i] = a_buffer[a_index + i];
        }

        PrintUint32Float info(Encode::decode_before_swap, val.intValue, val.charValue, nbOctet, val.floatValue);

        //We translate to the host system if necessary
        Swap<uint32_t>(val.intValue, Endian::big);

        info.reshow(Encode::decode_after_swap);

        _data = val.floatValue;
        return a_index + nbOctet;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        const uint nbOctet = sizeof(_data);
        union {
            uint32_t intValue;
            float floatValue;
            char charValue[nbOctet];
        } val;

        val.floatValue = _data;


        PrintUint32Float info(Encode::encode_before_swap, val.intValue, val.charValue, nbOctet, val.floatValue);


        //We translate from the host system if necessary
        Swap<uint32_t>(val.intValue, Endian::big);

        info.reshow(Encode::encode_after_swap);

        for(uint i=0; i < nbOctet; i++)
        {
            a_buffer.push_back(val.charValue[i]);
        }

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
        const uint nbOctet = sizeof(_data);
        union {
            uint32_t intValue;
            float floatValue;
            char charValue[nbOctet];
        } val;
        for(uint i=0; i<nbOctet; i++)
        {
            val.charValue[i] = a_buffer[a_index + i];
        }

        PrintUint32Float info(Encode::decode_before_swap, val.intValue, val.charValue, nbOctet, val.floatValue);

        //We translate to the host system if necessary
        Swap<uint32_t>(val.intValue, Endian::little);

        info.reshow(Encode::decode_after_swap);

        _data = val.floatValue;
        return a_index + nbOctet;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        const uint nbOctet = sizeof(_data);
        union {
            uint32_t intValue;
            float floatValue;
            char charValue[nbOctet];
        } val;

        val.floatValue = _data;

        PrintUint32Float info(Encode::encode_before_swap, val.intValue, val.charValue, nbOctet, val.floatValue);

        //We translate from the host system if necessary
        Swap<uint32_t>(val.intValue, Endian::little);

        info.reshow(Encode::encode_after_swap);

        for(uint i=0; i < nbOctet; i++)
        {
            a_buffer.push_back(val.charValue[i]);
        }
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
        const uint nbOctet = sizeof(_data);
        union {
            uint64_t intValue;
            double doubleValue;
            char charValue[nbOctet];
        } val;
        for(uint i=0; i<nbOctet; i++)
        {
            val.charValue[i] = a_buffer[a_index + i];
        }

        PrintUint64Double info(Encode::decode_before_swap, val.intValue, val.charValue, nbOctet, val.doubleValue);

        //We translate to the host system if necessary
        Swap<uint64_t>(val.intValue, Endian::big);

        info.reshow(Encode::decode_after_swap);

        _data = val.doubleValue;
        return a_index + nbOctet;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        const uint nbOctet = sizeof(_data);
        union {
            uint64_t intValue;
            double doubleValue;
            char charValue[nbOctet];
        } val;

        val.doubleValue = _data;

        PrintUint64Double info(Encode::encode_before_swap, val.intValue, val.charValue, nbOctet, val.doubleValue);

        //We translate from the host system if necessary
        Swap<uint64_t>(val.intValue, Endian::big);

        info.reshow(Encode::encode_after_swap);

        for(uint i=0; i < nbOctet; i++)
        {
            a_buffer.push_back(val.charValue[i]);
        }

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
        const uint nbOctet = sizeof(_data);

        union {
            uint64_t intValue;
            double doubleValue;
            char charValue[nbOctet];
        } val;

        for(uint i=0; i<nbOctet; i++)
        {
            val.charValue[i] = a_buffer[a_index + i];
        }

        PrintUint64Double info(Encode::decode_before_swap, val.intValue, val.charValue, nbOctet, val.doubleValue);

        //We translate to the host system if necessary
        Swap<uint64_t>(val.intValue, Endian::little);

        info.reshow(Encode::decode_after_swap);

        _data = val.doubleValue;
        return a_index + nbOctet;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        const uint nbOctet = sizeof(_data);
        union {
            uint64_t intValue;
            double doubleValue;
            char charValue[nbOctet];
        } val;

        val.doubleValue = _data;

        PrintUint64Double info(Encode::encode_before_swap, val.intValue, val.charValue, nbOctet, val.doubleValue);

        //We translate from the host system if necessary
        Swap<uint64_t>(val.intValue, Endian::little);

        info.reshow(Encode::encode_after_swap);

        for(uint i=0; i < nbOctet; i++)
        {
            a_buffer.push_back(val.charValue[i]);
        }

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

//    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAinteger16LE, Integer16,
//    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
//    throw (EncoderException)
//    {
//        const uint nbOctet = sizeof(_data);

//        union {
//            uint16_t intValue;
//            double doubleValue;
//            char charValue[nbOctet];
//        } val;

//        for(uint i=0; i<nbOctet; i++)
//        {
//            val.charValue[i] = a_buffer[a_index + i];
//        }

//        PrintUint16Int info(Encode::decode_before_swap, val.intValue, val.charValue, nbOctet, val.doubleValue);

//        //We translate to the host system if necessary
//        Swap<uint16_t>(val.intValue, Endian::little);

//        info.reshow(Encode::decode_after_swap);

//        _data = val.doubleValue;
//        return a_index + nbOctet;
//    }

//    void encodeInto(std::vector<Octet>& a_buffer) const
//    throw (EncoderException)
//    {
//        const uint nbOctet = sizeof(_data);
//        union {
//            uint16_t intValue;
//            double doubleValue;
//            char charValue[nbOctet];
//        } val;

//        val.doubleValue = _data;

//        PrintUint16Int info(Encode::encode_before_swap, val.intValue, val.charValue, nbOctet, val.doubleValue);

//        //We translate from the host system if necessary
//        Swap<uint16_t>(val.intValue, Endian::little);

//        info.reshow(Encode::encode_after_swap);

//        for(uint i=0; i < nbOctet; i++)
//        {
//            a_buffer.push_back(val.charValue[i]);
//        }

//    }

//    size_t getEncodedLength() const
//    {
//        return 8;
//    }

//    unsigned int getOctetBoundary() const
//    {
//        return 8;
//    }

//    Integer64 hash() const
//    {
//        return Integer64(_data);
//    }
//    )
}
