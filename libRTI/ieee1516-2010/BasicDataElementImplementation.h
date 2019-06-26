#include <RTI/VariableLengthData.h>
#include <RTI/encoding/EncodingConfig.h>
#include <RTI/encoding/EncodingExceptions.h>
#include <vector>
#include <cstring>
#include <iostream>

#include "HLAtypesIEEE1516.hh"

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
        SimpleDataType _data;                                                                                      \
    };                                                                                                              \
                                                                                                                    \

namespace rti1516e
{
    DEFINE_ENCODING_HELPER_IMPLEMENTATION_CLASS(HLAASCIIchar, char,
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
    throw (EncoderException)
    {
        ::libhla::HLAdata<::libhla::HLAASCIIchar> value;
        *value = a_buffer[a_index];
        if(int(*value) < 0 || int(*value) > 127)
        {
            throw EncoderException(L"This is not a Standard ASCII character(see ANSI X3.4-1986).");
        }
        _data = *value;
        return a_index + 1;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        ::libhla::HLAdata<::libhla::HLAASCIIchar> value;
        *value = _data;
        if(int(*value) < 0 || int(*value) > 127)
        {
            throw EncoderException(L"This is not a Standard ASCII character(see ANSI X3.4-1986).");
        }
        a_buffer.push_back(*value);
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
        ::libhla::HLAdata<::libhla::HLAbyte> value;
        *value = a_buffer[a_index];
        if(int(*value) < -128 || int(*value) > 127)
        {
            throw EncoderException(L"This is not an uninterpreted 8-bit byte.");
        }
        _data = *value;
        return a_index + 1;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    throw (EncoderException)
    {
        ::libhla::HLAdata<::libhla::HLAbyte> value;
        *value = _data;
        if(int(*value) < -128 || int(*value) > 127)
        {
            throw EncoderException(L"This is not a Standard ASCII character(see ANSI X3.4-1986).");
        }
        a_buffer.push_back(*value);
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
}
