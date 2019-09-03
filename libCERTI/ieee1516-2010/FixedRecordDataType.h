#ifndef FIXED_RECORD_DATA_TYPE_H
#define FIXED_RECORD_DATA_TYPE_H

#include "DataType.h"

namespace certi {

///
/// \brief The Field class
///
class Field
{
public:
    ///
    /// \brief Field
    ///
    Field() : _name(""), _typeElem(), _semantics(""), _typeElemTemp("") {

        _typeElem = std::make_shared<BasicDataType>(); //By default datatype will be unknown
        _typeElem->setCategory(EncodableDataType::CATEGORY::NA);
    }

    ///
    /// \brief Field
    /// \param a_name Name of the Field
    /// \param a_typeElem DataType of the Field
    /// \param a_semantics Semantics of the Field
    ///
    Field(const std::string &a_name, const std::shared_ptr<DataType> &a_typeElem, const std::string &a_semantics) :
            _name(a_name), _typeElem(a_typeElem), _semantics(a_semantics), _typeElemTemp("") {}

    ///
    /// \brief Field Copy constructor
    /// \param a_rhs The Field to copy
    ///
    Field(const Field &a_rhs) : _name(a_rhs.name()), _typeElem(a_rhs.typeElem()), _semantics(a_rhs.semantics()), _typeElemTemp(a_rhs.typeElemTemp()) {}

    ///
    /// \brief ~Field
    ///
    virtual ~Field() {}

    ///
    /// \brief name
    /// \return name of the Field
    ///
    std::string name() const
    {
        return _name;
    }

    ///
    /// \brief setName
    /// \param name
    ///
    void setName(const std::string &&name)
    {
        _name = name;
    }

    ///
    /// \brief typeElem
    /// \return DataType of the element
    ///
    std::shared_ptr<DataType> typeElem() const
    {
        return _typeElem;
    }

    ///
    /// \brief setTypeElem
    /// \param typeElem
    ///
    void setTypeElem(const std::shared_ptr<DataType> &typeElem)
    {
        _typeElem = typeElem;
    }

    ///
    /// \brief semantics
    /// \return semantiocs of the field
    ///
    std::string semantics() const
    {
        return _semantics;
    }

    ///
    /// \brief setSemantics
    /// \param semantics
    ///
    void setSemantics(const std::string &&semantics)
    {
        _semantics = semantics;
    }

    ///
    /// \brief typeElemTemp
    /// \return Name of the Field Datatype
    ///
    std::string typeElemTemp() const
    {
        return _typeElemTemp;
    }

    ///
    /// \brief setTypeElemTemp
    /// \param typeElemTemp
    ///
    void setTypeElemTemp(const std::string &typeElemTemp)
    {
        _typeElemTemp = typeElemTemp;
    }

private:
    std::string _name;
    std::shared_ptr<DataType> _typeElem;
    std::string _semantics;
    std::string _typeElemTemp;
};

///
/// \brief The FixedRecordDataType class
///
class FixedRecordDataType : public DataType
{
public:

    ///
    /// \brief FixedRecordDataType
    ///
    FixedRecordDataType() : DataType(EncodableDataType::CATEGORY::FixedRecordDataType, EncodableDataType::TYPE::HLAunknown, EncodableDataType::toString(EncodableDataType::TYPE::HLAunknown)),
                        _encoding(""), _semantics(""), _fields() {}

    ///
    /// \brief FixedRecordDataType
    /// \param a_name Name of the FixedRecordDataType, it's an EncodableDataType::TYPE enumerator
    /// \param a_encoding Encoding of the FixedRecordDataType
    /// \param a_semantics Semantics of the FixedRecordDataType
    /// \param a_fields Vector of Fields
    ///
    FixedRecordDataType(const EncodableDataType::TYPE &a_name, const std::string &a_encoding, const std::string &a_semantics, const std::vector<Field> &a_fields) :
                        DataType(EncodableDataType::CATEGORY::FixedRecordDataType, a_name, EncodableDataType::toString(a_name)), _encoding(a_encoding), _semantics(a_semantics)
                        , _fields(a_fields) {}

    ///
    /// \brief FixedRecordDataType
    /// \param a_rhs
    ///
    FixedRecordDataType(const FixedRecordDataType &a_rhs) : DataType(a_rhs.category(), a_rhs.name(), a_rhs.nameStr()), _encoding(a_rhs._encoding), _semantics(a_rhs._semantics)
      , _fields(a_rhs._fields) {}

    ///
    /// \brief ~FixedRecordDataType
    ///
    virtual ~FixedRecordDataType() {}

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
    /// \brief semantics
    /// \return semantics
    ///
    std::string semantics() const
    {
        return _semantics;
    }

    ///
    /// \brief setSemantics
    /// \param semantics
    ///
    void setSemantics(const std::string &semantics)
    {
        _semantics = semantics;
    }

    ///
    /// \brief fields
    /// \return Vector of Fields
    ///
    std::vector<Field>& fields()
    {
        return _fields;
    }

    ///
    /// \brief setFields
    /// \param fields
    ///
    void setFields(const std::vector<Field> &fields)
    {
        _fields = fields;
    }

    ///
    /// \brief addField
    /// \param a_field
    ///
    void addField(const Field &a_field) {
        _fields.push_back(a_field);
    }

    ///
    /// \brief calTotalSize Calcul total size with padding. Work only if there is no Dynamic Array inside
    /// \return
    ///
    virtual size_t calTotalSize() {
        _totalSize = 0;
        for(auto it = _fields.cbegin(); it != _fields.cend(); it++) {
            _totalSize += it->typeElem()->totalSize();
        }
        return _totalSize;
    }

private:
    std::string _encoding;
    std::string _semantics;
    std::vector<Field> _fields;

};



}

#endif
