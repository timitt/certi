#include <RTI/VariableLengthData.h>
#include <RTI/encoding/BasicDataElements.h>
#include <RTI/encoding/EncodingConfig.h>
#include <RTI/encoding/EncodingExceptions.h>
#include <RTI/encoding/HLAvariableArray.h>
#include <vector>
#include <cstring>
#include <iostream>

#include "PrintInfo.h"
#include "HLAvariableArrayImplementation.h"

#ifdef HOST_IS_BIG_ENDIAN
#define g_isBigEndian true
#else
#define g_isBigEndian false
#endif

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

#define DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(EncodableDataType, SimpleDataType, encoder)                     \
                                                                                                                    \
    class RTI_EXPORT EncodableDataType##Implementation                                                              \
    {                                                                                                               \
    public:                                                                                                         \
        EncodableDataType##Implementation();                                                                        \
        EncodableDataType##Implementation(const SimpleDataType& value);                                    \
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
            , const unsigned int a_nbOctet = sizeof(_data))                                                                 \
        {                                                                                                           \
            union {                                                                                                 \
                I intValue;                                                                                         \
                SimpleDataType value;                                                                               \
                char charValue[8];                                                                                  \
            } val;                                                                                                  \
                                                                                                                    \
            for(unsigned int i=0; i<a_nbOctet; i++)                                                                         \
            {                                                                                                       \
                val.charValue[i] = a_buffer[a_index + i];                                                           \
            }                                                                                                       \
            PrintInfo<I> info(Encode::decode_before_swap, val.intValue, val.charValue, a_nbOctet);          \
            Swap<I>(val.intValue, a_endianRepresentation);                                                          \
            info.reshow(Encode::decode_after_swap);                                                                 \
            _data = val.value;                                                                                      \
                                                                                                                    \
            return a_index + a_nbOctet;                                                                             \
        }                                                                                                           \
                                                                                                                    \
        template<typename I>                                                                                        \
        void encode(std::vector<Octet>& a_buffer, Endian a_endianRepresentation                                     \
                , const unsigned int a_nbOctet = sizeof(_data)) const                                                       \
        {                                                                                                           \
            union {                                                                                                 \
                I intValue;                                                                                         \
                SimpleDataType value;                                                                               \
                char charValue[8];                                                                                  \
            } val;                                                                                                  \
            val.value = _data;                                                                                      \
            PrintInfo<I> info(Encode::encode_before_swap, val.intValue, val.charValue, a_nbOctet);          \
            Swap<I>(val.intValue, a_endianRepresentation);                                                          \
            info.reshow(Encode::encode_after_swap);                                                                 \
            for(unsigned int i=0; i < a_nbOctet; i++)                                                                       \
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
//        PrintInfo<uint8_t>(Encode::decode, &a_buffer[0], 1);
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
//        PrintInfo<uint8_t>(Encode::encode, &a_buffer[0], 1);
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

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAASCIIstring, std::string,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        HLAASCIIchar hlaCharacterPrototype('a');
        HLAvariableArray hlaVariableArray(hlaCharacterPrototype);
        hlaVariableArray.decodeFrom(a_buffer, a_index);
        for(size_t i=0; i<hlaVariableArray.size(); i++) {
            HLAASCIIchar hlaCharacter(*static_cast<HLAASCIIchar*>(hlaVariableArray.get(i).clone().get()));
            _data.append(1, hlaCharacter.get());
        }
        return a_index + _data.length();
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        HLAASCIIchar hlaCharacterPrototype('a');
        HLAvariableArray hlaVariableArray(hlaCharacterPrototype);
        for(char character: _data) {
            HLAASCIIchar hlaCharacter(character);
            hlaVariableArray.addElement(hlaCharacter);
        }
        hlaVariableArray.encodeInto(a_buffer);
    }

    size_t getEncodedLength() const
    {
        return _data.length();
    }

    unsigned int getOctetBoundary() const
    {
        return _data.length();
    }

    Integer64 hash() const
    {
        return Integer64(0);
    }
    )

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAboolean, bool,

    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        HLAinteger32BE value;
        value.decodeFrom(a_buffer, a_index);
        _data = bool(value);
//        PrintInfo<uint8_t>(Encode::decode, &a_buffer[0], 1);
        return a_index + 1;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        HLAinteger32BE value(_data);
        value.encodeInto(a_buffer);
