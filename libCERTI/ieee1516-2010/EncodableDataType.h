#ifndef ENCODABLE_DATA_TYPE_H
#define ENCODABLE_DATA_TYPE_H

#include <vector>
#include <map>
#include <utility>
#include <memory>

namespace certi {

class EncodableDataType {
public:

    enum class CATEGORY {
        BasicDataType,
        SimpleDataType,
        ArrayDataType,
        FixedRecordDataType,
        VariantRecordDataType,
        EnumeratedDataType,
        NA
    };

    enum class TYPE : int {
        HLAunknown = 0,
        HLAASCIIchar = 1,
        HLAASCIIstring = 2,
        HLAboolean = 3,
        HLAbyte = 4,
        HLAfloat32BE = 5,
        HLAfloat32LE = 6,
        HLAfloat64BE = 7,
        HLAfloat64LE = 8,
        HLAinteger16LE = 9,
        HLAinteger16BE = 10,
        HLAinteger32LE = 11,
        HLAinteger32BE = 12,
        HLAinteger64LE = 13,
        HLAinteger64BE = 14,
        HLAoctet = 15,
        HLAoctetPairLE = 16,
        HLAoctetPairBE = 17,
        HLAunicodeChar = 18,
        HLAunicodeString = 19,
        HLAinteger64Time = 20,
        HLAfloat64Time = 21
    };

    enum class ENDIAN : int {
        Big,
        Little,
        NA
    };

    static std::string toString(const TYPE &a_type);

    static TYPE toType(const std::string &a_type);

    static std::string toString(const ENDIAN &a_endian);

    static ENDIAN toEndian(const std::string &a_endian);

    static std::string toString(const CATEGORY &a_category);

    static CATEGORY toCategory(const std::string &a_category);

    static EncodableDataType* getInstance();

private:

    EncodableDataType(){}

    virtual ~EncodableDataType();

    void initPairType();

    void initTypeMapFromTYPE();

    void initTypeMapFromString();

    void initPairEndian();

    void initEndianMapFromENDIAN();

    void initEndianMapFromString();

    void initPairCategory();

    void initCategoryMapFromCATEGORY();

    void initCategoryMapFromString();

private:

    static EncodableDataType *_pEncodableDataType;

    std::vector<std::pair<TYPE, std::string>> _pairType;
    std::map<TYPE, std::string> _mapTypeFromTYPE;
    std::map<std::string, TYPE> _mapTypeFromString;

    std::vector<std::pair<ENDIAN, std::string>> _pairEndian;
    std::map<ENDIAN, std::string> _mapEndianFromENDIAN;
    std::map<std::string, ENDIAN> _mapEndianFromString;

    std::vector<std::pair<CATEGORY, const char*>> _pairCategory;
    std::map<CATEGORY, const char*> _mapCategoryFromCATEGORY;
    std::map<const char*, CATEGORY> _mapCategoryFromString;

};

}

#endif
