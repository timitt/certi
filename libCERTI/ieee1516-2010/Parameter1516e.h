#ifndef CERTI_PARAMETER_1516e_HH
#define CERTI_PARAMETER_1516e_HH

#include <memory>

#include "../Parameter.hh"
#include "BasicDataType.h"
#include "EncodableDataType.h"

namespace certi {

class BasicDataType;

///
/// \brief The Parameter1516e class
///
class Parameter1516e : public Parameter
{
public:
    Parameter1516e()=delete;

    ///
    /// \brief Parameter1516e
    /// \param a_name Name of the paramter
    /// \param a_parameterHandle Unique Id of the parameter
    ///
    Parameter1516e(const std::string& a_name, const ParameterHandle &a_parameterHandle)
        : Parameter(a_name, a_parameterHandle), _type() {

        _type = std::make_shared<BasicDataType>(); //By default datatype will be unknown
        _type->setCategory(EncodableDataType::CATEGORY::NA);
    }

    ///
    /// \brief Parameter1516e Copy constructor
    /// \param a_rhs
    ///
    Parameter1516e(const Parameter1516e& a_rhs)
        : Parameter(a_rhs.getName(), a_rhs.getHandle()), _type(a_rhs._type) {}

    ///
    /// \brief ~Parameter1516e
    ///
    virtual ~Parameter1516e() {

    }

    ///
    /// \brief getType
    /// \return A shared poiunter to the Datatype of the paramter
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
