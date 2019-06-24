#include <RTI/VariableLengthData.h>
#include <RTI/encoding/EncodingConfig.h>
#include <vector>
#include <cstring>
#include <HLAtypesIEEE1516.hh>
#include <iostream>

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
    {
        ::libhla::HLAdata<::libhla::HLAASCIIchar> value;
        std::cout << "::libhla::HLAASCIIchar::emptysizeof() : " << ::libhla::HLAinteger16BE::emptysizeof() << std::endl;
        *value = a_buffer[a_index];
        std::cout << "value.size : " << value.size() << std::endl;
        std::cout << "value.capacity : " << value.mCapacity << std::endl;
        std::cout << "value.data : " << value.data() << std::endl;
        std::cout << "value : " << *value << std::endl;
        std::stringstream result;
        value.print(result);
        std::cout << "Result : " << result.str() << std::endl;
        _data = *value;
        return a_index + 1;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    {
        ::libhla::HLAdata<::libhla::HLAASCIIchar> value;
        *value = _data;
        std::stringstream result;
        value.print(result);
        std::cout << "Result : " << result.str() << std::endl;
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
    {
        _data = bool(a_buffer[a_index]);
        return a_index + 1;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    {
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
    {
        _data = a_buffer[a_index];
        return a_index + 1;
    }

    void encodeInto(std::vector<Octet>& a_buffer) const
    {
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
}
