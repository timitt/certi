#include "momcli1516e.h"
#include <RTI/encoding/BasicDataElements.h>
#include <RTI/encoding/HLAvariableArray.h>
#include <RTI/time/HLAfloat64Time.h>
#include <iostream>


MomCli1516e::MomCli1516e(rti1516e::RTIambassador &amb, const std::__cxx11::wstring &federation_name)
    : MomCli(amb, federation_name)
{

}

void MomCli1516e::command_federate_adjust_set_timing()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAadjust.HLAsetTiming\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    int report_period = std::stoi(requestArgument("HLAreportPeriod"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAinteger32LE hla_report_period(report_period);

    sendInteraction(L"HLAmanager.HLAfederate.HLAadjust.HLAsetTiming",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAreportPeriod", hla_report_period.encode()}});
}

void MomCli1516e::command_federate_adjust_modify_attribute_state()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAadjust.HLAmodifyAttributeState\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    int object_instance = std::stoi(requestArgument("HLAobjectInstance"));
    int attribute = std::stoi(requestArgument("HLAattribute"));
    bool attr_state = std::stoi(requestArgument("HLAattributeState"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAinteger32LE hla_object_instance(object_instance);
    ::rti1516e::HLAinteger32LE hla_attribute(attribute);
    ::rti1516e::HLAboolean hla_attr_state(attr_state);

    sendInteraction(L"HLAmanager.HLAfederate.HLAadjust.HLAmodifyAttributeState",
                    {{L"HLAfederate", hla_federate_handle.encode()},
                     {L"HLAobjectInstance", hla_object_instance.encode()},
                     {L"HLAattribute", hla_attribute.encode()},
                     {L"HLAattributeState", hla_attr_state.encode()}});
}

void MomCli1516e::command_federate_adjust_set_switches()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAadjust.HLAsetSwitches\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    bool crds = std::stoi(requestArgument("HLAconveyRegionDesignatorSets"));
    bool cpf = std::stoi(requestArgument("HLAconveyProducingFederate"));
    bool sr = std::stoi(requestArgument("HLAserviceReporting"));
    bool er = std::stoi(requestArgument("HLAexceptionReporting"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAboolean hla_crds(crds);
    ::rti1516e::HLAinteger32LE hla_cpf(cpf);
    ::rti1516e::HLAboolean hla_sr(sr);
    ::rti1516e::HLAboolean hla_er(er);

    sendInteraction(L"HLAmanager.HLAfederate.HLAadjust.HLAsetSwitches",
                    {{L"HLAfederate", hla_federate_handle.encode()},
                     {L"HLAconveyRegionDesignatorSets", hla_crds.encode()},
                     {L"HLAconveyProducingFederate", hla_cpf.encode()},
                     {L"HLAserviceReporting", hla_sr.encode()},
                     {L"HLAexceptionReporting", hla_er.encode()}});
}

void MomCli1516e::command_federate_request_publications()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLArequest.HLArequestPublications\n";

        int federate_handle = std::stoi(requestArgument("HLAfederate"));

        ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);

        sendInteraction(L"HLAmanager.HLAfederate.HLArequest.HLArequestPublications",
        {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_request_subscriptions()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLArequest.HLArequestSubscriptions\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);

    sendInteraction(L"HLAmanager.HLAfederate.HLArequest.HLArequestSubscriptions",
    {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_request_object_instances_that_can_be_deleted()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLArequest.HLArequestObjectInstancesThatCanBeDeleted\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);

    sendInteraction(L"HLAmanager.HLAfederate.HLArequest.HLArequestObjectInstancesThatCanBeDeleted",
    {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_request_object_instances_updated()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLArequest.HLArequestObjectInstancesUpdated\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);

    sendInteraction(L"HLAmanager.HLAfederate.HLArequest.HLArequestObjectInstancesUpdated",
    {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_request_object_instances_reflected()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLArequest.HLArequestObjectInstancesReflected\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    sendInteraction(L"HLAmanager.HLAfederate.HLArequest.HLArequestObjectInstancesReflected",
    {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_request_updates_sent()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLArequest.HLArequestUpdatesSent\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);

    sendInteraction(L"HLAmanager.HLAfederate.HLArequest.HLArequestUpdatesSent",
    {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_request_interactions_sent()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLArequest.HLArequestInteractionsSent\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);

    sendInteraction(L"HLAmanager.HLAfederate.HLArequest.HLArequestInteractionsSent",
    {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_request_reflections_received()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLArequest.HLArequestReflectionsReceived\n";

        int federate_handle = std::stoi(requestArgument("HLAfederate"));
        ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);

        sendInteraction(L"HLAmanager.HLAfederate.HLArequest.HLArequestReflectionsReceived",
        {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_request_interactions_received()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLArequest.HLArequestInteractionsReceived\n";

        int federate_handle = std::stoi(requestArgument("HLAfederate"));
        ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);

        sendInteraction(L"HLAmanager.HLAfederate.HLArequest.HLArequestInteractionsReceived",
        {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_request_object_instance_information()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLArequest.HLArequestObjectInstanceInformation\n";

        int federate_handle = std::stoi(requestArgument("HLAfederate"));
        int object = std::stoi(requestArgument("HLAobjectInstance"));
        ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
        ::rti1516e::HLAinteger32LE hla_object(object);

        sendInteraction(L"HLAmanager.HLAfederate.HLArequest.HLArequestObjectInstanceInformation",
        {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAobjectInstance", hla_object.encode()}});
}

void MomCli1516e::command_federate_request_FOM_module_data()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLArequest.HLArequestFOMmoduleData\n";

        int federate_handle = std::stoi(requestArgument("HLAfederate"));
        int fom_module = std::stoi(requestArgument("HLAFOMmoduleIndicator"));
        ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
        ::rti1516e::HLAinteger32LE hla_fom_module(fom_module);
        sendInteraction(L"HLAmanager.HLAfederate.HLArequest.HLArequestFOMmoduleData",
        {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAFOMmoduleIndicator", hla_fom_module.encode()}});
}

void MomCli1516e::command_federate_service_resign_federation_execution()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAresignFederationExecution\n";

        int federate_handle = std::stoi(requestArgument("HLAfederate"));
        int action = std::stoi(requestArgument("HLAresignAction"));
        ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
        ::rti1516e::HLAinteger32LE hla_action(action);
        sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAresignFederationExecution",
        {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAresignAction", hla_action.encode()}});
}

void MomCli1516e::command_federate_service_synchronization_point_achieved()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAsynchronizationPointAchieved\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    std::string label = requestArgument("HLAlabel");

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAASCIIstring hla_label(label);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAsynchronizationPointAchieved",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAlabel", hla_label.encode()}});
}

void MomCli1516e::command_federate_service_federate_save_begun()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAfederateSaveBegun\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAfederateSaveBegun",
    {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_service_federate_save_complete()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAfederateSaveComplete\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    bool success_indicator = std::stoi(requestArgument("HLAsuccessIndicator"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAboolean hla_success_indicator(success_indicator);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAfederateSaveComplete",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAsuccessIndicator", hla_success_indicator.encode()}});
}

void MomCli1516e::command_federate_service_federate_restore_complete()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAfederateRestoreComplete\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    bool success_indicator = std::stoi(requestArgument("HLAsuccessIndicator"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAboolean hla_success_indicator(success_indicator);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAfederateRestoreComplete",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAsuccessIndicator", hla_success_indicator.encode()}});
}

void MomCli1516e::command_federate_service_publish_object_class_attributes()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLApublishObjectClassAttributes\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    int object_class = std::stoi(requestArgument("HLAobjectClass"));
    auto attr_list = requestVectorArguments("HLAattributeList");
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAinteger32LE hla_object_class(object_class);
    ::rti1516e::HLAvariableArray hla_attr_list(hla_object_class);
    for(int attr : attr_list) {
        hla_attr_list.addElement(::rti1516e::HLAinteger32LE(attr));
    }

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLApublishObjectClassAttributes",
                    {{L"HLAfederate", hla_federate_handle.encode()},
                     {L"HLAobjectClass", hla_object_class.encode()},
                     {L"HLAattributeList", hla_attr_list.encode()}});
}

void MomCli1516e::command_federate_service_un_publish_object_class_attributes()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAunpublishObjectClassAttributes\n";

        int federate_handle = std::stoi(requestArgument("HLAfederate"));
        int object_class = std::stoi(requestArgument("HLAobjectClass"));
        auto attr_list = requestVectorArguments("HLAattributeList");

        ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
        ::rti1516e::HLAinteger32LE hla_object_class(object_class);
        ::rti1516e::HLAvariableArray hla_attr_list(hla_object_class);
        for(int attr : attr_list) {
            hla_attr_list.addElement(::rti1516e::HLAinteger32LE(attr));
        }

        sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAunpublishObjectClassAttributes",
                        {{L"HLAfederate", hla_federate_handle.encode()},
                         {L"HLAobjectClass", hla_object_class.encode()},
                         {L"HLAattributeList", hla_attr_list.encode()}});
}

void MomCli1516e::command_federate_service_publish_interaction_class()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLApublishInteractionClass\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    int interaction_class = std::stoi(requestArgument("HLAinteractionClass"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAinteger32LE hla_interaction_class(interaction_class);
    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLApublishInteractionClass",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAinteractionClass", hla_interaction_class.encode()}});
}

void MomCli1516e::command_federate_service_un_publish_interaction_class()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAunpublishInteractionClass\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    int interaction_class = std::stoi(requestArgument("HLAinteractionClass"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAinteger32LE hla_interaction_class(interaction_class);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAunpublishInteractionClass",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAinteractionClass", hla_interaction_class.encode()}});
}

void MomCli1516e::command_federate_service_subscribe_object_class_attributes()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAsubscribeObjectClassAttributes\n";

        int federate_handle = std::stoi(requestArgument("HLAfederate"));
        int object_class = std::stoi(requestArgument("HLAobjectClass"));
        auto attr_list = requestVectorArguments("HLAattributeList");
        bool active = std::stoi(requestArgument("HLAactive"));
        
        ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
        ::rti1516e::HLAinteger32LE hla_object_class(object_class);
        ::rti1516e::HLAvariableArray hla_attr_list(hla_object_class);
        for(int attr : attr_list) {
            hla_attr_list.addElement(::rti1516e::HLAinteger32LE(attr));
        }
        ::rti1516e::HLAboolean hla_active(active);
        
        sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAsubscribeObjectClassAttributes",
                        {{L"HLAfederate", hla_federate_handle.encode()},
                         {L"HLAobjectClass", hla_object_class.encode()},
                         {L"HLAattributeList", hla_attr_list.encode()},
                         {L"HLAactive", hla_active.encode()}});
}

