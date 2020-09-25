#include <RTI/encoding/BasicDataElements.h>
#include <chrono>
#include <cstring>

namespace rti1516e
{

//\brief Class that implement basic methods for HLAfloat implementation
class HLAfloat64 {
public:
    HLAfloat64();
    HLAfloat64(const double& value);
    HLAfloat64(const HLAfloat64 &other);
    void setInitial();
    bool isInitial() const;
    void setFinal();
    bool isFinal() const;
    double getTime() const;
    void setTime(double value);
    std::wstring toString() const;

    size_t encode(void *buffer, size_t bufferSize) const
        throw (rti1516e::CouldNotEncode);
    void decode(void *buffer, size_t bufferSize)
    throw (rti1516e::InternalError,
           rti1516e::CouldNotDecode);

protected:
    double _time;
    double _zero;
    const double _epsilon = std::numeric_limits<double>::epsilon();
    const double _positiveInfinity = std::numeric_limits<double>::infinity();
};

}
