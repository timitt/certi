/*
 * Following code is IEEE-1516_2010 specific
 */
#ifndef IEEE1516_2010
#define IEEE1516_2010
#endif

#include "testFederate.hh"
#include <RTI/RTI1516fedTime.h>

#include <iostream>

bool testFederate::createFederationExecution(std::wstring federationName, std::wstring FOMFile) {
    this->federationName = federationName;
    this->FOMFile        = FOMFile;

    try {
        rtiAmb->createFederationExecution(federationName,FOMFile);
    } catch (rti1516e::Exception& e) {
        std::wcout << L"CFE caught Error " << e.what() <<std::endl;
        status = false;
    }
    return status;
}

bool testFederate::destroyFederationExecution() {
    try {
        rtiAmb->destroyFederationExecution(federationName);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"DFE caught Error " << e.what() <<std::endl;
        status = false;
    }
    return status;
}

bool testFederate::joinFederationExecution() {
    try {
        rtiAmb->joinFederationExecution(name,federationName);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"JFE caught Error " << e.what() <<std::endl;
    }

    return status;
}

bool testFederate::resignFederationExecution() {
    try {
           rtiAmb->resignFederationExecution(rti1516e::CANCEL_THEN_DELETE_THEN_DIVEST);
       }
       catch (rti1516e::Exception& e) {
           std::wcout << L"RFE caught Error " << e.what() <<std::endl;
       }
    return status;
}

bool testFederate::getHandles() {
    status = false;
    try {
        OCH_Data = rtiAmb->getObjectClassHandle(L"Data");
        AH_Attr1 = rtiAmb->getAttributeHandle(OCH_Data,L"Attr1");
        AH_Attr2 = rtiAmb->getAttributeHandle(OCH_Data,L"Attr2");
        ICH_Message = rtiAmb->getInteractionClassHandle(L"Message");
        PH_Param1 = rtiAmb->getParameterHandle(ICH_Message,L"Param1");
        PH_Param2 = rtiAmb->getParameterHandle(ICH_Message,L"Param2");
        status = true;
    } catch (rti1516e::Exception& e) {
        std::wcout << "getHandles() caught " << e.what() <<std::endl;
        status = false;
    }
    return status;
}

bool testFederate::publishObjectClassAttribute() {
    std::wcout << "publishObjectClassAttribute" << std::endl;
    auto handleData = rtiAmb->getObjectClassHandle(L"Data");
    rti1516e::AttributeHandleSet attributes;
    attributes.insert(rtiAmb->getAttributeHandle(handleData, L"Attr1"));
    attributes.insert(rtiAmb->getAttributeHandle(handleData, L"Attr2"));
    try
    {
        rtiAmb->publishObjectClassAttributes(handleData, attributes);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"POCA caught Error " << e.what() <<std::endl;
        status = false;
    }

    return status;
}

bool testFederate::unpublishObjectClassAttribute() {
    std::wcout << "publishObjectClassAttribute" << std::endl;
    auto handleData = rtiAmb->getObjectClassHandle(L"Data");
    rti1516e::AttributeHandleSet attributes;
    attributes.insert(rtiAmb->getAttributeHandle(handleData, L"Attr1"));
    attributes.insert(rtiAmb->getAttributeHandle(handleData, L"Attr2"));
    try
    {
        rtiAmb->unpublishObjectClassAttributes(handleData, attributes);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"UPOCA caught Error " << e.what() <<std::endl;
        status = false;
    }

    return status;
}
bool testFederate::publishInteractionClass() {
    std::wcout << "publishInteractionClass" << std::endl;
    auto handleData = rtiAmb->getInteractionClassHandle(L"Message");
//    rti1516e::ParameterHandleSet parameters;
//    parameters.insert(rtiAmb->getParameterHandle(handleData, L"Param1"));
//    parameters.insert(rtiAmb->getParameterHandle(handleData, L"Param2"));
    try
    {
        rtiAmb->publishInteractionClass(handleData);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"PIA caught Error " << e.what() <<std::endl;
        status = false;
    }

    return status;
}
bool testFederate::unpublishInteractionClass() {
    std::wcout << "unpublishInteractionClass" << std::endl;
    auto handleData = rtiAmb->getInteractionClassHandle(L"Message");
//    rti1516e::ParameterHandleSet parameters;
//    parameters.insert(rtiAmb->getParameterHandle(handleData, L"Param1"));
//    parameters.insert(rtiAmb->getParameterHandle(handleData, L"Param2"));
    try
    {
        rtiAmb->unpublishInteractionClass(handleData);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"UPIA caught Error " << e.what() <<std::endl;
        status = false;
    }

    return status;
}