void MomCli1516e::command_federate_service_un_subscribe_object_class_attributes()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAunsubscribeObjectClassAttributes\n";

        int federate_handle = std::stoi(requestArgument("HLAfederate"));
        int object_class = std::stoi(requestArgument("HLAobjectClass"));
        auto attr_list = requestVectorArguments("HLAattributeList");

        ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
        ::rti1516e::HLAinteger32LE hla_object_class(object_class);
        ::rti1516e::HLAvariableArray hla_attr_list(hla_object_class);
        for(int attr : attr_list) {
            hla_attr_list.addElement(::rti1516e::HLAinteger32LE(attr));
        }

        sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAunsubscribeObjectClassAttributes",
                        {{L"HLAfederate", hla_federate_handle.encode()},
                         {L"HLAobjectClass", hla_object_class.encode()},
                         {L"HLAattributeList", hla_attr_list.encode()}});
}

void MomCli1516e::command_federate_service_subscribe_interaction_class()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLArequest.HLAsubscribeInteractionClass\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    int interaction_class = std::stoi(requestArgument("HLAinteractionClass"));
    bool active = std::stoi(requestArgument("HLAactive"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAinteger32LE hla_interaction_class(interaction_class);
    ::rti1516e::HLAboolean hla_active(active);
    sendInteraction(L"HLAmanager.HLAfederate.HLArequest.HLAsubscribeInteractionClass",
                    {{L"HLAfederate", hla_federate_handle.encode()},
                     {L"HLAinteractionClass", hla_interaction_class.encode()},
                     {L"HLAactive", hla_active.encode()}});
}

void MomCli1516e::command_federate_service_un_subscribe_interaction_class()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAunsubscribeInteractionClass\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    int interaction_class = std::stoi(requestArgument("HLAinteractionClass"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAinteger32LE hla_interaction_class(interaction_class);
    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAunsubscribeInteractionClass",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAinteractionClass", hla_interaction_class.encode()}});
}

