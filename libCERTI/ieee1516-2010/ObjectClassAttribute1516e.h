#ifndef CERTI_OBJECT_CLASS_ATTRIBUTE_1516e_HH
#define CERTI_OBJECT_CLASS_ATTRIBUTE_1516e_HH

#include "../ObjectClassAttribute.hh"
#include "DataType.h"
#include "BasicDataType.h"
#include "EncodableDataType.h"

namespace certi {

class BasicDataType;

class ObjectClassAttribute1516e : public ObjectClassAttribute
{
public:

    ObjectClassAttribute1516e()=delete;

    ObjectClassAttribute1516e(const std::string& a_name, const AttributeHandle &a_attributeHandle)
        : ObjectClassAttribute(a_name, a_attributeHandle), _type() {

        _type = std::make_shared<BasicDataType>(); //By default datatype will be unknown
        _type->setCategory(EncodableDataType::CATEGORY::NA);
    }

    ObjectClassAttribute1516e(const ObjectClassAttribute1516e& a_rhs)
        : ObjectClassAttribute(a_rhs), _type(a_rhs._type) {}

    virtual ~ObjectClassAttribute1516e() {
    }

    std::shared_ptr<DataType> getType() {
        return _type;
    }

    void setType(const std::shared_ptr<DataType> a_type) {
        _type = a_type;
    }


private:
    std::shared_ptr<DataType> _type;
};

}
#endif
