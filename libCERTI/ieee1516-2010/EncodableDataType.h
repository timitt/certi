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

//    std::string toStringType(const certi::EncodableDataType::TYPE &&a_type) ;

//    TYPE toType(const std::string &a_type) ;

//    std::string toString(const ENDIAN &a_endian) ;

//    ENDIAN toEndian(const std::string &a_endian) ;

//    std::string toString(const CATEGORY &a_category) ;

//    CATEGORY toCategory(const std::string &a_category) ;

//    static EncodableDataType* getInstance();

    std::string toStringType(const TYPE &&a_type) {
        initTypeMapFromTYPE();
        return _mapTypeFromTYPE.at(a_type);
    }

    TYPE toType(const std::__cxx11::string &a_type) {
        initTypeMapFromString();
        if(_mapTypeFromString.find(a_type) != _mapTypeFromString.end())
            return _mapTypeFromString.at(a_type);
        else
            return _mapTypeFromString.at("HLAunknown");
    }

    std::__cxx11::string toString(const ENDIAN &a_endian) {
        initEndianMapFromENDIAN();
        return _mapEndianFromENDIAN.at(a_endian);
    }

    ENDIAN toEndian(const std::__cxx11::string &a_endian) {
        initEndianMapFromString();
        if(_mapEndianFromString.find(a_endian) != _mapEndianFromString.end())
            return _mapEndianFromString.at(a_endian);
        else
            return _mapEndianFromString.at("NA");
    }

    std::__cxx11::string toString(const CATEGORY &a_category) {
        initCategoryMapFromCATEGORY();
        return _mapCategoryFromCATEGORY.at(a_category);
    }

    CATEGORY toCategory(const std::__cxx11::string &a_category) {
        initCategoryMapFromString();
        if(_mapCategoryFromString.find(a_category.c_str()) != _mapCategoryFromString.end())
            return _mapCategoryFromString.at(a_category.c_str());
        else
            return _mapCategoryFromString.at("NA");
    }

    EncodableDataType(){}

    virtual ~EncodableDataType(){}

private:   

//    void initPairType();

//    void initTypeMapFromTYPE();

//    void initTypeMapFromString();

//    void initPairEndian();

//    void initEndianMapFromENDIAN();

//    void initEndianMapFromString();

//    void initPairCategory();

//    void initCategoryMapFromCATEGORY();

//    void initCategoryMapFromString();

    void initPairType() {
        if(_pairType.size() == 0) {
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAunknown, "HLAunknown"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAASCIIchar, "HLAASCIIchar"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAASCIIstring, "HLAASCIIstring"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAboolean, "HLAboolean"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAbyte, "HLAbyte"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAfloat32BE, "HLAfloat32BE"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAfloat32LE, "HLAfloat32LE"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAfloat64BE, "HLAfloat64BE"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAfloat64LE, "HLAfloat64LE"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger16LE, "HLAinteger16LE"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger16BE, "HLAinteger16BE"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger32LE, "HLAinteger32LE"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger32BE, "HLAinteger32BE"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger64LE, "HLAinteger64LE"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger64BE, "HLAinteger64BE"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAoctet, "HLAoctet"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAoctetPairLE, "HLAoctetPairLE"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAoctetPairBE, "HLAoctetPairBE"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAunicodeChar, "HLAunicodeChar"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAunicodeString, "HLAunicodeString"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger64Time, "HLAinteger64Time"));
            _pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAfloat64Time, "HLAfloat64Time"));
        }
    }

    void initTypeMapFromTYPE() {
        initPairType();
        if(_mapTypeFromTYPE.size() == 0) {
            for(auto pair : _pairType) {
                _mapTypeFromTYPE.emplace(pair.first, pair.second);
            }
        }
    }

    void initTypeMapFromString() {
        initPairType();
        if(_mapTypeFromString.size() == 0) {
            for(auto pair : _pairType) {
                _mapTypeFromString.emplace(pair.second, pair.first);
            }
        }
    }

    void initPairEndian() {
        if(_pairEndian.size() == 0) {
            _pairEndian.push_back(std::pair<ENDIAN, std::string>(ENDIAN::Big, "Big"));
            _pairEndian.push_back(std::pair<ENDIAN, std::string>(ENDIAN::Little, "Little"));
            _pairEndian.push_back(std::pair<ENDIAN, std::string>(ENDIAN::NA, "NA"));
        }
    }

    void initEndianMapFromENDIAN() {
        initPairEndian();
        if(_mapEndianFromENDIAN.size() == 0) {
            for(auto pair : _pairEndian) {
                _mapEndianFromENDIAN.emplace(pair.first, pair.second);
            }
        }
    }

    void initEndianMapFromString() {
        initPairEndian();
        if(_mapEndianFromString.size() == 0) {
            for(auto pair : _pairEndian) {
                _mapEndianFromString.emplace(pair.second, pair.first);
            }
        }
    }

    void initPairCategory() {
        std::pair<CATEGORY, const char*> test(CATEGORY::BasicDataType, "basicDataRepresentations");
        test.second = "test";
        std::pair<CATEGORY, int> test2(CATEGORY::BasicDataType, 10);
        test2.second = 20;
        if(_pairCategory.size() == 0) {
            _pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::BasicDataType, "basicDataRepresentations"));
            _pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::SimpleDataType, "simpleDataTypes"));
            _pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::EnumeratedDataType, "enumeratedDataTypes"));
            _pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::ArrayDataType, "arrayDataTypes"));
            _pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::FixedRecordDataType, "fixedRecordDataTypes"));
            _pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::VariantRecordDataType, "variantRecordDataTypes"));
            _pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::NA, "NA"));
        }
    }

    void initCategoryMapFromCATEGORY() {
        initPairCategory();
        if(_mapCategoryFromCATEGORY.size() == 0) {
            for(auto pair : _pairCategory) {
                _mapCategoryFromCATEGORY.emplace(pair.first, pair.second);
            }
        }
    }

    void initCategoryMapFromString() {
        initPairCategory();
        if(_mapCategoryFromString.size() == 0) {
            for(auto pair : _pairCategory) {
                _mapCategoryFromString.emplace(pair.second, pair.first);
            }
        }
    }

private:

//    static EncodableDataType *_pEncodableDataType;

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
