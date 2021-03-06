/***********************************************************************
  IEEE 1516.1 High Level Architecture Interface Specification C++ API
  File: RTI/LogicalTimeInterval.h
***********************************************************************/

#ifndef RTI_LogicalTimeInterval_h
#define RTI_LogicalTimeInterval_h

// ****************************************************************
// ********************** Adjusted for CERTI **********************
// ****************************************************************
// This file is one of the SISO HLA 1516 header files that was edited
// by CERTI for compilation. By default, there exists a circular
// dependency between the RTI library (provided by CERTI) and the
// LogicalTime implementation (to be provided by the federate).
// In order to break this dependency some minor changes have been
// applied: The LogicalTime, LogicalTimeInterval and 
// LogicalTimeFactory classes are now exported by the LogicalTime 
// implementation. All three classes have been given an inline (empty)
// destructor.
// While these changes may break (direct) DLL compatibility, they
// greatly ease the implementation of the RTI1516 library for CERTI.
// ****************************************************************
// ********************** Adjusted for CERTI **********************
// ****************************************************************


// The classes associated with logical time allow a federation to provide
// their own representation for logical time and logical time interval. The
// federation is responsible to inherit from the abstract classes declared
// below. The encoded time classes are used to hold the arbitrary bit
// representation of the logical time and logical time intervals.

namespace rti1516e
{
  class LogicalTime;
}

#include <RTI/SpecificConfig.h>
#include <RTI/Exception.h>
#include <string>
#include <RTI/VariableLengthData.h>

namespace rti1516e
{
  class RTI_EXPORT_FEDTIME LogicalTimeInterval
  {
  public:
    virtual
    ~LogicalTimeInterval()
      throw () { };

    virtual
    void
    setZero() = 0;

    virtual
    bool
    isZero() const = 0;
  
    virtual
    void
    setEpsilon() = 0;

    virtual
    bool
    isEpsilon() const = 0;

    virtual
    LogicalTimeInterval &
    operator=(LogicalTimeInterval const & value)
      throw (InvalidLogicalTimeInterval) = 0;

    // Set self to the difference between two LogicalTimes
    virtual
    void
    setToDifference(LogicalTime const & minuend,
      LogicalTime const& subtrahend)
      throw (InvalidLogicalTime) = 0;

    virtual
    LogicalTimeInterval &
    operator+=(LogicalTimeInterval const & addend)
      throw (InvalidLogicalTimeInterval) = 0;

    virtual
    LogicalTimeInterval &
    operator-=(LogicalTimeInterval const & subtrahend)
      throw (InvalidLogicalTimeInterval) = 0;
    
    virtual
    bool
    operator>(LogicalTimeInterval const & value) const
      throw (InvalidLogicalTimeInterval) = 0;

    virtual
    bool
    operator<(LogicalTimeInterval const & value) const
      throw (InvalidLogicalTimeInterval) = 0;

    virtual
    bool
    operator==(LogicalTimeInterval const & value) const
      throw (InvalidLogicalTimeInterval) = 0;

    virtual
    bool
    operator>=(LogicalTimeInterval const & value) const
      throw (InvalidLogicalTimeInterval) = 0;

    virtual
    bool
    operator<=(LogicalTimeInterval const & value) const
      throw (InvalidLogicalTimeInterval) = 0;
    
    // Generates an encoded value that can be used to send
    // LogicalTimeIntervals to other federates in updates or interactions
    virtual VariableLengthData encode() const = 0;

    // Alternate encode for directly filling a buffer
    virtual size_t encodedLength() const = 0;
    virtual size_t encode(void* buffer, size_t bufferSize) const 
       throw (CouldNotEncode) = 0;

    // Decode encodedValue into self
    virtual void decode(VariableLengthData const & encodedValue)
      throw (InternalError,
             CouldNotDecode) = 0;

    // Alternate decode that reads directly from a buffer
    virtual void decode(void* buffer, size_t bufferSize)
      throw (InternalError,
             CouldNotDecode) = 0;

    virtual std::wstring toString() const = 0;

    // Returns the name of the implementation, as needed by
    // createFederationExecution.
    virtual std::wstring implementationName() const = 0;
  };

  // Output operator for LogicalTimeInterval
  std::wostream RTI_EXPORT &
    operator << (std::wostream &, LogicalTimeInterval const &);
}
#endif // RTI_LogicalTimeInterval_h