//        PrintInfo<uint8_t>(Encode::encode, &a_buffer[0], 1);
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

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAbyte, Octet,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        _data = a_buffer[a_index];
//        PrintInfo<uint8_t>(Encode::decode, &a_buffer[0], 1);
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
//        PrintInfo<uint8_t>(Encode::encode, &a_buffer[0], 1);
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

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAoctet, Octet,
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
//        union {
//            Integer64 intValue;
//            char charValue;
//        } val;
//        val.charValue = _data;
//        return val.intValue;
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

        for(unsigned int i=0; i<nbOctet; i++)
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
        const unsigned int nbOctet = 2;
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
        for(unsigned int i=0; i < nbOctet; i++)
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
        const unsigned int nbOctet = 2;
        union {
            uint16_t intValue;
            char charValue[nbOctet];
        } val;

        for(unsigned int i=0; i<nbOctet; i++)
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
        const unsigned int nbOctet = 2;
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
        for(unsigned int i=0; i < nbOctet; i++)
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

        for(unsigned int i=0; i<2; i++)
        {
            val.charValue[i] = a_buffer[a_index + i];
        }
        PrintInfo<uint16_t> info16(Encode::decode_before_swap, val.intValue, val.charValue, 2);
        PrintInfo<uint32_t> info32(Encode::decode_before_swap, val.int32Value, val.char4Value, 4);
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

        PrintInfo<uint16_t> info16(Encode::encode_before_swap, val.intValue, val.charValue, 2);
        PrintInfo<uint32_t> info32(Encode::encode_before_swap, val.int32Value, val.char4Value, 4);
        Swap<uint16_t>(val.intValue, Endian::big);
        info16.reshow(Encode::encode_after_swap);
        info32.reshow(Encode::encode_after_swap);
        for(unsigned int i=0; i < 2; i++)
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

    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAunicodeString, std::wstring,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        HLAunicodeChar hlaCharacterPrototype(wchar_t(L'a'));
        HLAvariableArray hlaVariableArray(hlaCharacterPrototype);
        hlaVariableArray.decodeFrom(a_buffer, a_index);
        for(size_t i=0; i<hlaVariableArray.size(); i++) {
            HLAunicodeChar hlaCharacter(*static_cast<HLAunicodeChar*>(hlaVariableArray.get(i).clone().get()));
            _data.append(1, hlaCharacter.get());
        }
        return a_index + _data.length();
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        HLAunicodeChar hlaCharacterPrototype(wchar_t(L'a'));
        HLAvariableArray hlaVariableArray(hlaCharacterPrototype);
        for(char character: _data) {
            HLAunicodeChar hlaCharacter(character);
            hlaVariableArray.addElement(hlaCharacter);
        }
        hlaVariableArray.encodeInto(a_buffer);
    }

    size_t getEncodedLength() const
    {
        return (_data.length()*2);
    }

    unsigned int getOctetBoundary() const
    {
        return (_data.length()*2);
    }

    Integer64 hash() const
    {
        return Integer64(0);
    }
    )

//    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLA3Byte, Integer32,
//    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
//    throw (EncoderException)
//    {
//        union {
//            uint16_t intValue;
//            uint32_t int32Value;
//            char charValue[4];
//        } val;

//        for(unsigned int i=0; i<3; i++)
//        {
//            val.charValue[i] = a_buffer[a_index + i];
//        }
//        val.charValue[3] = 0;
//        PrintInfo<uint32_t> info32(Encode::decode_before_swap, val.int32Value, val.charValue, 3);
//        Swap<uint16_t>(val.intValue, Endian::little);
//        info32.reshow(Encode::decode_after_swap);

//        if(!g_isBigEndian)
//            val.int32Value = val.int32Value & 0x0000ffff;
//        else
//            val.int32Value = val.int32Value & 0xffff0000;

//        info32.reshow(Encode::decode_after_swap);
//        _data = val.int32Value;

//        return a_index + 2;
//    }

//    void encodeInto(std::vector<Octet>& a_buffer) const
//    throw (EncoderException)
//    {
//        union {
//            uint16_t intValue;
//            uint32_t int32Value;
//            char charValue[4];
//        } val;
//        val.int32Value = _data;
//        if(sizeof(wchar_t) == 4)
//        {
//            if(!g_isBigEndian)
//                val.int32Value = val.int32Value & 0x0000ffff;
//            else {
//                val.int32Value = val.int32Value & 0xffff0000;
//                val.int32Value = val.int32Value >> 16;
//            }
//        }
//        else if(sizeof(wchar_t) != 2)
//        {
//            throw EncoderException(L"SimpleDataType have to be on 16 bits or on 32 bits");
//        }

//        PrintInfo<uint32_t> info32(Encode::encode_before_swap, val.int32Value, val.charValue, 3);
//        Swap<uint16_t>(val.intValue, Endian::little);
//        info32.reshow(Encode::encode_after_swap);
//        for(unsigned int i=0; i < 3; i++)
//        {
//            a_buffer.push_back(val.charValue[i]);
//        }
//    }

//    size_t getEncodedLength() const
//    {
//        return 3;
//    }

//    unsigned int getOctetBoundary() const
//    {
//        return 4;
//    }

//    Integer64 hash() const
//    {
//        return 0;
//    }
//    )
}
