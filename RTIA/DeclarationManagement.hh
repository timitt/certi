// -*- mode:C++ ; tab-width:4 ; c-basic-offset:4 ; indent-tabs-mode:nil -*-
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
// $Id: DeclarationManagement.hh,v 3.4 2003/02/19 15:45:22 breholee Exp $
// ----------------------------------------------------------------------------

#ifndef _CERTI_RTIA_DM
#define _CERTI_RTIA_DM

// Project
#include <config.h>
#include "FederationManagement.hh"
#include "Communications.hh"
#include "Files.hh"
#include "baseTypes.hh"
#include "RTItypes.hh"
#include "RootObject.hh"
#include "PrettyDebug.hh"

// Standard libraries
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

namespace certi {
namespace rtia {

// Prototypes de classes existantes
class Communications ;
class Queues ;
class FederationManagement ;

// Classe de gestion des declarations(publications et subscriptions).
class DeclarationManagement
{
public:
    DeclarationManagement(Communications*, FederationManagement*, RootObject*);
    ~DeclarationManagement(void);

    void publishObjectClass(ObjectClassHandle theClassHandle,
                            AttributeHandle *attribArray,
                            UShort attribArraySize,
                            TypeException &e);

    void unpublishObjectClass(ObjectClassHandle theClassHandle,
                              TypeException &e);

    void publishInteractionClass(InteractionClassHandle theInteractionHandle,
                                 TypeException &e);

    void unpublishInteractionClass(InteractionClassHandle theInteractionHandle,
                                   TypeException &e);

    void subscribeObjectClassAttribute(ObjectClassHandle theClassHandle,
                                       AttributeHandle *attribArray,
                                       UShort attribArraySize,
                                       TypeException &e);

    void subscribeObjectClassAttribute(ObjectClassHandle theClassHandle,
                                       AttributeHandle theAttribute,
                                       HLA_Region theRegion,
                                       TypeException &e);

    void unsubscribeObjectClassAttribute(ObjectClassHandle theClassHandle,
                                         TypeException &e);

    void unsubscribeObjectClassAttribute(ObjectClassHandle theClass,
                                         HLA_Region theRegion,
                                         TypeException &e);

    void subscribeInteractionClass(InteractionClassHandle theClassHandle,
                                   TypeException &e);

    void subscribeInteractionClass(InteractionClassHandle theClass,
                                   HLA_Region theRegion,
                                   TypeException &e);

    void unsubscribeInteractionClass(InteractionClassHandle theClassHandle,
                                     TypeException &e);

    void unsubscribeInteractionClass(InteractionClassHandle theClass,
                                     HLA_Region theRegion,
                                     TypeException &e);

    void startRegistrationForObjectClass(ObjectClassHandle theClass,
                                         TypeException &e);

    void stopRegistrationForObjectClass(ObjectClassHandle theClass,
                                        TypeException &e);

    void turnInteractionsOn(InteractionClassHandle theHandle,
                            TypeException &e);

    void turnInteractionsOff(InteractionClassHandle theHandle,
                             TypeException &e);

protected:
    Communications *comm ;
    Queues *queues ;
    FederationManagement *fm ;
    RootObject *rootObject ;
};

}} // namespace certi/rtia

#endif // _CERTI_RTIA_DM

// $Id: DeclarationManagement.hh,v 3.4 2003/02/19 15:45:22 breholee Exp $
