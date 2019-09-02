#ifndef ARRAY_DATA_TYPE_H
#define ARRAY_DATA_TYPE_H

#include "DataType.h"

namespace certi {

class ArrayDataType : public DataType
{
public:
    ArrayDataType() : DataType(EncodableDataType::CATEGORY::ArrayDataType, EncodableDataType::TYPE::HLAunknown, EncodableDataType::toString(EncodableDataType::TYPE::HLAunknown)),
        _typeElements(), _cardinality("0"), _encoding(""), _semantics(""), _typeElementsTemp("") {

        _typeElements = std::make_shared<BasicDataType>(); //By default datatype will be unknown
        _typeElements->setCategory(EncodableDataType::CATEGORY::NA);
    }

    ArrayDataType(const EncodableDataType::TYPE &a_name, std::shared_ptr<DataType> a_typeElements, std::string a_cardinality, std::string a_encoding, std::string a_semantics) :
        DataType(EncodableDataType::CATEGORY::ArrayDataType, a_name, EncodableDataType::toString(a_name)), _typeElements(a_typeElements), _cardinality(a_cardinality),
        _encoding(a_encoding), _semantics(a_semantics), _typeElementsTemp("") {}

    ArrayDataType(const ArrayDataType &a_rhs) :
        DataType(a_rhs.category(), a_rhs.name(), a_rhs.nameStr()), _typeElements(a_rhs.typeElements()), _cardinality(a_rhs.cardinality()), _encoding(a_rhs.encoding()),
        _semantics(a_rhs.semantics()), _typeElementsTemp(a_rhs.typeElementsTemp()) {}


    std::shared_ptr<DataType> typeElements() const
    {
        return _typeElements;
    }

    void setTypeElements(const std::shared_ptr<DataType> &typeElements)
    {
        _typeElements = typeElements;
    }

    std::string cardinality() const
    {
        return _cardinality;
    }

    void setCardinality(const std::string &cardinality)
    {
        _cardinality = cardinality;
    }

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

    std::string typeElementsTemp() const
    {
        return _typeElementsTemp;
    }

    void setTypeElementsTemp(const std::string &typeElementsTemp)
    {
        _typeElementsTemp = typeElementsTemp;
    }

    virtual size_t calTotalSize() {
        if(_cardinality == "Dynamic")
            _totalSize = 0;
        else
            _totalSize = atoi(_cardinality.c_str());
        return _totalSize;
    }


private:
    std::shared_ptr<DataType> _typeElements;
    std::string _cardinality;
    std::string _encoding;
    std::string _semantics;
    std::string _typeElementsTemp;
};


}


#endif
