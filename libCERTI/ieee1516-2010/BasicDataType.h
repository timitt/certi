#ifndef BASIC_DATA_TYPE_H
#define BASIC_DATA_TYPE_H
#include "DataType.h"

namespace certi {

class BasicDataType : public DataType
{
public:

    ///
    /// \brief BasicDataType
    ///
    BasicDataType() : DataType(EncodableDataType::CATEGORY::BasicDataType, EncodableDataType::TYPE::HLAunknown, "HLAunknown")
                        , _size(0), _endian(), _interpretation(""), _encoding(""){}

    ///
    /// \brief BasicDataType Copy constructor
    /// \param a_rhs Right hand side BasicDataType which have to be copied
    ///
    BasicDataType(const BasicDataType &a_rhs)
        : DataType(a_rhs), _endian(a_rhs.endian()),
          _interpretation(a_rhs.interpretation()), _encoding(a_rhs.encoding()) {}

    ///
    /// \brief ~BasicDataType
    ///
    virtual ~BasicDataType(){}

    ///
    /// \brief size
    /// \return The size of the basic datatype
    ///
    size_t size() const
    {
        return _size;
    }

    ///
    /// \brief setSize
    /// \param size
    ///
    void setSize(const size_t &size)
    {
        _size = size;
    }

    ///
    /// \brief endian
    /// \return the EncodableDataType::ENDIAN enumerator of the basicDataType
    ///
    EncodableDataType::ENDIAN endian() const
    {
        return _endian;
    }

    ///
    /// \brief setEndian
    /// \param endian
    ///
    void setEndian(const EncodableDataType::ENDIAN &endian)
    {
        _endian = endian;
    }

    ///
    /// \brief interpretation
    /// \return interpretation
    ///
    std::string interpretation() const
    {
        return _interpretation;
    }

    ///
    /// \brief setInterpretation
    /// \param interpretation
    ///
    void setInterpretation(const std::string &interpretation)
    {
        _interpretation = interpretation;
    }

    ///
    /// \brief encoding
    /// \return encoding
    ///
    std::string encoding() const
    {
        return _encoding;
    }

    ///
    /// \brief setEncoding
    /// \param encoding
    ///
    void setEncoding(const std::string &encoding)
    {
        _encoding = encoding;
    }

    ///
    /// \brief calTotalSize
    /// \return Total Size, for this component it's the same than size
    ///
    virtual size_t calTotalSize() {
        _totalSize = _size;
        return _totalSize;
    }

private:
    size_t _size;
    EncodableDataType::ENDIAN _endian;
    std::string _interpretation;
    std::string _encoding;
};













}

#endif
