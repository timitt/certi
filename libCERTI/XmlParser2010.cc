// ----------------------------------------------------------------------------
// CERTI - HLA RunTime Infrastructure
// Copyright (C) 2003-2006  ONERA
//
// This file is part of CERTI-libCERTI
//
// CERTI-libCERTI is free software ; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation ; either version 2 of
// the License, or (at your option) any later version.
//
// CERTI-libCERTI is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY ; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this program ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
// ----------------------------------------------------------------------------

#include "InteractionSet.hh"
#include "ObjectClassAttribute.hh"
#include "ObjectClassSet.hh"
#include "PrettyDebug.hh"
#include "RoutingSpace.hh"
#include "XmlParser.hh"
#include "XmlParser2010.hh"
#include "ieee1516-2010/ObjectClass1516e.h"
#include "ieee1516-2010/ObjectClassAttribute1516e.h"
#include "ieee1516-2010/Interaction1516e.h"
#include "ieee1516-2010/Parameter1516e.h"
#include "ieee1516-2010/EncodableDataType.h"
#include "ieee1516-2010/BasicDataType.h"
#include "ieee1516-2010/SimpleDataType.h"
#include "ieee1516-2010/EnumeratedDataType.h"
#include "ieee1516-2010/ArrayDataType.h"
#include "ieee1516-2010/FixedRecordDataType.h"
#include "ieee1516-2010/VariantRecordDataType.h"



using std::string;
using std::cerr;
using std::endl;

