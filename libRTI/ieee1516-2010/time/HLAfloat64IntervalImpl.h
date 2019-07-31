#include <RTI/encoding/BasicDataElements.h>
#include "HLAfloat64.h"

namespace rti1516e
{


class HLAfloat64IntervalImpl : public HLAfloat64 {

public:
    HLAfloat64IntervalImpl();
    HLAfloat64IntervalImpl(const double& time);
    HLAfloat64IntervalImpl(const HLAfloat64IntervalImpl &other);

    std::__cxx11::wstring toString() const;


    HLAfloat64IntervalImpl& operator= (HLAfloat64IntervalImpl const & other)
       throw (rti1516e::InvalidLogicalTimeInterval);
    HLAfloat64IntervalImpl& operator+=(HLAfloat64IntervalImpl const& other)
       throw (rti1516e::IllegalTimeArithmetic,
              rti1516e::InvalidLogicalTimeInterval);
    HLAfloat64IntervalImpl& operator-=(HLAfloat64IntervalImpl const& other)
       throw (rti1516e::IllegalTimeArithmetic,
              rti1516e::InvalidLogicalTimeInterval);
    bool operator> (HLAfloat64IntervalImpl const & other) const
       throw (rti1516e::InvalidLogicalTimeInterval);
    bool operator< (HLAfloat64IntervalImpl const & other) const
       throw (rti1516e::InvalidLogicalTimeInterval);
    bool operator== (HLAfloat64IntervalImpl const & other) const
       throw (rti1516e::InvalidLogicalTimeInterval);
    bool operator>= (HLAfloat64IntervalImpl const & other) const
       throw (rti1516e::InvalidLogicalTimeInterval);
    bool operator<= (HLAfloat64IntervalImpl const & other) const
       throw (rti1516e::InvalidLogicalTimeInterval);

    operator double () const;

};

}
