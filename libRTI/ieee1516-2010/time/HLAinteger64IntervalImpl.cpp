#include "HLAinteger64IntervalImpl.h"

namespace rti1516e
{

//\brief Default constructor of class HLAinteger64IntervalImpl
HLAinteger64IntervalImpl::HLAinteger64IntervalImpl() : HLAinteger64()
{

}

//\brief Constructor of class HLAinteger64IntervalImpl with a given time
//@param value The integer64 value of Time
HLAinteger64IntervalImpl::HLAinteger64IntervalImpl(const Integer64 &value) : HLAinteger64(value)
{

}

//\brief Copy constructor of class HLAinteger64IntervalImpl
//@param other The HLAinteger64IntervalImpl object which have to be copied
HLAinteger64IntervalImpl::HLAinteger64IntervalImpl(const HLAinteger64IntervalImpl &other) : HLAinteger64(other)
{
}

//\brief Overload orpérator = for class HLAinteger64IntervalImpl
//@param other The HLAinteger64IntervalImpl object to be copied (copy only the time Time and Zero)
HLAinteger64IntervalImpl &HLAinteger64IntervalImpl::operator=(HLAinteger64IntervalImpl const &other)
throw (rti1516e::InvalidLogicalTimeInterval)
{
    _time = other._time;
    _zero = other._zero;
    return *this;
}

//\brief Overload orpérator += for class HLAinteger64IntervalImpl
//@param other The HLAinteger64IntervalImpl object to be added (add only the interval time)
HLAinteger64IntervalImpl& HLAinteger64IntervalImpl::operator+=(HLAinteger64IntervalImpl const& other)
   throw (rti1516e::IllegalTimeArithmetic,
          rti1516e::InvalidLogicalTimeInterval)
{
    _time += other._time;
    return *this;
}

//\brief Overload orpérator += for class HLAinteger64IntervalImpl
//@param other The HLAinteger64IntervalImpl object to be subtracted (subtracts only the interval time)
HLAinteger64IntervalImpl& HLAinteger64IntervalImpl::operator-=(HLAinteger64IntervalImpl const& other)
   throw (rti1516e::IllegalTimeArithmetic,
          rti1516e::InvalidLogicalTimeInterval)
{
    _time -= other._time;
    return *this;
}

//\brief Overload orpérator > for class HLAinteger64IntervalImpl
//@param other The HLAinteger64IntervalImpl object to be compared (compare only the time Time)
//@return True if is higher, false if not
bool HLAinteger64IntervalImpl::operator> (HLAinteger64IntervalImpl const & other) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    return _time > other._time;
}

//\brief Overload orpérator < for class HLAinteger64IntervalImpl
//@param other The HLAinteger64IntervalImpl object to be compared (compare only the time Time)
//@return True if is smaller, false if not
bool HLAinteger64IntervalImpl::operator< (HLAinteger64IntervalImpl const & other) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    return _time < other._time;
}

//\brief Overload orpérator == for class HLAinteger64IntervalImpl
//@param other The HLAinteger64IntervalImpl object to be compared (compare only the time Time)
//@return True if the time froms both object are equals
bool HLAinteger64IntervalImpl::operator== (HLAinteger64IntervalImpl const & other) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    return _time == other._time;

}

//\brief Overload orpérator >= for class HLAinteger64IntervalImpl
//@param other The HLAinteger64IntervalImpl object to be compared (compare only the time Time)
//@return True if is higher or equal, false if not
bool HLAinteger64IntervalImpl::operator>= (HLAinteger64IntervalImpl const & other) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    return _time >= other._time;
}

//\brief Overload orpérator <= for class HLAinteger64IntervalImpl
//@param other The HLAinteger64IntervalImpl object to be compared (compare only the time Time)
//@return True if is smaller or equal, false if not
bool HLAinteger64IntervalImpl::operator<= (HLAinteger64IntervalImpl const & other) const
   throw (rti1516e::InvalidLogicalTimeInterval)
{
    return _time <= other._time;
}

//\brief Overload cast from HLAinteger64IntervalImpl to Integer64 (cast the time)
HLAinteger64IntervalImpl::operator Integer64() const
{
    return _time;
}

}