void MomCli1516e::command_federate_service_delete_object_instance()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAdeleteObjectInstance\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    int object = std::stoi(requestArgument("HLAobjectInstance"));
    std::string tag = requestArgument("HLAtag");
    double timestamp = std::stod(requestArgument("HLAtimeStamp"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAinteger32LE hla_object(object);
    ::rti1516e::HLAASCIIstring hla_tag(tag);
//    ::rti1516e::HLAfloat64LE hla_timestamp(timestamp);
    ::rti1516e::HLAfloat64Time hla_timestamp(timestamp);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAdeleteObjectInstance",
                    {{L"HLAfederate", hla_federate_handle.encode()},
                     {L"HLAobjectInstance", hla_object.encode()},
                     {L"HLAtag", hla_tag.encode()},
                     {L"HLAtimeStamp", hla_timestamp.encode()}});
}

void MomCli1516e::command_federate_service_local_delete_object_instance()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAlocalDeleteObjectInstance\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    int object = std::stoi(requestArgument("HLAobjectInstance"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAinteger32LE hla_object(object);
    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAlocalDeleteObjectInstance",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAobjectInstance", hla_object.encode()}});
}

void MomCli1516e::command_federate_service_request_attribute_transportation_type_change()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLArequestAttributeTransportationTypeChange\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    int object = std::stoi(requestArgument("HLAobjectInstance"));
    auto attr_list = requestVectorArguments("HLAattributeList");
    std::string transportation = requestArgument("HLAtransportation");

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAinteger32LE hla_object(object);
    ::rti1516e::HLAvariableArray hla_attr_list(hla_object);
    for(int attr : attr_list) {
        hla_attr_list.addElement(::rti1516e::HLAinteger32LE(attr));
    }
    ::rti1516e::HLAASCIIstring hla_transportation(transportation);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLArequestAttributeTransportationTypeChange",
                    {{L"HLAfederate", hla_federate_handle.encode()},
                     {L"HLAobjectInstance", hla_object.encode()},
                     {L"HLAattributeList", hla_attr_list.encode()},
                     {L"HLAtransportation", hla_transportation.encode()}});
}

