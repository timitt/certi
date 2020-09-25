#ifndef MOMCLI_H
#define MOMCLI_H

#include "cli.h"

#include <RTI/RTIambassador.h>

class MomCli : public cli {
public:
    MomCli(rti1516e::RTIambassador& amb, const std::wstring& federation_name);

private:
    void command_tick();
    void command_resign_federation_execution();
    void command_destroy_federation_execution(const std::wstring& federation_name);
    void command_enable_time_regulation();
    void command_enable_time_constrained();
    void command_disable_time_regulation();
    void command_disable_time_constrained();

    //////////////////////
    // MOM INTERACTIONS //
    //////////////////////

    virtual void command_federate_adjust_set_timing();
    virtual void command_federate_adjust_modify_attribute_state();
    virtual void command_federate_adjust_set_switches();
    virtual void command_federate_request_publications();
    virtual void command_federate_request_subscriptions();
    virtual void command_federate_request_object_instances_that_can_be_deleted();
    virtual void command_federate_request_object_instances_updated();
    virtual void command_federate_request_object_instances_reflected();
    virtual void command_federate_request_updates_sent();
    virtual void command_federate_request_interactions_sent();
    virtual void command_federate_request_reflections_received();
    virtual void command_federate_request_interactions_received();
    virtual void command_federate_request_object_instance_information();
    virtual void command_federate_request_FOM_module_data();
    virtual void command_federate_service_resign_federation_execution();
    virtual void command_federate_service_synchronization_point_achieved();
    virtual void command_federate_service_federate_save_begun();
    virtual void command_federate_service_federate_save_complete();
    virtual void command_federate_service_federate_restore_complete();
    virtual void command_federate_service_publish_object_class_attributes();
    virtual void command_federate_service_un_publish_object_class_attributes();
    virtual void command_federate_service_publish_interaction_class();
    virtual void command_federate_service_un_publish_interaction_class();
    virtual void command_federate_service_subscribe_object_class_attributes();
    virtual void command_federate_service_un_subscribe_object_class_attributes();
    virtual void command_federate_service_subscribe_interaction_class();
    virtual void command_federate_service_un_subscribe_interaction_class();
    virtual void command_federate_service_delete_object_instance();
    virtual void command_federate_service_local_delete_object_instance();
    virtual void command_federate_service_request_attribute_transportation_type_change();
    virtual void command_federate_service_request_interaction_transportation_type_change();
    virtual void command_federate_service_unconditional_attribute_ownership_divestiture();
    virtual void command_federate_service_enable_time_regulation();
    virtual void command_federate_service_disable_time_regulation();
    virtual void command_federate_service_enable_time_constrained();
    virtual void command_federate_service_disable_time_constrained();
    virtual void command_federate_service_time_advance_request();
    virtual void command_federate_service_time_advance_request_available();
    virtual void command_federate_service_next_message_request();
    virtual void command_federate_service_next_message_request_available();
    virtual void command_federate_service_flush_queue_request();
    virtual void command_federate_service_enable_asynchronous_delivery();
    virtual void command_federate_service_disable_asynchronous_delivery();
    virtual void command_federate_service_modify_lookahead();
    virtual void command_federate_service_change_attribute_order_type();
    virtual void command_federate_service_change_interaction_order_type();
    virtual void command_federation_adjust_set_switches();
    virtual void command_federation_request_synchronization_points();
    virtual void command_federation_request_synchronization_point_status();
    virtual void command_federation_request_FOM_module_data();
    virtual void command_federation_request_MIM_data();

    rti1516e::RTIambassador& my_ambassador;

protected:
    std::vector<int> requestVectorArguments(const std::string& name);
    void sendInteraction(const std::wstring& interaction,
                         std::map<std::wstring, rti1516e::VariableLengthData> parameters);

};

#endif // MOMCLI_H
