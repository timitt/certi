#include <iostream>
#include <bitset>
#include <stdio.h>

#ifdef DEBUG_ENCODE_DECODE
#define g_showingDebugInfo true
#else
#define g_showingDebugInfo false
#endif

#ifdef HOST_IS_BIG_ENDIAN
#define g_isBigEndian true
#else
#define g_isBigEndian false
#endif

enum class Encode { encode, decode, encode_before_swap, encode_after_swap, decode_before_swap, decode_after_swap };

template<class I=uint64_t>
class PrintInfo
{
public:

    PrintInfo(const Encode &a_encode, const char *a_octets, const uint &a_nbOctets)
        : _protoValue(0), _value(_protoValue), _octets(a_octets), _nbOctets(a_nbOctets), _noValue(true)
    {
        reshow(a_encode);
    }

    PrintInfo(const Encode &a_encode, const I &a_int, const char *a_octets, const uint &a_nbOctets)
        : _protoValue(0), _value(a_int), _octets(a_octets), _nbOctets(a_nbOctets), _noValue(false)
    {
        reshow(a_encode);
    }

    void reshow(const Encode &a_encode) const
    {
        if(g_showingDebugInfo)
        {
            if(a_encode == Encode::encode_before_swap)
                std::cout << "-------------------------------------ENCODE BEFORE SWAP--------------------------------------" << std::endl;
            else if(a_encode == Encode::encode_after_swap)
                std::cout << "-------------------------------------ENCODE AFTER SWAP---------------------------------------" << std::endl;
            else if(a_encode == Encode::decode_before_swap)
                std::cout << "-------------------------------------DECODE BEFORE SWAP--------------------------------------" << std::endl;
            else if(a_encode == Encode::decode_after_swap)
                std::cout << "-------------------------------------DECODE AFTER SWAP---------------------------------------" << std::endl;
            else if(a_encode == Encode::encode)
                std::cout << "-------------------------------------------ENCODE--------------------------------------------" << std::endl;
            else if(a_encode == Encode::decode)
                std::cout << "-------------------------------------------DECODE--------------------------------------------" << std::endl;

            if(!_noValue)
            {
                union {
                    I intVal;
                    float floatVal;
                    double doubleVal;
                } val;
                val.intVal = _value;
                std::cout << "value int : " << std::dec << val.intVal << std::endl;
                if(_nbOctets == 4) {
                    std::cout << "value float : " << std::dec << val.floatVal << std::endl;
                }
                else if(_nbOctets == 8) {
                    std::cout << "value double : " << std::dec << val.doubleVal << std::endl;
                }
                std::cout << "value hex : " << std::hex << val.intVal << std::endl;
            }
            std::cout << "bits : " << std::endl;
            if(g_isBigEndian || a_encode == Encode::encode || a_encode == Encode::decode) {
                for(int i=0; i < static_cast<int>(_nbOctets); i++) {
                    std::cout << std::bitset<8>(_octets[i]) << " ";
                }
            }
            else {
                for(int i=static_cast<int>(_nbOctets)-1; i >=0; i--) {
                    std::cout << std::bitset<8>(_octets[i]) << " ";
                }
            }
            std::cout << std::endl;
        }
    }

private:
    const I _protoValue;
    const I &_value;
    const char *_octets;
    const uint &_nbOctets;
    bool _noValue;
};