void MomCli1516e::command_federate_service_request_interaction_transportation_type_change()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLArequestInteractionTransportationTypeChange\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    int interaction_class = std::stoi(requestArgument("HLAinteractionClass"));
    std::string transportation = requestArgument("HLAtransportation");

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAinteger32LE hla_interaction_class(interaction_class);
    ::rti1516e::HLAASCIIstring hla_transportation(transportation);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLArequestInteractionTransportationTypeChange",
                    {{L"HLAfederate", {&federate_handle, 4}},
                     {L"HLAinteractionClass", {&interaction_class, 4}},
                     {L"HLAtransportation", {transportation.c_str(), transportation.size()}}});
}

void MomCli1516e::command_federate_service_unconditional_attribute_ownership_divestiture()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAunconditionalAttributeOwnershipDivestiture\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    int object = std::stoi(requestArgument("HLAobjectInstance"));
    auto attr_list = requestVectorArguments("HLAattributeList");

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAinteger32LE hla_object(object);
    ::rti1516e::HLAvariableArray hla_attr_list(hla_object);
    for(int attr : attr_list) {
        hla_attr_list.addElement(::rti1516e::HLAinteger32LE(attr));
    }


    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAunconditionalAttributeOwnershipDivestiture",
                    {{L"HLAfederate", hla_federate_handle.encode()},
                     {L"HLAobjectInstance", hla_object.encode()},
                     {L"HLAattributeList", hla_attr_list.encode()}});
}

