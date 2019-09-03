#ifndef CERTI_OBJECT_CLASS_ATTRIBUTE_1516e_HH
#define CERTI_OBJECT_CLASS_ATTRIBUTE_1516e_HH

#include "../ObjectClassAttribute.hh"
#include "DataType.h"
#include "BasicDataType.h"
#include "EncodableDataType.h"

namespace certi {

class BasicDataType;

///
/// \brief The ObjectClassAttribute1516e class
///
class ObjectClassAttribute1516e : public ObjectClassAttribute
{
public:

    ObjectClassAttribute1516e()=delete;

    ///
    /// \brief ObjectClassAttribute1516e
    /// \param a_name Name of the Attribute
    /// \param a_attributeHandle Unique id of the Attribute
    ///
    ObjectClassAttribute1516e(const std::string& a_name, const AttributeHandle &a_attributeHandle)
        : ObjectClassAttribute(a_name, a_attributeHandle), _type() {

        _type = std::make_shared<BasicDataType>(); //By default datatype will be unknown
        _type->setCategory(EncodableDataType::CATEGORY::NA);
    }

    ///
    /// \brief ObjectClassAttribute1516e Copy constructor
    /// \param a_rhs
    ///
    ObjectClassAttribute1516e(const ObjectClassAttribute1516e& a_rhs)
        : ObjectClassAttribute(a_rhs), _type(a_rhs._type) {}

    ///
    /// \brief ~ObjectClassAttribute1516e
    ///
    virtual ~ObjectClassAttribute1516e() {
    }

    ///
    /// \brief getType
    /// \return a shared pointer to the DataType of the Attribute
    ///
    std::shared_ptr<DataType> getType() {
        return _type;
    }

    ///
    /// \brief setType
    /// \param a_type
    ///
    void setType(const std::shared_ptr<DataType> a_type) {
        _type = a_type;
    }


private:
    std::shared_ptr<DataType> _type;
};

}
#endif
