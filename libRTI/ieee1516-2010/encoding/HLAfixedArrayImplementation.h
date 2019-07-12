#include <RTI/encoding/DataElement.h>
#include "HLAarrayImplementation.h"

namespace rti1516e
{

class HLAfixedArrayImplementation : public HLAarrayImplementation
{
public:
    HLAfixedArrayImplementation() = delete;
    HLAfixedArrayImplementation(const DataElement& a_prototype, size_t a_length);
    HLAfixedArrayImplementation(const HLAfixedArrayImplementation &a_rhs);
    HLAfixedArrayImplementation& operator=(const HLAfixedArrayImplementation &a_rhs) = delete;
    virtual ~HLAfixedArrayImplementation();

    void encodeInto(std::vector<Octet> &a_buffer) const throw(EncoderException);
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index) throw (EncoderException);

private:
    size_t _length;
};

}
