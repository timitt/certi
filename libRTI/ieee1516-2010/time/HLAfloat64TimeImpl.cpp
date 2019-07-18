#include "HLAfloat64TimeImpl.h"
#include <thread>
#include <iostream>

namespace rti1516e
{

HLAfloat64TimeImpl::HLAfloat64TimeImpl() : _epsilon(std::numeric_limits<double>::epsilon())
{

}

}
