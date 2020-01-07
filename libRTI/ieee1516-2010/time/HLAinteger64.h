#include <RTI/encoding/BasicDataElements.h>
#include <chrono>
#include <cstring>

namespace rti1516e
{

//\brief Class that implement basic methods for HLAInteger64 implementation
class HLAinteger64 {
public:
    HLAinteger64();
    HLAinteger64(const Integer64& value);
    HLAinteger64(const HLAinteger64 &other);
    void setInitial();
    bool isInitial() const;
    void setFinal();
    bool isFinal() const;
    Integer64 getTime() const;
    void setTime(Integer64 value);
    std::wstring toString() const;

    size_t encode(void *buffer, size_t bufferSize) const
        throw (rti1516e::CouldNotEncode);
    void decode(void *buffer, size_t bufferSize)
    throw (rti1516e::InternalError,
           rti1516e::CouldNotDecode);

protected:
    Integer64 _time;
    Integer64 _zero;
    const Integer64 _epsilon = std::numeric_limits<Integer64>::epsilon();
    const Integer64 _positiveInfinity = std::numeric_limits<Integer64>::infinity();
};

}
