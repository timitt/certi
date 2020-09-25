#include <RTI/encoding/BasicDataElements.h>
#include "HLAfloat64.h"

namespace rti1516e
{


class HLAfloat64IntervalImpl : public HLAfloat64 {

public:
    HLAfloat64IntervalImpl();
    HLAfloat64IntervalImpl(const double& time);
    HLAfloat64IntervalImpl(const HLAfloat64IntervalImpl &other);

    HLAfloat64IntervalImpl& operator= (HLAfloat64IntervalImpl const & other)
       throw (InvalidLogicalTimeInterval);
    HLAfloat64IntervalImpl& operator+=(HLAfloat64IntervalImpl const& other)
       throw (IllegalTimeArithmetic,
              InvalidLogicalTimeInterval);
    HLAfloat64IntervalImpl& operator-=(HLAfloat64IntervalImpl const& other)
       throw (IllegalTimeArithmetic,
              InvalidLogicalTimeInterval);
    bool operator> (HLAfloat64IntervalImpl const & other) const
       throw (InvalidLogicalTimeInterval);
    bool operator< (HLAfloat64IntervalImpl const & other) const
       throw (InvalidLogicalTimeInterval);
    bool operator== (HLAfloat64IntervalImpl const & other) const
       throw (InvalidLogicalTimeInterval);
    bool operator>= (HLAfloat64IntervalImpl const & other) const
       throw (InvalidLogicalTimeInterval);
    bool operator<= (HLAfloat64IntervalImpl const & other) const
       throw (InvalidLogicalTimeInterval);

    operator double () const;

};

}
