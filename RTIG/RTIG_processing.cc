// ----------------------------------------------------------------------------
// CERTI - HLA RunTime Infrastructure
// Copyright (C) 2002, 2003  ONERA
//
// This file is part of CERTI
//
// CERTI is free software ; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation ; either version 2 of the License, or
// (at your option) any later version.
//
// CERTI is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY ; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
// $Id: RTIG_processing.cc,v 3.17 2003/07/07 23:05:26 breholee Exp $
// ----------------------------------------------------------------------------

#include <config.h>
#include "RTIG.hh"

#include <assert.h>

namespace certi {
namespace rtig {

static pdCDebug D("RTIG", "(RTIG)- ");

// ----------------------------------------------------------------------------
//! Creates a new federation.
void
RTIG::processCreateFederation(Socket *link, NetworkMessage *req)
{
    char *federation = req->federationName ;

    if (federation == NULL) throw RTIinternalError("Invalid Federation Name.");

    auditServer->addToLinef("Federation Name : %s", federation);

#ifdef FEDERATION_USES_MULTICAST
    // multicast base address
    unsigned long base_adr_mc = inet_addr(ADRESSE_MULTICAST);
    SocketMC *com_mc = NULL ;

    // creer la communication multicast
    com_mc = new SocketMC();
    if (com_mc == NULL) {
        D.Out(pdExcept, "Unable to allocate Multicast socket.");
        throw RTIinternalError("Unable to allocate Multicast socket.");
    }

    com_mc->CreerSocketMC(base_adr_mc + nextFederationHandle, MC_PORT);

    // inserer la nouvelle federation dans la liste des federations
    federations->createFederation(federation, nextFederationHandle, com_mc);

    // inserer descripteur fichier pour le prochain appel a un select
    ClientSockets.push_front(com_mc);

#else
    federations->createFederation(federation, nextFederationHandle);
#endif

    nextFederationHandle++ ;

    // Prepare answer for RTIA
    NetworkMessage rep ;
    rep.type = NetworkMessage::CREATE_FEDERATION_EXECUTION ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federation = nextFederationHandle ;

    rep.write(link); // Send answer to RTIA

    D.Out(pdInit, "Federation \"%s\" created with Handle %d.",
          federation, rep.federation);
}

// ----------------------------------------------------------------------------
//! Add a new federate to federation.
void
RTIG::processJoinFederation(Socket *link, NetworkMessage *req)
{
    char *federation = req->federationName ;
    char *federate = req->federateName ;

    unsigned int peer = req->bestEffortPeer ;
    unsigned long address = req->bestEffortAddress ;

    Handle num_federation ;
    FederateHandle num_federe ;

    int nb_regulateurs ;
    int nb_federes ;
    bool pause ;

    if ((federation == NULL) || (federate == NULL))
        throw RTIinternalError("Invalid Federation/Federate Name.");

    auditServer->addToLinef("Federate \"%s\" joins Federation \"%s\"",
                            federate, federation);

    federations->exists(federation, num_federation);

    num_federe = federations->addFederate(num_federation,
                                          federate,
                                          (SecureTCPSocket *) link);

#ifdef FEDERATION_USES_MULTICAST
    SocketMC *com_mc = NULL ;

    federations->info(num_federation, nb_federes, nb_regulateurs,
                      pause, com_mc);
    assert(com_mc != NULL);
#else
    federations->info(num_federation, nb_federes, nb_regulateurs, pause);
#endif

    // Store Federate <->Socket reference.
    socketServer->setReferences(link->returnSocket(),
                                num_federation,
                                num_federe,
                                address,
                                peer);

    auditServer->addToLinef("(%d)with handle %d. Socket %d",
                            num_federation, num_federe,
                            link->returnSocket());

    // Prepare answer
    NetworkMessage rep ;
    rep.type = NetworkMessage::JOIN_FEDERATION_EXECUTION ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = num_federe ;
    rep.federation = num_federation ;
    rep.numberOfRegulators = nb_regulateurs ;
    rep.bestEffortPeer = peer ;
    rep.bestEffortAddress = address ;

#ifdef FEDERATION_USES_MULTICAST
    rep.AdresseMulticast = com_mc->returnAdress();
#endif

    D.Out(pdInit, "Federate \"%s\" has joined Federation %u under handle %u.",
          federate, num_federation, num_federe);

    // Send answer
    rep.write(link);
}

// ----------------------------------------------------------------------------
//! Removes a federate from federation.
void
RTIG::processResignFederation(Handle federation,
                              FederateHandle federe)
{
    federations->remove(federation, federe);
    D.Out(pdInit, "Federate %u is resigning from federation %u.",
          federe, federation);
}

// ----------------------------------------------------------------------------
//! Removes a federation.
void
RTIG::processDestroyFederation(Socket *link, NetworkMessage *req)
{
    Handle num_federation ;
    char *federation = req->federationName ;

    if (federation == NULL) throw RTIinternalError("Invalid Federation Name.");

    auditServer->addToLinef("Name \"%s\"", federation);
    federations->exists(federation, num_federation);
    federations->destroyFederation(num_federation);
    D.Out(pdInit, "Federation \"%s\" has been destroyed.", federation);

    NetworkMessage rep ;
    rep.type = NetworkMessage::DESTROY_FEDERATION_EXECUTION ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;

    rep.write(link);
}

// ----------------------------------------------------------------------------
//! Put federate as time regulating.
void
RTIG::processSetTimeRegulating(NetworkMessage *msg)
{
    if (msg->regulator) {
        auditServer->addToLinef("ON at time %f", msg->date);

        federations->createRegulator(msg->federation,
                                     msg->federate,
                                     msg->date);
        D.Out(pdTerm, "Federate %u of Federation %u sets TimeRegulation ON.",
              msg->federate, msg->federation);
    }
    else {
        auditServer->addToLine("OFF");

        federations->removeRegulator(msg->federation,
                                     msg->federate);
        D.Out(pdTerm, "Federate %u of Federation %u sets TimeRegulation OFF.",
              msg->federate, msg->federation);
    }
}

// ----------------------------------------------------------------------------
//! Put federate as time constrained
void
RTIG::processSetTimeConstrained(NetworkMessage *msg)
{
    if (msg->constrained) {
        auditServer->addToLinef("ON at time %f", msg->date);

        federations->addConstrained(msg->federation,
                                    msg->federate);
        D.Out(pdTerm, "Federate %u of Federation %u is now constrained.",
              msg->federate, msg->federation);
    }
    else {
        auditServer->addToLine("OFF");

        federations->removeConstrained(msg->federation,
                                       msg->federate);
        D.Out(pdTerm, "Federate %u of Federation %u is no more constrained.",
              msg->federate, msg->federation);
    }
}

// ----------------------------------------------------------------------------
//! processMessageNull.
void
RTIG::processMessageNull(NetworkMessage *msg)
{
    auditServer->addToLinef("Date %f", msg->date);

    // Catch all exceptions because RTIA does not expect an answer anyway.
    try {
        federations->updateRegulator(msg->federation,
                                     msg->federate,
                                     msg->date);
    } catch (Exception &e) {}
}

// ----------------------------------------------------------------------------
//! processRegisterSynchronization.
void
RTIG::processRegisterSynchronization(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Label \"%s\" registered. Tag is \"%s\"",
                            req->label, req->tag);
    federations->manageSynchronization(req->federation,
                                       req->federate,
                                       true,
                                       req->label,
                                       req->tag);
    D.Out(pdTerm, "Federation %u is now synchronizing.", req->federation);