namespace certi {

static PrettyDebug D("XMLPARSER", "(XmlParser) ");

XmlParser2010::XmlParser2010(RootObject* root, const bool is_parsing_modules) : XmlParser(root, is_parsing_modules)
{
}

XmlParser2010::~XmlParser2010()
{
    _predefinedDataType.clear();
}

RootObject *XmlParser2010::parse(std::__cxx11::string pathToXmlFile)
{
    XmlParser::parse(pathToXmlFile);
    return root;
}

#ifdef HAVE_XML
xmlChar* getText(xmlNodePtr node)
{
    if (node->children && node->children->content) {
        return node->children->content;
    }
    else {
        throw CouldNotOpenFED("Invalid FOM file structure - Attribute Class not found");
    }
}

std::string XmlParser2010::getName()
{
    xmlNodePtr tempNode = cur->children;
    while (tempNode != NULL) {
        if ((!xmlStrcmp(tempNode->name, ATTRIBUTE_NAME))) {
            return std::string((const char*) getText(tempNode));
        }
        tempNode = tempNode->next;
    }
    return std::string();
}

void XmlParser2010::parseClass(ObjectClass *parent)
{
    parseDataType();

    Debug(D, pdTrace) << "New Object Class" << endl;

    bool class_already_exists = false;

    xmlNodePtr prev = cur;

    /* note how objectHandle counter is incremented */
    std::string name = getName();
    if (name.empty()) {
        throw CouldNotOpenFED("Current node Name is nullptr!!");
    }

    auto current = new certi::ObjectClass1516e(name, root->getFreeObjectClassHandle());

    try {
        root->addObjectClass(current, parent);
    }
    catch (RTIinternalError& e) {
        if (my_is_parsing_module) {
            // The class already exists. point to it and do not add attributes
            class_already_exists = true;
            current = static_cast<certi::ObjectClass1516e*>(root->getObjectClass(root->ObjectClasses->getObjectClassHandle(name)));
        }
        else {
            throw;
        }
    }
    cur = cur->xmlChildrenNode;
    while (cur != nullptr) {
        // Attributes
        if ((!xmlStrcmp(cur->name, NODE_ATTRIBUTE))) {
            if (!class_already_exists) {

                HLAntos_t2010 objClassProp;
                objClassProp.name = nullptr;
                objClassProp.transportation = nullptr;
                objClassProp.order = nullptr;
                objClassProp.space = nullptr;
                objClassProp.dataType = nullptr;
                parseNTOS(&objClassProp);

                AttributeHandle attributeHandle = current->getHandleClassAttributeMap().size() + 1;
                ObjectClassAttribute1516e* attr = new ObjectClassAttribute1516e((const char*) objClassProp.name, attributeHandle);

                // Transportation
                if (!xmlStrcmp(objClassProp.transportation, VALUE_RELIABLE)) {
                    attr->transport = RELIABLE;
                }
                else {
                    if (!xmlStrcmp(objClassProp.transportation, VALUE_BESTEFFORT)) {
                        attr->transport = BEST_EFFORT;
                    }
                }

                // Order
                if (!xmlStrcmp(objClassProp.order, VALUE_TSO)) {
                    attr->order = TIMESTAMP;
                }
                else {
                    if (!xmlStrcmp(objClassProp.order, VALUE_RO)) {
                        attr->order = RECEIVE;
                    }
                }

                // Routing space
                char* space = (char*) objClassProp.space;
                if (space) {
                    try {
                        SpaceHandle h = root->getRoutingSpaceHandle(string(space));
                        attr->setSpace(h);
                    }
                    catch (Exception& e) {
                        cerr << "warning: Incorrect space name for attribute" << endl;
                    }
                }

                std::string dataTypeStr((const char*)objClassProp.dataType);
                if(_predefinedDataType.count(dataTypeStr) > 0)
                    attr->setType(_predefinedDataType.at(dataTypeStr));
                else if(dataTypeStr != "NA")
                    throw certi::DataTypeException("The datatype have to be defined in the FOM");

                // Attribute complete, adding to the class
                current->addAttribute(attr);
            }
        }
        // Subclass
        if ((!xmlStrcmp(cur->name, NODE_OBJECT_CLASS))) {
            this->parseClass(current);
        }
        cur = cur->next;
    }
    cur = prev;
}

void XmlParser2010::parseInteraction(Interaction *parent)
{
    parseDataType();

    Debug(D, pdTrace) << "New Interaction Class" << endl;

    bool interaction_already_exists = false;

    std::string name; // FIXME never used?

    xmlNodePtr prev = cur;

    HLAntos_t2010 intClassProp;
    intClassProp.name = nullptr;
    intClassProp.transportation = nullptr;
    intClassProp.order = nullptr;
    intClassProp.space = nullptr;
    intClassProp.dataType = nullptr;
    parseNTOS(&intClassProp);
    // Name

    // Transportation
    TransportType transport{RELIABLE};
    if (!xmlStrcmp(intClassProp.transportation, VALUE_RELIABLE)) {
        transport = RELIABLE;
    }
    else if (!xmlStrcmp(intClassProp.transportation, VALUE_BESTEFFORT)) {
        transport = BEST_EFFORT;
    }
    else {
        cerr << "warning: No transportation provided, defaulting to reliable" << endl;
    }

    // Order
    OrderType order{TIMESTAMP};
    if (!xmlStrcmp(intClassProp.order, VALUE_TSO)) {
        order = TIMESTAMP;
    }
    else if (!xmlStrcmp(intClassProp.order, VALUE_RO)) {
        order = RECEIVE;
    }
    else {
        cerr << "warning: No order provided, defaulting to TSO" << endl;
    }

    Interaction1516e* current = new Interaction1516e(
        reinterpret_cast<char*>(intClassProp.name), root->getFreeInteractionClassHandle(), transport, order);

    // Routing space
    char* space = (char*) intClassProp.space;
    if (space) {
        try {
            SpaceHandle h = root->getRoutingSpaceHandle(string(space));
            current->setSpace(h);
        }
        catch (Exception& e) {
            cerr << "warning: Incorrect space name for interaction" << endl;
        }
    }
    xmlFree(space);

    // Add to interactions list, and build inheritance relation

    try {
        root->addInteractionClass(current, parent);
    }
    catch (RTIinternalError& e) {
        if (my_is_parsing_module) {
            // The interaction already exists. point to it and do not add parameters
            interaction_already_exists = true;
            current = static_cast<certi::Interaction1516e*>(root->getInteractionClass(
                root->Interactions->getInteractionClassHandle(reinterpret_cast<char*>(intClassProp.name))));
        }
        else {
            throw;
        }
    }

    cur = cur->xmlChildrenNode;
    while (cur != nullptr) {
        if ((!xmlStrcmp(cur->name, NODE_PARAMETER))) {
            if (!interaction_already_exists) {
                std::string tmpName = getName();
                Parameter1516e* param = new Parameter1516e(tmpName.c_str(), root->getFreeParameterHandle());
                xmlNodePtr tempNode = cur->children;
                while (tempNode != NULL) {
                    if ((!xmlStrcmp(tempNode->name, NODE_DATATYPE))) {
                        std::string dataTypeStr((const char*)getText(tempNode));
                        if(_predefinedDataType.count(dataTypeStr) > 0)
                            param->setType(_predefinedDataType.at(dataTypeStr));
                    }
                    tempNode = tempNode->next;
                }

                current->addParameter(param);
            }
        }
        // Subinteraction
        if ((!xmlStrcmp(cur->name, NODE_INTERACTION_CLASS))) {
            this->parseInteraction(current);
        }
        cur = cur->next;
    }
    cur = prev;
}

void XmlParser2010::parseDataType()
{
    if(_predefinedDataType.size() == 0) {

        Debug(D, pdTrace) << "DataType Node" << endl;

        if(cur != nullptr) {
            std::string curName((const char*)cur->name);
        }

        xmlNodePtr prev = cur;

        cur = cur->parent;

        std::string basicDataString = EncodableDataType::toString(EncodableDataType::CATEGORY::BasicDataType);
        const xmlChar* basicDataTypeNode = (const xmlChar*)(basicDataString.c_str());
        std::string simpleDataString = EncodableDataType::toString(EncodableDataType::CATEGORY::SimpleDataType);
        const xmlChar* simpleDataTypeNode = (const xmlChar*)(simpleDataString.c_str());
        std::string arrayDataString = EncodableDataType::toString(EncodableDataType::CATEGORY::ArrayDataType);
        const xmlChar* arrayDataTypeNode = (const xmlChar*)(arrayDataString.c_str());
        std::string fixedRecordDataString = EncodableDataType::toString(EncodableDataType::CATEGORY::FixedRecordDataType);
        const xmlChar* fixedRecordDataTypeNode = (const xmlChar*)(fixedRecordDataString.c_str());
        std::string enumeratedDataString = EncodableDataType::toString(EncodableDataType::CATEGORY::EnumeratedDataType);
        const xmlChar* enumeratedDataTypeNode = (const xmlChar*)(enumeratedDataString.c_str());
        std::string variantRecordDataString = EncodableDataType::toString(EncodableDataType::CATEGORY::VariantRecordDataType);
        const xmlChar* variantRecordDataTypeNode = (const xmlChar*)(variantRecordDataString.c_str());

        while (cur != nullptr) {
            if ((!xmlStrcmp(cur->name, (const xmlChar*)"dataTypes"))) {
                xmlNodePtr prevTempNode = cur;
                xmlNodePtr tempNode = cur->children;
                while (tempNode != NULL) {
                    if(!xmlStrcmp(tempNode->name, basicDataTypeNode)) {
                        cur=tempNode;
                        parseBasicData();
                    }
                    else if(!xmlStrcmp(tempNode->name, simpleDataTypeNode)) {
                        cur=tempNode;
                        parseSimpleData();
                    }
                    else if(!xmlStrcmp(tempNode->name, arrayDataTypeNode)) {
                        cur=tempNode;
                        parseArrayData();
                    }
                    else if(!xmlStrcmp(tempNode->name, fixedRecordDataTypeNode)) {
                        cur=tempNode;
                        parseFixedRecordData();
                    }
                    else if(!xmlStrcmp(tempNode->name, enumeratedDataTypeNode)) {
                        cur=tempNode;
                        parseEnumeratedData();
                    }
                    else if(!xmlStrcmp(tempNode->name, variantRecordDataTypeNode)) {
                        cur=tempNode;
                        parseVariantRecordData();
                    }
                    tempNode = tempNode->next;
                }
                cur = prevTempNode;
            }
            cur = cur->next;
        }

        cur = prev;

        connectDataTypeBetweenThem();
    }

}

void XmlParser2010::parseBasicData()
{
    xmlNodePtr prev = cur;
    cur = cur->children;

    while (cur != NULL) {
        if(!xmlStrcmp(cur->name, (const xmlChar*)"basicData")) {
            xmlNodePtr tempNode = cur->children;
            std::shared_ptr<BasicDataType> basicDataType = std::make_shared<BasicDataType>();
            while(tempNode != NULL) {
                if(!xmlStrcmp(tempNode->name, (const xmlChar*)"name")) {
                    basicDataType->setName((const char*)getText(tempNode));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"size")) {
                    std::istringstream iss((const char*)getText(tempNode));
                    size_t sizeNumber;
                    iss >> sizeNumber;
                    basicDataType->setSize(sizeNumber);
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"interpretation")) {
                    basicDataType->setInterpretation((const char*)(getText(tempNode)));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"endian")) {
                    std::string endianStr((const char*)getText(tempNode));
                    basicDataType->setEndian(EncodableDataType::toEndian(endianStr));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"encoding")) {
                    basicDataType->setEncoding((const char*)(getText(tempNode)));
                }
                tempNode = tempNode->next;
            }
            _predefinedDataType.emplace(basicDataType->nameStr(), basicDataType);
        }
        cur = cur->next;
    }
    cur = prev;
}

