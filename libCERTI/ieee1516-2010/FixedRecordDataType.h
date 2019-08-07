#ifndef FIXED_RECORD_DATA_TYPE_H
#define FIXED_RECORD_DATA_TYPE_H

#include "DataType.h"

namespace certi {

class Field
{
public:
    Field() : _name(""), _typeElem(), _semantics(""), _typeElemTemp("") {

        _typeElem = std::make_shared<BasicDataType>(); //By default datatype will be unknown
        _typeElem->setCategory(EncodableDataType::CATEGORY::NA);
    }

    Field(const std::string &a_name, const std::shared_ptr<DataType> &a_typeElem, const std::string &a_semantics) :
            _name(a_name), _typeElem(a_typeElem), _semantics(a_semantics), _typeElemTemp("") {}

    Field(const Field &a_rhs) : _name(a_rhs.name()), _typeElem(a_rhs.typeElem()), _semantics(a_rhs.semantics()), _typeElemTemp(a_rhs.typeElemTemp()) {}

    virtual ~Field() {}

    std::string name() const
    {
        return _name;
    }

    void setName(const std::string &&name)
    {
        _name = name;
    }

    std::shared_ptr<DataType> typeElem() const
    {
        return _typeElem;
    }

    void setTypeElem(const std::shared_ptr<DataType> &typeElem)
    {
        _typeElem = typeElem;
    }

    std::string semantics() const
    {
        return _semantics;
    }

    void setSemantics(const std::string &&semantics)
    {
        _semantics = semantics;
    }

    std::string typeElemTemp() const
    {
        return _typeElemTemp;
    }

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

class FixedRecordDataType : public DataType
{
public:
    FixedRecordDataType() : DataType(EncodableDataType::CATEGORY::FixedRecordDataType, EncodableDataType::TYPE::HLAunknown, EncodableDataType::toString(EncodableDataType::TYPE::HLAunknown)),
                        _encoding(""), _semantics(""), _fields() {}

    FixedRecordDataType(const EncodableDataType::TYPE &a_name, const std::string &a_encoding, const std::string &a_semantics, const std::vector<Field> &a_fields) :
                        DataType(EncodableDataType::CATEGORY::FixedRecordDataType, a_name, EncodableDataType::toString(a_name)), _encoding(a_encoding), _semantics(a_semantics)
                        , _fields(a_fields) {}

    FixedRecordDataType(const FixedRecordDataType &a_rhs) : DataType(a_rhs.category(), a_rhs.name(), a_rhs.nameStr()), _encoding(a_rhs._encoding), _semantics(a_rhs._semantics)
      , _fields(a_rhs._fields) {}

    virtual ~FixedRecordDataType() {}



    std::string encoding() const
    {
        return _encoding;
    }

    void setEncoding(const std::string &encoding)
    {
        _encoding = encoding;
    }

    std::string semantics() const
    {
        return _semantics;
    }

    void setSemantics(const std::string &semantics)
    {
        _semantics = semantics;
    }

    std::vector<Field>& fields()
    {
        return _fields;
    }

    void setFields(const std::vector<Field> &fields)
    {
        _fields = fields;
    }

    void addField(const Field &a_field) {
        _fields.push_back(a_field);
    }

private:
    std::string _encoding;
    std::string _semantics;
    std::vector<Field> _fields;

};



}

#endif
