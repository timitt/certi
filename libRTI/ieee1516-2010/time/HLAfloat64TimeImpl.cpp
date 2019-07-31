#include "HLAfloat64TimeImpl.h"
#include <thread>
#include <iostream>
#include <cstring>

namespace rti1516e
{

//\brief Default constructor of class HLAfloat64TimeImpl
HLAfloat64TimeImpl::HLAfloat64TimeImpl() : HLAfloat64()
{

}

//\brief Constructor of class HLAfloat64TimeImpl with a given time
//@param value The double value of Time
HLAfloat64TimeImpl::HLAfloat64TimeImpl(const double &value) : HLAfloat64(value)
{

}

//\brief Copy constructor of class HLAfloat64TimeImpl
//@param other The HLAfloat64TimeImpl object which have to be copied
HLAfloat64TimeImpl::HLAfloat64TimeImpl(const HLAfloat64TimeImpl &other) : HLAfloat64(other)
{
}

//\brief String representation of the HLAfloat64Time
//@return the time in string format
std::__cxx11::wstring HLAfloat64TimeImpl::toString() const
{
    return std::to_wstring(_time);
}

//\brief Overload orpérator = for class HLAfloat64TimeImpl
//@param other The HLAfloat64TimeImpl object to be copied (copy only the time Time and Zero)
HLAfloat64TimeImpl &HLAfloat64TimeImpl::operator=(const HLAfloat64TimeImpl &other)
throw (rti1516e::InvalidLogicalTime)
{
    _time = other._time;
    _zero = other._zero;
    return *this;
}

//\brief Overload orpérator > for class HLAfloat64TimeImpl
//@param other The HLAfloat64TimeImpl object to be compared (compare only the time Time)
//@return True if is higher, false if not
bool HLAfloat64TimeImpl::operator>(const HLAfloat64TimeImpl &other) const
throw (rti1516e::InvalidLogicalTime)
{
    return _time > other._time;
}

//\brief Overload orpérator < for class HLAfloat64TimeImpl
//@param other The HLAfloat64TimeImpl object to be compared (compare only the time Time)
//@return True if is smaller, false if not
bool HLAfloat64TimeImpl::operator<(const HLAfloat64TimeImpl &other) const
throw (rti1516e::InvalidLogicalTime)
{
    return _time < other._time;
}

//\brief Overload orpérator == for class HLAfloat64TimeImpl
//@param other The HLAfloat64TimeImpl object to be compared (compare only the time Time)
//@return True if the time froms both object are equals
bool HLAfloat64TimeImpl::operator==(const HLAfloat64TimeImpl &other) const
throw (rti1516e::InvalidLogicalTime)
{
    return _time == other._time;
}

//\brief Overload orpérator >= for class HLAfloat64TimeImpl
//@param other The HLAfloat64TimeImpl object to be compared (compare only the time Time)
//@return True if is higher or equal, false if not
bool HLAfloat64TimeImpl::operator>=(const HLAfloat64TimeImpl &other) const
throw (rti1516e::InvalidLogicalTime)
{
    return _time >= other._time;
}

//\brief Overload orpérator <= for class HLAfloat64TimeImpl
//@param other The HLAfloat64TimeImpl object to be compared (compare only the time Time)
//@return True if is smaller or equal, false if not
bool HLAfloat64TimeImpl::operator<=(const HLAfloat64TimeImpl &other) const
throw (rti1516e::InvalidLogicalTime)
{
    return _time <= other._time;
}

//\brief Overload cast from HLAfloat64TimeImpl to double (cast the time)
HLAfloat64TimeImpl::operator double() const
{
    return _time;
}

}