bool testFederate::subscribeObjectClassAttribute() {
    std::wcout << "subscribeObjectClassAttribute" << std::endl;
    auto handleData = rtiAmb->getObjectClassHandle(L"Data");
    rti1516e::AttributeHandleSet attributes;
    attributes.insert(rtiAmb->getAttributeHandle(handleData, L"Attr1"));
    attributes.insert(rtiAmb->getAttributeHandle(handleData, L"Attr2"));

    try
    {
        rtiAmb->subscribeObjectClassAttributes(handleData, attributes);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"SOCA caught Error " << e.what() <<std::endl;
        status = false;
    }

    return status;
}
bool testFederate::unsubscribeObjectClassAttribute() {
    std::wcout << "unsubscribeObjectClassAttribute" << std::endl;
    auto handleData = rtiAmb->getObjectClassHandle(L"Data");
    rti1516e::AttributeHandleSet attributes;
    attributes.insert(rtiAmb->getAttributeHandle(handleData, L"Attr1"));
    attributes.insert(rtiAmb->getAttributeHandle(handleData, L"Attr2"));

    try
    {
        rtiAmb->unsubscribeObjectClassAttributes(handleData, attributes);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"USOCA caught Error " << e.what() <<std::endl;
        status = false;
    }

    return status;
}
bool testFederate::subscribreInteractionClass() {
    std::wcout << "subscribreInteractionClass" << std::endl;
    auto handleData = rtiAmb->getInteractionClassHandle(L"Message");
//    rti1516e::ParameterHandleSet parameters;
//    parameters.insert(rtiAmb->getParameterHandle(handleData, L"Param1"));
//    parameters.insert(rtiAmb->getParameterHandle(handleData, L"Param2"));
    try
    {
        rtiAmb->subscribeInteractionClass(handleData);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"SIA caught Error " << e.what() <<std::endl;
        status = false;
    }

    return status;
}
bool testFederate::unsubscribreInteractionClass() {
    std::wcout << "unsubscribreInteractionClass" << std::endl;
    auto handleData = rtiAmb->getInteractionClassHandle(L"Message");
//    rti1516e::ParameterHandleSet parameters;
//    parameters.insert(rtiAmb->getParameterHandle(handleData, L"Param1"));
//    parameters.insert(rtiAmb->getParameterHandle(handleData, L"Param2"));
    try
    {
        rtiAmb->unsubscribeInteractionClass(handleData);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"USIA caught Error " << e.what() <<std::endl;
        status = false;
    }

    return status;
}

bool testFederate::evoke() {
    std::wcout << "evoke" << std::endl;
    try
    {
        rtiAmb->evokeCallback(1.0);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"Evoke caught Error " << e.what() <<std::endl;
        status = false;
    }

    return status;
}
bool testFederate::TAR() {

    auto time_aux = RTI1516fedTime{0.0};
    auto time_interval = RTI1516fedTimeInterval{1.0};
    time_aux += time_interval;
    try {
        rtiAmb->timeAdvanceRequest(time_aux);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"TAR caught Error " << e.what() <<std::endl;
        status = false;
    }
    return true;
}
bool testFederate::TARA() {
    auto time_aux = RTI1516fedTime{0.0};
    auto time_interval = RTI1516fedTimeInterval{1.0};
    time_aux += time_interval;
    try {
        rtiAmb->timeAdvanceRequestAvailable(time_aux);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"TARA caught Error " << e.what() <<std::endl;
        status = false;
    }
    return true;
}
bool testFederate::enableTR() {
    std::wcout << "enableTR" << std::endl;
    try
    {
        auto time_interval = RTI1516fedTimeInterval{1.0};
        rtiAmb->enableTimeRegulation(time_interval);
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"enableTR caught Error " << e.what() <<std::endl;
        status = false;
    }

    return status;
}
bool testFederate::disableTR() {
    std::wcout << "disableTR" << std::endl;
    try
    {
        rtiAmb->disableTimeRegulation();
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"disableTR caught Error " << e.what() <<std::endl;
        status = false;
    }

    return status;
}
bool testFederate::enableAD() {
    std::wcout << "enableAD" << std::endl;
    try
    {
        rtiAmb->enableAsynchronousDelivery();
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"enableAD caught Error " << e.what() <<std::endl;
        status = false;
    }

    return status;
}
bool testFederate::disableAD() {
    std::wcout << "disableAD" << std::endl;
    try
    {
        rtiAmb->disableAsynchronousDelivery();
    }
    catch (rti1516e::Exception& e) {
        std::wcout << L"disableAD caught Error " << e.what() <<std::endl;
        status = false;
    }

    return status;
}

bool testFederate::specificInitialize() {
    try {
        std::vector< std::wstring > constructorArgs;

        std::unique_ptr <rti1516e::RTIambassadorFactory> rtiAmbFactory(new rti1516e::RTIambassadorFactory());
        std::unique_ptr <rti1516e::RTIambassador > rtiAmbAP = rtiAmbFactory->createRTIambassador();
        rtiAmb = rtiAmbAP.release();
    } catch (rti1516e::Exception& e) {
        std::wcout << L"RTIambassador creation caught Error " << e.what() <<std::endl;
        status = false;
    }
    /* HLA Evolved requires to 'connect' to the RTI before using RTIAmb */
    try {
        rtiAmb->connect((*this),rti1516e::HLA_EVOKED);
        connected = true;
    } catch (rti1516e::Exception& e) {
        std::wcout << L"RTIambassador connect caught Error " << e.what() <<std::endl;
        status = false;
    }
    return status;
}

