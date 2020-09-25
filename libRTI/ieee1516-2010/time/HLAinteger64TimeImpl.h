#include <RTI/LogicalTime.h>
#include <RTI/encoding/BasicDataElements.h>
#include "HLAinteger64.h"

namespace rti1516e
{

class HLAinteger64TimeImpl : public HLAinteger64 {
public:
    HLAinteger64TimeImpl();
    HLAinteger64TimeImpl(const Integer64 &value);
    HLAinteger64TimeImpl(const HLAinteger64TimeImpl &other);

    HLAinteger64TimeImpl &operator=(const HLAinteger64TimeImpl &other)
        throw (rti1516e::InvalidLogicalTime);
    bool operator>(const HLAinteger64TimeImpl &other) const
        throw (rti1516e::InvalidLogicalTime);
    bool operator<(const HLAinteger64TimeImpl &other) const
        throw (rti1516e::InvalidLogicalTime);
    bool operator==(const HLAinteger64TimeImpl &other) const
        throw (rti1516e::InvalidLogicalTime);
    bool operator>=(const HLAinteger64TimeImpl &other) const
        throw (rti1516e::InvalidLogicalTime);
    bool operator<=(const HLAinteger64TimeImpl &other) const
        throw (rti1516e::InvalidLogicalTime);
    operator Integer64() const;
};

}
