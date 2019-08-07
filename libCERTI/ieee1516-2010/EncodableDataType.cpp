#include "EncodableDataType.h"

certi::EncodableDataType* certi::EncodableDataType::_pEncodableDataType=nullptr;

std::__cxx11::string certi::EncodableDataType::toString(const certi::EncodableDataType::TYPE &a_type) {
    _pEncodableDataType->initTypeMapFromTYPE();
    return getInstance()->_mapTypeFromTYPE.at(a_type);
}

certi::EncodableDataType::TYPE certi::EncodableDataType::toType(const std::__cxx11::string &a_type) {
    _pEncodableDataType->initTypeMapFromString();
    if(getInstance()->_mapTypeFromString.find(a_type) != getInstance()->_mapTypeFromString.end())
        return getInstance()->_mapTypeFromString.at(a_type);
    else
        return getInstance()->_mapTypeFromString.at("HLAunknown");
}

std::__cxx11::string certi::EncodableDataType::toString(const certi::EncodableDataType::ENDIAN &a_endian) {
    _pEncodableDataType->initEndianMapFromENDIAN();
    return getInstance()->_mapEndianFromENDIAN.at(a_endian);
}

certi::EncodableDataType::ENDIAN certi::EncodableDataType::toEndian(const std::__cxx11::string &a_endian) {
    _pEncodableDataType->initEndianMapFromString();
    if(getInstance()->_mapEndianFromString.find(a_endian) != getInstance()->_mapEndianFromString.end())
        return getInstance()->_mapEndianFromString.at(a_endian);
    else
        return getInstance()->_mapEndianFromString.at("NA");
}

std::__cxx11::string certi::EncodableDataType::toString(const certi::EncodableDataType::CATEGORY &a_category) {
    _pEncodableDataType->initCategoryMapFromCATEGORY();
    return getInstance()->_mapCategoryFromCATEGORY.at(a_category);
}

certi::EncodableDataType::CATEGORY certi::EncodableDataType::toCategory(const std::__cxx11::string &a_category) {
    _pEncodableDataType->initCategoryMapFromString();
    if(getInstance()->_mapCategoryFromString.find(a_category.c_str()) != getInstance()->_mapCategoryFromString.end())
        return getInstance()->_mapCategoryFromString.at(a_category.c_str());
    else
        return getInstance()->_mapCategoryFromString.at("NA");
}

certi::EncodableDataType* certi::EncodableDataType::getInstance()
{
    if(!_pEncodableDataType) {
        _pEncodableDataType = new EncodableDataType();
    }

    return _pEncodableDataType;
}

certi::EncodableDataType::~EncodableDataType()
{
    if(_pEncodableDataType) {
        delete _pEncodableDataType;
        _pEncodableDataType = nullptr;
    }
}


void certi::EncodableDataType::initPairType() {
    if(getInstance()->_pairType.size() == 0) {
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAunknown, "HLAunknown"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAASCIIchar, "HLAASCIIchar"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAASCIIstring, "HLAASCIIstring"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAboolean, "HLAboolean"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAbyte, "HLAbyte"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAfloat32BE, "HLAfloat32BE"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAfloat32LE, "HLAfloat32LE"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAfloat64BE, "HLAfloat64BE"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAfloat64LE, "HLAfloat64LE"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger16LE, "HLAinteger16LE"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger16BE, "HLAinteger16BE"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger32LE, "HLAinteger32LE"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger32BE, "HLAinteger32BE"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger64LE, "HLAinteger64LE"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger64BE, "HLAinteger64BE"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAoctet, "HLAoctet"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAoctetPairLE, "HLAoctetPairLE"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAoctetPairBE, "HLAoctetPairBE"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAunicodeChar, "HLAunicodeChar"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAunicodeString, "HLAunicodeString"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAinteger64Time, "HLAinteger64Time"));
        getInstance()->_pairType.push_back(std::pair<TYPE, std::string>(TYPE::HLAfloat64Time, "HLAfloat64Time"));
    }
}

void certi::EncodableDataType::initTypeMapFromTYPE() {
    initPairType();
    if(getInstance()->_mapTypeFromTYPE.size() == 0) {
        for(auto pair : getInstance()->_pairType) {
            getInstance()->_mapTypeFromTYPE.emplace(pair.first, pair.second);
        }
    }
}

void certi::EncodableDataType::initTypeMapFromString() {
    initPairType();
    if(getInstance()->_mapTypeFromString.size() == 0) {
        for(auto pair : getInstance()->_pairType) {
            getInstance()->_mapTypeFromString.emplace(pair.second, pair.first);
        }
    }
}

void certi::EncodableDataType::initPairEndian() {
    if(getInstance()->_pairEndian.size() == 0) {
        getInstance()->_pairEndian.push_back(std::pair<ENDIAN, std::string>(ENDIAN::Big, "Big"));
        getInstance()->_pairEndian.push_back(std::pair<ENDIAN, std::string>(ENDIAN::Little, "Little"));
        getInstance()->_pairEndian.push_back(std::pair<ENDIAN, std::string>(ENDIAN::NA, "NA"));
    }
}

void certi::EncodableDataType::initEndianMapFromENDIAN() {
    initPairEndian();
    if(getInstance()->_mapEndianFromENDIAN.size() == 0) {
        for(auto pair : getInstance()->_pairEndian) {
            getInstance()->_mapEndianFromENDIAN.emplace(pair.first, pair.second);
        }
    }
}

void certi::EncodableDataType::initEndianMapFromString() {
    initPairEndian();
    if(getInstance()->_mapEndianFromString.size() == 0) {
        for(auto pair : getInstance()->_pairEndian) {
            getInstance()->_mapEndianFromString.emplace(pair.second, pair.first);
        }
    }
}

void certi::EncodableDataType::initPairCategory() {
    std::pair<CATEGORY, const char*> test(CATEGORY::BasicDataType, "basicDataRepresentations");
    test.second = "test";
    std::pair<CATEGORY, int> test2(CATEGORY::BasicDataType, 10);
    test2.second = 20;
    if(getInstance()->_pairCategory.size() == 0) {
        getInstance()->_pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::BasicDataType, "basicDataRepresentations"));
        getInstance()->_pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::SimpleDataType, "simpleDataTypes"));
        getInstance()->_pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::EnumeratedDataType, "enumeratedDataTypes"));
        getInstance()->_pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::ArrayDataType, "arrayDataTypes"));
        getInstance()->_pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::FixedRecordDataType, "fixedRecordDataTypes"));
        getInstance()->_pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::VariantRecordDataType, "variantRecordDataTypes"));
        getInstance()->_pairCategory.push_back(std::pair<CATEGORY, const char*>(CATEGORY::NA, "NA"));
    }
}

void certi::EncodableDataType::initCategoryMapFromCATEGORY() {
    initPairCategory();
    if(getInstance()->_mapCategoryFromCATEGORY.size() == 0) {
        for(auto pair : getInstance()->_pairCategory) {
            getInstance()->_mapCategoryFromCATEGORY.emplace(pair.first, pair.second);
        }
    }
}

void certi::EncodableDataType::initCategoryMapFromString() {
    initPairCategory();
    if(getInstance()->_mapCategoryFromString.size() == 0) {
        for(auto pair : getInstance()->_pairCategory) {
            getInstance()->_mapCategoryFromString.emplace(pair.second, pair.first);
        }
    }
}