void XmlParser2010::parseSimpleData()
{
    xmlNodePtr prev = cur;
    cur = cur->children;

    while (cur != NULL) {
        if(!xmlStrcmp(cur->name, (const xmlChar*)"simpleData")) {
            xmlNodePtr tempNode = cur->children;
            std::shared_ptr<SimpleDataType> simpleDataType = std::make_shared<SimpleDataType>();
            while(tempNode != NULL) {
                if(!xmlStrcmp(tempNode->name, (const xmlChar*)"name")) {
                    simpleDataType->setName((const char*)getText(tempNode));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"representation")) {
                    std::string representation((const char*)getText(tempNode));
                    auto it = _predefinedDataType.find(representation);
                    if(it != _predefinedDataType.end()) {
                        if(it->second->category() == EncodableDataType::CATEGORY::BasicDataType) {
                            std::shared_ptr<BasicDataType> pBasicDataType = std::make_shared<BasicDataType>(*static_cast<BasicDataType*>(it->second.get()));
                            simpleDataType->setRepresentation(pBasicDataType);
                        }
                    }
                    else
                        throw DataTypeException(std::string("The type ") + representation + std::string(" does not exist in the simple data"));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"units")) {
                    simpleDataType->setUnits((const char*)(getText(tempNode)));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"resolution")) {
                    simpleDataType->setResolution((const char*)(getText(tempNode)));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"accuracy")) {
                    simpleDataType->setAccuracy((const char*)(getText(tempNode)));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"semantics")) {
                    simpleDataType->setSemantics((const char*)(getText(tempNode)));
                }
                tempNode = tempNode->next;
            }
            _predefinedDataType.emplace(simpleDataType->nameStr(), simpleDataType);
        }
        cur = cur->next;
    }
    cur = prev;
}