void MomCli1516e::command_federate_service_enable_time_regulation()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAenableTimeRegulation\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    double lookahead = std::stod(requestArgument("HLAlookahead"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAfloat64LE hla_lookhead(lookahead);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAenableTimeRegulation",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAlookahead", hla_lookhead.encode()}});
}

void MomCli1516e::command_federate_service_disable_time_regulation()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAdisableTimeRegulation\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAdisableTimeRegulation",
    {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_service_enable_time_constrained()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAenableTimeConstrained\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAenableTimeConstrained",
    {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_service_disable_time_constrained()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAdisableTimeConstrained\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAdisableTimeConstrained",
    {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_service_time_advance_request()
{

    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAtimeAdvanceRequest\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    double timestamp = std::stod(requestArgument("HLAtimeStamp"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
//    ::rti1516e::HLAfloat64LE hla_timestamp(timestamp);
    ::rti1516e::HLAfloat64Time hla_timestamp(timestamp);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAtimeAdvanceRequest",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAtimeStamp", hla_timestamp.encode()}});
}

void MomCli1516e::command_federate_service_time_advance_request_available()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAtimeAdvanceRequestAvailable\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    double timestamp = std::stod(requestArgument("HLAtimeStamp"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
//    ::rti1516e::HLAfloat64LE hla_timestamp(timestamp);
    ::rti1516e::HLAfloat64Time hla_timestamp(timestamp);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAtimeAdvanceRequestAvailable",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAtimeStamp", hla_timestamp.encode()}});
}

void MomCli1516e::command_federate_service_next_message_request()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAnextMessageRequest\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    double timestamp = std::stod(requestArgument("HLAtimeStamp"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
//    ::rti1516e::HLAfloat64LE hla_timestamp(timestamp);
    ::rti1516e::HLAfloat64Time hla_timestamp(timestamp);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAnextMessageRequest",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAtimeStamp", hla_timestamp.encode()}});
}

void MomCli1516e::command_federate_service_next_message_request_available()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAnextMessageRequestAvailable\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    double timestamp = std::stod(requestArgument("HLAtimeStamp"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
//    ::rti1516e::HLAfloat64LE hla_timestamp(timestamp);
    ::rti1516e::HLAfloat64Time hla_timestamp(timestamp);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAnextMessageRequestAvailable",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAtimeStamp", hla_timestamp.encode()}});
}

void MomCli1516e::command_federate_service_flush_queue_request()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAflushQueueRequest\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    double timestamp = std::stod(requestArgument("HLAtimeStamp"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
//    ::rti1516e::HLAfloat64LE hla_timestamp(timestamp);
    ::rti1516e::HLAfloat64Time hla_timestamp(timestamp);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAflushQueueRequest",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAtimeStamp", hla_timestamp.encode()}});
}

void MomCli1516e::command_federate_service_enable_asynchronous_delivery()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAenableAsynchronousDelivery\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAenableAsynchronousDelivery",
    {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_service_disable_asynchronous_delivery()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAdisableAsynchronousDelivery\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAdisableAsynchronousDelivery",
    {{L"HLAfederate", hla_federate_handle.encode()}});
}

