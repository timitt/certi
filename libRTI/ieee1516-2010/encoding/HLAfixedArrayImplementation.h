#include <RTI/encoding/DataElement.h>
#include "HLAarrayImplementation.h"

namespace rti1516e
{

class HLAfixedArrayImplementation : public HLAarrayImplementation
{
public:
    HLAfixedArrayImplementation() = delete;
    HLAfixedArrayImplementation(const DataElement& a_prototype);
    HLAfixedArrayImplementation(const HLAfixedArrayImplementation &a_rhs);
    HLAfixedArrayImplementation& operator=(const HLAfixedArrayImplementation &a_rhs) = delete;
};

}
