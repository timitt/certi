#include "HLAfloat64IntervalImpl.h"

namespace rti1516e
{

//\brief Default constructor of class HLAfloat64IntervalImpl
HLAfloat64IntervalImpl::HLAfloat64IntervalImpl() : HLAfloat64()
{

}

//\brief Constructor of class HLAfloat64IntervalImpl with a given time
//@param value The double value of Time
HLAfloat64IntervalImpl::HLAfloat64IntervalImpl(const double &value) : HLAfloat64(value)
{

}

//\brief Copy constructor of class HLAfloat64IntervalImpl
//@param other The HLAfloat64IntervalImpl object which have to be copied
HLAfloat64IntervalImpl::HLAfloat64IntervalImpl(const HLAfloat64IntervalImpl &other) : HLAfloat64(other)
{
}

//\brief Overload orpérator = for class HLAfloat64IntervalImpl
//@param other The HLAfloat64IntervalImpl object to be copied (copy only the time Time and Zero)
HLAfloat64IntervalImpl &HLAfloat64IntervalImpl::operator=(HLAfloat64IntervalImpl const &other)
throw (rti1516e::InvalidLogicalTimeInterval)
{
    _time = other._time;
    _zero = other._zero;
    return *this;
}

//\brief Overload orpérator += for class HLAfloat64TimeImpl
//@param other The HLAfloat64IntervalImpl object to be added (add only the interval time)
HLAfloat64IntervalImpl& HLAfloat64IntervalImpl::operator+=(HLAfloat64IntervalImpl const& other)
   throw (rti1516e::IllegalTimeArithmetic,
          rti1516e::InvalidLogicalTimeInterval)
{
    _time += other._time;
    return *this;
}

//\brief Overload orpérator += for class HLAfloat64IntervalImpl
//@param other The HLAfloat64IntervalImpl object to be subtracted (subtracts only the interval time)
HLAfloat64IntervalImpl& HLAfloat64IntervalImpl::operator-=(HLAfloat64IntervalImpl const& other)
   throw (rti1516e::IllegalTimeArithmetic,
          rti1516e::InvalidLogicalTimeInterval)
{
    _time -= other._time;
    return *this;
}

//\brief Overload orpérator > for class HLAfloat64IntervalImpl
//@param other The HLAfloat64IntervalImpl object to be compared (compare only the time Time)
//@return True if is higher, false if not
bool HLAfloat64IntervalImpl::operator> (HLAfloat64IntervalImpl const & other) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    return _time > other._time;
}

//\brief Overload orpérator < for class HLAfloat64IntervalImpl
//@param other The HLAfloat64IntervalImpl object to be compared (compare only the time Time)
//@return True if is smaller, false if not
bool HLAfloat64IntervalImpl::operator< (HLAfloat64IntervalImpl const & other) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    return _time < other._time;
}

//\brief Overload orpérator == for class HLAfloat64IntervalImpl
//@param other The HLAfloat64IntervalImpl object to be compared (compare only the time Time)
//@return True if the time froms both object are equals
bool HLAfloat64IntervalImpl::operator== (HLAfloat64IntervalImpl const & other) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    return _time == other._time;

}

//\brief Overload orpérator >= for class HLAfloat64IntervalImpl
//@param other The HLAfloat64IntervalImpl object to be compared (compare only the time Time)
//@return True if is higher or equal, false if not
bool HLAfloat64IntervalImpl::operator>= (HLAfloat64IntervalImpl const & other) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    return _time >= other._time;
}

//\brief Overload orpérator <= for class HLAfloat64IntervalImpl
//@param other The HLAfloat64IntervalImpl object to be compared (compare only the time Time)
//@return True if is smaller or equal, false if not
bool HLAfloat64IntervalImpl::operator<= (HLAfloat64IntervalImpl const & other) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    return _time <= other._time;
}

//\brief String representation of the HLAfloat64IntervalImpl
//@return the time in string format
std::__cxx11::wstring HLAfloat64IntervalImpl::toString() const
{
    return std::to_wstring(_time);
}

//\brief Overload cast from HLAfloat64IntervalImpl to double (cast the time)
HLAfloat64IntervalImpl::operator double() const
{
    return _time;
}

}