void MomCli1516e::command_federate_service_modify_lookahead()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAmodifyLookahead\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    double lookahead = std::stod(requestArgument("HLAlookahead"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAfloat64LE hla_lookhead(lookahead);


    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAmodifyLookahead",
    {{L"HLAfederate", hla_federate_handle.encode()}, {L"HLAlookahead", hla_lookhead.encode()}});
}

void MomCli1516e::command_federate_service_change_attribute_order_type()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAchangeAttributeOrderType\n";

        int federate_handle = std::stoi(requestArgument("HLAfederate"));
        int object = std::stoi(requestArgument("HLAobjectInstance"));
        auto attr_list = requestVectorArguments("HLAattributeList");
        int order = std::stoi(requestArgument("HLAsendOrder"));

        ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
        ::rti1516e::HLAinteger32LE hla_object(object);
        ::rti1516e::HLAvariableArray hla_attr_list(hla_object);
        for(int attr : attr_list) {
            hla_attr_list.addElement(::rti1516e::HLAinteger32LE(attr));
        }
        ::rti1516e::HLAinteger32LE hla_order(order);

        sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAchangeAttributeOrderType",
                        {{L"HLAfederate", hla_federate_handle.encode()},
                         {L"HLAobjectInstance", hla_object.encode()},
                         {L"HLAattributeList", hla_attr_list.encode()},
                         {L"HLAsendOrder", hla_order.encode()}});
}

void MomCli1516e::command_federate_service_change_interaction_order_type()
{
    std::wcout << "1516e=>HLAmanager.HLAfederate.HLAservice.HLAchangeInteractionOrderType\n";

    int federate_handle = std::stoi(requestArgument("HLAfederate"));
    int interaction_class = std::stoi(requestArgument("HLAinteractionClass"));
    int order = std::stoi(requestArgument("HLAsendOrder"));

    ::rti1516e::HLAinteger32LE hla_federate_handle(federate_handle);
    ::rti1516e::HLAinteger32LE hla_interaction_class(interaction_class);
    ::rti1516e::HLAinteger32LE hla_order(order);

    sendInteraction(L"HLAmanager.HLAfederate.HLAservice.HLAchangeInteractionOrderType",
                    {{L"HLAfederate", hla_federate_handle.encode()},
                     {L"HLAinteractionClass", hla_interaction_class.encode()},
                     {L"HLAsendOrder", hla_order.encode()}});
}

void MomCli1516e::command_federation_adjust_set_switches()
{
    std::wcout << "1516e=>HLAmanager.HLAfederation.HLAadjust.HLAsetSwitches\n";

    bool auto_provide = std::stoi(requestArgument("HLAautoProvide"));
    ::rti1516e::HLAboolean hla_auto_provide(auto_provide);

    sendInteraction(L"HLAmanager.HLAfederation.HLAadjust.HLAsetSwitches", {{L"HLAautoProvide", hla_auto_provide.encode()}});
}

void MomCli1516e::command_federation_request_synchronization_point_status()
{
    std::wcout << "1516e=>HLAmanager.HLAfederation.HLArequest.HLArequestSynchronizationPointStatus\n";

    std::string sync_point = requestArgument("HLAsyncPointName");
    ::rti1516e::HLAASCIIstring hla_sync_point(sync_point);

    sendInteraction(L"HLAmanager.HLAfederation.HLArequest.HLArequestSynchronizationPointStatus",
    {{L"HLAsyncPointName", hla_sync_point.encode()}});
}

void MomCli1516e::command_federation_request_FOM_module_data()
{
    std::wcout << "1516e=>HLAmanager.HLAfederation.HLArequest.HLArequestFOMmoduleData\n";

    int fom_module = std::stoi(requestArgument("HLAFOMmoduleIndicator"));
    ::rti1516e::HLAinteger32LE hla_fom_module(fom_module);

    sendInteraction(L"HLAmanager.HLAfederation.HLArequest.HLArequestFOMmoduleData",
                    {{L"HLAFOMmoduleIndicator", hla_fom_module.encode()}});
}




