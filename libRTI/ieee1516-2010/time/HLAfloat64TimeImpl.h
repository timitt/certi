#include <RTI/LogicalTime.h>
#include <RTI/encoding/BasicDataElements.h>
#include "HLAfloat64.h"

namespace rti1516e
{

class HLAfloat64TimeImpl : public HLAfloat64 {
public:
    HLAfloat64TimeImpl();
    HLAfloat64TimeImpl(const double &value);
    HLAfloat64TimeImpl(const HLAfloat64TimeImpl &other);
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

    /*
    size_t encode(void *buffer, size_t bufferSize) const
        throw (rti1516e::CouldNotEncode);
    void decode(void *buffer, size_t bufferSize)
    throw (rti1516e::InternalError,
           rti1516e::CouldNotDecode); */
};

}