    // send synchronizationPointRegistrationSucceeded() to federate.
    NetworkMessage rep ;
    rep.type = NetworkMessage::SYNCHRONIZATION_POINT_REGISTRATION_SUCCEEDED ;
    rep.federate = req->federate ;
    rep.federation = req->federation ;
    rep.setLabel(req->label);
    rep.write(link);

    federations->broadcastSynchronization(req->federation,
                                          req->federate,
                                          req->label,
                                          req->tag);
}

// ----------------------------------------------------------------------------
//! processSynchronizationAchieved.
void
RTIG::processSynchronizationAchieved(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Label \"%s\" ended.", req->label);

    federations->manageSynchronization(req->federation,
                                       req->federate,
                                       false,
                                       req->label,
                                       "");
    D.Out(pdTerm, "Federate %u has synchronized.", req->federate);
}

// ----------------------------------------------------------------------------
void
RTIG::processRequestFederationSave(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Federation save request.");

    federations->requestFederationSave(req->federation, req->federate,
                                       req->label, req->date);
}
// ----------------------------------------------------------------------------
void
RTIG::processFederateSaveBegun(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Federate %u save begun.", req->federate);

    federations->federateSaveBegun(req->federation, req->federate);
}

// ----------------------------------------------------------------------------
void
RTIG::processFederateSaveStatus(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Federate %u save ended.", req->federate);

    bool status = req->type == NetworkMessage::FEDERATE_SAVE_COMPLETE ;
    federations->federateSaveStatus(req->federation, req->federate, status);
}

