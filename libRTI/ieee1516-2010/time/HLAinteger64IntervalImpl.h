#include <RTI/encoding/BasicDataElements.h>
#include "HLAinteger64.h"

namespace rti1516e
{


class HLAinteger64IntervalImpl : public HLAinteger64 {
public:
    HLAinteger64IntervalImpl();
    HLAinteger64IntervalImpl(const Integer64& time);
    HLAinteger64IntervalImpl(const HLAinteger64IntervalImpl &other);

    HLAinteger64IntervalImpl& operator= (HLAinteger64IntervalImpl const & other)
       throw (rti1516e::InvalidLogicalTimeInterval);
    HLAinteger64IntervalImpl& operator+=(HLAinteger64IntervalImpl const& other)
       throw (rti1516e::IllegalTimeArithmetic,
              rti1516e::InvalidLogicalTimeInterval);
    HLAinteger64IntervalImpl& operator-=(HLAinteger64IntervalImpl const& other)
       throw (rti1516e::IllegalTimeArithmetic,
              rti1516e::InvalidLogicalTimeInterval);
    bool operator> (HLAinteger64IntervalImpl const & other) const
       throw (rti1516e::InvalidLogicalTimeInterval);
    bool operator< (HLAinteger64IntervalImpl const & other) const
       throw (rti1516e::InvalidLogicalTimeInterval);
    bool operator== (HLAinteger64IntervalImpl const & other) const
       throw (rti1516e::InvalidLogicalTimeInterval);
    bool operator>= (HLAinteger64IntervalImpl const & other) const
       throw (rti1516e::InvalidLogicalTimeInterval);
    bool operator<= (HLAinteger64IntervalImpl const & other) const
       throw (rti1516e::InvalidLogicalTimeInterval);

    operator Integer64 () const;
};

}
