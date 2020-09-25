#include <RTI/time/HLAfloat64TimeFactory.h>
#include <RTI/time/HLAfloat64Time.h>
#include <RTI/time/HLAfloat64Interval.h>

namespace rti1516e {

HLAfloat64TimeFactory::HLAfloat64TimeFactory ()
{

}

HLAfloat64TimeFactory::~HLAfloat64TimeFactory ()
   throw ()
{

}

std::auto_ptr< HLAfloat64Time > HLAfloat64TimeFactory::makeLogicalTime (double value)
   throw (rti1516e::InternalError)
{
    return std::auto_ptr<HLAfloat64Time> (new HLAfloat64Time(value));
}

std::auto_ptr< LogicalTime > HLAfloat64TimeFactory::makeInitial()
   throw (InternalError)
{
    HLAfloat64Time *time = new HLAfloat64Time();
    time->setInitial();
    return std::auto_ptr<LogicalTime> (time);
}

std::auto_ptr< LogicalTime > HLAfloat64TimeFactory::makeFinal()
   throw (InternalError)
{
    HLAfloat64Time *time = new HLAfloat64Time();
    time->setFinal();
    return std::auto_ptr<LogicalTime> (time);
}

std::auto_ptr< HLAfloat64Interval > HLAfloat64TimeFactory::makeLogicalTimeInterval(double value)
   throw (rti1516e::InternalError)
{
    return std::auto_ptr<HLAfloat64Interval> (new HLAfloat64Interval(value));
}

std::auto_ptr< LogicalTimeInterval > HLAfloat64TimeFactory::makeZero()
   throw (InternalError)
{
    return std::auto_ptr<LogicalTimeInterval> (new HLAfloat64Interval(0));
}

std::auto_ptr< LogicalTimeInterval > HLAfloat64TimeFactory::makeEpsilon()
   throw (InternalError)
{
    HLAfloat64Interval *time = new HLAfloat64Interval();
    time->setEpsilon();
    return std::auto_ptr<LogicalTimeInterval> (time);
}

std::auto_ptr< LogicalTime > HLAfloat64TimeFactory::decodeLogicalTime (VariableLengthData const & encodedLogicalTime)
   throw (InternalError,
   CouldNotDecode)
{
    HLAfloat64Time *time = new HLAfloat64Time;
    time->decode(encodedLogicalTime);
    return std::auto_ptr<LogicalTime> (time);
}

std::auto_ptr< LogicalTime > HLAfloat64TimeFactory::decodeLogicalTime(void* buffer, size_t bufferSize)
   throw (InternalError, CouldNotDecode)
{
    HLAfloat64Time *time = new HLAfloat64Time;
    time->decode(buffer, bufferSize);
    return std::auto_ptr<LogicalTime> (time);
}

std::auto_ptr< LogicalTimeInterval > HLAfloat64TimeFactory::decodeLogicalTimeInterval(VariableLengthData const & encodedValue)
   throw (InternalError, CouldNotDecode)
{
    HLAfloat64Interval *time = new HLAfloat64Interval;
    time->decode(encodedValue);
    return std::auto_ptr<LogicalTimeInterval> (time);
}

std::auto_ptr< LogicalTimeInterval > HLAfloat64TimeFactory::decodeLogicalTimeInterval(void* buffer, size_t bufferSize)
   throw (InternalError, CouldNotDecode)
{
    HLAfloat64Interval *time = new HLAfloat64Interval;
    time->decode(buffer, bufferSize);
    return std::auto_ptr<LogicalTimeInterval> (time);
}

std::wstring HLAfloat64TimeFactory::getName () const
{
    return std::wstring (L"HLAfloat64TimeFactory");
}

}