void XmlParser2010::parseEnumeratedData()
{
    xmlNodePtr prev = cur;
    cur = cur->children;

    while (cur != NULL) {
        if(!xmlStrcmp(cur->name, (const xmlChar*)"enumeratedData")) {
            xmlNodePtr tempNode = cur->children;
            std::shared_ptr<EnumeratedDataType> enumeratedDataType = std::make_shared<EnumeratedDataType>();
            while(tempNode != NULL) {
                if(!xmlStrcmp(tempNode->name, (const xmlChar*)"name")) {
                    enumeratedDataType->setName((const char*)getText(tempNode));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"representation")) {
                    std::string dataTypeName((const char*)getText(tempNode));
                    enumeratedDataType->setRepresentationTemp(dataTypeName);
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"semantics")) {
                    enumeratedDataType->setSemantics((const char*)getText(tempNode));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"enumerator")) {
                    xmlNodePtr tempNodeField = tempNode->children;
                    std::shared_ptr<Enumerator> enumerator = std::make_shared<Enumerator>();
                    while(tempNodeField != NULL) {
                        if(!xmlStrcmp(tempNodeField->name, (const xmlChar*)"name")) {
                            enumerator->setName((const char*)getText(tempNodeField));
                        }

                        else if(!xmlStrcmp(tempNodeField->name, (const xmlChar*)"value")) {
                            enumerator->setValue((const char*)getText(tempNodeField));
                        }
                        tempNodeField = tempNodeField->next;
                    }
                    enumeratedDataType->addEnumerator(enumerator);
                }
                tempNode = tempNode->next;
            }
            _predefinedDataType.emplace(enumeratedDataType->nameStr(), enumeratedDataType);
        }
        cur = cur->next;
    }
    cur = prev;
}

