#include "Mom.hh"

#include "PrettyDebug.hh"

#include "InteractionSet.hh"
#include "ObjectClassSet.hh"

using std::cout;
using std::endl;

namespace {

enum class MomAction { Publish, Subsribe, Nothing };

static const std::map<std::string, std::pair<MomAction, std::set<std::string>>> the_required_object_classes{
    {"HLAobjectRoot", {MomAction::Nothing, std::set<std::string>{}}},
    {"HLAmanager", {MomAction::Nothing, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate",
     {MomAction::Publish,
      {"HLAfederateHandle",
       "HLAfederateName",
       "HLAfederateType",
       "HLAfederateHost",
       "HLARTIversion",
       "HLAFOMmoduleDesignatorList",
       "HLAtimeConstrained",
       "HLAtimeRegulating",
       "HLAasynchronousDelivery",
       "HLAfederateState",
       "HLAtimeManagerState",
       "HLAlogicalTime",
       "HLAlookahead",
       "HLAGALT",
       "HLALITS",
       "HLAROlength",
       "HLATSOlength",
       "HLAreflectionsReceived",
       "HLAupdatesSent",
       "HLAinteractionsReceived",
       "HLAinteractionsSent",
       "HLAobjectInstancesThatCanBeDeleted",
       "HLAobjectInstancesUpdated",
       "HLAobjectInstancesReflected",
       "HLAobjectInstancesDeleted",
       "HLAobjectInstancesRemoved",
       "HLAobjectInstancesRegistered",
       "HLAobjectInstancesDiscovered",
       "HLAtimeGrantedTime",
       "HLAtimeAdvancingTime",
       "HLAconveyRegionDesignatorSets",
       "HLAconveyProducingFederate"}}},
    {"HLAmanager.HLAfederation",
     {MomAction::Publish,
      {"HLAfederationName",
       "HLAfederatesInFederation",
       "HLARTIversion",
       "HLAMIMDesignator",
       "HLAFOMmoduleDesignatorList",
       "HLAcurrentFDD",
       "HLAtimeImplementationName",
       "HLAlastSaveName",
       "HLAlastSaveTime",
       "HLAnextSaveName",
       "HLAnextSaveTime",
       "HLAautoProvide"}}}};

static const std::map<std::string, std::pair<MomAction, std::set<std::string>>> the_required_interactions{
    {"HLAinteractionRoot", {MomAction::Nothing, std::set<std::string>{}}},
    {"HLAmanager", {MomAction::Nothing, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate", {MomAction::Nothing, {"HLAfederate"}}},
    {"HLAmanager.HLAfederate.HLAadjust", {MomAction::Nothing, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLAadjust.HLAsetTiming", {MomAction::Subsribe, {"HLAreportPeriod"}}},
    {"HLAmanager.HLAfederate.HLAadjust.HLAmodifyAttributeState",
     {MomAction::Subsribe, {"HLAobjectInstance", "HLAattribute", "HLAattributeState"}}},
    {"HLAmanager.HLAfederate.HLAadjust.HLAsetSwitches",
     {MomAction::Subsribe,
      {"HLAconveyRegionDesignatorSets", "HLAconveyProducingFederate", "HLAserviceReporting", "HLAexceptionReporting"}}},
    {"HLAmanager.HLAfederate.HLArequest", {MomAction::Nothing, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLArequest.HLArequestPublications", {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLArequest.HLArequestSubscriptions", {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLArequest.HLArequestObjectInstancesThatCanBeDeleted",
     {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLArequest.HLArequestObjectInstancesUpdated",
     {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLArequest.HLArequestObjectInstancesReflected",
     {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLArequest.HLArequestUpdatesSent", {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLArequest.HLArequestInteractionsSent", {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLArequest.HLArequestReflectionsReceived", {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLArequest.HLArequestInteractionsReceived",
     {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLArequest.HLArequestObjectInstanceInformation",
     {MomAction::Subsribe, {"HLAobjectInstance"}}},
    {"HLAmanager.HLAfederate.HLArequest.HLArequestFOMmoduleData", {MomAction::Subsribe, {"HLAFOMmoduleIndicator"}}},
    {"HLAmanager.HLAfederate.HLAreport", {MomAction::Nothing, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportObjectClassPublication",
     {MomAction::Publish, {"HLAnumberOfClasses", "HLAobjectClass", "HLAattributeList"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportObjectClassSubscription",
     {MomAction::Publish,
      {"HLAnumberOfClasses", "HLAobjectClass", "HLAactive", "HLAmaxUpdateRate", "HLAattributeList"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportInteractionPublication",
     {MomAction::Publish, {"HLAinteractionClassList"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportInteractionSubscription",
     {MomAction::Publish, {"HLAinteractionClassList"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportObjectInstancesThatCanBeDeleted",
     {MomAction::Publish, {"HLAobjectInstanceCounts"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportObjectInstancesUpdated",
     {MomAction::Publish, {"HLAobjectInstanceCounts"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportObjectInstancesReflected",
     {MomAction::Publish, {"HLAobjectInstanceCounts"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportUpdatesSent",
     {MomAction::Publish, {"HLAtransportation", "HLAupdateCounts"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportReflectionsReceived",
     {MomAction::Publish, {"HLAtransportation", "HLAreflectCounts"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportInteractionsSent",
     {MomAction::Publish, {"HLAtransportation", "HLAinteractionCounts"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportInteractionsReceived",
     {MomAction::Publish, {"HLAtransportation", "HLAinteractionCounts"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportObjectInstanceInformation",
     {MomAction::Publish,
      {"HLAobjectInstance", "HLAownedInstanceAttributeList", "HLAregisteredClass", "HLAknownClass"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportException", {MomAction::Publish, {"HLAservice", "HLAexception"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportServiceInvocation",
     {MomAction::Publish,
      {"HLAservice",
       "HLAsuccessIndicator",
       "HLAsuppliedArguments",
       "HLAreturnedArguments",
       "HLAexception",
       "HLAserialNumber"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportMOMexception",
     {MomAction::Publish, {"HLAservice", "HLAexception", "HLAparameterError"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportFederateLost",
     {MomAction::Publish, {"HLAfederateName", "HLAtimestamp", "HLAfaultDescription"}}},
    {"HLAmanager.HLAfederate.HLAreport.HLAreportFOMmoduleData",
     {MomAction::Publish, {"HLAFOMmoduleIndicator", "HLAFOMmoduleData"}}},
    {"HLAmanager.HLAfederate.HLAservice", {MomAction::Nothing, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAresignFederationExecution", {MomAction::Subsribe, {"HLAresignAction"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAsynchronizationPointAchieved", {MomAction::Subsribe, {"HLAlabel"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAfederateSaveBegun", {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAfederateSaveComplete", {MomAction::Subsribe, {"HLAsuccessIndicator"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAfederateRestoreComplete", {MomAction::Subsribe, {"HLAsuccessIndicator"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLApublishObjectClassAttributes",
     {MomAction::Subsribe, {"HLAobjectClass", "HLAattributeList"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAunpublishObjectClassAttributes",
     {MomAction::Subsribe, {"HLAobjectClass", "HLAattributeList"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLApublishInteractionClass", {MomAction::Subsribe, {"HLAinteractionClass"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAunpublishInteractionClass", {MomAction::Subsribe, {"HLAinteractionClass"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAsubscribeObjectClassAttributes",
     {MomAction::Subsribe, {"HLAobjectClass", "HLAattributeList", "HLAactive"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAunsubscribeObjectClassAttributes",
     {MomAction::Subsribe, {"HLAobjectClass", "HLAattributeList"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAsubscribeInteractionClass",
     {MomAction::Subsribe, {"HLAinteractionClass", "HLAactive"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAunsubscribeInteractionClass",
     {MomAction::Subsribe, {"HLAinteractionClass"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAdeleteObjectInstance",
     {MomAction::Subsribe, {"HLAobjectInstance", "HLAtag", "HLAtimeStamp"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAlocalDeleteObjectInstance", {MomAction::Subsribe, {"HLAobjectInstance"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLArequestAttributeTransportationTypeChange",
     {MomAction::Subsribe, {"HLAobjectInstance", "HLAattributeList", "HLAtransportation"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLArequestInteractionTransportationTypeChange",
     {MomAction::Subsribe, {"HLAinteractionClass", "HLAtransportation"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAunconditionalAttributeOwnershipDivestiture",
     {MomAction::Subsribe, {"HLAobjectInstance", "HLAattributeList"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAenableTimeRegulation", {MomAction::Subsribe, {"HLAlookahead"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAdisableTimeRegulation", {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAenableTimeConstrained", {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAdisableTimeConstrained", {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAtimeAdvanceRequest", {MomAction::Subsribe, {"HLAtimeStamp"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAtimeAdvanceRequestAvailable", {MomAction::Subsribe, {"HLAtimeStamp"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAnextMessageRequest", {MomAction::Subsribe, {"HLAtimeStamp"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAnextMessageRequestAvailable", {MomAction::Subsribe, {"HLAtimeStamp"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAflushQueueRequest", {MomAction::Subsribe, {"HLAtimeStamp"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAenableAsynchronousDelivery", {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAdisableAsynchronousDelivery",
     {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAmodifyLookahead", {MomAction::Subsribe, {"HLAlookahead"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAchangeAttributeOrderType",
     {MomAction::Subsribe, {"HLAobjectInstance", "HLAattributeList", "HLAsendOrder"}}},
    {"HLAmanager.HLAfederate.HLAservice.HLAchangeInteractionOrderType",
     {MomAction::Subsribe, {"HLAinteractionClass", "HLAsendOrder"}}},
    {"HLAmanager.HLAfederation", {MomAction::Nothing, std::set<std::string>{}}},
    {"HLAmanager.HLAfederation.HLAadjust", {MomAction::Nothing, std::set<std::string>{}}},
    {"HLAmanager.HLAfederation.HLAadjust.HLAsetSwitches", {MomAction::Subsribe, {"HLAautoProvide"}}},
    {"HLAmanager.HLAfederation.HLArequest", {MomAction::Nothing, std::set<std::string>{}}},
    {"HLAmanager.HLAfederation.HLArequest.HLArequestSynchronizationPoints",
     {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederation.HLArequest.HLArequestSynchronizationPointStatus",
     {MomAction::Subsribe, {"HLAsyncPointName"}}},
    {"HLAmanager.HLAfederation.HLArequest.HLArequestFOMmoduleData", {MomAction::Subsribe, {"HLAFOMmoduleIndicator"}}},
    {"HLAmanager.HLAfederation.HLArequest.HLArequestMIMData", {MomAction::Subsribe, std::set<std::string>{}}},
    {"HLAmanager.HLAfederation.HLAreport", {MomAction::Nothing, std::set<std::string>{}}},
    {"HLAmanager.HLAfederation.HLAreport.HLAreportSynchronizationPoints", {MomAction::Publish, {"HLAsyncPoints"}}},
    {"HLAmanager.HLAfederation.HLAreport.HLAreportSynchronizationPointStatus",
     {MomAction::Publish, {"HLAsyncPointName", "HLAsyncPointFederates"}}},
    {"HLAmanager.HLAfederation.HLAreport.HLAreportFOMmoduleData",
     {MomAction::Publish, {"HLAFOMmoduleIndicator", "HLAFOMmoduleData"}}},
    {"HLAmanager.HLAfederation.HLAreport.HLAreportMIMData", {MomAction::Publish, {"HLAMIMData"}}}};
}

namespace certi {
namespace rtig {

static PrettyDebug D("MOM", __FILE__);

bool Mom::isAvailableInRootObjectAndCompliant(const RootObject& root)
{
    // Pre check with sizes
    if (root.ObjectClasses->size() < the_required_object_classes.size()
        or root.Interactions->size() < the_required_interactions.size()) {
        Debug(D, pdDebug)
            << "the root object does not contain enough object classes or interactions to have a mom definition."
            << endl;
        return false;
    }

    // Chech for every classes and each attributes
    for (auto& pair : the_required_object_classes) {
        try {
            auto objectHandle = root.ObjectClasses->getHandleFromName(pair.first);

            for (const auto& attributeName : pair.second.second) {
                try {
                    root.ObjectClasses->getAttributeHandle(attributeName, objectHandle);
                }
                catch (NameNotFound& e) {
                    Debug(D, pdDebug) << "the object class \"" << pair.first << "\" does not contain the attribute "
                                      << attributeName << endl;
                    return false;
                }
            }
        }
        catch (NameNotFound& e) {
            Debug(D, pdDebug) << "the root object does not contain the object class " << pair.first << endl;
            return false;
        }
    }

    // Chech for every interactions and each parameters
    for (auto& pair : the_required_interactions) {
        try {
            auto objectHandle = root.Interactions->getInteractionClassHandle(pair.first);

            for (const auto& parameterName : pair.second.second) {
                try {
                    root.Interactions->getParameterHandle(parameterName, objectHandle);
                }
                catch (NameNotFound& e) {
                    Debug(D, pdDebug) << "the interaction class \"" << pair.first
                                      << "\" does not contain the parameter " << parameterName << endl;
                    return false;
                }
            }
        }
        catch (NameNotFound& e) {
            Debug(D, pdDebug) << "the root object does not contain the interaction class " << pair.first << endl;
            return false;
        }
    }

    return true;
}

Mom::Mom(const FederateHandle handle, Federation& federation, RootObject& root)
    : my_handle(handle), my_federation(federation), my_root(root)
{
}

FederateHandle Mom::getHandle() const
{
    return my_handle;
}

void Mom::publishObjects()
{
    Debug(D, pdGendoc) << "enter Mom::publishObjects" << endl;

    for (const auto& pair : the_required_object_classes) {
        if (pair.second.first != MomAction::Publish) {
            std::cout << "skip " << pair.first << std::endl;
            continue;
        }

        std::cout << "publish " << pair.first << std::endl;

        auto object_handle = my_root.ObjectClasses->getHandleFromName(pair.first);
        my_object_class_cache.insert(std::make_pair(pair.first, object_handle));

        // we gather only the attributes specified in the standard
        std::vector<AttributeHandle> attributes;
        for (const std::string& attributeName : pair.second.second) {
            auto attribute_handle = my_root.ObjectClasses->getAttributeHandle(attributeName, object_handle);
            my_attribute_cache.insert(std::make_pair(attributeName, attribute_handle));

            attributes.push_back(attribute_handle);
        }

        my_federation.publishObject(my_handle, object_handle, attributes, true);
    }

    Debug(D, pdGendoc) << "exit  Mom::publishObjects" << endl;
}

void Mom::publishAndSubscribeInteractions()
{
    Debug(D, pdGendoc) << "enter Mom::publishAndSubscribeInteractions" << endl;

    for (const auto& pair : the_required_interactions) {
        if (pair.second.first == MomAction::Nothing) {
            continue;
        }

        auto interaction_handle = my_root.Interactions->getInteractionClassHandle(pair.first);
        my_interaction_class_cache.insert(std::make_pair(pair.first, interaction_handle));

        if (pair.second.first == MomAction::Publish) {
            my_federation.publishInteraction(my_handle, interaction_handle, true);
        }
        else { // we already discarded MomAction::Nothing so there is only MomAction::subscribe left
            my_federation.subscribeInteraction(my_handle, interaction_handle, true);
        }
    }

    Debug(D, pdGendoc) << "exit  Mom::publishAndSubscribeInteractions" << endl;
}

void Mom::registerFederation()
{
    Debug(D, pdGendoc) << "enter Mom::registerFederation" << endl;

    auto objectName = "Federation_" + my_federation.getName();

    my_federation.reserveObjectInstanceName(my_handle, objectName);

    my_federation_object
        = my_federation.registerObject(my_handle, my_object_class_cache["HLAmanager.HLAfederation"], objectName);

    std::vector<AttributeHandle> attributes;

    AttributeHandle attribute;
    MessageBuffer mb;

    // Static
    attribute = my_attribute_cache["HLAfederationName"];
    mb.write_string(my_federation.getName());
    attributes.push_back(attribute);
    my_attribute_values_cache[attribute] = valueFromBuffer(mb);

    // Conditional
    attribute = my_attribute_cache["HLAfederatesInFederation"];
    attributes.push_back(attribute);
    my_attribute_values_cache[attribute] = {};

    // Static
    attribute = my_attribute_cache["HLARTIversion"];
    attributes.push_back(attribute);
    my_attribute_values_cache[attribute] = {'1', '5', '1', '6', '-', '2', '0', '1', '0'};

    // Static
    attribute = my_attribute_cache["HLAMIMDesignator"];
    attributes.push_back(attribute);
    my_attribute_values_cache[attribute] = {'H', 'L', 'A', 's', 't', 'a', 'n', 'd', 'a', 'r', 'd', 'M', 'I', 'M'};

    // Conditional
    attribute = my_attribute_cache["HLAFOMmoduleDesignatorList"];
    mb.write_string(my_federation.getFEDid());
    attributes.push_back(attribute);
    my_attribute_values_cache[attribute] = valueFromBuffer(mb);

    // Conditional
    attribute = my_attribute_cache["HLAcurrentFDD"];
    mb.write_string(my_federation.getFEDid());
    attributes.push_back(attribute);
    my_attribute_values_cache[attribute] = valueFromBuffer(mb);

    // Static
    attribute = my_attribute_cache["HLAtimeImplementationName"];
    attributes.push_back(attribute);
    my_attribute_values_cache[attribute] = {'H', 'L', 'A', 'f', 'l', 'o', 'a', 't', '6', '4', 'T', 'i', 'm', 'e'};

    // Conditional
    attribute = my_attribute_cache["HLAlastSaveName"];
    attributes.push_back(attribute);
    my_attribute_values_cache[attribute] = {};

    // Conditional
    attribute = my_attribute_cache["HLAlastSaveTime"];
    attributes.push_back(attribute);
    my_attribute_values_cache[attribute] = {};

    // Conditional
    attribute = my_attribute_cache["HLAnextSaveName"];
    attributes.push_back(attribute);
    my_attribute_values_cache[attribute] = {};

    // Conditional
    attribute = my_attribute_cache["HLAnextSaveTime"];
    attributes.push_back(attribute);
    my_attribute_values_cache[attribute] = {};

    // Conditional
    attribute = my_attribute_cache["HLAautoProvide"];
    attributes.push_back(attribute);
    my_attribute_values_cache[attribute] = {};

    provideAttributeValueUpdate(my_federation_object, attributes);

    Debug(D, pdGendoc) << "exit  Mom::registerFederation" << endl;
}

void Mom::updateFederatesInFederation()
{
    Debug(D, pdGendoc) << "enter Mom::updateFederatesInFederation" << endl;

    auto attribute = my_attribute_cache["HLAfederatesInFederation"];

    // TODO update value
    MessageBuffer mb;
    
    mb.write_uint32(my_federation.getNbFederates() * sizeof(uint32_t));
    for(const auto& federate: my_federation.my_federates) {
        mb.write_uint32(federate.first);
    }
    
    my_attribute_values_cache[attribute] = valueFromBuffer(mb);

    provideAttributeValueUpdate(my_federation_object, {attribute});

    Debug(D, pdGendoc) << "exit  Mom::updateFederatesInFederation" << endl;
}

void Mom::updateNextSave(const std::string& /*label*/, const FederationTime& /*time*/)
{
    Debug(D, pdGendoc) << "enter Mom::updateNextSave" << endl;
    std::cerr << "NOT IMPLEMENTED YET." << endl; // TODO
    Debug(D, pdGendoc) << "enter Mom::updateNextSave" << endl;
}

void Mom::provideAttributeValueUpdate(const ObjectHandle& object, const std::vector<AttributeHandle>& attributes)
{
    Debug(D, pdGendoc) << "enter Mom::provideAttributeValueUpdate" << endl;

    if (object == my_federation_object) {
        std::vector<AttributeValue_t> values;

        for (const auto& attribute : attributes) {
            values.push_back(my_attribute_values_cache[attribute]);
        }

        my_federation.updateAttributeValues(my_handle, object, attributes, values, "");
    }

    Debug(D, pdGendoc) << "exit  Mom::provideAttributeValueUpdate" << endl;
}

void Mom::display() const
{
    std::cout << "my_attribute_values_cache; " << std::endl;
    for (const auto& pair : my_attribute_values_cache) {
        std::cout << "\t" << pair.first << " == " << pair.second.size() << "{ ";
        for (const auto& byte : pair.second) {
            std::cout << byte << "(" << static_cast<int>(byte) << ") ";
        }
        std::cout << "}" << std::endl;
    }
}

AttributeValue_t Mom::valueFromBuffer(MessageBuffer& mb)
{
    AttributeValue_t value;
    value.resize(mb.read_uint32());
    mb.read_bytes(&(value[0]), value.size());
    return value;
}
}
}