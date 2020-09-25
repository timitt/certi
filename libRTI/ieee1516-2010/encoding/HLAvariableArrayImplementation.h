#include <memory>
#include <RTI/encoding/DataElement.h>
#include "HLAarrayImplementation.h"

namespace rti1516e
{

class HLAvariableArrayImplementation : public HLAarrayImplementation
{
public:
    HLAvariableArrayImplementation() = delete;
    HLAvariableArrayImplementation(const DataElement& a_prototype);
    HLAvariableArrayImplementation(const HLAvariableArrayImplementation &a_rhs);
    HLAvariableArrayImplementation& operator=(const HLAvariableArrayImplementation &a_rhs) = delete;

    void addElement(const DataElement &a_dataElement) throw (EncoderException);
    void addElementPointer(DataElement* a_dataElement) throw (EncoderException);

    void encodeInto(std::vector<Octet> &a_buffer) const throw(EncoderException);
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index) throw (EncoderException);

};

}
