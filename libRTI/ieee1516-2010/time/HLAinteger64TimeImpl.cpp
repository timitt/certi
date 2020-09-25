#include "HLAinteger64TimeImpl.h"

namespace rti1516e {

//\Brief Default HLAinteger64TimeImpl constructor
HLAinteger64TimeImpl::HLAinteger64TimeImpl() : HLAinteger64()
{

}

//\Brief HLAinteger64TimeImpl constructor with a specified time
//\@param value time to be set in the HLAinteger64TimeImpl
HLAinteger64TimeImpl::HLAinteger64TimeImpl(const Integer64 &value) : HLAinteger64(value)
{

}

//\Brief Default HLAinteger64TimeImpl constructor
//\@param other The HLAinteger64TimeImpl object to be copied
HLAinteger64TimeImpl::HLAinteger64TimeImpl(const HLAinteger64TimeImpl &other) : HLAinteger64(other)
{

}


//\brief Overload orpérator = for class HLAinteger64TimeImpl
//@param other The HLAinteger64TimeImpl object to be copied (copy only the time Time and Zero)
HLAinteger64TimeImpl &HLAinteger64TimeImpl::operator=(const HLAinteger64TimeImpl &other)
throw (rti1516e::InvalidLogicalTime)
{
    _time = other._time;
    _zero = other._zero;
    return *this;
}

//\brief Overload orpérator > for class HLAinteger64TimeImpl
//@param other The HLAinteger64TimeImpl object to be compared (compare only the time Time)
//@return True if is higher, false if not
bool HLAinteger64TimeImpl::operator> (HLAinteger64TimeImpl const & other) const
   throw (rti1516e::InvalidLogicalTime)
{
    return _time > other._time;
}

//\brief Overload orpérator < for class HLAinteger64TimeImpl
//@param other The HLAinteger64TimeImpl object to be compared (compare only the time Time)
//@return True if is smaller, false if not
bool HLAinteger64TimeImpl::operator< (HLAinteger64TimeImpl const & other) const
   throw (rti1516e::InvalidLogicalTime)
{
    return _time < other._time;
}

//\brief Overload orpérator == for class HLAinteger64TimeImpl
//@param other The HLAinteger64TimeImpl object to be compared (compare only the time Time)
//@return True if the time froms both object are equals
bool HLAinteger64TimeImpl::operator== (HLAinteger64TimeImpl const & other) const
   throw (rti1516e::InvalidLogicalTime)
{
    return _time == other._time;

}

//\brief Overload orpérator >= for class HLAinteger64TimeImpl
//@param other The HLAinteger64TimeImpl object to be compared (compare only the time Time)
//@return True if is higher or equal, false if not
bool HLAinteger64TimeImpl::operator>= (HLAinteger64TimeImpl const & other) const
   throw (rti1516e::InvalidLogicalTime)
{
    return _time >= other._time;
}

//\brief Overload orpérator <= for class HLAinteger64TimeImpl
//@param other The HLAinteger64TimeImpl object to be compared (compare only the time Time)
//@return True if is smaller or equal, false if not
bool HLAinteger64TimeImpl::operator<= (HLAinteger64TimeImpl const & other) const
   throw (rti1516e::InvalidLogicalTime)
{
    return _time <= other._time;
}

//\brief Overload cast from HLAinteger64TimeImpl to Integer64 (cast the time)
HLAinteger64TimeImpl::operator Integer64() const
{
    return _time;
}

}
