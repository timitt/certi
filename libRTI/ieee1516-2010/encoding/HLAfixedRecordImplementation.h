#include <vector>
#include <RTI/encoding/DataElement.h>

namespace rti1516e {

class HLAfixedRecordImplementation {

public:
    HLAfixedRecordImplementation();
    HLAfixedRecordImplementation(const HLAfixedRecordImplementation & a_rhs);
    virtual ~HLAfixedRecordImplementation();

    bool isSameTypeAs(HLAfixedRecordImplementation const& a_rhs) const;
    bool hasElementSameTypeAs(size_t a_index, DataElement const& a_inData ) const;

    size_t size () const;

    void appendElement(const DataElement& a_dataElement);
    void appendElementPointer(DataElement* a_pdataElement) throw (EncoderException);
    void set(size_t a_index, const DataElement& a_dataElement) throw (EncoderException);
    void setElementPointer(size_t a_index, DataElement* a_pDataElement) throw (EncoderException);
    const DataElement& get(size_t a_index) const throw (EncoderException);

    size_t getEncodedLength () const throw (EncoderException);
    unsigned int getOctetBoundary () const;

    void encodeInto(std::vector<Octet> &a_buffer) const throw(EncoderException);
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index) throw (EncoderException);

private:
    unsigned int calculPaddingAfterEachElements(unsigned int &a_offset, const unsigned int a_index) const;

private:
    std::vector<DataElement*> _vectorpDataElement;

};

}
