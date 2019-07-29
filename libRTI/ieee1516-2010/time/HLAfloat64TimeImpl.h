#include <chrono>
#include <RTI/LogicalTime.h>
#include <RTI/encoding/BasicDataElements.h>

namespace rti1516e
{

class HLAfloat64TimeImpl {
public:
    HLAfloat64TimeImpl();
    HLAfloat64TimeImpl(const double &value);
    HLAfloat64TimeImpl(const HLAfloat64TimeImpl &other);

    void setInitial();
    bool isInitial() const;
    void setFinal();
    bool isFinal() const;
    double getTime() const;
    void setTime(double value);
    std::__cxx11::wstring toString() const;

    HLAfloat64TimeImpl &operator=(const HLAfloat64TimeImpl &other)
        throw (rti1516e::InvalidLogicalTime);
    bool operator>(const HLAfloat64TimeImpl &other) const
        throw (rti1516e::InvalidLogicalTime);
    bool operator<(const HLAfloat64TimeImpl &other) const
        throw (rti1516e::InvalidLogicalTime);
    bool operator==(const HLAfloat64TimeImpl &other) const
        throw (rti1516e::InvalidLogicalTime);
    bool operator>=(const HLAfloat64TimeImpl &other) const
        throw (rti1516e::InvalidLogicalTime);
    bool operator<=(const HLAfloat64TimeImpl &other) const
        throw (rti1516e::InvalidLogicalTime);
    operator double() const;

    size_t encode(void *buffer, size_t bufferSize) const
        throw (rti1516e::CouldNotEncode);
    void decode(void *buffer, size_t bufferSize)
    throw (rti1516e::InternalError,
           rti1516e::CouldNotDecode);
private:
    double _time;
    double _zero;
    const double _epsilon = std::numeric_limits<double>::epsilon();
    const double _positiveInfinity = std::numeric_limits<double>::infinity();
};

}