void XmlParser2010::parseArrayData()
{
    xmlNodePtr prev = cur;
    cur = cur->children;

    while (cur != NULL) {
        if(!xmlStrcmp(cur->name, (const xmlChar*)"arrayData")) {
            xmlNodePtr tempNode = cur->children;
            std::shared_ptr<ArrayDataType> arrayDataType = std::make_shared<ArrayDataType>();
            while(tempNode != NULL) {
                if(!xmlStrcmp(tempNode->name, (const xmlChar*)"name")) {
                    arrayDataType->setName((const char*)getText(tempNode));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"dataType")) {
                    std::string dataTypeName((const char*)getText(tempNode));
                    arrayDataType->setTypeElementsTemp(dataTypeName);
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"cardinality")) {
                    arrayDataType->setCardinality((const char*)getText(tempNode));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"encoding")) {
                    arrayDataType->setEncoding((const char*)(getText(tempNode)));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"semantics")) {
                    arrayDataType->setSemantics((const char*)(getText(tempNode)));
                }
                tempNode = tempNode->next;
            }
            _predefinedDataType.emplace(arrayDataType->nameStr(), arrayDataType);
        }
        cur = cur->next;
    }
    cur = prev;
}

void XmlParser2010::parseFixedRecordData()
{
    xmlNodePtr prev = cur;
    cur = cur->children;

    while (cur != NULL) {
        if(!xmlStrcmp(cur->name, (const xmlChar*)"fixedRecordData")) {
            xmlNodePtr tempNode = cur->children;
            std::shared_ptr<FixedRecordDataType> fixedRecordDataType = std::make_shared<FixedRecordDataType>();
            while(tempNode != NULL) {
                if(!xmlStrcmp(tempNode->name, (const xmlChar*)"name")) {
                    fixedRecordDataType->setName((const char*)getText(tempNode));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"encoding")) {
                    fixedRecordDataType->setEncoding((const char*)getText(tempNode));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"semantics")) {
                    fixedRecordDataType->setSemantics((const char*)getText(tempNode));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"field")) {
                    xmlNodePtr tempNodeField = tempNode->children;
                    Field field;
                    while(tempNodeField != NULL) {
                        if(!xmlStrcmp(tempNodeField->name, (const xmlChar*)"name")) {
                            field.setName((const char*)getText(tempNodeField));
                        }
                        else if(!xmlStrcmp(tempNodeField->name, (const xmlChar*)"dataType")) {
                            std::string dataTypeName((const char*)getText(tempNodeField));
                            field.setTypeElemTemp(dataTypeName);
                        }
                        else if(!xmlStrcmp(tempNodeField->name, (const xmlChar*)"semantics")) {
                            field.setSemantics((const char*)getText(tempNodeField));
                        }
                        tempNodeField = tempNodeField->next;
                    }
                    fixedRecordDataType->addField(field);
                }
                tempNode = tempNode->next;
            }
            _predefinedDataType.emplace(fixedRecordDataType->nameStr(), fixedRecordDataType);
        }
        cur = cur->next;
    }
    cur = prev;
}

void XmlParser2010::parseVariantRecordData()
{
    xmlNodePtr prev = cur;
    cur = cur->children;

    while (cur != NULL) {
        if(!xmlStrcmp(cur->name, (const xmlChar*)"variantRecordData")) {
            xmlNodePtr tempNode = cur->children;
            std::shared_ptr<VariantRecordDataType> variantRecordDataType = std::make_shared<VariantRecordDataType>();
            while(tempNode != NULL) {
                if(!xmlStrcmp(tempNode->name, (const xmlChar*)"name")) {
                    variantRecordDataType->setName((const char*)getText(tempNode));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"discriminant")) {
                    variantRecordDataType->setDiscriminantName((const char*)getText(tempNode));
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"dataType")) {
                    std::string dataTypeName((const char*)getText(tempNode));
                    variantRecordDataType->setDiscriminantTypeTemp(dataTypeName);
                }
                else if(!xmlStrcmp(tempNode->name, (const xmlChar*)"alternative")) {
                    xmlNodePtr tempNodeAlternative = tempNode->children;
                    Alternative alternative;
                    while(tempNodeAlternative != NULL) {
                        if(!xmlStrcmp(tempNodeAlternative->name, (const xmlChar*)"enumerator")) {
                            alternative.setEnumeratorTemp((const char*)getText(tempNodeAlternative));
                        }
                        else if(!xmlStrcmp(tempNodeAlternative->name, (const xmlChar*)"name")) {
                            alternative.setName((const char*)getText(tempNodeAlternative));
                        }
                        else if(!xmlStrcmp(tempNodeAlternative->name, (const xmlChar*)"dataType")) {
                            std::string dataTypeName((const char*)getText(tempNodeAlternative));
                            alternative.setTypeTemp(dataTypeName);
                        }
                        else if(!xmlStrcmp(tempNodeAlternative->name, (const xmlChar*)"semantics")) {
                            alternative.setSemantics((const char*)getText(tempNodeAlternative));
                        }
                        tempNodeAlternative = tempNodeAlternative->next;
                    }
                    variantRecordDataType->addAlternative(alternative);
                }
                tempNode = tempNode->next;
            }
            _predefinedDataType.emplace(variantRecordDataType->nameStr(), variantRecordDataType);
        }
        cur = cur->next;
    }
    cur = prev;
}

