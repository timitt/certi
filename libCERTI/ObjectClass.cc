// -*- mode:C++ ; tab-width:4 ; c-basic-offset:4 ; indent-tabs-mode:nil -*-
// ----------------------------------------------------------------------------
// CERTI - HLA RunTime Infrastructure
// Copyright (C) 2002, 2003  ONERA
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// $Id: ObjectClass.cc,v 3.12 2003/03/04 09:47:04 breholee Exp $
// ----------------------------------------------------------------------------

#include "ObjectClass.hh"

namespace certi {

static pdCDebug D("OBJECTCLASS", "(ObjClass) - ");

// ----------------------------------------------------------------------------
//! To be used only by CRead, it returns the new Attribute's Handle.
AttributeHandle
ObjectClass::addAttribute(ObjectClassAttribute *theAttribute,
                          Boolean is_inherited)
{
    if (theAttribute == NULL)
        throw RTIinternalError("Tried to add NULL attribute.");

    theAttribute->setHandle(attributeSet.size() + 1);
    theAttribute->server = server ;

    // If the attribute is inherited, it keeps its security level.
    // If not, it takes the default security level of the class.
    if (is_inherited != RTI_TRUE)
        theAttribute->LevelID = LevelID ;

    attributeSet.push_front(theAttribute);

    D.Out(pdProtocol, "ObjectClass %u has a new attribute %u.",
          handle, theAttribute->getHandle());

    return theAttribute->getHandle();
}

// ----------------------------------------------------------------------------
//! Add the class' attributes to the 'Child' Class.
void
ObjectClass::addAttributesToChild(ObjectClass *the_child)
{
    // The Attribute List is read backwards to respect the same attribute order
    // for the child(Attributes are inserted at the beginning of the list).
    ObjectClassAttribute *childAttribute = NULL ;
    list<ObjectClassAttribute *>::reverse_iterator a ;
    for (a = attributeSet.rbegin(); a != attributeSet.rend(); a++) {
        assert((*a) != NULL);

        childAttribute = new ObjectClassAttribute(*a);
        assert(childAttribute != NULL);

        D.Out(pdProtocol,
              "ObjectClass %u adding new attribute %d to child class %u.",
              handle, (*a)->getHandle(), the_child->getHandle());

        the_child->addAttribute(childAttribute);

        if (childAttribute->getHandle() != (*a)->getHandle())
            throw RTIinternalError("Error while copying child's attributes.");
    }
}

// ----------------------------------------------------------------------------
/*! Take a Broadcast List and continue to send messages. You should
  take a look at ObjectClassSet::RegisterObject to understand
  what is going on...
*/
void
ObjectClass::broadcastClassMessage(ObjectClassBroadcastList *ocbList)
{
    // 1. Set ObjectHandle to local class Handle.
    ocbList->message->objectClass = handle ;

    // 2. Update message attribute list by removing child's attributes.
    if ((ocbList->message->type == m_REFLECT_ATTRIBUTE_VALUES) ||
        (ocbList->message->type == m_REQUEST_ATTRIBUTE_OWNERSHIP_ASSUMPTION)) {
        int attr = 0 ;
        while (attr < ocbList->message->handleArraySize) {
            // If the attribute is not in that class, remove it from
            // the message.
            try {
                getAttributeWithHandle(ocbList->message->handleArray[attr]);
                attr++ ;
            }
            catch (AttributeNotDefined &e) {
                ocbList->message->removeAttribute(attr);
            }
        }
    }

    // 3. Add class/attributes subscribers to the list.
    switch(ocbList->message->type) {

    case m_DISCOVER_OBJECT:
    case m_REMOVE_OBJECT: {
        // For each federate, add it to list if at least one attribute has
        // been subscribed.
        FederateHandle federate = 0 ;
        for (federate = 1 ; federate <= MaxSubscriberHandle ; federate++) {
            if (isFederateSubscriber(federate) == RTI_TRUE) {
                ocbList->addFederate(federate);
            }
        }
    }
        break ;

    case m_REFLECT_ATTRIBUTE_VALUES:
    case m_REQUEST_ATTRIBUTE_OWNERSHIP_ASSUMPTION: {
        // For each class attribute, update the list be adding federates who
        // subscribed to the attribute.
        list<ObjectClassAttribute *>::const_iterator a ;
        for (a = attributeSet.begin(); a != attributeSet.end(); a++) {
            (*a)->updateBroadcastList(ocbList);
        }
    }
        break ;

    default:
        throw RTIinternalError("BroadcastClassMsg: Unknown type.");
    }

    // 4. Send pending messages.
    ocbList->sendPendingMessage(server);
}

// ----------------------------------------------------------------------------
//! sendToFederate.
void
ObjectClass::sendToFederate(NetworkMessage *msg, FederateHandle theFederate)
{
    // Send the message 'msg' to the Federate which Handle is theFederate.
    Socket *socket = NULL ;
    try {
#ifdef HLA_USES_UDP
        socket = server->getSocketLink(theFederate, BEST_EFFORT);
#else
        socket = server->getSocketLink(theFederate);
#endif
        msg->write(socket);
    }
    catch (RTIinternalError &e) {
        D.Out(pdExcept,
              "Reference to a killed Federate while broadcasting.");
    }
    catch (NetworkError &e) {
        D.Out(pdExcept, "Network error while broadcasting, ignoring.");
    }
    // BUG: If except = 0, could use Multicast.
}

// ----------------------------------------------------------------------------
//! sendToOwners.
void
ObjectClass::sendToOwners(CDiffusion *diffusionList,
                          ObjectHandle theObjectHandle,
                          FederateHandle theFederate,
                          const char *theTag,
                          TypeNetworkMessage type)
{
    int nbAttributes = diffusionList->size ;

    FederateHandle toFederate ;
    for (int i = 0 ; i < nbAttributes ; i++) {
        toFederate = diffusionList->DiffArray[i].federate ;
        if (toFederate != 0) {
            NetworkMessage *answer = new NetworkMessage ;
            answer->type = type ;
            answer->federation = server->federation();
            answer->federate = theFederate ;
            answer->exception = e_NO_EXCEPTION ;
            answer->object = theObjectHandle ;
            strcpy(answer->label, theTag);

            int index = 0 ;
            for (int j = i ; j < nbAttributes ; j++) {
                if (diffusionList->DiffArray[j].federate == toFederate) {
                    D.Out(pdDebug, "handle : %u",
                          diffusionList->DiffArray[j].attribute);
                    diffusionList->DiffArray[j].federate = 0 ;
                    answer->handleArray[index] = diffusionList
                        ->DiffArray[j].attribute ;
                    index++ ;
                }
            }
            answer->handleArraySize = index ;
            D.Out(pdDebug, "Envoi message type %u ", type);
            sendToFederate(answer, toFederate);
        }
    }
}

// ----------------------------------------------------------------------------
/*! Throw SecurityError if the Federate is not allowed to access the
  Object Class, and print an Audit message containing reason.
*/
void
ObjectClass::checkFederateAccess(FederateHandle the_federate,
                                 const char *reason)
    throw (SecurityError)
{
    D.Out(pdInit, "Beginning of CheckFederateAccess for the federate %d",
          the_federate);

    // BUG: Should at least but a line in Audit
    if (server == NULL)
        return ;

    Boolean result = server->canFederateAccessData(the_federate, LevelID);

    // BUG: Should use Audit.
    if (result != RTI_TRUE) {
        cout << "Object Class " << handle << " : SecurityError for federate "
             << the_federate << '(' << reason << ")." << endl ;
        throw SecurityError("Federate should not access Object Class.");
    }
}

// ----------------------------------------------------------------------------
//! ObjectClass constructor (only one).
ObjectClass::ObjectClass(void)
    : handle(0), Father(0), server(NULL), Depth(0),
      Name(NULL), LevelID(PublicLevelID), MaxSubscriberHandle(0)
{
}

// ----------------------------------------------------------------------------
//! ObjectClass destructor (frees allocated memory).
ObjectClass::~ObjectClass(void)
{
    if (Name != NULL) {
        free(Name);
        Name = NULL ;
    }

    // Deleting instances
    if (!objectSet.empty())
        D.Out(pdError,
              "ObjectClass %d : Instances remaining while exiting...", handle);

    while (!objectSet.empty()) {
        delete objectSet.front();
        objectSet.pop_front();
    }

    // Deleting Class Attributes
    while (!attributeSet.empty()) {
        delete attributeSet.front();
        attributeSet.pop_front();
    }

    // Deleting Sons
    while (!sonSet.empty()) {
        sonSet.pop_front();
    }
}

// ----------------------------------------------------------------------------
/*! Delete the object instance 'theObjectHandle', and starts to
  broadcast the RemoveObject message. Return a BroadcastList of
  Federates, in order to allow our ObjectClassSet to go on with
  the message broadcasting, by giving the list to our parent
  class.
*/
ObjectClassBroadcastList *
ObjectClass::deleteInstance(FederateHandle theFederateHandle,
                            ObjectHandle theObjectHandle,
                            const char *theUserTag)
    throw (DeletePrivilegeNotHeld,
           ObjectNotKnown,
           RTIinternalError)
{
    // 1. Pre-conditions checking(may throw ObjectNotKnown)
    Object *object = getInstanceWithID(theObjectHandle);

    // Is the Federate really the Object Owner?(Checked only on RTIG)
    if ((server != NULL) && (object->Owner != theFederateHandle)) {
        D.Out(pdExcept, "Delete Object %d: Federate %d not owner.",
              theObjectHandle, theFederateHandle);
        throw DeletePrivilegeNotHeld();
    }

    // 2. Remove Instance
    list<Object *>::iterator o ;
    list<Object *>::iterator tmp ;
    for (o = objectSet.begin(); o != objectSet.end(); o++) {
        if ((*o)->getHandle() == theObjectHandle) {
            tmp = o ;
            tmp-- ;
            delete (*o);
            objectSet.erase(o); // i is dereferenced.
            o = tmp ; // loop will increment o
            // A break may be enough ?!
        }
    }

    // 3. Prepare and broadcast message.
    ObjectClassBroadcastList *ocbList = NULL ;
    if (server != NULL) {
        D.Out(pdRegister,
              "Object %u deleted in class %u, now broadcasting...",
              theObjectHandle, handle);

        NetworkMessage *answer = new NetworkMessage ;
        answer->type = m_REMOVE_OBJECT ;
        answer->federation = server->federation();
        answer->federate = theFederateHandle ;
        answer->exception = e_NO_EXCEPTION ;
        answer->objectClass = handle ; // Class Handle
        answer->object = theObjectHandle ;
        strcpy(answer->label, theUserTag);

        ocbList = new ObjectClassBroadcastList(answer, 0);
        broadcastClassMessage(ocbList);
    }
    else {
        D.Out(pdRegister,
              "Object %u deleted in class %u, no broadcast to do.",
              theObjectHandle, handle);
    }

    // Return the BroadcastList in case it had to be passed to the parent
    // class.
    return ocbList ;
}

// ----------------------------------------------------------------------------
//! Print the ObjectClasses tree to the standard output.
void ObjectClass::display(void) const
{
    cout << " ObjectClass #" << handle << " \"" << Name << "\":" << endl ;

    // Display inheritance
    cout << " Parent Class Handle: " << Father << endl ;
    cout << " Security Level: " << LevelID << endl ;
    cout << " " << sonSet.size() << " Child(s):" << endl ;

    list<ObjectClassHandle>::const_iterator s = sonSet.begin();
    for (int i = 1 ; s != sonSet.end(); s++, i++) {
        cout << " Son " << i << " handle: "<< (*s) << endl ;
    }

    // Display Attributes
    cout << " " << attributeSet.size() << " Attribute(s):" << endl ;
    list<ObjectClassAttribute *>::const_iterator a ;
    for (a = attributeSet.begin(); a != attributeSet.end(); a++) {
        (*a)->display();
    }

    // Display Instances
    cout << " " << objectSet.size() << " Instances(s):" << endl ;
    list<Object *>::const_iterator o ;
    for (o = objectSet.begin(); o != objectSet.end(); o++) {
        (*o)->display();
    }
}

// ----------------------------------------------------------------------------
//! getAttributeHandle.
AttributeHandle
ObjectClass::getAttributeHandle(const char *the_name) const
    throw (AttributeNotDefined,
           RTIinternalError)
{
    list<ObjectClassAttribute *>::const_iterator a ;
    for (a = attributeSet.begin(); a != attributeSet.end(); a++) {
        if (strcmp((*a)->getName(), the_name) == 0)
            return (*a)->getHandle();
    }

    D.Out(pdExcept, "ObjectClass %u: Attribute \"%s\" not defined.",
          handle, the_name);

    throw AttributeNotDefined();
}

// ----------------------------------------------------------------------------
//! getAttributeName.
const char *
ObjectClass::getAttributeName(AttributeHandle the_handle) const
    throw (AttributeNotDefined,
           RTIinternalError)
{
    return getAttributeWithHandle(the_handle)->getName();
}


// ----------------------------------------------------------------------------
//! getAttributeWithHandle (private module).
ObjectClassAttribute *
ObjectClass::getAttributeWithHandle(AttributeHandle the_handle) const
    throw (AttributeNotDefined)
{
    list<ObjectClassAttribute *>::const_iterator a ;
    for (a = attributeSet.begin(); a != attributeSet.end(); a++) {
        if ((*a)->getHandle() == the_handle)
            return (*a);
    }

    D.Out(pdDebug, "ObjectClass %d: Attribute %d not defined.",
          handle, the_handle);

    throw AttributeNotDefined();
}

// ----------------------------------------------------------------------------
//! getInstanceWithId (private module).
Object *
ObjectClass::getInstanceWithID(ObjectHandle the_id) const
    throw (ObjectNotKnown)
{
    list<Object *>::const_iterator o ;
    for (o = objectSet.begin(); o != objectSet.end(); o++) {
        if ((*o)->getHandle() == the_id)
            return (*o);
    }

    throw ObjectNotKnown();
}

// ----------------------------------------------------------------------------
//! Return RTI_TRUE if the Federate is publishing any attribute of this class.
Boolean
ObjectClass::isFederatePublisher(FederateHandle the_federate) const
{
    D.Out(pdRegister, "attributeSet.size() = %d.", attributeSet.size());

    list<ObjectClassAttribute *>::const_iterator a ;
    for (a = attributeSet.begin(); a != attributeSet.end(); a++) {
        if ((*a)->isPublishing(the_federate) == RTI_TRUE)
            return RTI_TRUE ;
    }

    return RTI_FALSE ;
}

// ----------------------------------------------------------------------------
//! Return RTI_TRUE if the Federate has subscribe to any attribute.
Boolean
ObjectClass::isFederateSubscriber(FederateHandle the_federate) const
{
    list<ObjectClassAttribute *>::const_iterator a ;
    for (a = attributeSet.begin(); a != attributeSet.end(); a++) {
        if ((*a)->hasSubscribed(the_federate) == RTI_TRUE)
            return RTI_TRUE ;
    }

    return RTI_FALSE ;
}

// ----------------------------------------------------------------------------
/*! Return RTI_TRUE if the object instance designated by 'theID' is
  present in that class, else return RTI_FALSE.
*/
Boolean
ObjectClass::isInstanceInClass(ObjectHandle theID)
{
    try {
        getInstanceWithID(theID);
    }
    catch (ObjectNotKnown &e) {
        return RTI_FALSE ;
    }

    return RTI_TRUE ;
}

// ----------------------------------------------------------------------------
//! killFederate.
ObjectClassBroadcastList *
ObjectClass::killFederate(FederateHandle the_federate)
    throw ()
{
    D.Out(pdRegister, "Object Class %d: Killing Federate %d.",
          handle, the_federate);

    try {
        // Does federate is publishing something ? (not important)
        if (isFederatePublisher(the_federate) == RTI_TRUE)
            publish(the_federate, NULL, 0, RTI_FALSE);

        // Does federate subscribed something ?
        // Problem, no removeObject or discover must not be sent to it.
        if (isFederateSubscriber(the_federate) == RTI_TRUE)
            subscribe(the_federate, NULL, 0, RTI_FALSE);
    }
    catch (SecurityError &e) {}

    // Does federate owns instances ?
    list<Object *>::iterator o ;
    for (o = objectSet.begin(); o != objectSet.end(); o++) {
        if ((*o)->Owner == the_federate)
            // Return non-NULL to indicate that :
            // 1- A RemoveObject message should be broadcasted through parent
            // class
            // 2- The federate may own another instance, and this function
            // must be called again.
            // BUG: String \/
            return deleteInstance(the_federate, (*o)->getHandle(), "Killed");
    }

    D.Out(pdRegister, "Object Class %d:Federate %d killed.",
          handle, the_federate);

    // Return NULL if the Federate did not own any instance.
    return NULL ;
}

// ----------------------------------------------------------------------------
//! publish.
void
ObjectClass::publish(FederateHandle theFederateHandle,
                     AttributeHandle *theAttributeList,
                     UShort theListSize,
                     bool PubOrUnpub)
    throw (AttributeNotDefined,
           RTIinternalError,
           SecurityError)
{
    D.Out(pdInit, "Beginning of Publish for the federate %d",
          theFederateHandle);

    // Do all attribute handles exist ? It may throw AttributeNotDefined.
    for (UShort index = 0 ; index < theListSize ; index++)
        getAttributeWithHandle(theAttributeList[index]);

    // Check Security Levels
    checkFederateAccess(theFederateHandle, "Publish");

    // Reset all previous publish information.
    D.Out(pdInit, "ObjectClass %d: Reset publish info of Federate %d.",
          handle, theFederateHandle);

    list<ObjectClassAttribute *>::const_iterator a ;
    for (a = attributeSet.begin(); a != attributeSet.end(); a++) {
        if ((*a)->isPublishing(theFederateHandle) == RTI_TRUE)
            (*a)->publish(theFederateHandle, RTI_FALSE);
    }

    // Publish attributes one by one.
    ObjectClassAttribute * attribute ;
    for (UShort i = 0 ; i < theListSize ; i++) {
        D.Out(pdInit, "ObjectClass %d: Federate %d publishes attribute %d.",
              handle, theFederateHandle, theAttributeList[i]);
        attribute = getAttributeWithHandle(theAttributeList[i]);
        attribute->publish(theFederateHandle, PubOrUnpub);
    }
}

// ----------------------------------------------------------------------------
/*! Register a new object instance, and start to broadcast the DiscoverObject
  Message to class subscribers. Return a Broadcast List of Federates, in
  order to allow our ObjectClassSet to go on with the message broadcasting,
  by giving the list to our parent class.
*/
ObjectClassBroadcastList *
ObjectClass::registerInstance(FederateHandle the_federate_handle,
                              ObjectHandle the_object_handle,
                              const char *the_object_name)
    throw (ObjectClassNotPublished,
           ObjectAlreadyRegistered,
           RTIinternalError)
{
    D.Out(pdInit, "ObjectClass.");

    // Pre-conditions checking
    if (isInstanceInClass(the_object_handle) == RTI_TRUE) {
        D.Out(pdExcept, "exception : ObjectAlreadyRegistered.");
        throw ObjectAlreadyRegistered();
    }

    // This condition is only to be checked on the RTIG
    if ((server != NULL) &&
        (isFederatePublisher(the_federate_handle) != RTI_TRUE)) {
        D.Out(pdExcept, "exception : ObjectClassNotPublished.");
        throw ObjectClassNotPublished();
    }

    D.Out(pdInit, "ObjectClass1.");

    // Register new Object.
    Object *object = new Object(the_federate_handle);

    object->setHandle(the_object_handle);
    if (the_object_name != NULL)
        object->setName(the_object_name);

    // Ownership management :
    // Copy instance attributes
    // Federate only owns attributes it publishes.
    ObjectAttribute * oa ;
    list<ObjectClassAttribute *>::reverse_iterator a ;
    for (a = attributeSet.rbegin(); a != attributeSet.rend(); a++) {
        if ((*a)->isPublishing(the_federate_handle))
            oa = new ObjectAttribute((*a)->getHandle(), the_federate_handle);
        else
            oa = new ObjectAttribute((*a)->getHandle(), 0);

        // privilegeToDelete is owned by federate even not published.
        if (strcmp((*a)->getName(), "privilegeToDelete") == 0)
            oa->setOwner(the_federate_handle);

        object->addAttribute(oa);
    }

    objectSet.push_front(object);

    // Prepare and Broadcast message for this class
    ObjectClassBroadcastList *ocbList = NULL ;
    if (server != NULL) {
        D.Out(pdRegister,
              "Object %u registered in class %u, now broadcasting...",
              the_object_handle, handle);

        NetworkMessage *answer = new NetworkMessage ;
        answer->type = m_DISCOVER_OBJECT ;
        answer->federation = server->federation();
        answer->federate = the_federate_handle ;
        answer->exception = e_NO_EXCEPTION ;
        answer->objectClass = handle ; // Class Handle
        answer->object = the_object_handle ;
        strcpy(answer->label, the_object_name);

        ocbList = new ObjectClassBroadcastList(answer, 0);
        broadcastClassMessage(ocbList);
    }
    else {
        D.Out(pdRegister,
              "Object %u registered in class %u, no broadcast to do.",
              the_object_handle, handle);
    }

    // Return the BroadcastList in case it had to be passed to the parent
    // class.
    return ocbList ;
}

// ----------------------------------------------------------------------------
//! sendDiscoverMessages.
Boolean
ObjectClass::sendDiscoverMessages(FederateHandle theFederate,
                                  ObjectClassHandle theOriginalClass)
{
    // 1- If this class is not the original class, and the Federate is a
    // subscriber of the class, the Recursive process must be stopped,
    // because the Federate must have received all previous DiscoverObject
    // Message for this class and its sub-classes.
    if ((handle != theOriginalClass) &&
        (isFederateSubscriber(theFederate) == RTI_TRUE))
        return RTI_FALSE ;

    // 2- If there is no instance of the class, return.(the recursive process
    // must continue).
    if (objectSet.empty())
        return RTI_TRUE ;

    // 3- Else prepare the common part of the Message.
    // Messages are sent on behalf of the original class.
    NetworkMessage *message = new NetworkMessage ;
    message->type = m_DISCOVER_OBJECT ;
    message->federation = server->federation();
    message->federate = theFederate ;
    message->exception = e_NO_EXCEPTION ;
    message->objectClass = theOriginalClass ;

    // 4- For each Object instance in the class, send a Discover message.
    Socket *socket = NULL ;
    list<Object *>::const_iterator o ;
    for (o = objectSet.begin(); o != objectSet.end(); o++) {
        D.Out(pdInit,
              "Sending DiscoverObj to Federate %d for Object %u in class %u ",
              theFederate, (*o)->getHandle(), handle, message->label);

        message->object = (*o)->getHandle();
        (*o)->getName(message->label);

        // Send Message to Federate
        try {
            socket = server->getSocketLink(theFederate);
            message->write(socket);
        }
        catch (RTIinternalError &e) {
            D.Out(pdExcept,
                  "Reference to a killed Federate while sending DO msg.");
        }
        catch (NetworkError &e) {
            D.Out(pdExcept, "Network error while sending DO msg, ignoring.");
        }
    } // for each object instance

    delete message ;

    // 5- The same method must be called on my sub-classes.
    return RTI_TRUE ;
}

// ----------------------------------------------------------------------------
//! setName.
void
ObjectClass::setName(const char *new_name)
    throw (ValueLengthExceeded, RTIinternalError)
{
    // Check Length
    if ((new_name == NULL) || (strlen(new_name) > MAX_USER_TAG_LENGTH)) {
        D.Out(pdExcept, "Object class Name %s too long.", new_name);
        throw ValueLengthExceeded("Object class name too long.");
    }

    // Free previous name
    if (Name != NULL)
        free(Name);

    // Store new name
    Name = strdup(new_name);
    if (Name == NULL)
        throw RTIinternalError("Memory Exhausted.");
}

// ----------------------------------------------------------------------------
//! A class' LevelID can only be increased.
void
ObjectClass::setLevelId(SecurityLevelID new_levelID)
{
    if (server->dominates(new_levelID, LevelID) == RTI_FALSE)
        throw SecurityError("Attempt to lower object class level.");

    LevelID = new_levelID ;
}

// ----------------------------------------------------------------------------
/*! Return RTI_TRUE if theFederate had never subscribed to this class
  before. In that case, ObjectClassSet will call SendDiscoverMessages
  on this class and on all child classes to allow them to send
  Discover Messages for already registered instances.
*/
Boolean
ObjectClass::subscribe(FederateHandle theFederate,
                       AttributeHandle *theAttributeList,
                       UShort theListSize,
                       bool SubOrUnsub)
    throw (AttributeNotDefined, RTIinternalError, SecurityError)
{
    // Check Security Levels
    checkFederateAccess(theFederate, "Subscribe");

    // Do all attribute handles exist ? It may throw AttributeNotDefined.
    for (UShort index = 0 ; index < theListSize ; index++)
        getAttributeWithHandle(theAttributeList[index]);

    // Save the Federate number.
    if (theFederate > MaxSubscriberHandle)
        MaxSubscriberHandle = theFederate ;

    // A new subscribtion invalidates all previous subscription to the same
    // object class, so we first remove all previous subscription information.
    D.Out(pdInit,
          "ObjectClass %d: Resetting previous Sub info of Federate %d.",
          handle, theFederate);

    Boolean wasPreviousSubscriber = RTI_FALSE ;
    list<ObjectClassAttribute *>::iterator a ;
    for (a = attributeSet.begin(); a != attributeSet.end(); a++) {
        if ((*a)->hasSubscribed(theFederate) == RTI_TRUE) {
            (*a)->subscribe(theFederate, RTI_FALSE);
            wasPreviousSubscriber = RTI_TRUE ;
        }
    }

    // Subscribe to attributes one by one.
    ObjectClassAttribute *attribute ;
    for (UShort index = 0 ; index < theListSize ; index++) {
        D.Out(pdInit,
              "ObjectClass %d: Federate %d subscribes to attribute %d.",
              handle, theFederate, theAttributeList[index]);
        attribute = getAttributeWithHandle(theAttributeList[index]);
        attribute->subscribe(theFederate, SubOrUnsub);
    }

    // If the Federate was not a subscriber before, and has now subscribed
    // to at least one attribute, it must discover class' current instances.

    // BUG: If the Federate unsubscribe, he should receive RemoveObject msgs?
    if ((wasPreviousSubscriber == RTI_FALSE) && // Not sub. before
        (SubOrUnsub == RTI_TRUE) && // subscribe(and not Unsub.)
        (theListSize > 0)) // at least to 1 attribute.
        return RTI_TRUE ;
    else
        return RTI_FALSE ;
}

// ----------------------------------------------------------------------------
//! update Attribute Values (with time).
ObjectClassBroadcastList *
ObjectClass::updateAttributeValues(FederateHandle theFederateHandle,
                                   ObjectHandle theObjectHandle,
                                   AttributeHandle *theAttributeArray,
                                   AttributeValue *theValueArray,
                                   UShort theArraySize,
                                   FederationTime theTime,
                                   const char *theUserTag)
    throw (ObjectNotKnown,
           AttributeNotDefined,
           AttributeNotOwned,
           RTIinternalError,
           InvalidObjectHandle)
{
    // Pre-conditions checking
    Object *object = getInstanceWithID(theObjectHandle);

    // Ownership management: Test ownership on each attribute before updating.
    ObjectAttribute * oa ;
    for (int i = 0 ; i < theArraySize ; i++) {
        oa = object->getAttribute(theAttributeArray[i]);

        if (oa->getOwner() != theFederateHandle)
            throw AttributeNotOwned();
    }

    // Federate must be Owner of all attributes(not Owner of the instance).
    // if (object->Owner != theFederateHandle)
    // throw AttributeNotOwned();

    // Prepare and Broadcast message for this class
    ObjectClassBroadcastList *ocbList = NULL ;
    if (server != NULL) {
        NetworkMessage *answer = new NetworkMessage ;
        answer->type = m_REFLECT_ATTRIBUTE_VALUES ;
        answer->federation = server->federation();
        answer->federate = theFederateHandle ;
        answer->exception = e_NO_EXCEPTION ;
        answer->object = theObjectHandle ;
        answer->date = theTime ;

        strcpy(answer->label, theUserTag);

        answer->handleArraySize = theArraySize ;

        for (int i = 0 ; i < theArraySize ; i++) {
            answer->handleArray[i] = theAttributeArray[i] ;
            answer->setValue(i, theValueArray[i]);
        }

        ocbList = new ObjectClassBroadcastList(answer, attributeSet.size());

        D.Out(pdProtocol,
              "Object %u updated in class %u, now broadcasting...",
              theObjectHandle, handle);

        broadcastClassMessage(ocbList);
    }
    else {
        D.Out(pdExcept,
              "UpdateAttributeValues should not be called on the RTIA.");
        throw RTIinternalError("UpdateAttributeValues called on the RTIA.");
    }

    // Return the BroadcastList in case it had to be passed to the parent
    // class.
    return ocbList ;
}

// ----------------------------------------------------------------------------
//! isAttributeOwnedByFederate.
Boolean
ObjectClass::isAttributeOwnedByFederate(ObjectHandle theObject,
                                        AttributeHandle theAttribute,
                                        FederateHandle theFederateHandle)
    throw (ObjectNotKnown,
           AttributeNotDefined,
           RTIinternalError)
{
    // Pre-conditions checking

    // may throw ObjectNotKnown
    Object* object = getInstanceWithID(theObject);

    // It may throw AttributeNotDefined
    getAttributeWithHandle(theAttribute);

    D.Out(pdDebug,
          "isAttributeOwnedByFederate sur l'attribut %u de l'objet %u",
          theAttribute, theObject);

    ObjectAttribute * oa ;
    if (server != NULL) {
        oa = object->getAttribute(theAttribute);

        return (oa->getOwner() == theFederateHandle) ? RTI_TRUE : RTI_FALSE ;
    }
    else {
        D.Out(pdDebug, "Only called on RTIG");
    }

    return RTI_FALSE ;
}

// ----------------------------------------------------------------------------
//! queryAttributeOwnership.
void
ObjectClass::queryAttributeOwnership(ObjectHandle theObject,
                                     AttributeHandle theAttribute,
                                     FederateHandle theFederateHandle)
    throw (ObjectNotKnown,
           AttributeNotDefined,
           RTIinternalError)
{
    // Pre-conditions checking

    // may throw ObjectNotKnown
    Object* object = getInstanceWithID(theObject);

    // It may throw AttributeNotDefined
    getAttributeWithHandle(theAttribute);

    D.Out(pdDebug, "queryAttributeOwnership sur l'attribut %u de l'objet %u",
          theAttribute, theObject);

    if (server != NULL) {
        ObjectAttribute * oa ;
        oa = object->getAttribute(theAttribute);

        NetworkMessage *answer = new NetworkMessage ;
        answer->federation = server->federation();
        answer->exception = e_NO_EXCEPTION ;
        answer->object = theObject ;
        answer->handleArray[0] = theAttribute ;
        answer->federate = oa->getOwner();

        if (answer->federate != 0)
            answer->type = m_INFORM_ATTRIBUTE_OWNERSHIP ;
        else
            answer->type = m_ATTRIBUTE_IS_NOT_OWNED ;

        sendToFederate(answer, theFederateHandle);
    }
    else {
        D.Out(pdDebug, "Only called on RTIG");
    }
}

// ----------------------------------------------------------------------------
//! negotiatedAttributeOwnershipDivestiture.
ObjectClassBroadcastList * ObjectClass::
negotiatedAttributeOwnershipDivestiture(FederateHandle theFederateHandle,
                                        ObjectHandle theObjectHandle,
                                        AttributeHandle *theAttributeList,
                                        UShort theListSize,
                                        const char *theTag)
    throw (ObjectNotKnown,
           AttributeNotDefined,
           AttributeNotOwned,
           AttributeAlreadyBeingDivested,
           RTIinternalError)
{
    // Pre-conditions checking

    // may throw ObjectNotKnown
    Object *object = getInstanceWithID(theObjectHandle);

    // Do all attribute handles exist ? It may throw AttributeNotDefined.
    for (int index = 0 ; index < theListSize ; index++)
        getAttributeWithHandle(theAttributeList[index]);

    // Does federate owns every attributes.
    // Does federate has called NegotiatedAttributeOwnershipDivestiture.
    D.Out(pdDebug, "NegotiatedDivestiture Demandeur : %u", theFederateHandle);

    ObjectAttribute * oa ;
    ObjectClassAttribute * oca ;
    for (int i = 0 ; i < theListSize ; i++) {
        oca = getAttributeWithHandle(theAttributeList[i]);
        oa = object->getAttribute(theAttributeList[i]);

        D.Out(pdDebug, "Attribute Name : %s", oca->getName());
        D.Out(pdDebug, "Attribute Handle : %u", oa->getHandle());
        D.Out(pdDebug, "Attribute Owner : %u", oa->getOwner());
        if (oa->getOwner() != theFederateHandle)
            throw AttributeNotOwned();
        if (oa->beingDivested())
            throw AttributeAlreadyBeingDivested();
    }

    int compteur_acquisition = 0 ;
    int compteur_assumption = 0 ;
    int compteur_divestiture = 0 ;
    ObjectClassBroadcastList *List = NULL ;
    FederateHandle NewOwner ;

    if (server != NULL) {
        NetworkMessage *AnswerAssumption = NULL ;
        NetworkMessage *AnswerDivestiture = NULL ;
        AnswerDivestiture = new NetworkMessage ;
        AnswerAssumption = new NetworkMessage ;

        AnswerAssumption->handleArraySize = theListSize ;

        CDiffusion *diffusionAcquisition = new CDiffusion();

        ObjectAttribute * oa ;
        ObjectClassAttribute * oca ;
        for (int i = 0 ; i < theListSize ; i++) {
            oa = object->getAttribute(theAttributeList[i]);

            if (oa->hasCandidates()) {
                // An attributeOwnershipAcquisition is on the way
                // with this attribute.

                // Le demandeur le plus r�cent devient propri�taire
                NewOwner = oa->getCandidate(1);

                oa->setOwner(NewOwner);

                // On le supprime de la liste des demandeurs
                oa->removeCandidate(NewOwner);

                // On r�initialise divesting
                oa->setDivesting(RTI_FALSE);

                diffusionAcquisition->DiffArray[compteur_acquisition]
                    .federate = NewOwner ;
                diffusionAcquisition->DiffArray[compteur_acquisition]
                    .attribute = oa->getHandle();
                compteur_acquisition++ ;

                AnswerDivestiture->handleArray[compteur_divestiture]
                    = theAttributeList[i] ;
                compteur_divestiture++ ;

                if (strcmp(oca->getName(), "privilegeToDelete") == 0)
                    object->Owner = NewOwner ;
            }
            else {
                AnswerAssumption->handleArray[compteur_assumption]
                    = theAttributeList[i] ;
                oa->setDivesting(RTI_TRUE);
                compteur_assumption++ ;
            }
        }

        if (compteur_acquisition != 0) {
            diffusionAcquisition->size = compteur_acquisition ;
            sendToOwners(diffusionAcquisition, theObjectHandle,
                         theFederateHandle, theTag,
                         m_ATTRIBUTE_OWNERSHIP_ACQUISITION_NOTIFICATION);
        }
        delete diffusionAcquisition ;

        if (compteur_divestiture !=0) {
            AnswerDivestiture->type =
                m_ATTRIBUTE_OWNERSHIP_DIVESTITURE_NOTIFICATION ;
            AnswerDivestiture->federation = server->federation();
            AnswerDivestiture->federate = theFederateHandle ;
            AnswerDivestiture->exception = e_NO_EXCEPTION ;
            AnswerDivestiture->object = theObjectHandle ;
            strcpy(AnswerDivestiture->label, "\0");
            AnswerDivestiture->handleArraySize = compteur_divestiture ;

            sendToFederate(AnswerDivestiture, theFederateHandle);
        }
        else
            delete AnswerDivestiture ;

        if (compteur_assumption !=0) {
            AnswerAssumption->type = m_REQUEST_ATTRIBUTE_OWNERSHIP_ASSUMPTION ;
            AnswerAssumption->federation = server->federation();
            AnswerAssumption->federate = theFederateHandle ;
            AnswerAssumption->exception = e_NO_EXCEPTION ;
            AnswerAssumption->object = theObjectHandle ;
            strcpy(AnswerAssumption->label, theTag);
            AnswerAssumption->handleArraySize = compteur_assumption ;

            List = new ObjectClassBroadcastList(AnswerAssumption,
                                                attributeSet.size());

            D.Out(pdProtocol,
                  "Object %u divestiture in class %u, now broadcasting...",
                  theObjectHandle, handle);
            broadcastClassMessage(List);
        }
        else
            delete AnswerAssumption ;
    }
    else {
        D.Out(pdExcept,
              "NegotiatedAttributeOwnershipDivestiture should not "
              "be called on the RTIA.");
        throw RTIinternalError("NegotiatedAttributeOwnershipDivestiture "
                               "called on the RTIA.");
    }

    // Return the BroadcastList in case it had to be passed to the parent
    // class.
    return List ;
}

// ----------------------------------------------------------------------------
//! attributeOwnershipAcquisitionIfAvailable.
void ObjectClass::
attributeOwnershipAcquisitionIfAvailable(FederateHandle theFederateHandle,
                                         ObjectHandle theObjectHandle,
                                         AttributeHandle *theAttributeList,
                                         UShort theListSize)
    throw (ObjectNotKnown,
           ObjectClassNotPublished,
           AttributeNotDefined,
           AttributeNotPublished,
           FederateOwnsAttributes,
           AttributeAlreadyBeingAcquired,
           RTIinternalError)
{
    // Pre-conditions checking

    //It may throw ObjectNotKnown.
    Object *object = getInstanceWithID(theObjectHandle);

    // Do all attribute handles exist ? It may throw AttributeNotDefined.
    for (int index = 0 ; index < theListSize ; index++)
        getAttributeWithHandle(theAttributeList[index]);

    if (server != NULL) {
        // Federate must publish the class.
        if (!isFederatePublisher(theFederateHandle)) {
            D.Out(pdExcept, "exception : ObjectClassNotPublished.");
            throw ObjectClassNotPublished();
        }

        //rem attributeSet.size()=attributeState.size()
        ObjectAttribute * oa ;
        ObjectClassAttribute * oca ;
        for (int i = 0 ; i < theListSize ; i++) {
            oca = getAttributeWithHandle(theAttributeList[i]);
            oa = object->getAttribute(theAttributeList[i]);

            // The federate has to publish attributes he desire to
            // acquire.
            if (!oca->isPublishing(theFederateHandle) &&
                (strcmp(oca->getName(), "privilegeToDelete") != 0))
                throw AttributeNotPublished();
            // Does federate already owns some attributes.
            if (oa->getOwner() == theFederateHandle)
                throw FederateOwnsAttributes();
            // Does federate is on the way to acquire something.
            if (oa->isCandidate(theFederateHandle) != 0)
                throw AttributeAlreadyBeingAcquired();
        }

        NetworkMessage *Answer_notification = new NetworkMessage ;
        Answer_notification->type =
            m_ATTRIBUTE_OWNERSHIP_ACQUISITION_NOTIFICATION ;
        Answer_notification->federation = server->federation();
        Answer_notification->federate = theFederateHandle ;
        Answer_notification->exception = e_NO_EXCEPTION ;
        Answer_notification->object = theObjectHandle ;

        NetworkMessage *Answer_unavailable = new NetworkMessage ;
        Answer_unavailable->type = m_ATTRIBUTE_OWNERSHIP_UNAVAILABLE ;
        Answer_unavailable->federation = server->federation();
        Answer_unavailable->federate = theFederateHandle ;
        Answer_unavailable->exception = e_NO_EXCEPTION ;
        Answer_unavailable->object = theObjectHandle ;

        CDiffusion *diffusionDivestiture = new CDiffusion();

        //
        //Ce service ne doit pas ajouter le f�d�r� demandeur � la liste
        //des candidats!!!
        //
        int compteur_unavailable = 0 ;
        int compteur_notification = 0 ;
        int compteur_divestiture = 0 ;
        FederateHandle oldOwner ;

        for (int i = 0 ; i < theListSize ; i++) {
            oca = getAttributeWithHandle(theAttributeList[i]);
            oa = object->getAttribute(theAttributeList[i]);

            oldOwner = oa->getOwner();
            if ((oldOwner == 0) || (oa->beingDivested())) {
                //Cet attribut est libre ou offert par son propri�taire
                //S'il est offert
                if (oa->beingDivested()) {
                    diffusionDivestiture->DiffArray[compteur_divestiture].federate
                        = oldOwner ;
                    diffusionDivestiture->DiffArray[compteur_divestiture].attribute
                        = oa->getHandle();
                    compteur_divestiture++ ;
                }
                //Qu'il soit offert ou libre
                Answer_notification->handleArray[compteur_notification]
                    = theAttributeList[i] ;
                oa->setOwner(theFederateHandle);
                oa->setDivesting(RTI_FALSE);
                compteur_notification++ ;
                //object->Owner reste le champ de r�f�rence
                //pour le privilegeToDelete
                if (strcmp(oca->getName(), "privilegeToDelete") == 0)
                    object->Owner = theFederateHandle ;
            }
            else {
                //Cet attribut n'est pas disponible!!!
                Answer_unavailable->handleArray[compteur_unavailable]
                    = theAttributeList[i] ;
                oa->addCandidate(theFederateHandle);
                compteur_unavailable++ ;
            }
        }

        if (compteur_notification != 0) {
            Answer_notification->handleArraySize = compteur_notification ;
            sendToFederate(Answer_notification, theFederateHandle);
        }
        else
            delete Answer_notification ;

        D.Out(pdDebug,
              "Debut traitement : send divestiture notification message");

        if (compteur_divestiture != 0) {
            diffusionDivestiture->size =compteur_divestiture ;
            sendToOwners(diffusionDivestiture, theObjectHandle,
                         theFederateHandle, "\0",
                         m_ATTRIBUTE_OWNERSHIP_DIVESTITURE_NOTIFICATION);
        }
        delete diffusionDivestiture ;

        if (compteur_unavailable != 0) {
            Answer_unavailable->handleArraySize = compteur_unavailable ;
            sendToFederate(Answer_unavailable, theFederateHandle);
        }
        else
            delete Answer_unavailable ;
    }
    else {
        D.Out(pdExcept, "AttributeOwnershipAcquisitionIfAvailable should not "
              "be called on the RTIA.");
        throw RTIinternalError("AttributeOwnershipAcquisitionIfAvailable "
                               "called on the RTIA.");
    }
}

// ----------------------------------------------------------------------------
//! unconditionalAttributeOwnershipDivestiture.
ObjectClassBroadcastList * ObjectClass::
unconditionalAttributeOwnershipDivestiture(FederateHandle theFederateHandle,
                                           ObjectHandle theObjectHandle,
                                           AttributeHandle *theAttributeList,
                                           UShort theListSize)
    throw (ObjectNotKnown,
           AttributeNotDefined,
           AttributeNotOwned,
           RTIinternalError)
{
    // Pre-conditions checking

    // may throw ObjectNotKnown
    Object *object = getInstanceWithID(theObjectHandle);

    // Do all attribute handles exist ? It may throw AttributeNotDefined.
    for (int index = 0 ; index < theListSize ; index++)
        getAttributeWithHandle(theAttributeList[index]);

    //Le f�d�r� est-il propri�taire de tous les attributs
    ObjectAttribute * oa ;
    for (int i = 0 ; i < theListSize ; i++) {
        oa = object->getAttribute(theAttributeList[i]);

        if (oa->getOwner() != theFederateHandle)
            throw AttributeNotOwned();
    }

    int compteur_assumption = 0 ;
    int compteur_acquisition = 0 ;
    NetworkMessage *AnswerAssumption = NULL ;
    ObjectClassBroadcastList *List = NULL ;
    FederateHandle NewOwner ;
    if (server != NULL) {
        AnswerAssumption = new NetworkMessage ;
        AnswerAssumption->handleArraySize = theListSize ;
        CDiffusion *diffusionAcquisition = new CDiffusion();

        ObjectAttribute * oa ;
        ObjectClassAttribute * oca ;
        for (int i = 0 ; i < theListSize ; i++) {
            oca = getAttributeWithHandle(theAttributeList[i]);
            oa = object->getAttribute(theAttributeList[i]);

            if (oa->hasCandidates()) {
                // An attributeOwnershipAcquisition is on the way
                // on this attribute.

                // Le demandeur le plus r�cent devient propri�taire
                NewOwner = oa->getCandidate(1);

                oa->setOwner(NewOwner);

                // We remove it from candidate list.
                oa->removeCandidate(NewOwner);

                // We reinitialize divesting.
                oa->setDivesting(RTI_FALSE);

                diffusionAcquisition->DiffArray[compteur_acquisition]
                    .federate = NewOwner ;
                diffusionAcquisition->DiffArray[compteur_acquisition]
                    .attribute = oa->getHandle();
                compteur_acquisition++ ;

                if (strcmp(oca->getName(), "privilegeToDelete") == 0)
                    object->Owner = NewOwner ;
            }
            else {
                AnswerAssumption->handleArray[compteur_assumption] =
                    theAttributeList[i] ;
                oa->setOwner(0);
                oa->setDivesting(RTI_FALSE);
                compteur_assumption++ ;
            }
        }

        if (compteur_assumption != 0) {
            AnswerAssumption->type = m_REQUEST_ATTRIBUTE_OWNERSHIP_ASSUMPTION ;
            AnswerAssumption->federation = server->federation();
            AnswerAssumption->federate = theFederateHandle ;
            AnswerAssumption->exception = e_NO_EXCEPTION ;
            AnswerAssumption->object = theObjectHandle ;
            strcpy(AnswerAssumption->label, "\0");
            AnswerAssumption->handleArraySize = compteur_assumption ;

            List = new ObjectClassBroadcastList(AnswerAssumption,
                                                attributeSet.size());

            D.Out(pdProtocol,
                  "Object %u updated in class %u, now broadcasting...",
                  theObjectHandle, handle);

            broadcastClassMessage(List);
        }
        else
            delete AnswerAssumption ;

        if (compteur_acquisition != 0) {
            diffusionAcquisition->size =compteur_acquisition ;
            sendToOwners(diffusionAcquisition, theObjectHandle,
                         theFederateHandle, "\0",
                         m_ATTRIBUTE_OWNERSHIP_ACQUISITION_NOTIFICATION);
        }
        delete diffusionAcquisition ;
    }
    else {
        D.Out(pdExcept, "UnconditionalAttributeOwnershipDivestiture should "
              "not be called on the RTIA.");
        throw RTIinternalError("UnconditionalAttributeOwnershipDivestiture "
                               "called on the RTIA.");
    }

    // Return the BroadcastList in case it had to be passed to the parent
    // class.
    return List ;
}

// ----------------------------------------------------------------------------
//! attributeOwnershipAcquisition.
void
ObjectClass::attributeOwnershipAcquisition(FederateHandle theFederateHandle,
                                           ObjectHandle theObjectHandle,
                                           AttributeHandle *theAttributeList,
                                           UShort theListSize,
                                           const char *theTag)
    throw (ObjectNotKnown,
           ObjectClassNotPublished,
           AttributeNotDefined,
           AttributeNotPublished,
           FederateOwnsAttributes,
           RTIinternalError)
{
    //Les v�rifications des conditions doivent �tre faites avant
    //toute modification!!!

    // Pre-conditions checking

    // may throw ObjectNotKnown
    Object *object = getInstanceWithID(theObjectHandle);

    ObjectAttribute * oa ;
    ObjectClassAttribute * oca ;
    for (int i = 0 ; i < theListSize ; i++) {
        // Do all attribute handles exist ? It may throw AttributeNotDefined.
        oca = getAttributeWithHandle(theAttributeList[i]);
        oa = object->getAttribute(theAttributeList[i]);

        //Le f�d�r� est-il d�j� propri�taire de certains attributs
        if (oa->getOwner() == theFederateHandle)
            throw FederateOwnsAttributes();
        //Le f�d�r� publie-t-il les attributs
        if (!oca->isPublishing(theFederateHandle) &&
            (strcmp(oca->getName(), "privilegeToDelete") != 0))
            throw AttributeNotPublished();
    }

    int compteur_notification = 0 ;
    int compteur_divestiture = 0 ;
    int compteur_release = 0 ;
    FederateHandle oldOwner ;
    if (server != NULL) {
        //Le f�d�r� doit publier la classe
        if (!isFederatePublisher(theFederateHandle)) {
            D.Out(pdExcept, "exception : ObjectClassNotPublished.");
            throw ObjectClassNotPublished();
        }

        NetworkMessage *AnswerNotification = new NetworkMessage ;
        AnswerNotification->type =
            m_ATTRIBUTE_OWNERSHIP_ACQUISITION_NOTIFICATION ;
        AnswerNotification->federation = server->federation();
        AnswerNotification->federate = theFederateHandle ;
        AnswerNotification->exception = e_NO_EXCEPTION ;
        AnswerNotification->object = theObjectHandle ;

        CDiffusion *diffusionDivestiture = new CDiffusion();

        CDiffusion *diffusionRelease = new CDiffusion();

        ObjectAttribute * oa ;
        ObjectClassAttribute * oca ;
        for (int i = 0 ; i < theListSize ; i++) {
            oca = getAttributeWithHandle(theAttributeList[i]);
            oa = object->getAttribute(theAttributeList[i]);

            oldOwner = oa->getOwner();
            if ((oldOwner == 0) || (oa->beingDivested())) {
                //Cet attribut est libre ou offert par son propri�taire s'il est offert
                if (oa->beingDivested()) {
                    diffusionDivestiture->DiffArray[compteur_divestiture].federate =
                        oldOwner ;
                    diffusionDivestiture->DiffArray[compteur_divestiture].attribute =
                        oa->getHandle();
                    compteur_divestiture++ ;
                }
                //Qu'il soit offert ou libre
                if (oa->isCandidate(theFederateHandle) != 0)
                    oa->removeCandidate(theFederateHandle);
                AnswerNotification->handleArray[compteur_notification]
                    = theAttributeList[i] ;
                oa->setOwner(theFederateHandle);
                oa->setDivesting(RTI_FALSE);
                compteur_notification++ ;

                // object->Owner reste le champ de r�f�rence pour
                // le privilegeToDelete
                if (strcmp(oca->getName(), "privilegeToDelete") == 0)
                    object->Owner = theFederateHandle ;
            }
            else {
                diffusionRelease->DiffArray[compteur_release].federate = oldOwner ;
                diffusionRelease->DiffArray[compteur_release].attribute =
                    oa->getHandle();
                compteur_release++ ;

                //On l'enl�ve de la liste des demandeurs s'il y �tait
                oa->removeCandidate(theFederateHandle);

                //pour le rajouter en 1ere position
                oa->addCandidate(theFederateHandle);
            }
        }

        if (compteur_notification != 0) {
            AnswerNotification->handleArraySize = compteur_notification ;
            sendToFederate(AnswerNotification, theFederateHandle);
        }
        else
            delete AnswerNotification ;

        if (compteur_divestiture != 0) {
            diffusionDivestiture->size =compteur_divestiture ;
            sendToOwners(diffusionDivestiture, theObjectHandle,
                         theFederateHandle, "\0",
                         m_ATTRIBUTE_OWNERSHIP_DIVESTITURE_NOTIFICATION);
        }
        delete diffusionDivestiture ;

        if (compteur_release != 0) {
            diffusionRelease->size =compteur_release ;
            sendToOwners(diffusionRelease, theObjectHandle, theFederateHandle,
                         theTag, m_REQUEST_ATTRIBUTE_OWNERSHIP_RELEASE);
        }
        delete diffusionRelease ;
    }
    else {
        D.Out(pdExcept,
              "AttributeOwnershipAcquisition should not be called "
              "on the RTIA.");
        throw RTIinternalError("AttributeOwnershipAcquisition called "
                               "on the RTIA");
    }
}

// ----------------------------------------------------------------------------
//! cancelNegotiatedAttributeOwnershipDivestiture.
void
ObjectClass::
cancelNegotiatedAttributeOwnershipDivestiture(FederateHandle theFederateHandle,
                                              ObjectHandle theObjectHandle,
                                              AttributeHandle *attributeList,
                                              UShort theListSize)
    throw (ObjectNotKnown,
           AttributeNotDefined,
           AttributeNotOwned,
           AttributeDivestitureWasNotRequested,
           RTIinternalError)
{
    // Pre-conditions checking

    // may throw ObjectNotKnown
    Object *object = getInstanceWithID(theObjectHandle);

    // Do all attribute handles exist ? It may throw AttributeNotDefined.
    for (int index = 0 ; index < theListSize ; index++)
        getAttributeWithHandle(attributeList[index]);

    ObjectAttribute * oa ;
    for (int i = 0 ; i < theListSize ; i++) {
        oa = object->getAttribute(attributeList[i]);

        // Does federate owns every attributes.
        if (oa->getOwner() != theFederateHandle)
            throw AttributeNotOwned();
        // Does federate called NegotiatedAttributeOwnershipDivestiture
        if (!oa->beingDivested())
            throw AttributeDivestitureWasNotRequested();
    }

    if (server != NULL) {
        for (int i = 0 ; i < theListSize ; i++) {
            oa = object->getAttribute(attributeList[i]);
            oa->setDivesting(RTI_FALSE);
        }
    }
    else {
        D.Out(pdExcept, "CancelNegotiatedAttributeOwnershipDivestiture should "
              "not be called on the RTIA.");
        throw RTIinternalError("CancelNegotiatedAttributeOwnershipDivestiture "
                               "called on the RTIA.");
    }
}

// ----------------------------------------------------------------------------
//! attributeOwnershipReleaseResponse.
AttributeHandleSet *
ObjectClass::
attributeOwnershipReleaseResponse(FederateHandle theFederateHandle,
                                  ObjectHandle theObjectHandle,
                                  AttributeHandle *theAttributeList,
                                  UShort theListSize)
    throw (ObjectNotKnown,
           AttributeNotDefined,
           AttributeNotOwned,
           FederateWasNotAskedToReleaseAttribute,
           RTIinternalError)
{
    // Pre-conditions checking

    // may throw ObjectNotKnown
    Object *object = getInstanceWithID(theObjectHandle);

    // Do all attribute handles exist ? It may throw AttributeNotDefined.
    for (int index = 0 ; index < theListSize ; index++)
        getAttributeWithHandle(theAttributeList[index]);

    //Le f�d�r� est-il propri�taire de tous les attributs
    //Y a-t-il des acqu�reurs pour les attributs
    ObjectAttribute * oa ;
    for (int i = 0 ; i < theListSize ; i++) {
        oa = object->getAttribute(theAttributeList[i]);

        if (oa->getOwner() != theFederateHandle)
            throw AttributeNotOwned();
        if (!oa->hasCandidates())
            throw FederateWasNotAskedToReleaseAttribute();
    }

    int compteur_acquisition = 0 ;
    FederateHandle newOwner ;
    AttributeHandleSet *theAttribute ;
    if (server != NULL) {
        CDiffusion *diffusionAcquisition = new CDiffusion();

        theAttribute = AttributeHandleSetFactory::create(theListSize);

        ObjectClassAttribute * oca ;
        for (int i = 0 ; i < theListSize ; i++) {
            oca = getAttributeWithHandle(theAttributeList[i]);
            oa = object->getAttribute(theAttributeList[i]);

            //Le demandeur le plus r�cent devient propri�taire
            newOwner = oa->getCandidate(1);

            oa->setOwner(newOwner);

            //On le supprime de la liste des demandeurs
            oa->removeCandidate(newOwner);

            //On r�initialise divesting
            oa->setDivesting(RTI_FALSE);

            diffusionAcquisition->DiffArray[compteur_acquisition].federate = newOwner ;
            diffusionAcquisition->DiffArray[compteur_acquisition].attribute =
                oa->getHandle();
            compteur_acquisition++ ;
            theAttribute->add(oa->getHandle());

            D.Out(pdDebug, "Acquisition handle %u compteur %u",
                  theAttributeList[i], compteur_acquisition);

            if (strcmp(oca->getName(), "privilegeToDelete") == 0)
                object->Owner = newOwner ;
        }

        if (compteur_acquisition != 0) {
            diffusionAcquisition->size =compteur_acquisition ;
            sendToOwners(diffusionAcquisition, theObjectHandle,
                         theFederateHandle, "\0",
                         m_ATTRIBUTE_OWNERSHIP_ACQUISITION_NOTIFICATION);
        }
        delete diffusionAcquisition ;
    }
    else {
        D.Out(pdExcept, "NegotiatedAttributeOwnershipDivestiture should not "
              "be called on the RTIA.");
        throw RTIinternalError("NegotiatedAttributeOwnershipDivestiture called"
                               " on the RTIA.");
    }

    return(theAttribute);
}

// ----------------------------------------------------------------------------
//! cancelAttributeOwnershipAcquisition.
void
ObjectClass::
cancelAttributeOwnershipAcquisition(FederateHandle federate_handle,
                                    ObjectHandle object_handle,
                                    AttributeHandle *attribute_list,
                                    UShort list_size)
    throw (ObjectNotKnown,
           AttributeNotDefined,
           AttributeAlreadyOwned,
           AttributeAcquisitionWasNotRequested,
           RTIinternalError)
{
    // Pre-conditions checking

    //It may throw ObjectNotKnown.
    Object *object = getInstanceWithID(object_handle);

    // Do all attribute handles exist ? It may throw AttributeNotDefined.
    for (int index = 0 ; index < list_size ; index++)
        getAttributeWithHandle(attribute_list[index]);

    for (int i = 0 ; i < list_size ; i++)
        D.Out(pdDebug, "CancelAcquisition Object %u Attribute %u ",
              object_handle, attribute_list[i]);

    if (server != NULL) {
        //rem attributeSet.size()=attributeState.size()
        ObjectAttribute * oa ;
        ObjectClassAttribute * oca ;

        for (int i = 0 ; i < list_size ; i++) {
            oca = getAttributeWithHandle(attribute_list[i]);
            oa = object->getAttribute(attribute_list[i]);

            D.Out(pdDebug, "Attribut %u Owner %u", attribute_list[i], oa->getOwner());
            // Does federate is already owning some attributes ?
            if (oa->getOwner() == federate_handle)
                throw AttributeAlreadyOwned();
            // Does federate is already doing an acquisition ?
            if (oa->isCandidate(federate_handle) == 0)
                throw AttributeAcquisitionWasNotRequested();
        }

        NetworkMessage *answer_confirmation = new NetworkMessage ;
        answer_confirmation->type =
            m_CONFIRM_ATTRIBUTE_OWNERSHIP_ACQUISITION_CANCELLATION ;
        answer_confirmation->federation = server->federation();
        answer_confirmation->federate = federate_handle ;
        answer_confirmation->exception = e_NO_EXCEPTION ;
        answer_confirmation->object = object_handle ;

        int compteur_confirmation = 0 ;
        for (int i = 0 ; i < list_size ; i++) {
            oa = object->getAttribute(attribute_list[i]);

            answer_confirmation->handleArray[compteur_confirmation] = attribute_list[i] ;

            // We remove federate from candidates.
            oa->removeCandidate(federate_handle);
            D.Out(pdDebug, "Adding federate %u to attribute %u object %u",
                  federate_handle, attribute_list[i], object_handle);
            compteur_confirmation++ ;
        }

        if (compteur_confirmation != 0) {
            answer_confirmation->handleArraySize = compteur_confirmation ;
            sendToFederate(answer_confirmation, federate_handle);
        }
        else
            delete answer_confirmation ;
    }
    else {
        D.Out(pdExcept, "CancelAttributeOwnershipAcquisition should not "
              "be called on the RTIA.");
        throw RTIinternalError("CancelAttributeOwnershipAcquisition called "
                               "on the RTIA.");
    }
}

// ----------------------------------------------------------------------------
void
ObjectClass::setHandle(ObjectClassHandle new_handle)
{
    handle = new_handle ;
}

// ----------------------------------------------------------------------------
ObjectClassHandle
ObjectClass::getHandle(void) const
{
    return handle ;
}

} // namespace certi

// $Id: ObjectClass.cc,v 3.12 2003/03/04 09:47:04 breholee Exp $
