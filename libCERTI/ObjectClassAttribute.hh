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
// $Id: ObjectClassAttribute.hh,v 3.12 2003/07/10 22:44:42 breholee Exp $
// ----------------------------------------------------------------------------

#ifndef CERTI_OBJECT_CLASS_ATTRIBUTE_HH
#define CERTI_OBJECT_CLASS_ATTRIBUTE_HH

#include "RTItypes.hh"
#include "SecurityLevel.hh"

#include "Subscriber.hh"
#include "Publisher.hh"
#include "ObjectClassBroadcastList.hh"


namespace certi {

/*! This class descrives an object class attribute (handle, level id, ordering,
  transportation mode and name). This class also keeps track of published and
  subscribed federates.
*/
class ObjectClassAttribute {

public:
    ObjectClassAttribute();
    ObjectClassAttribute(ObjectClassAttribute *source);
    ~ObjectClassAttribute();

    void display() const ;

    const char *getName() const { return name.c_str(); };
    void setName(char *);

    void setHandle(AttributeHandle h);
    AttributeHandle getHandle() const ;

    void setSpace(SpaceHandle);
    SpaceHandle getSpace() const ;

    // Security methods
    void checkFederateAccess(FederateHandle the_federate, const char *reason)
        throw (SecurityError);

    // Publish & subscribe methods
    bool isPublishing(FederateHandle) const ;
    bool hasSubscribed(FederateHandle) const ;
    bool hasSubscribed(FederateHandle, RegionImp *) const ;

    void publish(FederateHandle) throw (RTIinternalError, SecurityError);
    void unpublish(FederateHandle) throw (RTIinternalError, SecurityError);

    void subscribe(FederateHandle) throw (RTIinternalError, SecurityError);
    void subscribe(FederateHandle, RegionImp *)
	throw (RTIinternalError, SecurityError);
    void unsubscribe(FederateHandle) throw (RTIinternalError);
    void unsubscribe(FederateHandle, RegionImp *) throw (RTIinternalError);
    
    // Update attribute values
    void updateBroadcastList(ObjectClassBroadcastList *ocb_list);

    // Attributes
    SecurityLevelID level ;
    OrderType order ;
    TransportType transport ;
    SecurityServer *server ;

private:
    void deletePublisher(FederateHandle);
    void deleteSubscriber(FederateHandle, RegionImp *);

    AttributeHandle handle ; //!< The attribute handle.
    std::string name ; //!< The attribute name, must be locally allocated.
    SpaceHandle space ; //!< Routing space

    std::list<Subscriber *> subscribers ; //!< The subscriber's list.
    std::list<Publisher *> publishers ; //!< The publisher's list.
};

} // namespace

#endif // CERTI_OBJECT_CLASS_ATTRIBUTE_HH

// $Id: ObjectClassAttribute.hh,v 3.12 2003/07/10 22:44:42 breholee Exp $