void XmlParser2010::connectDataTypeBetweenThem()
{
    for(auto itDataType = _predefinedDataType.begin(); itDataType != _predefinedDataType.end(); itDataType++)
    {
        if(itDataType->second->category() == EncodableDataType::CATEGORY::EnumeratedDataType) {
            std::shared_ptr<EnumeratedDataType> enumeratedDataType = std::dynamic_pointer_cast<EnumeratedDataType>(itDataType->second);
            auto it = _predefinedDataType.find(enumeratedDataType->representationTemp());
            if(it != _predefinedDataType.end()) {
                if(it->second->category() == EncodableDataType::CATEGORY::BasicDataType) {
                    std::shared_ptr<BasicDataType> pBasicDataType = std::dynamic_pointer_cast<BasicDataType>(it->second);
                    enumeratedDataType->setRepresentation(pBasicDataType);
                }
                else if(it->second->category() == EncodableDataType::CATEGORY::SimpleDataType) {
                    std::shared_ptr<SimpleDataType> pSimpleDataType = std::dynamic_pointer_cast<SimpleDataType>(it->second);
                    enumeratedDataType->setRepresentation(pSimpleDataType);
                }
                else if(it->second->category() == EncodableDataType::CATEGORY::EnumeratedDataType) {
                    std::shared_ptr<EnumeratedDataType> pEnumDataType = std::dynamic_pointer_cast<EnumeratedDataType>(it->second);
                    enumeratedDataType->setRepresentation(pEnumDataType);
                }
                else if(it->second->category() == EncodableDataType::CATEGORY::ArrayDataType) {
                    std::shared_ptr<ArrayDataType> pArrayDataType = std::dynamic_pointer_cast<ArrayDataType>(it->second);
                    enumeratedDataType->setRepresentation(pArrayDataType);
                }
                else if(it->second->category() == EncodableDataType::CATEGORY::FixedRecordDataType) {
                    std::shared_ptr<FixedRecordDataType> pFixedRecordDataType = std::dynamic_pointer_cast<FixedRecordDataType>(it->second);
                    enumeratedDataType->setRepresentation(pFixedRecordDataType);
                }
                else if(it->second->category() == EncodableDataType::CATEGORY::VariantRecordDataType) {
                    std::shared_ptr<VariantRecordDataType> pVariantRecordDataType = std::dynamic_pointer_cast<VariantRecordDataType>(it->second);
                    enumeratedDataType->setRepresentation(pVariantRecordDataType);
                }
            }
            else
                throw DataTypeException(std::string("The type ") + enumeratedDataType->representationTemp() + std::string(" does not exist in the enumerated data"));
        }
        else if(itDataType->second->category() == EncodableDataType::CATEGORY::ArrayDataType) {
            std::shared_ptr<ArrayDataType> arrayDataType = std::dynamic_pointer_cast<ArrayDataType>(itDataType->second);
            auto it = _predefinedDataType.find(arrayDataType->typeElementsTemp());
            if(it != _predefinedDataType.end()) {
                if(it->second->category() == EncodableDataType::CATEGORY::BasicDataType) {
                    std::shared_ptr<BasicDataType> pBasicDataType = std::dynamic_pointer_cast<BasicDataType>(it->second);
                    arrayDataType->setTypeElements(pBasicDataType);
                }
                else if(it->second->category() == EncodableDataType::CATEGORY::SimpleDataType) {
                    std::shared_ptr<SimpleDataType> pSimpleDataType = std::dynamic_pointer_cast<SimpleDataType>(it->second);
                    arrayDataType->setTypeElements(pSimpleDataType);
                }
                else if(it->second->category() == EncodableDataType::CATEGORY::EnumeratedDataType) {
                    std::shared_ptr<EnumeratedDataType> pEnumDataType = std::dynamic_pointer_cast<EnumeratedDataType>(it->second);
                    arrayDataType->setTypeElements(pEnumDataType);
                }
                else if(it->second->category() == EncodableDataType::CATEGORY::ArrayDataType) {
                    std::shared_ptr<ArrayDataType> pArrayDataType = std::dynamic_pointer_cast<ArrayDataType>(it->second);
                    arrayDataType->setTypeElements(pArrayDataType);
                }
                else if(it->second->category() == EncodableDataType::CATEGORY::FixedRecordDataType) {
                    std::shared_ptr<FixedRecordDataType> pFixedRecordDataType = std::dynamic_pointer_cast<FixedRecordDataType>(it->second);
                    arrayDataType->setTypeElements(pFixedRecordDataType);
                }
                else if(it->second->category() == EncodableDataType::CATEGORY::VariantRecordDataType) {
                    std::shared_ptr<VariantRecordDataType> pVariantRecordDataType = std::dynamic_pointer_cast<VariantRecordDataType>(it->second);
                    arrayDataType->setTypeElements(pVariantRecordDataType);
                }
            }
            else
                throw DataTypeException(std::string("The type ") + arrayDataType->typeElementsTemp() + std::string(" does not exist in the array data"));
        }
        else if(itDataType->second->category() == EncodableDataType::CATEGORY::FixedRecordDataType) {
            std::shared_ptr<FixedRecordDataType> fixedRecordDataType = std::dynamic_pointer_cast<FixedRecordDataType>(itDataType->second);

            for(uint i = 0; i < fixedRecordDataType->fields().size(); i++)
            {
                auto it = _predefinedDataType.find(fixedRecordDataType->fields()[i].typeElemTemp());
                if(it != _predefinedDataType.end())
                {

                    if(it->second->category() == EncodableDataType::CATEGORY::BasicDataType) {
                        std::shared_ptr<BasicDataType> pBasicDataType = std::dynamic_pointer_cast<BasicDataType>(it->second);
                        fixedRecordDataType->fields()[i].setTypeElem(pBasicDataType);
                    }
                    else if(it->second->category() == EncodableDataType::CATEGORY::SimpleDataType) {
                        std::shared_ptr<SimpleDataType> pSimpleDataType = std::dynamic_pointer_cast<SimpleDataType>(it->second);
                        fixedRecordDataType->fields()[i].setTypeElem(pSimpleDataType);
                    }
                    else if(it->second->category() == EncodableDataType::CATEGORY::EnumeratedDataType) {
                        std::shared_ptr<EnumeratedDataType> pSimpleDataType = std::dynamic_pointer_cast<EnumeratedDataType>(it->second);
                        fixedRecordDataType->fields()[i].setTypeElem(pSimpleDataType);
                    }
                    else if(it->second->category() == EncodableDataType::CATEGORY::ArrayDataType) {
                        std::shared_ptr<ArrayDataType> pArrayDataType = std::dynamic_pointer_cast<ArrayDataType>(it->second);
                        fixedRecordDataType->fields()[i].setTypeElem(pArrayDataType);
                    }
                    else if(it->second->category() == EncodableDataType::CATEGORY::FixedRecordDataType) {
                        std::shared_ptr<FixedRecordDataType> pArrayDataType = std::dynamic_pointer_cast<FixedRecordDataType>(it->second);
                        fixedRecordDataType->fields()[i].setTypeElem(pArrayDataType);
                    }
                    else if(it->second->category() == EncodableDataType::CATEGORY::VariantRecordDataType) {
                        std::shared_ptr<VariantRecordDataType> pArrayDataType = std::dynamic_pointer_cast<VariantRecordDataType>(it->second);
                        fixedRecordDataType->fields()[i].setTypeElem(pArrayDataType);
                    }
                }
                else
                    throw DataTypeException(std::string("The type ") + fixedRecordDataType->fields()[i].typeElemTemp() + std::string(" does not exist in the fixed record"));
            }
        }
        else if(itDataType->second->category() == EncodableDataType::CATEGORY::VariantRecordDataType) {
            std::shared_ptr<VariantRecordDataType> variantRecordDataType = std::dynamic_pointer_cast<VariantRecordDataType>(itDataType->second);
            auto it = _predefinedDataType.find(variantRecordDataType->discriminantTypeTemp());
            if(it != _predefinedDataType.end()) {
                if(it->second->category() == EncodableDataType::CATEGORY::EnumeratedDataType) {
                    std::shared_ptr<EnumeratedDataType> pBasicDataType = std::dynamic_pointer_cast<EnumeratedDataType>(it->second);
                    variantRecordDataType->setDiscriminantType(pBasicDataType);
                }
            }
            else
                throw EnumerateException(std::string("The enumerate type ") + variantRecordDataType->discriminantTypeTemp() + std::string(" does not exist"));

            std::shared_ptr<EnumeratedDataType> enumDataType = variantRecordDataType->discriminantType();
            for(uint i=0; i<variantRecordDataType->alternatives().size(); i++)
            {
                std::shared_ptr<Enumerator> enumerator = enumDataType->getEnumerator(variantRecordDataType->alternatives()[i].enumeratorTemp());
                variantRecordDataType->alternatives()[i].setEnumerator(enumerator);

                auto it = _predefinedDataType.find(variantRecordDataType->alternatives()[i].typeTemp());
                if(it != _predefinedDataType.end()) {
                    if(it->second->category() == EncodableDataType::CATEGORY::BasicDataType) {
                        std::shared_ptr<BasicDataType> pBasicDataType = std::dynamic_pointer_cast<BasicDataType>(it->second);
                        variantRecordDataType->alternatives()[i].setType(pBasicDataType);
                    }
                    else if(it->second->category() == EncodableDataType::CATEGORY::SimpleDataType) {
                        std::shared_ptr<SimpleDataType> pSimpleDataType = std::dynamic_pointer_cast<SimpleDataType>(it->second);
                        variantRecordDataType->alternatives()[i].setType(pSimpleDataType);
                    }
                    else if(it->second->category() == EncodableDataType::CATEGORY::EnumeratedDataType) {
                        std::shared_ptr<EnumeratedDataType> pArrayDataType = std::dynamic_pointer_cast<EnumeratedDataType>(it->second);
                        variantRecordDataType->alternatives()[i].setType(pArrayDataType);
                    }
                    else if(it->second->category() == EncodableDataType::CATEGORY::ArrayDataType) {
                        std::shared_ptr<ArrayDataType> pArrayDataType = std::dynamic_pointer_cast<ArrayDataType>(it->second);
                        variantRecordDataType->alternatives()[i].setType(pArrayDataType);
                    }
                    else if(it->second->category() == EncodableDataType::CATEGORY::FixedRecordDataType) {
                        std::shared_ptr<FixedRecordDataType> pArrayDataType = std::dynamic_pointer_cast<FixedRecordDataType>(it->second);
                        variantRecordDataType->alternatives()[i].setType(pArrayDataType);
                    }
                    else if(it->second->category() == EncodableDataType::CATEGORY::VariantRecordDataType) {
                        std::shared_ptr<VariantRecordDataType> pArrayDataType = std::dynamic_pointer_cast<VariantRecordDataType>(it->second);
                        variantRecordDataType->alternatives()[i].setType(pArrayDataType);
                    }

                }
                else
                    throw DataTypeException(std::string("The type ") + variantRecordDataType->alternatives()[i].typeTemp() + std::string(" does not exist in the variant record"));
            }

        }

    }
}

