#include <gtest/gtest.h>

#include <libCERTI/RootObject.hh>
#include <libCERTI/ObjectClassSet.hh>
#include <libCERTI/ObjectClassAttribute.hh>
#include <libCERTI/ieee1516-2010/ObjectClassAttribute1516e.h>
#include <libCERTI/InteractionSet.hh>
#include <libCERTI/ieee1516-2010/Parameter1516e.h>
#include <libCERTI/ieee1516-2010/BasicDataType.h>
#include <libCERTI/ieee1516-2010/SimpleDataType.h>
#include <libCERTI/ieee1516-2010/EnumeratedDataType.h>
#include <libCERTI/ieee1516-2010/ArrayDataType.h>
#include <libCERTI/ieee1516-2010/FixedRecordDataType.h>
#include <libCERTI/ieee1516-2010/VariantRecordDataType.h>
#include <libCERTI/XmlParser2010.hh>
#include <fstream>

//#define CERTI_SOURCE_DIR @CERTI_SOURCE_DIR@

namespace certi {

class ObjectClassSetCustom : public ObjectClassSet
{
public:
    ObjectClassSetCustom() : ObjectClassSet(NULL) {}
    ~ObjectClassSetCustom();

    Name2ObjectMap_t getMapFromName() {
        return fromName;
    }

    Handle2ObjectMap_t getMapFromHandle() {
        return fromHandle;
    }
};

class ObjectInteractionSetCustom : public InteractionSet
{
public:
    ObjectInteractionSetCustom() : InteractionSet(NULL) {}
    ~ObjectInteractionSetCustom();

    Name2ObjectMap_t getMapFromName() {
        return fromName;
    }

    Handle2ObjectMap_t getMapFromHandle() {
        return fromHandle;
    }
};

}

void print_state(const std::ios& stream) {
  std::cout << " good()=" << stream.good();
  std::cout << " eof()=" << stream.eof();
  std::cout << " fail()=" << stream.fail();
  std::cout << " bad()=" << stream.bad();
  std::cout << std::endl;
}

