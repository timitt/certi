#ifndef MOMCLI1516E_H
#define MOMCLI1516E_H

#include "momcli.h"

class MomCli1516e : public MomCli {
public:
    MomCli1516e(rti1516e::RTIambassador& amb, const std::wstring& federation_name);

    virtual ~MomCli1516e() = default;


    // MomCli interface
private:
    void command_federate_adjust_set_timing() override;
    void command_federate_adjust_modify_attribute_state() override;
    void command_federate_adjust_set_switches() override;
    void command_federate_request_publications() override;
    void command_federate_request_subscriptions() override;
    void command_federate_request_object_instances_that_can_be_deleted() override;
    void command_federate_request_object_instances_updated() override;
    void command_federate_request_object_instances_reflected() override;
    void command_federate_request_updates_sent() override;
    void command_federate_request_interactions_sent() override;
    void command_federate_request_reflections_received() override;
    void command_federate_request_interactions_received() override;
    void command_federate_request_object_instance_information() override;
    void command_federate_request_FOM_module_data() override;
    void command_federate_service_resign_federation_execution() override;
    void command_federate_service_synchronization_point_achieved() override;
    void command_federate_service_federate_save_begun() override;
    void command_federate_service_federate_save_complete() override;
    void command_federate_service_federate_restore_complete() override;
    void command_federate_service_publish_object_class_attributes() override;
    void command_federate_service_un_publish_object_class_attributes() override;
    void command_federate_service_publish_interaction_class() override;
    void command_federate_service_un_publish_interaction_class() override;
    void command_federate_service_subscribe_object_class_attributes() override;
    void command_federate_service_un_subscribe_object_class_attributes() override;
    void command_federate_service_subscribe_interaction_class() override;
    void command_federate_service_un_subscribe_interaction_class() override;
    void command_federate_service_delete_object_instance() override;
    void command_federate_service_local_delete_object_instance() override;
    void command_federate_service_request_attribute_transportation_type_change() override;
    void command_federate_service_request_interaction_transportation_type_change() override;
    void command_federate_service_unconditional_attribute_ownership_divestiture() override;
    void command_federate_service_enable_time_regulation() override;
    void command_federate_service_disable_time_regulation() override;
    void command_federate_service_enable_time_constrained() override;
    void command_federate_service_disable_time_constrained() override;
    void command_federate_service_time_advance_request() override;
    void command_federate_service_time_advance_request_available() override;
    void command_federate_service_next_message_request() override;
    void command_federate_service_next_message_request_available() override;
    void command_federate_service_flush_queue_request() override;
    void command_federate_service_enable_asynchronous_delivery() override;
    void command_federate_service_disable_asynchronous_delivery() override;
    void command_federate_service_modify_lookahead() override;
    void command_federate_service_change_attribute_order_type() override;
    void command_federate_service_change_interaction_order_type() override;
    void command_federation_adjust_set_switches() override;
    void command_federation_request_synchronization_point_status() override;
    void command_federation_request_FOM_module_data() override;
};

#endif
