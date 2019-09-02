#ifndef VARIANT_RECORD_DATA_TYPE_H
#define VARIANT_RECORD_DATA_TYPE_H

#include <vector>

#include "EnumeratedDataType.h"

namespace certi {

class Alternative
{
public:

    Alternative() : _enumerator(), _name(""), _type(), _semantics(""), _enumeratorTemp(""), _typeTemp("")
    {
        _type = std::make_shared<BasicDataType>(); //By default datatype will be unknown
        _type->setCategory(EncodableDataType::CATEGORY::NA);
    }

    Alternative(const Alternative &a_rhs) : _enumerator(a_rhs.enumerator()), _name(a_rhs.name()), _type(a_rhs.type()), _semantics(a_rhs.semantics()),
                                            _enumeratorTemp(a_rhs.enumeratorTemp()), _typeTemp(a_rhs.typeTemp())
    {

    }

    virtual ~Alternative()
    {

    }

    std::shared_ptr<Enumerator> enumerator() const
    {
        return _enumerator;
    }

    void setEnumerator(const std::shared_ptr<Enumerator> &enumerator)
    {
        _enumerator = enumerator;
    }

    std::string name() const
    {
        return _name;
    }

    void setName(const std::string &name)
    {
        _name = name;
    }

    std::shared_ptr<DataType> type() const
    {
        return _type;
    }

    void setType(const std::shared_ptr<DataType> &type)
    {
        _type = type;
    }

    std::string semantics() const
    {
        return _semantics;
    }

    void setSemantics(const std::string &semantics)
    {
        _semantics = semantics;
    }

    std::string enumeratorTemp() const
    {
        return _enumeratorTemp;
    }

    void setEnumeratorTemp(const std::string &enumeratorTemp)
    {
        _enumeratorTemp = enumeratorTemp;
    }

    std::string typeTemp() const
    {
        return _typeTemp;
    }

    void setTypeTemp(const std::string &typeTemp)
    {
        _typeTemp = typeTemp;
    }


private:
    std::shared_ptr<Enumerator> _enumerator;
    std::string _name;
    std::shared_ptr<DataType> _type;
    std::string _semantics;
    std::string _enumeratorTemp;
    std::string _typeTemp;
};

class VariantRecordDataType : public DataType
{
public:
    VariantRecordDataType() : DataType(EncodableDataType::CATEGORY::VariantRecordDataType, EncodableDataType::TYPE::HLAunknown, EncodableDataType::toString(EncodableDataType::TYPE::HLAunknown)),
                                _discriminantName(""), _discriminantType(), _alternatives(), _discriminantTypeTemp("")
    {
        _discriminantType = std::make_shared<EnumeratedDataType>(); //By default datatype will be unknown
        _discriminantType->setCategory(EncodableDataType::CATEGORY::NA);
    }

    VariantRecordDataType(const VariantRecordDataType &a_rhs) : DataType(a_rhs.category(), a_rhs.name(), a_rhs.nameStr()),
                                                                _discriminantName(a_rhs.discriminantName()), _discriminantType(a_rhs.discriminantType()),
                                                                _alternatives(a_rhs.calternatives()), _discriminantTypeTemp(a_rhs._discriminantTypeTemp)
    {

    }

    std::string discriminantName() const
    {
        return _discriminantName;
    }

    void setDiscriminantName(const std::string &discriminantName)
    {
        _discriminantName = discriminantName;
    }

    std::shared_ptr<EnumeratedDataType> discriminantType() const
    {
        return _discriminantType;
    }

    void setDiscriminantType(const std::shared_ptr<EnumeratedDataType> &discriminantType)
    {
        _discriminantType = discriminantType;
    }

    std::vector<Alternative> calternatives() const
    {
        return _alternatives;
    }

    std::vector<Alternative>& alternatives()
    {
        return _alternatives;
    }

    void setAlternatives(const std::vector<Alternative> &alternatives)
    {
        _alternatives = alternatives;
    }

    void addAlternative(const Alternative &alternative)
    {
        _alternatives.push_back(alternative);
    }

    std::string discriminantTypeTemp() const
    {
        return _discriminantTypeTemp;
    }

    void setDiscriminantTypeTemp(const std::string &discriminantTypeTemp)
    {
        _discriminantTypeTemp = discriminantTypeTemp;
    }

    virtual size_t calTotalSize() {
        _totalSize = 0;
        return _totalSize;
    }

private:
    std::string _discriminantName;
    std::shared_ptr<EnumeratedDataType> _discriminantType;
    std::vector<Alternative> _alternatives;
    std::string _discriminantTypeTemp;

};

}

#endif