// ----------------------------------------------------------------------------
void
RTIG::processRequestFederationRestore(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Federate %u request restore.", req->federate);

    federations->requestFederationRestore(req->federation, req->federate,
                                          req->label);
}

// ----------------------------------------------------------------------------
void
RTIG::processFederateRestoreStatus(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Federate %u restore ended.", req->federate);

    bool status = (req->type == NetworkMessage::FEDERATE_RESTORE_COMPLETE) ? true : false ;
    federations->federateRestoreStatus(req->federation, req->federate, status);
}

// ----------------------------------------------------------------------------
// processPublishObjectClass
void
RTIG::processPublishObjectClass(Socket *link, NetworkMessage *req)
{
    bool pub = (req->type == NetworkMessage::PUBLISH_OBJECT_CLASS);

    auditServer->addToLinef("Class = %u, # of att. = %u",
                            req->objectClass,
                            req->handleArraySize);

    federations->publishObject(req->federation,
                               req->federate,
                               req->objectClass,
                               req->handleArray,
                               req->handleArraySize,
                               pub);

    D.Out(pdRegister, "Federate %u of Federation %u published object class %d.",
          req->federate, req->federation, req->objectClass);

    NetworkMessage rep ;
    rep.type = req->type ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.objectClass = req->objectClass ;
    rep.handleArraySize = 0 ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processSubscribeObjectClass
void
RTIG::processSubscribeObjectClass(Socket *link, NetworkMessage *req)
{
    bool sub = (req->type == NetworkMessage::SUBSCRIBE_OBJECT_CLASS);

    auditServer->addToLinef("Class = %u, # of att. = %u",
                            req->objectClass,
                            req->handleArraySize);

    federations->subscribeObject(req->federation,
                                 req->federate,
                                 req->objectClass,
                                 req->handleArray,
                                 req->handleArraySize,
                                 sub);

    D.Out(pdRegister,
          "Federate %u of Federation %u subscribed to object class %d.",
          req->federate, req->federation, req->objectClass);

    NetworkMessage rep ;
    rep.type = req->type ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.objectClass = req->objectClass ;
    rep.handleArraySize = 0 ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processPublishInteractionClass
void
RTIG::processPublishInteractionClass(Socket *link, NetworkMessage *req)
{
    assert(link != NULL && req != NULL);

    bool pub = (req->type == NetworkMessage::PUBLISH_INTERACTION_CLASS);

    auditServer->addToLinef("Class = %u", req->interactionClass);
    federations->publishInteraction(req->federation,
                                    req->federate,
                                    req->interactionClass,
                                    pub);
    D.Out(pdRequest, "Federate %u of Federation %u publishes Interaction %d.",
          req->federate,
          req->federation,
          req->interactionClass);

    NetworkMessage rep ;
    rep.type = req->type ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.interactionClass = req->interactionClass ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processSubscribeInteractionClass
void
RTIG::processSubscribeInteractionClass(Socket *link, NetworkMessage *req)
{
    bool sub = (req->type == NetworkMessage::SUBSCRIBE_INTERACTION_CLASS);

    auditServer->addToLinef("Class = %u", req->interactionClass);
    federations->subscribeInteraction(req->federation,
                                      req->federate,
                                      req->interactionClass,
                                      sub);
    D.Out(pdRequest,
          "Federate %u of Federation %u subscribed to Interaction %d.",
          req->federate,
          req->federation,
          req->interactionClass);

    NetworkMessage rep ;
    rep.type = req->type ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.interactionClass = req->interactionClass ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processRequestId
void
RTIG::processRequestId(Socket *link, NetworkMessage *req)
{
    NetworkMessage rep ;

    auditServer->addToLinef("Asked %u", req->idCount);
    federations->requestId(req->federation,
                           req->idCount,
                           rep.firstId,
                           rep.lastId);
    auditServer->addToLinef(", given range from %u to %u",
                            rep.firstId, rep.lastId);

    D.Out(pdInit, "%d IDs have been sent for Federation %u.",
          req->idCount, req->federation);

    rep.type = req->type ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processRegisterObject
void
RTIG::processRegisterObject(Socket *link, NetworkMessage *req)
{
    NetworkMessage rep ;

    auditServer->addToLinef("Class = %u", req->objectClass);
    rep.object = federations->registerObject(req->federation,
                                             req->federate,
                                             req->objectClass,
                                             req->label);
    auditServer->addToLinef(", Handle = %u", rep.object);

    D.Out(pdRegister,
          "Object \"%s\" of Federate %u has been registered under ID %u.",
          req->label, req->federate, rep.object);

    rep.type = req->type ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    // rep.object is set by the call of registerObject

    rep.write(link); // Send answer to RTIA
}

// ----------------------------------------------------------------------------
// processUpdateAttributeValues
void
RTIG::processUpdateAttributeValues(Socket *link, NetworkMessage *req)
{
    AttributeValue *ValueArray = NULL ;

    auditServer->addToLinef("ObjID = %u, Date = %f", req->object, req->date);

    // Prepare le Value Array
    ValueArray = req->getAttribValueArray();

    // Propage l'appel
    federations->updateAttribute(req->federation,
                                 req->federate,
                                 req->object,
                                 req->handleArray,
                                 ValueArray,
                                 req->handleArraySize,
                                 req->date,
                                 req->label);
    free(ValueArray);

    // Prepare la reponse
    NetworkMessage rep ;
    rep.type = NetworkMessage::UPDATE_ATTRIBUTE_VALUES ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.object = req->object ;
    rep.date = req->date ;
    rep.handleArraySize = 0 ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processSendInteraction
void
RTIG::processSendInteraction(Socket *link, NetworkMessage *req)
{
    AttributeValue *values = NULL ;

    // Prepare le Value Array
    auditServer->addToLinef("IntID = %u, date = %f",
                            req->interactionClass, req->date);
    values = req->getParamValueArray();

    federations->updateParameter(req->federation,
                                 req->federate,
                                 req->interactionClass,
                                 req->handleArray,
                                 values,
                                 req->handleArraySize,
                                 req->date,
                                 req->label);
    free(values);

    D.Out(pdDebug, "Mise A Jour des parametres de l'interaction %d terminee",
          req->interactionClass);

    NetworkMessage rep ;
    rep.type = NetworkMessage::SEND_INTERACTION ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.interactionClass = req->interactionClass ;
    rep.handleArraySize = 0 ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processdeleteObject
void
RTIG::processDeleteObject(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("ObjID = %u", req->object);

    federations->destroyObject(req->federation,
                               req->federate,
                               req->object,
                               req->label);

    D.Out(pdRegister, "Object # %u of Federation %u has been deleted.",
          req->object, req->federation);

    NetworkMessage rep ;
    rep.type = NetworkMessage::DELETE_OBJECT ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.object = req->object ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processqueryAttributeOwnership
void
RTIG::processQueryAttributeOwnership(Socket *link, NetworkMessage *req)
{
    D.Out(pdDebug, "Owner of Attribute %u of Object %u .",
          req->handleArray[0], req->object);

    auditServer->addToLinef("AttributeHandle = %u", req->handleArray[0]);

    federations->searchOwner(req->federation,
                             req->federate,
                             req->object,
                             req->handleArray[0]);

    D.Out(pdDebug, "Owner of Attribute %u of Object %u .",
          req->handleArray[0], req->object);

    NetworkMessage rep ;
    rep.type = NetworkMessage::QUERY_ATTRIBUTE_OWNERSHIP ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.object = req->object ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processAttributeOwnedByFederate
void
RTIG::processAttributeOwnedByFederate(Socket *link, NetworkMessage *req)
{
    NetworkMessage rep ;

    D.Out(pdDebug, "Owner of Attribute %u of Object %u .",
          req->handleArray[0], req->object);

    auditServer->addToLinef("AttributeHandle = %u", req->handleArray[0]);

    if (federations->isOwner(req->federation,
                             req->federate,
                             req->object,
                             req->handleArray[0]))
        strcpy(rep.label, "RTI_TRUE");
    else
        strcpy(rep.label, "RTI_FALSE");

    D.Out(pdDebug, "Owner of Attribute %u of Object %u .",
          req->handleArray[0], req->object);

    rep.type = NetworkMessage::IS_ATTRIBUTE_OWNED_BY_FEDERATE ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.object = req->object ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processNegotiatedOwnershipDivestiture
void
RTIG::processNegotiatedOwnershipDivestiture(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Object = %u, # of att. = %u",
                            req->object,
                            req->handleArraySize);
    federations->negotiateDivestiture(req->federation,
                                      req->federate,
                                      req->object,
                                      req->handleArray,
                                      req->handleArraySize,
                                      req->label);

    D.Out(pdDebug, "Federate %u of Federation %u negotiate "
          "divestiture of object %u.",
          req->federate, req->federation, req->object);

    NetworkMessage rep ;
    rep.type = NetworkMessage::NEGOTIATED_ATTRIBUTE_OWNERSHIP_DIVESTITURE ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.object = req->object ;
    rep.handleArraySize = 0 ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processAcquisitionIfAvailable
void
RTIG::processAcquisitionIfAvailable(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Object = %u, # of att. = %u",
                            req->object,
                            req->handleArraySize);

    federations->acquireIfAvailable(req->federation,
                                    req->federate,
                                    req->object,
                                    req->handleArray,
                                    req->handleArraySize);

    D.Out(pdDebug,
          "Federate %u of Federation %u acquisitionIfAvailable "
          "of object %u.",
          req->federate, req->federation, req->object);

    NetworkMessage rep ;
    rep.type = NetworkMessage::ATTRIBUTE_OWNERSHIP_ACQUISITION_IF_AVAILABLE ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.object = req->object ;
    rep.handleArraySize = 0 ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processUnconditionalDivestiture
void
RTIG::processUnconditionalDivestiture(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Object = %u, # of att. = %u",
                            req->object,
                            req->handleArraySize);

    federations->divest(req->federation,
                        req->federate,
                        req->object,
                        req->handleArray,
                        req->handleArraySize);

    D.Out(pdDebug,
          "Federate %u of Federation %u UnconditionalDivestiture "
          "of object %u.",
          req->federate, req->federation, req->object);

    NetworkMessage rep ;
    rep.type = NetworkMessage::UNCONDITIONAL_ATTRIBUTE_OWNERSHIP_DIVESTITURE ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.object = req->object ;
    rep.handleArraySize = 0 ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processOwnershipAcquisition
void
RTIG::processOwnershipAcquisition(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Object = %u, # of att. = %u",
                            req->object,
                            req->handleArraySize);

    federations->acquire(req->federation,
                         req->federate,
                         req->object,
                         req->handleArray,
                         req->handleArraySize,
                         req->label);

    D.Out(pdDebug,
          "Federate %u of Federation %u ownership acquisition of object %u.",
          req->federate, req->federation, req->object);

    NetworkMessage rep ;
    rep.type = NetworkMessage::ATTRIBUTE_OWNERSHIP_ACQUISITION ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.object = req->object ;
    rep.handleArraySize = 0 ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processCancelNegotiatedDivestiture
void
RTIG::processCancelNegotiatedDivestiture(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Object = %u, # of att. = %u",
                            req->object,
                            req->handleArraySize);

    federations->cancelDivestiture(req->federation,
                                   req->federate,
                                   req->object,
                                   req->handleArray,
                                   req->handleArraySize);

    D.Out(pdDebug, "Federate %u of Federation %u cancel negotiate "
          "divestiture of object %u.",
          req->federate, req->federation, req->object);

    NetworkMessage rep ;
    rep.type = NetworkMessage::CANCEL_NEGOTIATED_ATTRIBUTE_OWNERSHIP_DIVESTITURE ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.object = req->object ;
    rep.handleArraySize = 0 ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processReleaseResponse
void
RTIG::processReleaseResponse(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Object = %u, # of att. = %u",
                            req->object,
                            req->handleArraySize);

    AttributeHandleSet *attributes =
        federations->respondRelease(req->federation,
                                    req->federate,
                                    req->object,
                                    req->handleArray,
                                    req->handleArraySize);

    D.Out(pdDebug, "Federate %u of Federation %u release response "
          "of object %u.",
          req->federate, req->federation, req->object);

    NetworkMessage rep ;
    rep.handleArraySize = attributes->size();

    for (unsigned int i = 0 ; i < attributes->size(); i++) {
        rep.handleArray[i] = attributes->getHandle(i);
    }

    rep.type = NetworkMessage::ATTRIBUTE_OWNERSHIP_RELEASE_RESPONSE ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.object = req->object ;

    rep.write(link); // Send answer to RTIA
}

// ----------------------------------------------------------------------------
// processCancelAcquisition
void
RTIG::processCancelAcquisition(Socket *link, NetworkMessage *req)
{
    auditServer->addToLinef("Object = %u, # of att. = %u",
                            req->object,
                            req->handleArraySize);

    federations->cancelAcquisition(req->federation,
                                   req->federate,
                                   req->object,
                                   req->handleArray,
                                   req->handleArraySize);

    D.Out(pdDebug,
          "Federate %u of Federation %u release response of object %u.",
          req->federate, req->federation, req->object);

    NetworkMessage rep ;
    rep.type = NetworkMessage::CANCEL_ATTRIBUTE_OWNERSHIP_ACQUISITION ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.object = req->object ;
    rep.handleArraySize = 0 ;

    rep.write(link); // send answer to RTIA
}

// ----------------------------------------------------------------------------
// processCreateRegion
void
RTIG::processCreateRegion(Socket *link, NetworkMessage *req)
{
    // TODO: audit...

    NetworkMessage rep ;

    rep.region = federations->createRegion(req->federation,
                                           req->federate,
                                           req->space,
                                           req->nbExtents);

    D[pdDebug] << "Federate " << req->federate << " of Federation "
               << req->federation << " creates region " << rep.region
               << endl ;

    rep.type = NetworkMessage::DDM_CREATE_REGION ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.write(link);
}

// ----------------------------------------------------------------------------
// processModifyRegion
void
RTIG::processModifyRegion(Socket *link, NetworkMessage *req)
{
    federations->modifyRegion(req->federation, req->federate,
			      req->region, req->getExtents());
    
    D[pdDebug] << "Federate " << req->federate << " of Federation "
               << req->federation << " modifies region " << req->region
               << endl ;

    NetworkMessage rep ;

    rep.type = NetworkMessage::DDM_MODIFY_REGION ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.write(link);
}

// ----------------------------------------------------------------------------
// processDeleteRegion
void
RTIG::processDeleteRegion(Socket *link, NetworkMessage *req)
{
    // TODO: audit...

    federations->deleteRegion(req->federation, req->federate, req->region);

    D[pdDebug] << "Federate " << req->federate << " of Federation "
               << req->federation << " deletes region " << req->region
               << endl ;

    NetworkMessage rep ;
    rep.type = NetworkMessage::DDM_DELETE_REGION ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.region = req->region ;
    rep.write(link);
}

// ----------------------------------------------------------------------------
// processAssociateRegion
void
RTIG::processAssociateRegion(Socket *link, NetworkMessage *req)
{
    // TODO: audit...

    federations->associateRegion(req->federation, req->federate, req->object,
				 req->region, req->handleArraySize,
				 req->handleArray);

    D[pdDebug] << "Federate " << req->federate << " of Federation "
               << req->federation << " associates region " << req->region
               << " to some attributes of object " << req->object << endl ;

    NetworkMessage rep ;
    rep.type = NetworkMessage::DDM_ASSOCIATE_REGION ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.write(link);
}

// ----------------------------------------------------------------------------
// processAssociateRegion
void
RTIG::processUnassociateRegion(Socket *link, NetworkMessage *req)
{
    // TODO: audit...

    federations->unassociateRegion(req->federation, req->federate,
				   req->region);

    D[pdDebug] << "Federate " << req->federate << " of Federation "
               << req->federation << " associates region " << req->region
               << " from object " << req->object << endl ;

    NetworkMessage rep ;
    rep.type = NetworkMessage::DDM_UNASSOCIATE_REGION ;
    rep.exception = e_NO_EXCEPTION ;
    rep.federate = req->federate ;
    rep.write(link);
}

}} // namespace certi/rtig

// $Id: RTIG_processing.cc,v 3.17 2003/07/07 23:05:26 breholee Exp $
