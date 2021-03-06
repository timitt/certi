#include <RTI/encoding/BasicDataElements.h>
#include "HLAfloat64.h"

namespace rti1516e
{

class HLAfloat64TimeImpl : public HLAfloat64 {
public:
    HLAfloat64TimeImpl();
    HLAfloat64TimeImpl(const double &value);
    HLAfloat64TimeImpl(const HLAfloat64TimeImpl &other);

    HLAfloat64TimeImpl &operator=(const HLAfloat64TimeImpl &other)
        throw (InvalidLogicalTime);
    bool operator>(const HLAfloat64TimeImpl &other) const
        throw (InvalidLogicalTime);
    bool operator<(const HLAfloat64TimeImpl &other) const
        throw (InvalidLogicalTime);
    bool operator==(const HLAfloat64TimeImpl &other) const
        throw (InvalidLogicalTime);
    bool operator>=(const HLAfloat64TimeImpl &other) const
        throw (InvalidLogicalTime);
    bool operator<=(const HLAfloat64TimeImpl &other) const
        throw (InvalidLogicalTime);
    operator double() const;
};

}