void XmlParser2010::parseNTOS(HLAntos_t* ntos_p)
{
    HLAntos_t2010* ntos_p2010 = static_cast<HLAntos_t2010*>(ntos_p);
    //try to load a 1516e FOM file instead
    xmlNodePtr tempNode = cur->children;
    while (tempNode != NULL) {
        if ((!xmlStrcmp(tempNode->name, ATTRIBUTE_NAME))) {
            ntos_p2010->name = getText(tempNode);
        }
        else if ((!xmlStrcmp(tempNode->name, ATTRIBUTE_TRANSPORTATION))) {
            ntos_p2010->transportation = getText(tempNode);
        }
        else if ((!xmlStrcmp(tempNode->name, ATTRIBUTE_ORDER))) {
            ntos_p2010->order = getText(tempNode);
        }
        else if ((!xmlStrcmp(tempNode->name, ATTRIBUTE_SPACE))) {
            ntos_p2010->space = getText(tempNode);
        }
        else if ((!xmlStrcmp(tempNode->name, NODE_DATATYPE))) {
            ntos_p2010->dataType = getText(tempNode);
        }
        tempNode = tempNode->next;
    }
}

#else // !HAVE_XML

void XmlParser2010::parseNTOS(HLAntos_t* /*ntos_p*/)
{
}

std::string XmlParser2010::getName()
{
    return std::string("");
}
#endif // HAVE_XML

} // namespace certi