TEST(XMLparserTest, CreateFederationTest)
{
    certi::RootObject rootObject;

    certi::XmlParser2010 parser(&rootObject);

    std::string fomPath = std::string(SOURCE_DIR) + "/tests/LibCERTI/Base.xml";
    std::cout << "PATH : " << fomPath << std::endl;

    std::ifstream try_to_open(fomPath);
    print_state(try_to_open);
    parser.parse(fomPath);

    for(auto handleObject : static_cast<certi::ObjectClassSetCustom*>(rootObject.ObjectClasses)->getMapFromHandle()) {
        std::cout << "Handle object class : " << handleObject.first << std::endl;
        auto objectClass = rootObject.ObjectClasses->getObjectFromHandle(handleObject.first);
        std::cout << "Name object class : " << objectClass->getName() << std::endl;
        for(auto classAttributes : objectClass->getHandleClassAttributeMap()) {
            std::cout << "\tAttributes handle : " << classAttributes.first << std::endl;

            certi::ObjectClassAttribute1516e* classAttribute = objectClass->getAttribute1516e(classAttributes.first);
            std::cout << "\tAttributes name : " << classAttribute->getName() << std::endl;

            if(classAttribute->getType()->category() == certi::EncodableDataType::CATEGORY::BasicDataType) {
                auto basicDataType = static_cast<certi::BasicDataType*>(classAttribute->getType().get());
                std::cout << "\t\tAttributes type name : " << basicDataType->nameStr() << std::endl;
                ASSERT_EQ("HLAinteger16LE", basicDataType->nameStr());
                std::cout << "\t\tAttributes type size : " << basicDataType->size() << std::endl;
                ASSERT_EQ(16, basicDataType->size());
                std::cout << "\t\tAttributes type interpretation : " << basicDataType->interpretation() << std::endl;
                ASSERT_EQ("Integer in the range [-2^15, 2^15 - 1]", basicDataType->interpretation());
                std::cout << "\t\tAttributes type encoding : " << basicDataType->encoding() << std::endl;
                ASSERT_EQ("16-bit two's complement signed integer. The most significant bit contains the sign.", basicDataType->encoding());
            }
            else if(classAttribute->getType()->category() == certi::EncodableDataType::CATEGORY::SimpleDataType) {
                auto simpleDataType = static_cast<certi::SimpleDataType*>(classAttribute->getType().get());
                std::cout << "\t\tAttributes type name : " << simpleDataType->nameStr() << std::endl;
                ASSERT_EQ("HLAbyte", simpleDataType->nameStr());
                std::cout << "\t\tAttributes type representation : " << simpleDataType->representation()->nameStr() << std::endl;
                ASSERT_EQ("HLAoctet", simpleDataType->representation()->nameStr());
                std::cout << "\t\tAttributes type units : " << simpleDataType->units() << std::endl;
                ASSERT_EQ("Byte", simpleDataType->units());
                std::cout << "\t\tAttributes type resolution : " << simpleDataType->resolution() << std::endl;
                ASSERT_EQ("1", simpleDataType->resolution());
                std::cout << "\t\tAttributes type accuracy : " << simpleDataType->accuracy() << std::endl;
                ASSERT_EQ("8", simpleDataType->accuracy());
                std::cout << "\t\tAttributes type semantics : " << simpleDataType->semantics() << std::endl;
                ASSERT_EQ("Uninterpreted 8-bit byte", simpleDataType->semantics());
            }
            else if(classAttribute->getType()->category() == certi::EncodableDataType::CATEGORY::ArrayDataType) {
                auto arrayDataType = static_cast<certi::ArrayDataType*>(classAttribute->getType().get());
                std::cout << "\t\tAttributes type name : " << arrayDataType->nameStr() << std::endl;
                ASSERT_EQ("HLAunicodeString", arrayDataType->nameStr());
                std::cout << "\t\tAttributes dataType : " << arrayDataType->typeElements()->nameStr() << std::endl;
                ASSERT_EQ("HLAunicodeChar", arrayDataType->typeElements()->nameStr());
                std::cout << "\t\tAttributes caridinalité : " << arrayDataType->cardinality() << std::endl;
                ASSERT_EQ("Dynamic", arrayDataType->cardinality());
                std::cout << "\t\tAttributes type encoding : " << arrayDataType->encoding() << std::endl;
                ASSERT_EQ("HLAvariableArray", arrayDataType->encoding());
                std::cout << "\t\tAttributes type semantics : " << arrayDataType->semantics() << std::endl;
                ASSERT_EQ("Unicode string representation", arrayDataType->semantics());
            }
            else if(classAttribute->getType()->category() == certi::EncodableDataType::CATEGORY::FixedRecordDataType) {
                auto fixedRecordDataType = static_cast<certi::FixedRecordDataType*>(classAttribute->getType().get());
                std::cout << "\t\tAttributes type name : " << fixedRecordDataType->nameStr() << std::endl;
                ASSERT_EQ("HLAinteractionSubscription", fixedRecordDataType->nameStr());
                std::cout << "\t\tAttributes type encoding : " << fixedRecordDataType->encoding() << std::endl;
                ASSERT_EQ("HLAfixedRecord", fixedRecordDataType->encoding());
                std::cout << "\t\tAttributes type semantics : " << fixedRecordDataType->semantics() << std::endl;
                ASSERT_EQ("Interaction subscription information", fixedRecordDataType->semantics());

                certi::Field field1 = fixedRecordDataType->fields()[0];

                    std::cout << "\t\t\tField name : " << field1.name() << std::endl;
                    ASSERT_EQ("HLAinteractionClass", field1.name());
                    std::cout << "\t\t\tField dataType : " << field1.typeElem()->nameStr() << std::endl;
                    ASSERT_EQ("HLAhandle", field1.typeElem()->nameStr());
                    std::cout << "\t\t\tField semantics : " << field1.semantics() << std::endl;
                    ASSERT_EQ("Encoded interaction class handle", field1.semantics());

                certi::Field field2 = fixedRecordDataType->fields()[1];

                    std::cout << "\t\t\tField name : " << field2.name() << std::endl;
                    ASSERT_EQ("HLAactive", field2.name());
                    std::cout << "\t\t\tField dataType : " << field2.typeElem()->nameStr() << std::endl;
                    ASSERT_EQ("HLAbyte", field2.typeElem()->nameStr());
                    std::cout << "\t\t\tField semantics : " << field2.semantics() << std::endl;
                    ASSERT_EQ("Whether subscription is active (HLAtrue=active)", field2.semantics());
            }
            else if(classAttribute->getType()->category() == certi::EncodableDataType::CATEGORY::EnumeratedDataType) {
                auto enumeratedRecordDataType = static_cast<certi::EnumeratedDataType*>(classAttribute->getType().get());
                std::cout << "\t\tAttributes type name : " << enumeratedRecordDataType->nameStr() << std::endl;
                ASSERT_EQ("ActiveSonarScanPatternEnum16", enumeratedRecordDataType->nameStr());
                std::cout << "\t\tAttributes type representation : " << enumeratedRecordDataType->representation() << std::endl;
                ASSERT_EQ("HLAinteger16BE", enumeratedRecordDataType->representation()->nameStr());
                std::cout << "\t\tAttributes type semantics : " << enumeratedRecordDataType->semantics() << std::endl;
                ASSERT_EQ("-NULL-", enumeratedRecordDataType->semantics());

                std::shared_ptr<certi::Enumerator> enumerator1 = enumeratedRecordDataType->getEnumerator("ScanPatternNotUsed");

                    std::cout << "\t\t\tEnumerator name : " << enumerator1->name() << std::endl;
                    ASSERT_EQ("ScanPatternNotUsed", enumerator1->name());
                    std::cout << "\t\t\tEnumerator value : " << enumerator1->value() << std::endl;
                    ASSERT_EQ("0", enumerator1->value());

                std::shared_ptr<certi::Enumerator> enumerator2 = enumeratedRecordDataType->getEnumerator("Conical");;

                    std::cout << "\t\t\tEnumerator name : " << enumerator2->name() << std::endl;
                    ASSERT_EQ("Conical", enumerator2->name());
                    std::cout << "\t\t\tEnumerator value : " << enumerator2->value() << std::endl;
                    ASSERT_EQ("1", enumerator2->value());
            }
            else if(classAttribute->getType()->category() == certi::EncodableDataType::CATEGORY::VariantRecordDataType) {
                auto variantRecordDataType = static_cast<certi::VariantRecordDataType*>(classAttribute->getType().get());
                std::cout << "\t\tAttributes type name : " << variantRecordDataType->nameStr() << std::endl;
                ASSERT_EQ("SpatialStruct", variantRecordDataType->nameStr());
                std::cout << "\t\tAttributes type discriminent : " << variantRecordDataType->discriminantName() << std::endl;
                ASSERT_EQ("DeadReckoningAlgorithm", variantRecordDataType->discriminantName());
                std::cout << "\t\tAttributes datatype : " << variantRecordDataType->discriminantType()->nameStr() << std::endl;
                ASSERT_EQ("ActiveSonarScanPatternEnum16", variantRecordDataType->discriminantType()->nameStr());

                certi::Alternative alternative1 = variantRecordDataType->alternatives()[0];

                    std::cout << "\t\t\tEnumerator : " << alternative1.enumerator()->name() << std::endl;
                    ASSERT_EQ("ScanPatternNotUsed", alternative1.enumerator()->name());
                    std::cout << "\t\t\tName : " << alternative1.name() << std::endl;
                    ASSERT_EQ("SpatialStatic", alternative1.name());
                    std::cout << "\t\t\tDataType : " << alternative1.type()->nameStr() << std::endl;
                    ASSERT_EQ("HLAinteractionSubscription", alternative1.type()->nameStr());
                    std::cout << "\t\t\tSemantics : " << alternative1.semantics() << std::endl;
                    ASSERT_EQ("-NULL-", alternative1.semantics());

                certi::Alternative alternative2 = variantRecordDataType->alternatives()[1];

                    std::cout << "\t\t\tEnumerator : " << alternative2.enumerator()->name() << std::endl;
                    ASSERT_EQ("Conical", alternative2.enumerator()->name());
                    std::cout << "\t\t\tName : " << alternative2.name() << std::endl;
                    ASSERT_EQ("SpatialFPW", alternative2.name());
                    std::cout << "\t\t\tDataType : " << alternative2.type()->nameStr() << std::endl;
                    ASSERT_EQ("HLAcount", alternative2.type()->nameStr());
                    std::cout << "\t\t\tSemantics : " << alternative2.semantics() << std::endl;
                    ASSERT_EQ("-NULL-", alternative2.semantics());
            }
        }
    }


    for(auto handleInteraction : static_cast<certi::ObjectInteractionSetCustom*>(rootObject.Interactions)->getMapFromHandle()) {
        std::cout << "Handle interaction class : " << handleInteraction.first << std::endl;
        auto interaction = rootObject.Interactions->getObjectFromHandle(handleInteraction.first);
        std::cout << "Name interaction : " << interaction->getName() << std::endl;
        for(auto classParameters : interaction->getHandleParameterMap()) {
            std::cout << "\tAttributes handle : " << classParameters.first << std::endl;

            auto classParameter = interaction->getParameter1516e(classParameters.first);
            std::cout << "\tAttributes name : " << classParameter->getName() << std::endl;

            if(classParameter->getType()->category() == certi::EncodableDataType::CATEGORY::BasicDataType) {
                auto basicDataType = static_cast<certi::BasicDataType*>(classParameter->getType().get());
                std::cout << "\t\tAttributes type name : " << basicDataType->nameStr() << std::endl;
                ASSERT_EQ("HLAinteger32BE", basicDataType->nameStr());
                std::cout << "\t\tAttributes type size : " << basicDataType->size() << std::endl;
                ASSERT_EQ(32, basicDataType->size());
                std::cout << "\t\tAttributes type interpretation : " << basicDataType->interpretation() << std::endl;
                ASSERT_EQ("Integer in the range [-2^31, 2^31 - 1]", basicDataType->interpretation());
                std::cout << "\t\tAttributes type encoding : " << basicDataType->encoding() << std::endl;
                ASSERT_EQ("32-bit two's complement signed integer. The most significant bit contains the sign.", basicDataType->encoding());
            }
            else if(classParameter->getType()->category() == certi::EncodableDataType::CATEGORY::SimpleDataType) {
                auto simpleDataType = static_cast<certi::SimpleDataType*>(classParameter->getType().get());
                std::cout << "\t\tAttributes type name : " << simpleDataType->nameStr() << std::endl;
                ASSERT_EQ("HLAASCIIchar", simpleDataType->nameStr());
                std::cout << "\t\tAttributes type representation : " << simpleDataType->representation()->nameStr() << std::endl;
                ASSERT_EQ("HLAoctet", simpleDataType->representation()->nameStr());
                std::cout << "\t\tAttributes type units : " << simpleDataType->units() << std::endl;
                ASSERT_EQ("NA", simpleDataType->units());
                std::cout << "\t\tAttributes type resolution : " << simpleDataType->resolution() << std::endl;
                ASSERT_EQ("NA", simpleDataType->resolution());
                std::cout << "\t\tAttributes type accuracy : " << simpleDataType->accuracy() << std::endl;
                ASSERT_EQ("NA", simpleDataType->accuracy());
                std::cout << "\t\tAttributes type semantics : " << simpleDataType->semantics() << std::endl;
                ASSERT_EQ("Standard ASCII character (see ANSI Std x3.4-1986)", simpleDataType->semantics());
            }

        }
    }

}
