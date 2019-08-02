#include <RTI/time/HLAinteger64TimeFactory.h>
#include <RTI/time/HLAinteger64Time.h>
#include <RTI/time/HLAinteger64Interval.h>

namespace rti1516e {

HLAinteger64TimeFactory::HLAinteger64TimeFactory ()
{
}

HLAinteger64TimeFactory::~HLAinteger64TimeFactory ()
   throw()
{

}

std::auto_ptr< LogicalTime > HLAinteger64TimeFactory::makeInitial()
   throw (InternalError)
{
    HLAinteger64Time *time = new HLAinteger64Time();
    time->setInitial();
    return std::auto_ptr<LogicalTime> (time);
}

std::auto_ptr< LogicalTime > HLAinteger64TimeFactory::makeFinal()
   throw (InternalError)
{
    HLAinteger64Time *time = new HLAinteger64Time();
    time->setFinal();
    return std::auto_ptr<LogicalTime> (time);
}

std::auto_ptr< LogicalTimeInterval > HLAinteger64TimeFactory::makeZero()
   throw (InternalError)
{
    return std::auto_ptr<LogicalTimeInterval> (new HLAinteger64Interval(0));
}

std::auto_ptr< LogicalTimeInterval > HLAinteger64TimeFactory::makeEpsilon()
   throw (InternalError)
{
    HLAinteger64Interval *time = new HLAinteger64Interval();
    time->setEpsilon();
    return std::auto_ptr<LogicalTimeInterval> (time);
}

std::auto_ptr< HLAinteger64Time > HLAinteger64TimeFactory::makeLogicalTime(Integer64 value)
   throw (rti1516e::InternalError)
{
    return std::auto_ptr<HLAinteger64Time> (new HLAinteger64Time(value));
}

std::auto_ptr< HLAinteger64Interval > HLAinteger64TimeFactory::makeLogicalTimeInterval(Integer64 value)
   throw (rti1516e::InternalError)
{
    return std::auto_ptr<HLAinteger64Interval> (new HLAinteger64Interval(value));
}

std::auto_ptr< LogicalTime > HLAinteger64TimeFactory::decodeLogicalTime(VariableLengthData const & encodedLogicalTime)
   throw (InternalError, CouldNotDecode)
{
    HLAinteger64Time *time = new HLAinteger64Time;
    time->decode(encodedLogicalTime);
    return std::auto_ptr<LogicalTime> (time);
}

std::auto_ptr< LogicalTime > HLAinteger64TimeFactory::decodeLogicalTime(void* buffer, size_t bufferSize)
   throw (InternalError, CouldNotDecode)
{
    HLAinteger64Time *time = new HLAinteger64Time;
    time->decode(buffer, bufferSize);
    return std::auto_ptr<LogicalTime> (time);
}

std::auto_ptr< LogicalTimeInterval > HLAinteger64TimeFactory::decodeLogicalTimeInterval(VariableLengthData const & encodedValue)
   throw (InternalError, CouldNotDecode)
{
    HLAinteger64Interval *time = new HLAinteger64Interval();
    time->decode(encodedValue);
    return std::auto_ptr<LogicalTimeInterval> (time);
}

std::auto_ptr< LogicalTimeInterval > HLAinteger64TimeFactory::decodeLogicalTimeInterval(void* buffer, size_t bufferSize)
   throw (InternalError, CouldNotDecode)
{
    HLAinteger64Interval *time = new HLAinteger64Interval();
    time->decode(buffer, bufferSize);
    return std::auto_ptr<LogicalTimeInterval> (time);
}

std::wstring HLAinteger64TimeFactory::getName () const
{
   return std::wstring(L"HLAinteger64TimeFactory");
}

}
