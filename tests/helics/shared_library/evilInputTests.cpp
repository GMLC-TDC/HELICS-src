/*
Copyright (c) 2017-2019,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See
the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/
#include "ctestFixtures.hpp"
#include "helics/shared_api_library/helicsCallbacks.h"

#include <gtest/gtest.h>

/**
 tests of evil inputs for all HELICS API function calls*/

TEST(evil_general_test, helicsErrorInitialize)
{
    //helics_error helicsErrorInitialize(void);
    auto E = helicsErrorInitialize();
    EXPECT_EQ(E.error_code, 0);
    EXPECT_TRUE(std::string(E.message).empty());
}

TEST(evil_general_test, helicsErrorClear)
{
    //void helicsErrorClear(helics_error* err);
    auto E = helicsErrorInitialize();
    E.error_code = 55;
    E.message = "this is a test";

    helicsErrorClear(&E);
    EXPECT_EQ(E.error_code, 0);
    EXPECT_TRUE(std::string(E.message).empty());
}

TEST(evil_general_test, helicsIsCoreTypeAvailable)
{
    //helics_bool helicsIsCoreTypeAvailable(const char* type);
    EXPECT_EQ(helicsIsCoreTypeAvailable(nullptr), helics_false);
}

TEST(evil_general_test, helicsGetFederateByName)
{
    //helics_federate helicsGetFederateByName(const char* fedName, helics_error* err);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res = helicsGetFederateByName(nullptr, &err);
    EXPECT_EQ(helicsFederateIsValid(res), helics_false);

    res = helicsGetFederateByName("bob", &err);
    EXPECT_EQ(helicsFederateIsValid(res), helics_false);
    helicsErrorClear(&err);
    res = helicsGetFederateByName(nullptr, &err);
    EXPECT_EQ(helicsFederateIsValid(res), helics_false);

    res = helicsGetFederateByName("bob", &err);
    EXPECT_EQ(helicsFederateIsValid(res), helics_false);

    res = helicsGetFederateByName(nullptr, nullptr);
    EXPECT_EQ(helicsFederateIsValid(res), helics_false);
}

TEST(evil_general_test, helicsGetPropertyIndex)
{
    //int helicsGetPropertyIndex(const char* val);
    auto res = helicsGetPropertyIndex(nullptr);
    EXPECT_LT(res, 0);
    res = helicsGetPropertyIndex("not_a_property");
    EXPECT_LT(res, 0);
}

TEST(evil_general_test, helicsGetOptionIndex)
{
    //int helicsGetOptionIndex(const char* val);
    auto res = helicsGetOptionIndex(nullptr);
    EXPECT_LT(res, 0);
    res = helicsGetOptionIndex("not_a_property");
    EXPECT_LT(res, 0);
}

TEST(evil_general_test, helicsCloseLibrary)
{
    //void helicsCloseLibrary(void);
    helicsCloseLibrary();
}

TEST(evil_general_test, helicsCleanupLibrary)
{
    //void helicsCleanupLibrary(void);
    helicsCleanupLibrary();
}

//section Creation Function Functions to create the different objects in the library
TEST(evil_creation_test, helicsCreateCore)
{
    //helics_core helicsCreateCore(const char* type, const char* name, const char* initString, helics_error* err);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCreateCore(nullptr, "name", "", &err);
    EXPECT_TRUE(helicsCoreIsValid(res1) == helics_false);
    helicsErrorClear(&err);
    auto res2 = helicsCreateCore("invalid", "name", "", &err);
    EXPECT_TRUE(helicsCoreIsValid(res2) == helics_false);
    auto res3 = helicsCreateCore("invalid", "name", "", nullptr);
    EXPECT_TRUE(helicsCoreIsValid(res3) == helics_false);
}

TEST(evil_creation_test, helicsCreateCoreFromArgs)
{
    //helics_core helicsCreateCoreFromArgs(const char* type, const char* name, int argc, const char* const* argv, helics_error* err);
    //helics_core helicsCreateCoreFromArgs(const char* type, const char* name, int argc, const char* const* argv, helics_error* err);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCreateCoreFromArgs("bob", "bob", 0, nullptr, &err);
    EXPECT_TRUE(helicsCoreIsValid(res1) == helics_false);
    helicsErrorClear(&err);
    auto res2 = helicsCreateCoreFromArgs("bob", "bob", 0, nullptr, &err);
    EXPECT_EQ(helicsCoreIsValid(res2), helics_false);
    auto res3 = helicsCreateCoreFromArgs("bob", "bob", 0, nullptr, nullptr);
    EXPECT_EQ(helicsCoreIsValid(res3), helics_false);
}

TEST(evil_creation_test, helicsCreateBroker)
{
    //helics_broker helicsCreateBroker(const char* type, const char* name, const char* initString, helics_error* err);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCreateBroker(nullptr, "name", "", &err);
    EXPECT_TRUE(helicsBrokerIsValid(res1) == helics_false);
    helicsErrorClear(&err);
    auto res2 = helicsCreateBroker("invalid", "name", "", &err);
    EXPECT_TRUE(helicsBrokerIsValid(res2) == helics_false);
    auto res3 = helicsCreateBroker("invalid", "name", "", nullptr);
    EXPECT_TRUE(helicsBrokerIsValid(res3) == helics_false);
}

TEST(evil_creation_test, helicsCreateBrokerFromArgs)
{
    //helics_broker helicsCreateBrokerFromArgs(const char* type, const char* name, int argc, const char* const* argv, helics_error* err);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCreateBrokerFromArgs("bob", "bob", 0, nullptr, &err);
    EXPECT_TRUE(helicsBrokerIsValid(res1) == helics_false);
    helicsErrorClear(&err);
    auto res2 = helicsCreateBrokerFromArgs("bob", "bob", 0, nullptr, &err);
    EXPECT_EQ(helicsBrokerIsValid(res2), helics_false);
    auto res3 = helicsCreateBrokerFromArgs("bob", "bob", 0, nullptr, nullptr);
    EXPECT_EQ(helicsBrokerIsValid(res3), helics_false);
}

TEST(evil_creation_test, helicsCreateValueFederate)
{
    //helics_federate helicsCreateValueFederate(const char* fedName, helics_federate_info fi, helics_error* err);
    //helics_federate helicsCreateValueFederate(const char* fedName, helics_federate_info fi, helics_error* err);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCreateValueFederate("billy", nullptr, &err);
    EXPECT_EQ(helicsFederateIsValid(res1), helics_false);
    helicsErrorClear(&err);

    auto fi = helicsCreateFederateInfo();
    helicsFederateInfoSetCoreType(fi, helics_core_type_nng, nullptr);
    auto res2 = helicsCreateValueFederate("billy", fi, &err);
    EXPECT_EQ(helicsFederateIsValid(res2), helics_false);
    //auto res2=helicsCreateValueFederate(const char* fedName, helics_federate_info fi, nullptr);
}

TEST(evil_creation_test, helicsCreateValueFederateFromConfig)
{
    //helics_federate helicsCreateValueFederateFromConfig(const char* configFile, helics_error* err);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCreateValueFederateFromConfig("unknownfile.json", &err);
    helicsErrorClear(&err);
    EXPECT_EQ(helicsFederateIsValid(res1), helics_false);
    auto res2 = helicsCreateValueFederateFromConfig("unknownfile.json", &err);
    EXPECT_EQ(helicsFederateIsValid(res2), helics_false);
    auto res3 = helicsCreateValueFederateFromConfig(nullptr, nullptr);
    EXPECT_EQ(helicsFederateIsValid(res3), helics_false);
}

TEST(evil_creation_test, helicsCreateMessageFederate)
{
    //auto res2=helicsCreateMessageFederate(const char* fedName, helics_federate_info fi, nullptr);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCreateMessageFederate("billy", nullptr, &err);
    EXPECT_EQ(helicsFederateIsValid(res1), helics_false);
    helicsErrorClear(&err);

    auto fi = helicsCreateFederateInfo();
    helicsFederateInfoSetCoreType(fi, helics_core_type_nng, nullptr);
    auto res2 = helicsCreateMessageFederate("billy", fi, &err);
    EXPECT_EQ(helicsFederateIsValid(res2), helics_false);
}

TEST(evil_creation_test, helicsCreateMessageFederateFromConfig)
{
    //helics_federate helicsCreateMessageFederateFromConfig(const char* configFile, helics_error* err);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCreateMessageFederateFromConfig("unknownfile.json", &err);
    helicsErrorClear(&err);
    EXPECT_EQ(helicsFederateIsValid(res1), helics_false);
    auto res2 = helicsCreateMessageFederateFromConfig("unknownfile.json", &err);
    EXPECT_EQ(helicsFederateIsValid(res2), helics_false);
    auto res3 = helicsCreateMessageFederateFromConfig(nullptr, nullptr);
    EXPECT_EQ(helicsFederateIsValid(res3), helics_false);
}

TEST(evil_creation_test, helicsCreateCombinationFederate)
{
    //helics_federate helicsCreateCombinationFederate(const char* fedName, helics_federate_info fi, helics_error* err);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCreateCombinationFederate("billy", nullptr, &err);
    EXPECT_EQ(helicsFederateIsValid(res1), helics_false);
    helicsErrorClear(&err);

    auto fi = helicsCreateFederateInfo();
    helicsFederateInfoSetCoreType(fi, helics_core_type_nng, nullptr);
    auto res2 = helicsCreateCombinationFederate("billy", fi, &err);
    EXPECT_EQ(helicsFederateIsValid(res2), helics_false);
}

TEST(evil_creation_test, helicsCreateCombinationFederateFromConfig)
{
    //helics_federate helicsCreateCombinationFederateFromConfig(const char* configFile, helics_error* err);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCreateCombinationFederateFromConfig("unknownfile.json", &err);
    helicsErrorClear(&err);
    EXPECT_EQ(helicsFederateIsValid(res1), helics_false);
    auto res2 = helicsCreateCombinationFederateFromConfig("unknownfile.json", &err);
    EXPECT_EQ(helicsFederateIsValid(res2), helics_false);
    auto res3 = helicsCreateCombinationFederateFromConfig(nullptr, nullptr);
    EXPECT_EQ(helicsFederateIsValid(res3), helics_false);
}

TEST(evil_creation_test, helicsCreateFederateInfo)
{
    //helics_federate_info helicsCreateFederateInfo(void);
    auto fi = helicsCreateFederateInfo();
    EXPECT_NE(fi, nullptr);
}

TEST(evil_creation_test, helicsCreateQuery)
{
    //helics_query helicsCreateQuery(const char* target, const char* query);
    auto q = helicsCreateQuery(nullptr, nullptr);
    EXPECT_NE(q, nullptr);
}

//section Core Functions
//functions applying to a \ref helics_core object
TEST(evil_core_test, helicsCoreClone)
{
    //helics_core helicsCoreClone(helics_core core, helics_error* err);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCoreClone(nullptr, &err);
    EXPECT_NE(helicsCoreIsValid(res1), helics_true);
    helicsErrorClear(&err);
    auto res2 = helicsCoreClone(evil_core, nullptr);
    EXPECT_NE(helicsCoreIsValid(res2), helics_true);
    auto res3 = helicsCoreClone(evil_core, &err);
    EXPECT_NE(helicsCoreIsValid(res3), helics_true);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_core_test, helicsCoreIsValid)
{
    //helics_bool helicsCoreIsValid(helics_core core);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    EXPECT_NE(helicsCoreIsValid(evil_core), helics_true);
    EXPECT_NE(helicsCoreIsValid(nullptr), helics_true);
}

TEST(evil_core_test, helicsCoreWaitForDisconnect)
{
    //helics_bool helicsCoreWaitForDisconnect(helics_core core, int msToWait, helics_error* err);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCoreWaitForDisconnect(nullptr, 1, &err);
    EXPECT_EQ(res1, helics_true);
    helicsErrorClear(&err);
    auto res2 = helicsCoreWaitForDisconnect(nullptr, 1, nullptr);
    EXPECT_EQ(res2, helics_true);
    auto res3 = helicsCoreWaitForDisconnect(evil_core, 1, &err);
    EXPECT_EQ(res3, helics_true);
}

TEST(evil_core_test, helicsCoreIsConnected)
{
    //helics_bool helicsCoreIsConnected(helics_core core);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto res1 = helicsCoreIsConnected(evil_core);
    EXPECT_EQ(res1, helics_false);
    auto res2 = helicsCoreIsConnected(nullptr);
    EXPECT_EQ(res2, helics_false);
}

TEST(evil_core_test, helicsCoreDataLink)
{
    //void helicsCoreDataLink(helics_core core, const char* source, const char* target, helics_error* err);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsCoreDataLink(nullptr, nullptr, nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsCoreDataLink(helics_core core, const char* source, const char* target, nullptr);
    helicsCoreDataLink(evil_core, "source", "target", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_core_test, helicsCoreAddSourceFilterToEndpoint)
{
    //void helicsCoreAddSourceFilterToEndpoint(helics_core core, const char* filter, const char* endpoint, helics_error* err);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsCoreAddSourceFilterToEndpoint(nullptr, "filter", "ept", &err);
    helicsErrorClear(&err);
    //auto res2=helicsCoreAddSourceFilterToEndpoint(helics_core core, const char* filter, const char* endpoint, nullptr);
    helicsCoreAddSourceFilterToEndpoint(evil_core, "filter", "ept", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_core_test, helicsCoreAddDestinationFilterToEndpoint)
{
    //void helicsCoreAddDestinationFilterToEndpoint(helics_core core, const char* filter, const char* endpoint, helics_error* err);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsCoreAddDestinationFilterToEndpoint(nullptr, "filter", "ept", &err);
    helicsErrorClear(&err);
    //auto res2=helicsCoreAddDestinationFilterToEndpoint(helics_core core, const char* filter, const char* endpoint, nullptr);
    helicsCoreAddDestinationFilterToEndpoint(evil_core, "filter", "ept", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_core_test, helicsCoreMakeConnections)
{
    //void helicsCoreMakeConnections(helics_core core, const char* file, helics_error* err);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsCoreMakeConnections(nullptr, "invalidfile.json", &err);
    helicsErrorClear(&err);
    //auto res2=helicsCoreMakeConnections(helics_core core, const char* file, nullptr);
    helicsCoreMakeConnections(evil_core, "invalidfile", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_core_test, helicsCoreGetIdentifier)
{
    //const char*  helicsCoreGetIdentifier(helics_core core);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto res1 = helicsCoreGetIdentifier(evil_core);
    EXPECT_STREQ(res1, "");
}

TEST(evil_core_test, helicsCoreGetAddress)
{
    //const char*  helicsCoreGetAddress(helics_core core);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto res1 = helicsCoreGetAddress(evil_core);
    EXPECT_STREQ(res1, "");
}

TEST(evil_core_test, helicsCoreSetReadyToInit)
{
    //void helicsCoreSetReadyToInit(helics_core core, helics_error* err);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsCoreSetReadyToInit(nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsCoreSetReadyToInit(helics_core core, nullptr);
    helicsCoreSetReadyToInit(evil_core, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_core_test, helicsCoreDisconnect)
{
    //void helicsCoreDisconnect(helics_core core, helics_error* err);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsCoreDisconnect(nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsCoreDisconnect(helics_core core, nullptr);
    helicsCoreDisconnect(evil_core, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_core_test, helicsCoreDestroy)
{
    //void helicsCoreDestroy(helics_core core);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    EXPECT_NO_THROW(helicsCoreDestroy(evil_core));
    EXPECT_NO_THROW(helicsCoreFree(nullptr));
}

TEST(evil_core_test, helicsCoreFree)
{
    //void helicsCoreFree(helics_core core);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    EXPECT_NO_THROW(helicsCoreFree(evil_core));
    EXPECT_NO_THROW(helicsCoreFree(nullptr));
}

TEST(evil_core_test, helicsCoreSetGlobal)
{
    //void helicsCoreSetGlobal(helics_core core, const char* valueName, const char* value, helics_error* err);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsCoreSetGlobal(nullptr, "value", "value", &err);
    helicsErrorClear(&err);
    //auto res2=helicsCoreSetGlobal(helics_core core, const char* valueName, const char* value, nullptr);
    helicsCoreSetGlobal(evil_core, "value", "value", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_core_test, helicsCoreSetLogFile)
{
    //void helicsCoreSetLogFile(helics_core core, const char* logFileName, helics_error* err);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsCoreSetLogFile(nullptr, "unknownfile.log", &err);
    helicsErrorClear(&err);
    //auto res2=helicsCoreSetLogFile(helics_core core, const char* logFileName, nullptr);
    helicsCoreSetLogFile(evil_core, "unknownfile.log", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_core_test, helicsCoreSetLoggingCallback)
{
    //void helicsCoreSetLoggingCallback(     helics_core core,     void (*logger)(int loglevel, const char* identifier, const char* message, void* userData),     void* userdata,     helics_error* err);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsCoreSetLoggingCallback(nullptr, nullptr, nullptr, &err);
    helicsErrorClear(&err);
    EXPECT_EQ(err.error_code, 0);
    //auto res2=helicsCoreSetLoggingCallback(     helics_core core,     void (*logger)(int loglevel, const char* identifier, const char* message, void* userData),     void* userdata,     nullptr);
    helicsCoreSetLoggingCallback(evil_core, nullptr, nullptr, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_core_test, helicsCoreRegisterFilter)
{
    //helics_filter helicsCoreRegisterFilter(helics_core core, helics_filter_type type, const char* name, helics_error* err);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCoreRegisterFilter(nullptr, helics_filter_type_delay, "delay", &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2 = helicsCoreRegisterFilter(nullptr, helics_filter_type_delay, "delay", nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3 = helicsCoreRegisterFilter(evil_core, helics_filter_type_delay, "delay", &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_core_test, helicsCoreRegisterCloningFilter)
{
    //helics_filter helicsCoreRegisterCloningFilter(helics_core core, const char* deliveryEndpoint, helics_error* err);
    char rdata[256];
    helics_core evil_core = reinterpret_cast<helics_core>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsCoreRegisterCloningFilter(nullptr, "delivery", &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2 = helicsCoreRegisterCloningFilter(nullptr, "delivery", nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3 = helicsCoreRegisterCloningFilter(evil_core, "delivery", &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

//section Broker Functions
//Functions applying to a \ref helics_broker object
TEST(evil_broker_test, helicsBrokerClone)
{
    //helics_broker helicsBrokerClone(helics_broker broker, helics_error* err);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsBrokerClone(nullptr, &err);
    EXPECT_NE(helicsBrokerIsValid(res1), helics_true);
    helicsErrorClear(&err);
    auto res2 = helicsBrokerClone(evil_broker, nullptr);
    EXPECT_NE(helicsBrokerIsValid(res2), helics_true);
    auto res3 = helicsBrokerClone(evil_broker, &err);
    EXPECT_NE(helicsBrokerIsValid(res3), helics_true);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_broker_test, helicsBrokerIsValid)
{
    //helics_bool helicsBrokerIsValid(helics_broker broker);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    EXPECT_NE(helicsBrokerIsValid(evil_broker), helics_true);
    EXPECT_NE(helicsBrokerIsValid(nullptr), helics_true);
}

TEST(evil_broker_test, helicsBrokerIsConnected)
{
    //helics_bool helicsBrokerIsConnected(helics_broker broker);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    auto res1 = helicsBrokerIsConnected(evil_broker);
    EXPECT_EQ(res1, helics_false);
    auto res2 = helicsBrokerIsConnected(nullptr);
    EXPECT_EQ(res2, helics_false);
}

TEST(evil_broker_test, helicsBrokerDataLink)
{
    //void helicsBrokerDataLink(helics_broker broker, const char* source, const char* target, helics_error* err);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsBrokerDataLink(nullptr, nullptr, nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsBrokerDataLink(helics_core core, const char* source, const char* target, nullptr);
    helicsBrokerDataLink(evil_broker, "source", "target", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_broker_test, helicsBrokerAddSourceFilterToEndpoint)
{
    //void helicsBrokerAddSourceFilterToEndpoint(helics_broker broker, const char* filter, const char* endpoint, helics_error* err);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsBrokerAddSourceFilterToEndpoint(nullptr, "filter", "ept", &err);
    helicsErrorClear(&err);
    //auto res2=helicsBrokerAddSourceFilterToEndpoint(helics_core core, const char* filter, const char* endpoint, nullptr);
    helicsBrokerAddSourceFilterToEndpoint(evil_broker, "filter", "ept", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_broker_test, helicsBrokerAddDestinationFilterToEndpoint)
{
    //void helicsBrokerAddDestinationFilterToEndpoint(helics_broker broker, const char* filter, const char* endpoint, helics_error* err);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsBrokerAddDestinationFilterToEndpoint(nullptr, "filter", "ept", &err);
    helicsErrorClear(&err);
    //auto res2=helicsBrokerAddDestinationFilterToEndpoint(helics_core core, const char* filter, const char* endpoint, nullptr);
    helicsBrokerAddDestinationFilterToEndpoint(evil_broker, "filter", "ept", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_broker_test, helicsBrokerMakeConnections)
{
    //void helicsBrokerMakeConnections(helics_broker broker, const char* file, helics_error* err);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsBrokerMakeConnections(nullptr, "invalidfile.json", &err);
    helicsErrorClear(&err);
    //auto res2=helicsBrokerMakeConnections(helics_core core, const char* file, nullptr);
    helicsBrokerMakeConnections(evil_broker, "invalidfile", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_broker_test, helicsBrokerWaitForDisconnect)
{
    //helics_bool helicsBrokerWaitForDisconnect(helics_broker broker, int msToWait, helics_error* err);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsBrokerWaitForDisconnect(nullptr, 1, &err);
    EXPECT_EQ(res1, helics_true);
    helicsErrorClear(&err);
    auto res2 = helicsBrokerWaitForDisconnect(nullptr, 1, nullptr);
    EXPECT_EQ(res2, helics_true);
    auto res3 = helicsBrokerWaitForDisconnect(evil_broker, 1, &err);
    EXPECT_EQ(res3, helics_true);
}

TEST(evil_broker_test, helicsBrokerGetIdentifier)
{
    //const char*  helicsBrokerGetIdentifier(helics_broker broker);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    auto res1 = helicsBrokerGetIdentifier(evil_broker);
    EXPECT_STREQ(res1, "");
}

TEST(evil_broker_test, helicsBrokerGetAddress)
{
    //const char*  helicsBrokerGetAddress(helics_broker broker);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    auto res1 = helicsBrokerGetAddress(evil_broker);
    EXPECT_STREQ(res1, "");
}

TEST(evil_broker_test, helicsBrokerDisconnect)
{
    //void helicsBrokerDisconnect(helics_broker broker, helics_error* err);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsBrokerDisconnect(nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsBrokerDisconnect(helics_core core, nullptr);
    helicsBrokerDisconnect(evil_broker, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_broker_test, helicsBrokerDestroy)
{
    //void helicsBrokerDestroy(helics_broker broker);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    EXPECT_NO_THROW(helicsBrokerDestroy(evil_broker));
    EXPECT_NO_THROW(helicsBrokerFree(nullptr));
}

TEST(evil_broker_test, helicsBrokerFree)
{
    //void helicsBrokerFree(helics_broker broker);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    EXPECT_NO_THROW(helicsBrokerFree(evil_broker));
    EXPECT_NO_THROW(helicsBrokerFree(nullptr));
}

TEST(evil_broker_test, helicsBrokerSetGlobal)
{
    //void helicsBrokerSetGlobal(helics_broker broker, const char* valueName, const char* value, helics_error* err);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsBrokerSetGlobal(nullptr, "value", "value", &err);
    helicsErrorClear(&err);
    //auto res2=helicsBrokerSetGlobal(helics_core core, const char* valueName, const char* value, nullptr);
    helicsBrokerSetGlobal(evil_broker, "value", "value", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_broker_test, helicsBrokerSetLogFile)
{
    //void helicsBrokerSetLogFile(helics_broker broker, const char* logFileName, helics_error* err);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsBrokerSetLogFile(nullptr, "unknownfile.log", &err);
    helicsErrorClear(&err);
    //auto res2=helicsBrokerSetLogFile(helics_core core, const char* logFileName, nullptr);
    helicsBrokerSetLogFile(evil_broker, "unknownfile.log", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_broker_test, helicsBrokerSetLoggingCallback)
{
    //void helicsBrokerSetLoggingCallback(     helics_broker broker,     void (*logger)(int loglevel, const char* identifier, const char* message, void* userData),     void* userdata,     helics_error* err);
    char rdata[256];
    helics_broker evil_broker = reinterpret_cast<helics_broker>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsBrokerSetLoggingCallback(nullptr, nullptr, nullptr, &err);
    helicsErrorClear(&err);
    EXPECT_EQ(err.error_code, 0);
    //auto res2=helicsBrokerSetLoggingCallback(     helics_core core,     void (*logger)(int loglevel, const char* identifier, const char* message, void* userData),     void* userdata,     nullptr);
    helicsBrokerSetLoggingCallback(evil_broker, nullptr, nullptr, &err);
    EXPECT_NE(err.error_code, 0);
}

//section Federate Info Functions
//Functions applying to a \ref helics_federate_info object
TEST(evil_fedInfo_test, helicsFederateInfoClone)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res = helicsFederateInfoClone(nullptr, &err);
    EXPECT_EQ(res, nullptr);
    helicsErrorClear(&err);
    auto res2 = helicsFederateInfoClone(evil_fi, &err);
    EXPECT_EQ(res2, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoLoadFromArgs)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoLoadFromArgs(nullptr, 0, nullptr, &err);
    helicsErrorClear(&err);
    helicsFederateInfoLoadFromArgs(evil_fi, 0, nullptr, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoFree)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    EXPECT_NO_THROW(helicsFederateInfoFree(nullptr));
    EXPECT_NO_THROW(helicsFederateInfoFree(evil_fi));
}

TEST(evil_fedInfo_test, helicsFederateInfoSetCoreName)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoSetCoreName(nullptr, "core", &err);
    helicsErrorClear(&err);
    helicsFederateInfoSetCoreName(evil_fi, "core", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoSetCoreInitString)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoSetCoreInitString(nullptr, "", &err);
    helicsErrorClear(&err);
    helicsFederateInfoSetCoreInitString(evil_fi, "", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoSetBrokerInitString)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoSetBrokerInitString(nullptr, "", &err);
    helicsErrorClear(&err);
    helicsFederateInfoSetBrokerInitString(evil_fi, "", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoSetCoreType)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoSetCoreType(nullptr, -97, &err);
    helicsErrorClear(&err);
    helicsFederateInfoSetCoreType(evil_fi, -97, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoSetCoreTypeFromString)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoSetCoreTypeFromString(nullptr, "nng", &err);
    helicsErrorClear(&err);
    helicsFederateInfoSetCoreTypeFromString(evil_fi, "nng", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoSetBroker)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoSetBroker(nullptr, nullptr, &err);
    helicsErrorClear(&err);
    helicsFederateInfoSetBroker(evil_fi, "10.0.0.1", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoSetBrokerKey)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoSetBrokerKey(nullptr, "key", &err);
    helicsErrorClear(&err);
    helicsFederateInfoSetBrokerKey(evil_fi, "key", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoSetBrokerPort)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoSetBrokerPort(nullptr, 9999, &err);
    helicsErrorClear(&err);
    helicsFederateInfoSetBrokerPort(nullptr, 9999, &err);
    EXPECT_NE(err.error_code, 0);
    helicsErrorClear(&err);
    helicsFederateInfoSetBrokerPort(evil_fi, 9999, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoSetLocalPort)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoSetLocalPort(nullptr, "9999", &err);
    helicsErrorClear(&err);
    helicsFederateInfoSetLocalPort(nullptr, "9999", &err);
    EXPECT_NE(err.error_code, 0);
    helicsErrorClear(&err);
    helicsFederateInfoSetLocalPort(evil_fi, "9999", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoSetFlagOption)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoSetFlagOption(nullptr, 9, helics_false, &err);
    helicsErrorClear(&err);
    helicsFederateInfoSetFlagOption(evil_fi, 9, helics_false, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoSetSeparator)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoSetSeparator(nullptr, '-', &err);
    helicsErrorClear(&err);
    helicsFederateInfoSetSeparator(evil_fi, '-', &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoSetTimeProperty)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoSetTimeProperty(nullptr, 99, 4.35, &err);
    helicsErrorClear(&err);
    helicsFederateInfoSetTimeProperty(evil_fi, 99, 4.35, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_fedInfo_test, helicsFederateInfoSetIntegerProperty)
{
    char rdata[256];
    helics_federate_info evil_fi = reinterpret_cast<helics_federate_info>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateInfoSetIntegerProperty(nullptr, 987, -54, &err);
    EXPECT_EQ(err.error_code, 45);
    helicsErrorClear(&err);
    helicsFederateInfoSetIntegerProperty(evil_fi, 987, -54, &err);
    EXPECT_NE(err.error_code, 0);
}

//section Federate Functions
//Functions applying to all \ref helics_federate objects
TEST(evil_federate_test, helicsFederateDestroy)
{
    //void helicsFederateDestroy(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    EXPECT_NO_THROW(helicsFederateDestroy(evil_federate));
    EXPECT_NO_THROW(helicsFederateDestroy(nullptr));
}

TEST(evil_federate_test, helicsFederateClone)
{
    //helics_federate helicsFederateClone(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateClone(nullptr, &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2 = helicsFederateClone(nullptr, nullptr);
    EXPECT_EQ(res2, nullptr);
    helicsErrorClear(&err);
    auto res3 = helicsFederateClone(evil_federate, &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateIsValid)
{
    //helics_bool helicsFederateIsValid(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    EXPECT_EQ(helicsFederateIsValid(evil_federate), helics_false);
    EXPECT_EQ(helicsFederateIsValid(nullptr), helics_false);
}

TEST(evil_federate_test, helicsFederateRegisterInterfaces)
{
    //void helicsFederateRegisterInterfaces(helics_federate fed, const char* file, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateRegisterInterfaces(nullptr, "invalidfile.txt", &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateRegisterInterfaces(helics_federate fed, const char* file, nullptr);
    helicsFederateRegisterInterfaces(evil_federate, "invalid_file.txt", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateFinalize)
{
    //void helicsFederateFinalize(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateFinalize(nullptr, &err);
    helicsErrorClear(&err);
    helicsFederateFinalize(evil_federate, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateFinalizeAsync)
{
    //void helicsFederateFinalizeAsync(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateFinalizeAsync(nullptr, &err);
    helicsErrorClear(&err);
    helicsFederateFinalizeAsync(evil_federate, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateFinalizeComplete)
{
    //void helicsFederateFinalizeComplete(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateFinalizeComplete(nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateFinalizeComplete(helics_federate fed, nullptr);
    helicsFederateFinalizeComplete(evil_federate, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateFree)
{
    //void helicsFederateFree(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    EXPECT_NO_THROW(helicsFederateFree(evil_federate));
    EXPECT_NO_THROW(helicsFederateFree(nullptr));
}

TEST(evil_federate_test, helicsFederateEnterInitializingMode)
{
    //void helicsFederateEnterInitializingMode(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateEnterInitializingMode(nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateEnterInitializingMode(helics_federate fed, nullptr);
    helicsFederateEnterInitializingMode(evil_federate, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateEnterInitializingModeAsync)
{
    //void helicsFederateEnterInitializingModeAsync(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateEnterInitializingModeAsync(nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateEnterInitializingModeAsync(helics_federate fed, nullptr);
    helicsFederateEnterInitializingModeAsync(evil_federate, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateIsAsyncOperationCompleted)
{
    //helics_bool helicsFederateIsAsyncOperationCompleted(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateIsAsyncOperationCompleted(nullptr, &err);
    EXPECT_EQ(res1, helics_false);
    helicsErrorClear(&err);
    auto res2 = helicsFederateIsAsyncOperationCompleted(nullptr, nullptr);
    EXPECT_EQ(res2, helics_false);
    auto res3 = helicsFederateIsAsyncOperationCompleted(evil_federate, &err);
    EXPECT_EQ(res3, helics_false);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateEnterInitializingModeComplete)
{
    //void helicsFederateEnterInitializingModeComplete(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateEnterInitializingModeComplete(nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateEnterInitializingModeComplete(helics_federate fed, nullptr);
    helicsFederateEnterInitializingModeComplete(evil_federate, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateEnterExecutingMode)
{
    //void helicsFederateEnterExecutingMode(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateEnterExecutingMode(nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateEnterExecutingMode(helics_federate fed, nullptr);
    helicsFederateEnterExecutingMode(evil_federate, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateEnterExecutingModeAsync)
{
    //void helicsFederateEnterExecutingModeAsync(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateEnterExecutingModeAsync(nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateEnterExecutingModeAsync(helics_federate fed, nullptr);
    helicsFederateEnterExecutingModeAsync(evil_federate, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateEnterExecutingModeComplete)
{
    //void helicsFederateEnterExecutingModeComplete(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateEnterExecutingModeComplete(nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateEnterExecutingModeComplete(helics_federate fed, nullptr);
    helicsFederateEnterExecutingModeComplete(evil_federate, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateEnterExecutingModeIterative)
{
    //helics_iteration_result helicsFederateEnterExecutingModeIterative(helics_federate fed, helics_iteration_request iterate, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateEnterExecutingModeIterative(
        nullptr, helics_iteration_request_no_iteration, &err);
    EXPECT_EQ(res1, helics_iteration_result_error);
    helicsErrorClear(&err);
    auto res2 = helicsFederateEnterExecutingModeIterative(
        nullptr, helics_iteration_request_no_iteration, nullptr);
    EXPECT_EQ(res2, helics_iteration_result_error);
    auto res3 = helicsFederateEnterExecutingModeIterative(
        evil_federate, helics_iteration_request_no_iteration, &err);
    EXPECT_EQ(res3, helics_iteration_result_error);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateEnterExecutingModeIterativeAsync)
{
    //void helicsFederateEnterExecutingModeIterativeAsync(helics_federate fed, helics_iteration_request iterate, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateEnterExecutingModeIterativeAsync(
        nullptr, helics_iteration_request_no_iteration, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateEnterExecutingModeIterativeAsync(helics_federate fed, helics_iteration_request iterate, nullptr);
    helicsFederateEnterExecutingModeIterativeAsync(
        evil_federate, helics_iteration_request_no_iteration, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateEnterExecutingModeIterativeComplete)
{
    //helics_iteration_result helicsFederateEnterExecutingModeIterativeComplete(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateEnterExecutingModeIterativeComplete(nullptr, &err);
    EXPECT_EQ(res1, helics_iteration_result_error);
    helicsErrorClear(&err);
    auto res2 = helicsFederateEnterExecutingModeIterativeComplete(nullptr, nullptr);
    EXPECT_EQ(res2, helics_iteration_result_error);
    auto res3 = helicsFederateEnterExecutingModeIterativeComplete(evil_federate, &err);
    EXPECT_EQ(res3, helics_iteration_result_error);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateGetState)
{
    //helics_federate_state helicsFederateGetState(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateGetState(nullptr, &err);
    EXPECT_EQ(res1, helics_state_error);
    helicsErrorClear(&err);
    auto res2 = helicsFederateGetState(nullptr, nullptr);
    EXPECT_EQ(res2, helics_state_error);
    auto res3 = helicsFederateGetState(evil_federate, &err);
    EXPECT_EQ(res3, helics_state_error);
}

TEST(evil_federate_test, helicsFederateGetCoreObject)
{
    //helics_core helicsFederateGetCoreObject(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateGetCoreObject(nullptr, &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2 = helicsFederateGetCoreObject(nullptr, nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3 = helicsFederateGetCoreObject(evil_federate, &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateRequestTime)
{
    //helics_time helicsFederateRequestTime(helics_federate fed, helics_time requestTime, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateRequestTime(nullptr, 1.0, &err);
    helicsErrorClear(&err);
    EXPECT_DOUBLE_EQ(res1, helics_time_invalid);
    auto res2 = helicsFederateRequestTime(nullptr, 1.0, nullptr);
    EXPECT_DOUBLE_EQ(res2, helics_time_invalid);
    auto res3 = helicsFederateRequestTime(evil_federate, 1.0, &err);
    EXPECT_DOUBLE_EQ(res3, helics_time_invalid);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateRequestTimeAdvance)
{
    //helics_time helicsFederateRequestTimeAdvance(helics_federate fed, helics_time timeDelta, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateRequestTimeAdvance(nullptr, 1.0, &err);
    EXPECT_DOUBLE_EQ(res1, helics_time_invalid);
    helicsErrorClear(&err);
    auto res2 = helicsFederateRequestTimeAdvance(nullptr, 1.0, nullptr);
    EXPECT_DOUBLE_EQ(res2, helics_time_invalid);
    auto res3 = helicsFederateRequestTimeAdvance(evil_federate, 1.0, &err);
    EXPECT_DOUBLE_EQ(res3, helics_time_invalid);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateRequestNextStep)
{
    //helics_time helicsFederateRequestNextStep(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateRequestNextStep(nullptr, &err);
    EXPECT_DOUBLE_EQ(res1, helics_time_invalid);
    helicsErrorClear(&err);
    auto res2 = helicsFederateRequestNextStep(nullptr, nullptr);
    EXPECT_DOUBLE_EQ(res2, helics_time_invalid);
    auto res3 = helicsFederateRequestNextStep(evil_federate, &err);
    EXPECT_DOUBLE_EQ(res3, helics_time_invalid);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateRequestTimeIterative)
{
    //helics_time helicsFederateRequestTimeIterative(     helics_federate fed,     helics_time requestTime,     helics_iteration_request iterate,     helics_iteration_result* outIterate,     helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helics_iteration_result iteration{helics_iteration_result_iterating};
    auto res1 = helicsFederateRequestTimeIterative(
        nullptr, 1.0, helics_iteration_request_force_iteration, &iteration, &err);
    EXPECT_DOUBLE_EQ(res1, helics_time_invalid);
    EXPECT_EQ(iteration, helics_iteration_result_error);
    helicsErrorClear(&err);
    iteration = helics_iteration_result_iterating;
    auto res2 = helicsFederateRequestTimeIterative(
        nullptr, 1.0, helics_iteration_request_force_iteration, &iteration, nullptr);
    EXPECT_DOUBLE_EQ(res2, helics_time_invalid);
    EXPECT_EQ(iteration, helics_iteration_result_error);
    auto res3 = helicsFederateRequestTimeIterative(
        evil_federate, 1.0, helics_iteration_request_force_iteration, nullptr, &err);
    EXPECT_DOUBLE_EQ(res3, helics_time_invalid);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateRequestTimeAsync)
{
    //void helicsFederateRequestTimeAsync(helics_federate fed, helics_time requestTime, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateRequestTimeAsync(nullptr, 1.0, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateRequestTimeAsync(helics_federate fed, helics_time requestTime, nullptr);
    helicsFederateRequestTimeAsync(evil_federate, 1.0, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateRequestTimeComplete)
{
    //helics_time helicsFederateRequestTimeComplete(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateRequestTimeComplete(nullptr, &err);
    EXPECT_DOUBLE_EQ(res1, helics_time_invalid);
    helicsErrorClear(&err);
    auto res2 = helicsFederateRequestTimeComplete(nullptr, nullptr);
    EXPECT_DOUBLE_EQ(res2, helics_time_invalid);
    auto res3 = helicsFederateRequestTimeComplete(evil_federate, &err);
    EXPECT_DOUBLE_EQ(res3, helics_time_invalid);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateRequestTimeIterativeAsync)
{
    //void helicsFederateRequestTimeIterativeAsync(     helics_federate fed,     helics_time requestTime,     helics_iteration_request iterate,     helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateRequestTimeIterativeAsync(
        nullptr, 1.0, helics_iteration_request_force_iteration, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateRequestTimeIterativeAsync(     helics_federate fed,     helics_time requestTime,     helics_iteration_request iterate,     nullptr);
    helicsFederateRequestTimeIterativeAsync(
        evil_federate, 1.0, helics_iteration_request_force_iteration, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateRequestTimeIterativeComplete)
{
    //helics_time helicsFederateRequestTimeIterativeComplete(helics_federate fed, helics_iteration_result* outIterate, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helics_iteration_result iteration{helics_iteration_result_iterating};
    auto res1 = helicsFederateRequestTimeIterativeComplete(nullptr, &iteration, &err);
    EXPECT_EQ(iteration, helics_iteration_result_error);
    EXPECT_DOUBLE_EQ(res1, helics_time_invalid);
    iteration = helics_iteration_result_iterating;
    helicsErrorClear(&err);
    auto res2 = helicsFederateRequestTimeIterativeComplete(nullptr, nullptr, nullptr);
    EXPECT_DOUBLE_EQ(res2, helics_time_invalid);
    auto res3 = helicsFederateRequestTimeIterativeComplete(evil_federate, &iteration, &err);
    EXPECT_DOUBLE_EQ(res3, helics_time_invalid);
    EXPECT_NE(err.error_code, 0);
    EXPECT_EQ(iteration, helics_iteration_result_error);
}

TEST(evil_federate_test, helicsFederateGetName)
{
    //const char*  helicsFederateGetName(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto res1 = helicsFederateGetName(evil_federate);
    EXPECT_STREQ(res1, "");
    auto res2 = helicsFederateGetName(nullptr);
    EXPECT_STREQ(res2, "");
}

TEST(evil_federate_test, helicsFederateSetTimeProperty)
{
    //void helicsFederateSetTimeProperty(helics_federate fed, int timeProperty, helics_time time, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateSetTimeProperty(nullptr, 1, 4.0, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateSetTimeProperty(helics_federate fed, int timeProperty, helics_time time, nullptr);
    helicsFederateSetTimeProperty(evil_federate, 4, 1.0, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateSetFlagOption)
{
    //void helicsFederateSetFlagOption(helics_federate fed, int flag, helics_bool flagValue, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateSetFlagOption(nullptr, 99, helics_false, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateSetFlagOption(helics_federate fed, int flag, helics_bool flagValue, nullptr);
    helicsFederateSetFlagOption(evil_federate, 99, helics_true, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateSetSeparator)
{
    //void helicsFederateSetSeparator(helics_federate fed, char separator, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateSetSeparator(nullptr, '-', &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateSetSeparator(helics_federate fed, char separator, nullptr);
    helicsFederateSetSeparator(evil_federate, '-', &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateSetIntegerProperty)
{
    //void helicsFederateSetIntegerProperty(helics_federate fed, int intProperty, int propertyVal, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateSetIntegerProperty(nullptr, 0, 99, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateSetIntegerProperty(helics_federate fed, int intProperty, int propertyVal, nullptr);
    helicsFederateSetIntegerProperty(evil_federate, 99, 99, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateGetTimeProperty)
{
    //helics_time helicsFederateGetTimeProperty(helics_federate fed, int timeProperty, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateGetTimeProperty(nullptr, 99, &err);
    EXPECT_DOUBLE_EQ(res1, helics_time_invalid);
    helicsErrorClear(&err);
    auto res2 = helicsFederateGetTimeProperty(nullptr, 88, nullptr);
    EXPECT_DOUBLE_EQ(res2, helics_time_invalid);
    auto res3 = helicsFederateGetTimeProperty(evil_federate, 77, &err);
    EXPECT_DOUBLE_EQ(res3, helics_time_invalid);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateGetFlagOption)
{
    //helics_bool helicsFederateGetFlagOption(helics_federate fed, int flag, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateGetFlagOption(nullptr, 1, &err);
    EXPECT_EQ(res1, helics_false);
    helicsErrorClear(&err);
    auto res2 = helicsFederateGetFlagOption(nullptr, 1, nullptr);
    EXPECT_EQ(res2, helics_false);
    auto res3 = helicsFederateGetFlagOption(evil_federate, 1, &err);
    EXPECT_EQ(res3, helics_false);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateGetIntegerProperty)
{
    //int helicsFederateGetIntegerProperty(helics_federate fed, int intProperty, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateGetIntegerProperty(nullptr, 99, &err);
    EXPECT_LT(res1, 0);
    helicsErrorClear(&err);
    auto res2 = helicsFederateGetIntegerProperty(nullptr, 20, nullptr);
    EXPECT_LT(res2, 0);
    auto res3 = helicsFederateGetIntegerProperty(evil_federate, 20, &err);
    EXPECT_LT(res3, 0);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateGetCurrentTime)
{
    //helics_time helicsFederateGetCurrentTime(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateGetCurrentTime(nullptr, &err);
    EXPECT_DOUBLE_EQ(res1, helics_time_invalid);
    helicsErrorClear(&err);
    auto res2 = helicsFederateGetCurrentTime(nullptr, nullptr);
    EXPECT_DOUBLE_EQ(res2, helics_time_invalid);
    auto res3 = helicsFederateGetCurrentTime(evil_federate, &err);
    EXPECT_DOUBLE_EQ(res3, helics_time_invalid);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateSetGlobal)
{
    //void helicsFederateSetGlobal(helics_federate fed, const char* valueName, const char* value, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateSetGlobal(nullptr, nullptr, nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateSetGlobal(helics_federate fed, const char* valueName, const char* value, nullptr);
    helicsFederateSetGlobal(evil_federate, "global", "glob", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateSetLogFile)
{
    //void helicsFederateSetLogFile(helics_federate fed, const char* logFile, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateSetLogFile(nullptr, "unknownfile.txt", &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateSetLogFile(helics_federate fed, const char* logFile, nullptr);
    helicsFederateSetLogFile(evil_federate, "unknownfile.txt", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateLogErrorMessage)
{
    //void helicsFederateLogErrorMessage(helics_federate fed, const char* logmessage, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateLogErrorMessage(nullptr, "null log", &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateLogErrorMessage(helics_federate fed, const char* logmessage, nullptr);
    helicsFederateLogErrorMessage(evil_federate, "null log", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateLogWarningMessage)
{
    //void helicsFederateLogWarningMessage(helics_federate fed, const char* logmessage, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateLogWarningMessage(nullptr, "null log", &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateLogWarningMessage(helics_federate fed, const char* logmessage, nullptr);
    helicsFederateLogWarningMessage(evil_federate, "null log", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateLogInfoMessage)
{
    //void helicsFederateLogInfoMessage(helics_federate fed, const char* logmessage, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateLogInfoMessage(nullptr, "null log", &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateLogInfoMessage(helics_federate fed, "null log", nullptr);
    helicsFederateLogInfoMessage(evil_federate, "null log", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateLogDebugMessage)
{
    //void helicsFederateLogDebugMessage(helics_federate fed, const char* logmessage, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFederateLogDebugMessage(nullptr, "null log", &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateLogDebugMessage(helics_federate fed, const char* logmessage, nullptr);
    helicsFederateLogDebugMessage(evil_federate, "null log", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateLogLevelMessage)
{
    //void helicsFederateLogLevelMessage(helics_federate fed, int loglevel, const char* logmessage, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateLogLevelMessage(nullptr, 0, "null log", &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateLogLevelMessage(helics_federate fed, int loglevel, const char* logmessage, nullptr);
    helicsFederateLogLevelMessage(evil_federate, 0, "null log", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_federate_test, helicsFederateSetLoggingCallback)
{
    //void helicsFederateSetLoggingCallback(     helics_federate fed,     void (*logger)(int loglevel, const char* identifier, const char* message, void* userData),     void* userdata,     helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateSetLoggingCallback(nullptr, nullptr, nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateSetLoggingCallback(     helics_federate fed,     void (*logger)(int loglevel, const char* identifier, const char* message, void* userData),     void* userdata,     nullptr);
    helicsFederateSetLoggingCallback(evil_federate, nullptr, nullptr, &err);
    EXPECT_NE(err.error_code, 0);
}

//section Value Federate Functions
//functions applying to federates created as a value or combination federate \ref helics_federate objects
TEST(evil_value_federate_test, helicsFederateRegisterSubscription)
{
    //helics_input helicsFederateRegisterSubscription(helics_federate fed, const char* key, const char* units, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 = helicsFederateRegisterSubscription(nullptr, "key", nullptr, &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2 = helicsFederateRegisterSubscription(nullptr, nullptr, nullptr, nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3 = helicsFederateRegisterSubscription(evil_federate, "key", "pu", &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederateRegisterPublication)
{
    //helics_publication helicsFederateRegisterPublication(helics_federate fed, const char* key, helics_data_type type, const char* units, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1 =
        helicsFederateRegisterPublication(nullptr, "key", helics_data_type_complex, nullptr, &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2 = helicsFederateRegisterPublication(
        nullptr, nullptr, helics_data_type_complex, nullptr, nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3 = helicsFederateRegisterPublication(
        evil_federate, "key", helics_data_type_complex, nullptr, &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederateRegisterTypePublication)
{
    //helics_publication helicsFederateRegisterTypePublication(helics_federate fed, const char* key, const char* type, const char* units, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsFederateRegisterTypePublication(nullptr, "key", "type", "", &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2=helicsFederateRegisterTypePublication(nullptr, nullptr, nullptr, nullptr, nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3=helicsFederateRegisterTypePublication(evil_federate, "key", "type", "", &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederateRegisterGlobalPublication)
{
    //helics_publication helicsFederateRegisterGlobalPublication(     helics_federate fed,     const char* key,     helics_data_type type,     const char* units,     helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsFederateRegisterGlobalPublication(nullptr, "key", helics_data_type_any, "",     &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2=helicsFederateRegisterGlobalPublication(nullptr, nullptr, helics_data_type_any, nullptr,     nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3=helicsFederateRegisterGlobalPublication(     evil_federate, "key", helics_data_type_any, "",     &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederateRegisterGlobalTypePublication)
{
    //helics_publication helicsFederateRegisterGlobalTypePublication(     helics_federate fed,     const char* key,     const char* type,     const char* units,     helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsFederateRegisterGlobalTypePublication(nullptr, "key", "type", "",     &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2=helicsFederateRegisterGlobalTypePublication(nullptr,  nullptr, nullptr, nullptr,     nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3=helicsFederateRegisterGlobalTypePublication(     evil_federate, "key", "type", "",     &err);
    EXPECT_EQ(res3, nullptr);
}

TEST(evil_value_federate_test, helicsFederateRegisterInput)
{
    //helics_input helicsFederateRegisterInput(helics_federate fed, const char* key, helics_data_type type, const char* units, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsFederateRegisterInput(nullptr, "key", helics_data_type_any, "", &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2=helicsFederateRegisterInput(nullptr,nullptr, helics_data_type_any, nullptr, nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3=helicsFederateRegisterInput(evil_federate, "key", helics_data_type_any, "", &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederateRegisterTypeInput)
{
    //helics_input helicsFederateRegisterTypeInput(helics_federate fed, const char* key, const char* type, const char* units, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsFederateRegisterTypeInput(nullptr, "key", "type", "", &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2=helicsFederateRegisterTypeInput(nullptr, nullptr, nullptr, nullptr, nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3=helicsFederateRegisterTypeInput(evil_federate, "key", "type", "", &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederateRegisterGlobalInput)
{
    //helics_publication helicsFederateRegisterGlobalInput(helics_federate fed, const char* key, helics_data_type type, const char* units, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsFederateRegisterGlobalInput(nullptr, "key", helics_data_type_any, "", &err);
    EXPECT_EQ(res1, nullptr); helicsErrorClear(&err);
    auto res2=helicsFederateRegisterGlobalInput(nullptr,nullptr, helics_data_type_any, nullptr, nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3=helicsFederateRegisterGlobalInput(evil_federate, "key", helics_data_type_any, "", &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederateRegisterGlobalTypeInput)
{
    //helics_publication helicsFederateRegisterGlobalTypeInput(helics_federate fed, const char* key, const char* type, const char* units, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
   auto res1=helicsFederateRegisterGlobalTypeInput(nullptr, "key", "type", "", &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2=helicsFederateRegisterGlobalTypeInput(nullptr,nullptr, nullptr, nullptr, nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3=helicsFederateRegisterGlobalTypeInput(evil_federate, "key", "type", "", &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederateGetPublication)
{
    //helics_publication helicsFederateGetPublication(helics_federate fed, const char* key, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsFederateGetPublication(nullptr, "key", &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2=helicsFederateGetPublication(nullptr, nullptr, nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3=helicsFederateGetPublication(evil_federate, "key", &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederateGetPublicationByIndex)
{
    //helics_publication helicsFederateGetPublicationByIndex(helics_federate fed, int index, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsFederateGetPublicationByIndex(nullptr, 0, &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2=helicsFederateGetPublicationByIndex(nullptr, 0, nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3=helicsFederateGetPublicationByIndex(evil_federate,0, &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederateGetInput)
{
    //helics_input helicsFederateGetInput(helics_federate fed, const char* key, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsFederateGetInput(nullptr, "key", &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2=helicsFederateGetInput(nullptr, nullptr, nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3=helicsFederateGetInput(evil_federate, "key", &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederateGetInputByIndex)
{
    //helics_input helicsFederateGetInputByIndex(helics_federate fed, int index, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsFederateGetInputByIndex(nullptr, 0, &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2=helicsFederateGetInputByIndex(nullptr, 0, nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3=helicsFederateGetInputByIndex(evil_federate, 0, &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederateGetSubscription)
{
    //helics_input helicsFederateGetSubscription(helics_federate fed, const char* key, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsFederateGetSubscription(nullptr, "key", &err);
    EXPECT_EQ(res1, nullptr);
    helicsErrorClear(&err);
    auto res2=helicsFederateGetSubscription(nullptr, nullptr, nullptr);
    EXPECT_EQ(res2, nullptr);
    auto res3=helicsFederateGetSubscription(evil_federate, "key", &err);
    EXPECT_EQ(res3, nullptr);
    EXPECT_NE(err.error_code, 0);  
}

TEST(evil_value_federate_test, helicsFederateClearUpdates)
{
    //void helicsFederateClearUpdates(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    EXPECT_NO_THROW(helicsFederateClearUpdates(evil_federate));
}

TEST(evil_value_federate_test, helicsFederateRegisterFromPublicationJSON)
{
    //void helicsFederateRegisterFromPublicationJSON(helics_federate fed, const char* json, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederateRegisterFromPublicationJSON(nullptr, "json.json", &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateRegisterFromPublicationJSON(helics_federate fed, const char* json, nullptr);
    helicsFederateRegisterFromPublicationJSON(evil_federate, "json.json", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederatePublishJSON)
{
    //void helicsFederatePublishJSON(helics_federate fed, const char* json, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsFederatePublishJSON(nullptr, "json.json", &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederatePublishJSON(helics_federate fed, const char* json, nullptr);
    helicsFederatePublishJSON(evil_federate, "json.json", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_value_federate_test, helicsFederateGetPublicationCount)
{
    //int helicsFederateGetPublicationCount(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto res1 = helicsFederateGetPublicationCount(nullptr);
    EXPECT_EQ(res1, 0);
    auto res2=helicsFederateGetPublicationCount(evil_federate);
    EXPECT_EQ(res2, 0);
}

TEST(evil_value_federate_test, helicsFederateGetInputCount)
{
    //int helicsFederateGetInputCount(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto res1=helicsFederateGetInputCount(nullptr);
    EXPECT_EQ(res1, 0);
    auto res2 = helicsFederateGetInputCount(evil_federate);
    EXPECT_EQ(res2, 0);
}

//section Publication interface Functions
//functions applying to a \ref helics_publication object
TEST(evil_pub_test, helicsPublicationPublishRaw)
{
    //void helicsPublicationPublishRaw(helics_publication pub, const void* data, int inputDataLength, helics_error* err);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsPublicationPublishRaw(nullptr, nullptr, 85, &err);
    helicsErrorClear(&err);
    //auto res2=helicsPublicationPublishRaw(helics_publication pub, const void* data, int inputDataLength, nullptr);
    helicsPublicationPublishRaw(evil_pub, nullptr, 14654181, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_pub_test, helicsPublicationPublishString)
{
    //void helicsPublicationPublishString(helics_publication pub, const char* str, helics_error* err);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsPublicationPublishString(nullptr, nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsPublicationPublishString(helics_publication pub, const char* str, nullptr);
    helicsPublicationPublishString(evil_pub, "String", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_pub_test, helicsPublicationPublishInteger)
{
    //void helicsPublicationPublishInteger(helics_publication pub, int64_t val, helics_error* err);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsPublicationPublishInteger(nullptr, 1, &err);
    helicsErrorClear(&err);
    //auto res2=helicsPublicationPublishInteger(helics_publication pub, int64_t val, nullptr);
    helicsPublicationPublishInteger(evil_pub, 1, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_pub_test, helicsPublicationPublishBoolean)
{
    //void helicsPublicationPublishBoolean(helics_publication pub, helics_bool val, helics_error* err);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsPublicationPublishBoolean(nullptr, 0, &err);
    helicsErrorClear(&err);
    //auto res2=helicsPublicationPublishBoolean(helics_publication pub, helics_bool val, nullptr);
    helicsPublicationPublishBoolean(evil_pub, 99, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_pub_test, helicsPublicationPublishDouble)
{
    //void helicsPublicationPublishDouble(helics_publication pub, double val, helics_error* err);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsPublicationPublishDouble(nullptr, 1.7, &err);
    helicsErrorClear(&err);
    //auto res2=helicsPublicationPublishDouble(helics_publication pub, double val, nullptr);
    helicsPublicationPublishDouble(evil_pub, 2.0, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_pub_test, helicsPublicationPublishTime)
{
    //void helicsPublicationPublishTime(helics_publication pub, helics_time val, helics_error* err);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsPublicationPublishTime(nullptr, 4.3, &err);
    helicsErrorClear(&err);
    //auto res2=helicsPublicationPublishTime(helics_publication pub, helics_time val, nullptr);
    helicsPublicationPublishTime(evil_pub, 5.0, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_pub_test, helicsPublicationPublishChar)
{
    //void helicsPublicationPublishChar(helics_publication pub, char val, helics_error* err);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsPublicationPublishChar(nullptr, '\0' , &err);
    helicsErrorClear(&err);
    //auto res2=helicsPublicationPublishChar(helics_publication pub, char val, nullptr);
    helicsPublicationPublishChar(evil_pub, 'c', &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_pub_test, helicsPublicationPublishComplex)
{
    //void helicsPublicationPublishComplex(helics_publication pub, double real, double imag, helics_error* err);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsPublicationPublishComplex(nullptr, 2.0, -6.5, &err);
    helicsErrorClear(&err);
    //auto res2=helicsPublicationPublishComplex(helics_publication pub, double real, double imag, nullptr);
    helicsPublicationPublishComplex(evil_pub, 4.507,11.3, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_pub_test, helicsPublicationPublishVector)
{
    //void helicsPublicationPublishVector(helics_publication pub, const double* vectorInput, int vectorLength, helics_error* err);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsPublicationPublishVector(nullptr, nullptr, 99, &err);
    helicsErrorClear(&err);
    //auto res2=helicsPublicationPublishVector(helics_publication pub, const double* vectorInput, int vectorLength, nullptr);
    helicsPublicationPublishVector(evil_pub, nullptr, 99, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_pub_test, helicsPublicationPublishNamedPoint)
{
    //void helicsPublicationPublishNamedPoint(helics_publication pub, const char* str, double val, helics_error* err);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsPublicationPublishNamedPoint(nullptr, "string", 5.0, &err);
    helicsErrorClear(&err);
    //auto res2=helicsPublicationPublishNamedPoint(helics_publication pub, const char* str, double val, nullptr);
    helicsPublicationPublishNamedPoint(evil_pub, "string", 5.0, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_pub_test, helicsPublicationAddTarget)
{
    //void helicsPublicationAddTarget(helics_publication pub, const char* target, helics_error* err);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsPublicationAddTarget(nullptr, "target", &err);
    helicsErrorClear(&err);
    //auto res2=helicsPublicationAddTarget(helics_publication pub, const char* target, nullptr);
    helicsPublicationAddTarget(evil_pub, "target", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_pub_test, helicsPublicationGetType)
{
    //const char*  helicsPublicationGetType(helics_publication pub);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto res1=helicsPublicationGetType(nullptr);
    EXPECT_STREQ(res1, "");
    auto res2 = helicsPublicationGetType(evil_pub);
    EXPECT_STREQ(res2, "");
}

TEST(evil_pub_test, helicsPublicationGetKey)
{
    //const char*  helicsPublicationGetKey(helics_publication pub);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto res1 = helicsPublicationGetKey(nullptr);
    EXPECT_STREQ(res1, "");
    auto res2 = helicsPublicationGetKey(evil_pub);
    EXPECT_STREQ(res2, "");
}

TEST(evil_pub_test, helicsPublicationGetUnits)
{
    //const char*  helicsPublicationGetUnits(helics_publication pub);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto res1 = helicsPublicationGetUnits(nullptr);
    EXPECT_STREQ(res1, "");
    auto res2 = helicsPublicationGetUnits(evil_pub);
    EXPECT_STREQ(res2, "");
}

TEST(evil_pub_test, helicsPublicationGetInfo)
{
    //const char*  helicsPublicationGetInfo(helics_publication pub);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto res1 = helicsPublicationGetInfo(nullptr);
    EXPECT_STREQ(res1, "");
        auto res2 = helicsPublicationGetInfo(evil_pub);
        EXPECT_STREQ(res2, "");
}

TEST(evil_pub_test, helicsPublicationSetInfo)
{
    //void helicsPublicationSetInfo(helics_publication pub, const char* info, helics_error* err);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsPublicationSetInfo(nullptr, "info", &err);
    helicsErrorClear(&err);
    //auto res2=helicsPublicationSetInfo(helics_publication pub, const char* info, nullptr);
    helicsPublicationSetInfo(evil_pub, "", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_pub_test, helicsPublicationGetOption)
{
    //helics_bool helicsPublicationGetOption(helics_publication pub, int option);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto res1=helicsPublicationGetOption(nullptr, -45);
    EXPECT_EQ(res1, helics_false);
    auto res2=helicsPublicationGetOption(evil_pub, 15);
    EXPECT_EQ(res2, helics_false);
}

TEST(evil_pub_test, helicsPublicationSetOption)
{
    //void helicsPublicationSetOption(helics_publication pub, int option, helics_bool val, helics_error* err);
    char rdata[256];
    helics_publication evil_pub = reinterpret_cast<helics_publication>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsPublicationSetOption(nullptr, -10, helics_true, &err);
    helicsErrorClear(&err);
    //auto res2=helicsPublicationSetOption(helics_publication pub, int option, helics_bool val, nullptr);
    helicsPublicationSetOption(evil_pub, 45, helics_true, &err);
    EXPECT_NE(err.error_code, 0);
}

//section Input interface Functions
//functions applying to a \ref helics_input object
TEST(evil_input_test, helicsInputAddTarget)
{
    //void helicsInputAddTarget(helics_input ipt, const char* target, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputAddTarget(nullptr, nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsInputAddTarget(helics_input ipt, const char* target, nullptr);
    helicsInputAddTarget(evil_input, "target", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputGetRawValueSize)
{
    //int helicsInputGetRawValueSize(helics_input ipt);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsInputGetRawValueSize(nullptr);
    EXPECT_EQ(res1, 0);
    auto res2=helicsInputGetRawValueSize(evil_input);
    EXPECT_EQ(res2, 0);
}

TEST(evil_input_test, helicsInputGetRawValue)
{
    //void helicsInputGetRawValue(helics_input ipt, void* data, int maxDatalen, int* actualSize, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    int actLen = 99;
    helicsInputGetRawValue(nullptr, nullptr, 87, &actLen, &err);
    EXPECT_EQ(actLen, 0);
    helicsErrorClear(&err);
    actLen = 99;
    //auto res2=helicsInputGetRawValue(helics_input ipt, void* data, int maxDatalen, int* actualSize, nullptr);
    helicsInputGetRawValue(evil_input, rdata, 10, &actLen, &err);
    EXPECT_EQ(actLen, 0);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputGetStringSize)
{
    //int helicsInputGetStringSize(helics_input ipt);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsInputGetStringSize(nullptr);
    EXPECT_EQ(res1, 0);
    auto res2=helicsInputGetStringSize(evil_input);
    EXPECT_EQ(res2, 0);
}

TEST(evil_input_test, helicsInputGetString)
{
    //void helicsInputGetString(helics_input ipt, char* outputString, int maxStringLen, int* actualLength, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    int actLen = 99;
    helicsInputGetString(nullptr, nullptr, 67, &actLen, &err);
    EXPECT_EQ(actLen, 0);
    helicsErrorClear(&err);
    //auto res2=helicsInputGetString(helics_input ipt, char* outputString, int maxStringLen, int* actualLength, nullptr);
    actLen = 99;
    helicsInputGetString(evil_input, nullptr, 45, &actLen, &err);
    EXPECT_EQ(actLen, 0);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputGetInteger)
{
    //int64_t helicsInputGetInteger(helics_input ipt, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsInputGetInteger(nullptr, &err);
    helicsErrorClear(&err);
    auto res2=helicsInputGetInteger(nullptr, nullptr);
    auto res3=helicsInputGetInteger(evil_input, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputGetBoolean)
{
    //helics_bool helicsInputGetBoolean(helics_input ipt, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsInputGetBoolean(nullptr, &err);
    EXPECT_EQ(res1, helics_false);
    helicsErrorClear(&err);
    auto res2=helicsInputGetBoolean(nullptr, nullptr);
    EXPECT_EQ(res2, helics_false);
    auto res3=helicsInputGetBoolean(evil_input, &err);
    EXPECT_EQ(res3, helics_false);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputGetDouble)
{
    //double helicsInputGetDouble(helics_input ipt, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsInputGetDouble(nullptr, &err);
    EXPECT_EQ(res1, helics_time_invalid);
    helicsErrorClear(&err);
    auto res2=helicsInputGetDouble(nullptr, nullptr);
    EXPECT_EQ(res2, helics_time_invalid);
    auto res3=helicsInputGetDouble(evil_input, &err);
    EXPECT_EQ(res3, helics_time_invalid);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputGetTime)
{
    //helics_time helicsInputGetTime(helics_input ipt, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsInputGetTime(nullptr, &err);
    EXPECT_EQ(res1, helics_time_invalid);
    helicsErrorClear(&err);
    auto res2=helicsInputGetTime(nullptr, nullptr);
    EXPECT_EQ(res2, helics_time_invalid);
    auto res3=helicsInputGetTime(evil_input, &err);
    EXPECT_EQ(res3, helics_time_invalid);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputGetChar)
{
    //char helicsInputGetChar(helics_input ipt, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    char res1=helicsInputGetChar(nullptr, &err);
    char tc = '\x15';
    EXPECT_TRUE(res1==tc);
    helicsErrorClear(&err);
    char res2=helicsInputGetChar(nullptr, nullptr);
    EXPECT_TRUE(res2==tc);
    char res3=helicsInputGetChar(evil_input, &err);
    EXPECT_TRUE(res3==tc);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputGetComplexObject)
{
    //helics_complex helicsInputGetComplexObject(helics_input ipt, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    auto res1=helicsInputGetComplexObject(nullptr, &err);
    EXPECT_EQ(res1.real, helics_time_invalid);
    helicsErrorClear(&err);
    auto res2=helicsInputGetComplexObject(nullptr, nullptr);
    EXPECT_EQ(res2.real, helics_time_invalid);
    auto res3=helicsInputGetComplexObject(evil_input, &err);
    EXPECT_EQ(res3.real, helics_time_invalid);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputGetComplex)
{
    //void helicsInputGetComplex(helics_input ipt, double* real, double* imag, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputGetComplex(nullptr, nullptr, nullptr, &err);
    helicsErrorClear(&err);
    double v1 = 19.4;
    double v2 = 18.3;
    //auto res2=helicsInputGetComplex(helics_input ipt, double* real, double* imag, nullptr);
    helicsInputGetComplex(evil_input,&v1, &v2, &err);
    EXPECT_DOUBLE_EQ(v1, 19.4);
    EXPECT_DOUBLE_EQ(v2, 18.3);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputGetVectorSize)
{
    //int helicsInputGetVectorSize(helics_input ipt);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsInputGetVectorSize(nullptr);
    EXPECT_EQ(res1, 0);
    auto res2=helicsInputGetVectorSize(evil_input);
    EXPECT_EQ(res2, 0);
}

TEST(evil_input_test, helicsInputGetVector)
{
    //void helicsInputGetVector(helics_input ipt, double data[], int maxlen, int* actualSize, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    int actLen = -56;
    helicsInputGetVector(nullptr, nullptr, 99, &actLen, &err);
    EXPECT_EQ(actLen, 0);
    helicsErrorClear(&err);
    //auto res2=helicsInputGetVector(helics_input ipt, double data[], int maxlen, int* actualSize, nullptr);
    helicsInputGetVector(evil_input, nullptr, 107, &actLen, &err);
    EXPECT_EQ(actLen, 0);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputGetNamedPoint)
{
    //void helicsInputGetNamedPoint(helics_input ipt, char* outputString, int maxStringLen, int* actualLength, double* val, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    int actLen = -56;
    double val = -15.0;
    helicsInputGetNamedPoint(nullptr, nullptr, 55, &actLen, &val, &err);
    EXPECT_EQ(actLen, 0);
    EXPECT_EQ(val, -15.0);
    helicsErrorClear(&err);
    helicsInputGetNamedPoint(nullptr, nullptr, 99, nullptr, nullptr, nullptr);
    actLen = -56;
    helicsInputGetNamedPoint(evil_input, rdata, 256, &actLen, &val, &err);
    EXPECT_EQ(actLen, 0);
    EXPECT_EQ(val, -15.0);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputSetDefaultRaw)
{
    //void helicsInputSetDefaultRaw(helics_input ipt, const void* data, int inputDataLength, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputSetDefaultRaw(nullptr, nullptr, -87, &err);
    helicsErrorClear(&err);
    //auto res2=helicsInputSetDefaultRaw(helics_input ipt, const void* data, int inputDataLength, nullptr);
    helicsInputSetDefaultRaw(evil_input, nullptr, 15, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputSetDefaultString)
{
    //void helicsInputSetDefaultString(helics_input ipt, const char* str, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputSetDefaultString(nullptr, nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsInputSetDefaultString(helics_input ipt, const char* str, nullptr);
    helicsInputSetDefaultString(evil_input, "string", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputSetDefaultInteger)
{
    //void helicsInputSetDefaultInteger(helics_input ipt, int64_t val, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputSetDefaultInteger(nullptr, -1798524456525, &err);
    helicsErrorClear(&err);
    //auto res2=helicsInputSetDefaultInteger(helics_input ipt, int64_t val, nullptr);
    helicsInputSetDefaultInteger(evil_input, 99, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputSetDefaultBoolean)
{
    //void helicsInputSetDefaultBoolean(helics_input ipt, helics_bool val, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputSetDefaultBoolean(nullptr, helics_false, &err);
    helicsErrorClear(&err);
    //auto res2=helicsInputSetDefaultBoolean(helics_input ipt, helics_bool val, nullptr);
    helicsInputSetDefaultBoolean(evil_input, helics_true, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputSetDefaultTime)
{
    //void helicsInputSetDefaultTime(helics_input ipt, helics_time val, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputSetDefaultTime(nullptr, 5.7, &err);
    helicsErrorClear(&err);
    //auto res2=helicsInputSetDefaultTime(helics_input ipt, helics_time val, nullptr);
    helicsInputSetDefaultTime(evil_input, helics_time_invalid, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputSetDefaultChar)
{
    //void helicsInputSetDefaultChar(helics_input ipt, char val, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputSetDefaultChar(nullptr, 'b', &err);
    helicsErrorClear(&err);
    //auto res2=helicsInputSetDefaultChar(helics_input ipt, char val, nullptr);
    helicsInputSetDefaultChar(evil_input, 'a', &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputSetDefaultDouble)
{
    //void helicsInputSetDefaultDouble(helics_input ipt, double val, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputSetDefaultDouble(nullptr, 1.0, &err);
    helicsErrorClear(&err);
    //auto res2=helicsInputSetDefaultDouble(helics_input ipt, double val, nullptr);
    helicsInputSetDefaultDouble(evil_input, 1.0, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputSetDefaultComplex)
{
    //void helicsInputSetDefaultComplex(helics_input ipt, double real, double imag, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputSetDefaultComplex(nullptr, 1.0, 1.0, &err);
    helicsErrorClear(&err);
    //auto res2=helicsInputSetDefaultComplex(helics_input ipt, double real, double imag, nullptr);
    helicsInputSetDefaultComplex(evil_input, 1.0, 1.0, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputSetDefaultVector)
{
    //void helicsInputSetDefaultVector(helics_input ipt, const double* vectorInput, int vectorLength, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputSetDefaultVector(nullptr, nullptr, 28, &err);
    helicsErrorClear(&err);
    //auto res2=helicsInputSetDefaultVector(helics_input ipt, const double* vectorInput, int vectorLength, nullptr);
    helicsInputSetDefaultVector(evil_input, nullptr, 87, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputSetDefaultNamedPoint)
{
    //void helicsInputSetDefaultNamedPoint(helics_input ipt, const char* str, double val, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputSetDefaultNamedPoint(nullptr, nullptr, 0.0, &err);
    helicsErrorClear(&err);
    //auto res2=helicsInputSetDefaultNamedPoint(helics_input ipt, const char* str, double val, nullptr);
    helicsInputSetDefaultNamedPoint(evil_input, "string", 19, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputGetType)
{
    //const char*  helicsInputGetType(helics_input ipt);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsInputGetType(nullptr);
    EXPECT_STREQ(res1, "");
     auto res2=helicsInputGetType(evil_input);
     EXPECT_STREQ(res2, "");
}

TEST(evil_input_test, helicsInputGetPublicationType)
{
    //const char*  helicsInputGetPublicationType(helics_input ipt);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsInputGetPublicationType(nullptr);
    EXPECT_STREQ(res1, "");
     auto res2=helicsInputGetPublicationType(evil_input);
     EXPECT_STREQ(res2, "");
}

TEST(evil_input_test, helicsInputGetKey)
{
    //const char*  helicsInputGetKey(helics_input ipt);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsInputGetKey(nullptr);
    EXPECT_STREQ(res1, "");
    auto res2=helicsInputGetKey(evil_input);
    EXPECT_STREQ(res2, "");
}

TEST(evil_input_test, helicsSubscriptionGetKey)
{
    //const char*  helicsSubscriptionGetKey(helics_input ipt);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsSubscriptionGetKey(nullptr);
    EXPECT_STREQ(res1, "");
    auto res2=helicsSubscriptionGetKey(evil_input);
    EXPECT_STREQ(res2, "");
}

TEST(evil_input_test, helicsInputGetUnits)
{
    //const char*  helicsInputGetUnits(helics_input ipt);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsInputGetUnits(nullptr);
    EXPECT_STREQ(res1, "");
     auto res2=helicsInputGetUnits(evil_input);
     EXPECT_STREQ(res2, "");
}

TEST(evil_input_test, helicsInputGetInjectionUnits)
{
    //const char*  helicsInputGetInjectionUnits(helics_input ipt);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsInputGetInjectionUnits(nullptr);
    EXPECT_STREQ(res1, "");
    auto res2=helicsInputGetInjectionUnits(evil_input);
    EXPECT_STREQ(res2, "");
}

TEST(evil_input_test, helicsInputGetExtractionUnits)
{
    //const char*  helicsInputGetExtractionUnits(helics_input ipt);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsInputGetExtractionUnits(nullptr);
    EXPECT_STREQ(res1, "");
    auto res2=helicsInputGetExtractionUnits(evil_input);
    EXPECT_STREQ(res2, "");
}

TEST(evil_input_test, helicsInputGetInfo)
{
    //const char*  helicsInputGetInfo(helics_input inp);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsInputGetInfo(nullptr);
    EXPECT_STREQ(res1, "");
    auto res2=helicsInputGetInfo(evil_input);
    EXPECT_STREQ(res2, "");
}

TEST(evil_input_test, helicsInputSetInfo)
{
    //void helicsInputSetInfo(helics_input inp, const char* info, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputSetInfo(nullptr, nullptr, &err);
    helicsErrorClear(&err);
    //auto res2=helicsInputSetInfo(helics_input inp, const char* info, nullptr);
    helicsInputSetInfo(evil_input, "info", &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputGetOption)
{
    //helics_bool helicsInputGetOption(helics_input inp, int option);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsInputGetOption(nullptr, 99);
    EXPECT_EQ(res1, helics_false);
    auto res2=helicsInputGetOption(evil_input, 5);
    EXPECT_EQ(res2, helics_false);
}

TEST(evil_input_test, helicsInputSetOption)
{
    //void helicsInputSetOption(helics_input inp, int option, helics_bool value, helics_error* err);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    helicsInputSetOption(nullptr, 0, helics_true, &err);
    helicsErrorClear(&err);
    //auto res2=helicsInputSetOption(helics_input inp, int option, helics_bool value, nullptr);
    helicsInputSetOption(evil_input, 45, helics_true, &err);
    EXPECT_NE(err.error_code, 0);
}

TEST(evil_input_test, helicsInputIsUpdated)
{
    //helics_bool helicsInputIsUpdated(helics_input ipt);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsInputIsUpdated(nullptr);
    EXPECT_EQ(res1, helics_false);
    auto res2=helicsInputIsUpdated(evil_input);
    EXPECT_EQ(res2, helics_false);
}

TEST(evil_input_test, helicsInputLastUpdateTime)
{
    //helics_time helicsInputLastUpdateTime(helics_input ipt);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    auto res1=helicsInputLastUpdateTime(nullptr);
    EXPECT_DOUBLE_EQ(res1, helics_time_invalid);
    auto res2=helicsInputLastUpdateTime(evil_input);
    EXPECT_DOUBLE_EQ(res2, helics_time_invalid);
    
}

TEST(evil_input_test, helicsInputClearUpdate)
{
    //void helicsInputClearUpdate(helics_input ipt);
    char rdata[256];
    helics_input evil_input = reinterpret_cast<helics_input>(rdata);
    EXPECT_NO_THROW(helicsInputClearUpdate(nullptr));
    EXPECT_NO_THROW(helicsInputClearUpdate(evil_input));
}

//section Message Federate Functions
//Functions applying to federates created as a value or combination federate \ref helics_federate objects
TEST(evil_message_fed_test, helicsFederateRegisterEndpoint)
{
    //helics_endpoint helicsFederateRegisterEndpoint(helics_federate fed, const char* name, const char* type, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFederateRegisterEndpoint(helics_federate fed, const char* name, const char* type, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateRegisterEndpoint(helics_federate fed, const char* name, const char* type, nullptr);
    //auto res3=helicsFederateRegisterEndpoint(evil_federate, const char* name, const char* type, &err);
}

TEST(evil_message_fed_test, helicsFederateRegisterGlobalEndpoint)
{
    //helics_endpoint helicsFederateRegisterGlobalEndpoint(helics_federate fed, const char* name, const char* type, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFederateRegisterGlobalEndpoint(helics_federate fed, const char* name, const char* type, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateRegisterGlobalEndpoint(helics_federate fed, const char* name, const char* type, nullptr);
    //auto res3=helicsFederateRegisterGlobalEndpoint(evil_federate, const char* name, const char* type, &err);
}

TEST(evil_message_fed_test, helicsFederateGetEndpoint)
{
    //helics_endpoint helicsFederateGetEndpoint(helics_federate fed, const char* name, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFederateGetEndpoint(helics_federate fed, const char* name, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateGetEndpoint(helics_federate fed, const char* name, nullptr);
    //auto res3=helicsFederateGetEndpoint(evil_federate, const char* name, &err);
}

TEST(evil_message_fed_test, helicsFederateGetEndpointByIndex)
{
    //helics_endpoint helicsFederateGetEndpointByIndex(helics_federate fed, int index, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFederateGetEndpointByIndex(helics_federate fed, int index, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateGetEndpointByIndex(helics_federate fed, int index, nullptr);
    //auto res3=helicsFederateGetEndpointByIndex(evil_federate, int index, &err);
}

TEST(evil_message_fed_test, helicsFederateHasMessage)
{
    //helics_bool helicsFederateHasMessage(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    //auto res1=helicsFederateHasMessage(evil_federate);
}

TEST(evil_message_fed_test, helicsFederatePendingMessages)
{
    //int helicsFederatePendingMessages(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    //auto res1=helicsFederatePendingMessages(evil_federate);
}

TEST(evil_message_fed_test, helicsFederateGetMessage)
{
    //helics_message helicsFederateGetMessage(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    //auto res1=helicsFederateGetMessage(evil_federate);
}

TEST(evil_message_fed_test, helicsFederateGetMessageObject)
{
    //helics_message_object helicsFederateGetMessageObject(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    //auto res1=helicsFederateGetMessageObject(evil_federate);
}

TEST(evil_message_fed_test, helicsFederateCreateMessageObject)
{
    //helics_message_object helicsFederateCreateMessageObject(helics_federate fed, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFederateCreateMessageObject(helics_federate fed, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateCreateMessageObject(helics_federate fed, nullptr);
    //auto res3=helicsFederateCreateMessageObject(evil_federate, &err);
}

TEST(evil_message_fed_test, helicsFederateClearMessages)
{
    //void helicsFederateClearMessages(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    //auto res1=helicsFederateClearMessages(evil_federate);
}

TEST(evil_message_fed_test, helicsFederateGetEndpointCount)
{
    //int helicsFederateGetEndpointCount(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    //auto res1=helicsFederateGetEndpointCount(evil_federate);
}

//section Message object Functions
//functions applying to a \ref helics_message_object
TEST(evil_message_object_test, helicsMessageGetSource)
{
    //const char*  helicsMessageGetSource(helics_message_object message);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    //auto res1=helicsMessageGetSource(evil_mo);
}

TEST(evil_message_object_test, helicsMessageGetDestination)
{
    //const char*  helicsMessageGetDestination(helics_message_object message);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    //auto res1=helicsMessageGetDestination(evil_mo);
}

TEST(evil_message_object_test, helicsMessageGetOriginalSource)
{
    //const char*  helicsMessageGetOriginalSource(helics_message_object message);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    //auto res1=helicsMessageGetOriginalSource(evil_mo);
}

TEST(evil_message_object_test, helicsMessageGetOriginalDestination)
{
    //const char*  helicsMessageGetOriginalDestination(helics_message_object message);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    //auto res1=helicsMessageGetOriginalDestination(evil_mo);
}

TEST(evil_message_object_test, helicsMessageGetTime)
{
    //helics_time helicsMessageGetTime(helics_message_object message);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    //auto res1=helicsMessageGetTime(evil_mo);
}

TEST(evil_message_object_test, helicsMessageGetString)
{
    //const char*  helicsMessageGetString(helics_message_object message);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    //auto res1=helicsMessageGetString(evil_mo);
}

TEST(evil_message_object_test, helicsMessageGetMessageID)
{
    //int helicsMessageGetMessageID(helics_message_object message);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    //auto res1=helicsMessageGetMessageID(evil_mo);
}

TEST(evil_message_object_test, helicsMessageCheckFlag)
{
    //helics_bool helicsMessageCheckFlag(helics_message_object message, int flag);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    //auto res1=helicsMessageCheckFlag(evil_mo, int flag);
}

TEST(evil_message_object_test, helicsMessageGetRawDataSize)
{
    //int helicsMessageGetRawDataSize(helics_message_object message);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    //auto res1=helicsMessageGetRawDataSize(evil_mo);
}

TEST(evil_message_object_test, helicsMessageGetRawData)
{
    //void helicsMessageGetRawData(helics_message_object message, void* data, int maxMessagelen, int* actualSize, helics_error* err);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsMessageGetRawData(helics_message_object message, void* data, int maxMessagelen, int* actualSize, &err);
    helicsErrorClear(&err);
    //auto res2=helicsMessageGetRawData(helics_message_object message, void* data, int maxMessagelen, int* actualSize, nullptr);
    //auto res3=helicsMessageGetRawData(evil_mo, void* data, int maxMessagelen, int* actualSize, &err);
}

TEST(evil_message_object_test, helicsMessageIsValid)
{
    //helics_bool helicsMessageIsValid(helics_message_object message);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    //auto res1=helicsMessageIsValid(evil_mo);
}

TEST(evil_message_object_test, helicsMessageSetSource)
{
    //void helicsMessageSetSource(helics_message_object message, const char* src, helics_error* err);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsMessageSetSource(helics_message_object message, const char* src, &err);
    helicsErrorClear(&err);
    //auto res2=helicsMessageSetSource(helics_message_object message, const char* src, nullptr);
    //auto res3=helicsMessageSetSource(evil_mo, const char* src, &err);
}

TEST(evil_message_object_test, helicsMessageSetDestination)
{
    //void helicsMessageSetDestination(helics_message_object message, const char* dest, helics_error* err);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsMessageSetDestination(helics_message_object message, const char* dest, &err);
    helicsErrorClear(&err);
    //auto res2=helicsMessageSetDestination(helics_message_object message, const char* dest, nullptr);
    //auto res3=helicsMessageSetDestination(evil_mo, const char* dest, &err);
}

TEST(evil_message_object_test, helicsMessageSetOriginalSource)
{
    //void helicsMessageSetOriginalSource(helics_message_object message, const char* src, helics_error* err);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsMessageSetOriginalSource(helics_message_object message, const char* src, &err);
    helicsErrorClear(&err);
    //auto res2=helicsMessageSetOriginalSource(helics_message_object message, const char* src, nullptr);
    //auto res3=helicsMessageSetOriginalSource(evil_mo, const char* src, &err);
}

TEST(evil_message_object_test, helicsMessageSetOriginalDestination)
{
    //void helicsMessageSetOriginalDestination(helics_message_object message, const char* dest, helics_error* err);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsMessageSetOriginalDestination(helics_message_object message, const char* dest, &err);
    helicsErrorClear(&err);
    //auto res2=helicsMessageSetOriginalDestination(helics_message_object message, const char* dest, nullptr);
    //auto res3=helicsMessageSetOriginalDestination(evil_mo, const char* dest, &err);
}

TEST(evil_message_object_test, helicsMessageSetTime)
{
    //void helicsMessageSetTime(helics_message_object message, helics_time time, helics_error* err);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsMessageSetTime(helics_message_object message, helics_time time, &err);
    helicsErrorClear(&err);
    //auto res2=helicsMessageSetTime(helics_message_object message, helics_time time, nullptr);
    //auto res3=helicsMessageSetTime(evil_mo, helics_time time, &err);
}

TEST(evil_message_object_test, helicsMessageResize)
{
    //void helicsMessageResize(helics_message_object message, int newSize, helics_error* err);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsMessageResize(helics_message_object message, int newSize, &err);
    helicsErrorClear(&err);
    //auto res2=helicsMessageResize(helics_message_object message, int newSize, nullptr);
    //auto res3=helicsMessageResize(evil_mo, int newSize, &err);
}

TEST(evil_message_object_test, helicsMessageReserve)
{
    //void helicsMessageReserve(helics_message_object message, int reserveSize, helics_error* err);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsMessageReserve(helics_message_object message, int reserveSize, &err);
    helicsErrorClear(&err);
    //auto res2=helicsMessageReserve(helics_message_object message, int reserveSize, nullptr);
    //auto res3=helicsMessageReserve(evil_mo, int reserveSize, &err);
}

TEST(evil_message_object_test, helicsMessageSetMessageID)
{
    //void helicsMessageSetMessageID(helics_message_object message, int32_t messageID, helics_error* err);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsMessageSetMessageID(helics_message_object message, int32_t messageID, &err);
    helicsErrorClear(&err);
    //auto res2=helicsMessageSetMessageID(helics_message_object message, int32_t messageID, nullptr);
    //auto res3=helicsMessageSetMessageID(evil_mo, int32_t messageID, &err);
}

TEST(evil_message_object_test, helicsMessageClearFlags)
{
    //void helicsMessageClearFlags(helics_message_object message);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    //auto res1=helicsMessageClearFlags(evil_mo);
}

TEST(evil_message_object_test, helicsMessageSetFlagOption)
{
    //void helicsMessageSetFlagOption(helics_message_object message, int flag, helics_bool flagValue, helics_error* err);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsMessageSetFlagOption(helics_message_object message, int flag, helics_bool flagValue, &err);
    helicsErrorClear(&err);
    //auto res2=helicsMessageSetFlagOption(helics_message_object message, int flag, helics_bool flagValue, nullptr);
    //auto res3=helicsMessageSetFlagOption(evil_mo, int flag, helics_bool flagValue, &err);
}

TEST(evil_message_object_test, helicsMessageSetString)
{
    //void helicsMessageSetString(helics_message_object message, const char* str, helics_error* err);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsMessageSetString(helics_message_object message, const char* str, &err);
    helicsErrorClear(&err);
    //auto res2=helicsMessageSetString(helics_message_object message, const char* str, nullptr);
    //auto res3=helicsMessageSetString(evil_mo, const char* str, &err);
}

TEST(evil_message_object_test, helicsMessageSetData)
{
    //void helicsMessageSetData(helics_message_object message, const void* data, int inputDataLength, helics_error* err);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsMessageSetData(helics_message_object message, const void* data, int inputDataLength, &err);
    helicsErrorClear(&err);
    //auto res2=helicsMessageSetData(helics_message_object message, const void* data, int inputDataLength, nullptr);
    //auto res3=helicsMessageSetData(evil_mo, const void* data, int inputDataLength, &err);
}

TEST(evil_message_object_test, helicsMessageAppendData)
{
    //void helicsMessageAppendData(helics_message_object message, const void* data, int inputDataLength, helics_error* err);
    char rdata[256];
    helics_message_object evil_mo = reinterpret_cast<helics_message_object>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsMessageAppendData(helics_message_object message, const void* data, int inputDataLength, &err);
    helicsErrorClear(&err);
    //auto res2=helicsMessageAppendData(helics_message_object message, const void* data, int inputDataLength, nullptr);
    //auto res3=helicsMessageAppendData(evil_mo, const void* data, int inputDataLength, &err);
}

//section Endpoint interface Functions
//functions applying to a \ref helics_endpoint object
TEST(evil_endpoint_test, helicsEndpointSetDefaultDestination)
{
    //void helicsEndpointSetDefaultDestination(helics_endpoint endpoint, const char* dest, helics_error* err);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsEndpointSetDefaultDestination(helics_endpoint endpoint, const char* dest, &err);
    helicsErrorClear(&err);
    //auto res2=helicsEndpointSetDefaultDestination(helics_endpoint endpoint, const char* dest, nullptr);
    //auto res3=helicsEndpointSetDefaultDestination(helics_endpoint endpoint, const char* dest, &err);
}

TEST(evil_endpoint_test, helicsEndpointGetDefaultDestination)
{
    //const char*  helicsEndpointGetDefaultDestination(helics_endpoint endpoint);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    //auto res1=helicsEndpointGetDefaultDestination(helics_endpoint endpoint);
}

TEST(evil_endpoint_test, helicsEndpointSendMessageRaw)
{
    //void helicsEndpointSendMessageRaw(helics_endpoint endpoint, const char* dest, const void* data, int inputDataLength, helics_error* err);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsEndpointSendMessageRaw(helics_endpoint endpoint, const char* dest, const void* data, int inputDataLength, &err);
    helicsErrorClear(&err);
    //auto res2=helicsEndpointSendMessageRaw(helics_endpoint endpoint, const char* dest, const void* data, int inputDataLength, nullptr);
    //auto res3=helicsEndpointSendMessageRaw(helics_endpoint endpoint, const char* dest, const void* data, int inputDataLength, &err);
}

TEST(evil_endpoint_test, helicsEndpointSendEventRaw)
{
    //void helicsEndpointSendEventRaw(     helics_endpoint endpoint,     const char* dest,     const void* data,     int inputDataLength,     helics_time time,     helics_error* err);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsEndpointSendEventRaw(     helics_endpoint endpoint,     const char* dest,     const void* data,     int inputDataLength,     helics_time time,     &err);
    helicsErrorClear(&err);
    //auto res2=helicsEndpointSendEventRaw(     helics_endpoint endpoint,     const char* dest,     const void* data,     int inputDataLength,     helics_time time,     nullptr);
    //auto res3=helicsEndpointSendEventRaw(     helics_endpoint endpoint,     const char* dest,     const void* data,     int inputDataLength,     helics_time time,     &err);
}

TEST(evil_endpoint_test, helicsEndpointSendMessage)
{
    //void helicsEndpointSendMessage(helics_endpoint endpoint, helics_message* message, helics_error* err);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsEndpointSendMessage(helics_endpoint endpoint, helics_message* message, &err);
    helicsErrorClear(&err);
    //auto res2=helicsEndpointSendMessage(helics_endpoint endpoint, helics_message* message, nullptr);
    //auto res3=helicsEndpointSendMessage(helics_endpoint endpoint, helics_message* message, &err);
}

TEST(evil_endpoint_test, helicsEndpointSendMessageObject)
{
    //void helicsEndpointSendMessageObject(helics_endpoint endpoint, helics_message_object message, helics_error* err);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsEndpointSendMessageObject(helics_endpoint endpoint, helics_message_object message, &err);
    helicsErrorClear(&err);
    //auto res2=helicsEndpointSendMessageObject(helics_endpoint endpoint, helics_message_object message, nullptr);
    //auto res3=helicsEndpointSendMessageObject(helics_endpoint endpoint, helics_message_object message, &err);
}

TEST(evil_endpoint_test, helicsEndpointSubscribe)
{
    //void helicsEndpointSubscribe(helics_endpoint endpoint, const char* key, helics_error* err);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsEndpointSubscribe(helics_endpoint endpoint, const char* key, &err);
    helicsErrorClear(&err);
    //auto res2=helicsEndpointSubscribe(helics_endpoint endpoint, const char* key, nullptr);
    //auto res3=helicsEndpointSubscribe(helics_endpoint endpoint, const char* key, &err);
}

TEST(evil_endpoint_test, helicsEndpointHasMessage)
{
    //helics_bool helicsEndpointHasMessage(helics_endpoint endpoint);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    //auto res1=helicsEndpointHasMessage(helics_endpoint endpoint);
}

TEST(evil_endpoint_test, helicsEndpointPendingMessages)
{
    //int helicsEndpointPendingMessages(helics_endpoint endpoint);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    //auto res1=helicsEndpointPendingMessages(helics_endpoint endpoint);
}

TEST(evil_endpoint_test, helicsEndpointGetMessage)
{
    //helics_message helicsEndpointGetMessage(helics_endpoint endpoint);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    //auto res1=helicsEndpointGetMessage(helics_endpoint endpoint);
}

TEST(evil_endpoint_test, helicsEndpointGetMessageObject)
{
    //helics_message_object helicsEndpointGetMessageObject(helics_endpoint endpoint);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    //auto res1=helicsEndpointGetMessageObject(helics_endpoint endpoint);
}

TEST(evil_endpoint_test, helicsEndpointClearMessages)
{
    //void helicsEndpointClearMessages(helics_endpoint endpoint);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    //auto res1=helicsEndpointClearMessages(helics_endpoint endpoint);
}

TEST(evil_endpoint_test, helicsEndpointGetType)
{
    //const char*  helicsEndpointGetType(helics_endpoint endpoint);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    //auto res1=helicsEndpointGetType(helics_endpoint endpoint);
}

TEST(evil_endpoint_test, helicsEndpointGetName)
{
    //const char*  helicsEndpointGetName(helics_endpoint endpoint);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    //auto res1=helicsEndpointGetName(helics_endpoint endpoint);
}

TEST(evil_endpoint_test, helicsEndpointGetInfo)
{
    //const char*  helicsEndpointGetInfo(helics_endpoint end);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    //auto res1=helicsEndpointGetInfo(helics_endpoint end);
}

TEST(evil_endpoint_test, helicsEndpointSetInfo)
{
    //void helicsEndpointSetInfo(helics_endpoint end, const char* info, helics_error* err);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsEndpointSetInfo(helics_endpoint end, const char* info, &err);
    helicsErrorClear(&err);
    //auto res2=helicsEndpointSetInfo(helics_endpoint end, const char* info, nullptr);
    //auto res3=helicsEndpointSetInfo(helics_endpoint end, const char* info, &err);
}

TEST(evil_endpoint_test, helicsEndpointSetOption)
{
    //void helicsEndpointSetOption(helics_endpoint end, int option, helics_bool value, helics_error* err);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsEndpointSetOption(helics_endpoint end, int option, helics_bool value, &err);
    helicsErrorClear(&err);
    //auto res2=helicsEndpointSetOption(helics_endpoint end, int option, helics_bool value, nullptr);
    //auto res3=helicsEndpointSetOption(helics_endpoint end, int option, helics_bool value, &err);
}

TEST(evil_endpoint_test, helicsEndpointGetOption)
{
    //helics_bool helicsEndpointGetOption(helics_endpoint end, int option);
    char rdata[256];
    helics_endpoint evil_ept = reinterpret_cast<helics_endpoint>(rdata);
    //auto res1=helicsEndpointGetOption(helics_endpoint end, int option);
}

//section Filter Federate Functions
//Functions applying to all federates but related to \ref helics_filter
TEST(evil_filter_fed_test, helicsFederateRegisterFilter)
{
    //helics_filter helicsFederateRegisterFilter(helics_federate fed, helics_filter_type type, const char* name, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFederateRegisterFilter(helics_federate fed, helics_filter_type type, const char* name, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateRegisterFilter(helics_federate fed, helics_filter_type type, const char* name, nullptr);
    //auto res3=helicsFederateRegisterFilter(evil_federate, helics_filter_type type, const char* name, &err);
}

TEST(evil_filter_fed_test, helicsFederateRegisterGlobalFilter)
{
    //helics_filter helicsFederateRegisterGlobalFilter(helics_federate fed, helics_filter_type type, const char* name, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFederateRegisterGlobalFilter(helics_federate fed, helics_filter_type type, const char* name, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateRegisterGlobalFilter(helics_federate fed, helics_filter_type type, const char* name, nullptr);
    //auto res3=helicsFederateRegisterGlobalFilter(evil_federate, helics_filter_type type, const char* name, &err);
}

TEST(evil_filter_fed_test, helicsFederateRegisterCloningFilter)
{
    //helics_filter helicsFederateRegisterCloningFilter(helics_federate fed, const char* deliveryEndpoint, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFederateRegisterCloningFilter(helics_federate fed, const char* deliveryEndpoint, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateRegisterCloningFilter(helics_federate fed, const char* deliveryEndpoint, nullptr);
    //auto res3=helicsFederateRegisterCloningFilter(evil_federate, const char* deliveryEndpoint, &err);
}

TEST(evil_filter_fed_test, helicsFederateRegisterGlobalCloningFilter)
{
    //helics_filter helicsFederateRegisterGlobalCloningFilter(helics_federate fed, const char* deliveryEndpoint, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFederateRegisterGlobalCloningFilter(helics_federate fed, const char* deliveryEndpoint, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateRegisterGlobalCloningFilter(helics_federate fed, const char* deliveryEndpoint, nullptr);
    //auto res3=helicsFederateRegisterGlobalCloningFilter(evil_federate, const char* deliveryEndpoint, &err);
}

TEST(evil_filter_fed_test, helicsFederateGetFilterCount)
{
    //int helicsFederateGetFilterCount(helics_federate fed);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    //auto res1=helicsFederateGetFilterCount(evil_federate);
}

TEST(evil_filter_fed_test, helicsFederateGetFilter)
{
    //helics_filter helicsFederateGetFilter(helics_federate fed, const char* name, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFederateGetFilter(helics_federate fed, const char* name, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateGetFilter(helics_federate fed, const char* name, nullptr);
    //auto res3=helicsFederateGetFilter(evil_federate, const char* name, &err);
}

TEST(evil_filter_fed_test, helicsFederateGetFilterByIndex)
{
    //helics_filter helicsFederateGetFilterByIndex(helics_federate fed, int index, helics_error* err);
    char rdata[256];
    helics_federate evil_federate = reinterpret_cast<helics_federate>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFederateGetFilterByIndex(helics_federate fed, int index, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFederateGetFilterByIndex(helics_federate fed, int index, nullptr);
    //auto res3=helicsFederateGetFilterByIndex(evil_federate, int index, &err);
}

//section Filter interface Functions
//Functions applying to a \ref helics_filter object
TEST(evil_filter_test, helicsFilterGetName)
{
    //const char*  helicsFilterGetName(helics_filter filt);
    char rdata[256];
    helics_filter evil_filt = reinterpret_cast<helics_filter>(rdata);
    //auto res1=helicsFilterGetName(evil_filt);
}

TEST(evil_filter_test, helicsFilterSet)
{
    //void helicsFilterSet(helics_filter filt, const char* prop, double val, helics_error* err);
    char rdata[256];
    helics_filter evil_filt = reinterpret_cast<helics_filter>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFilterSet(helics_filter filt, const char* prop, double val, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFilterSet(helics_filter filt, const char* prop, double val, nullptr);
    //auto res3=helicsFilterSet(evil_filt, const char* prop, double val, &err);
}

TEST(evil_filter_test, helicsFilterSetString)
{
    //void helicsFilterSetString(helics_filter filt, const char* prop, const char* val, helics_error* err);
    char rdata[256];
    helics_filter evil_filt = reinterpret_cast<helics_filter>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFilterSetString(helics_filter filt, const char* prop, const char* val, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFilterSetString(helics_filter filt, const char* prop, const char* val, nullptr);
    //auto res3=helicsFilterSetString(evil_filt, const char* prop, const char* val, &err);
}

TEST(evil_filter_test, helicsFilterAddDestinationTarget)
{
    //void helicsFilterAddDestinationTarget(helics_filter filt, const char* dest, helics_error* err);
    char rdata[256];
    helics_filter evil_filt = reinterpret_cast<helics_filter>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFilterAddDestinationTarget(helics_filter filt, const char* dest, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFilterAddDestinationTarget(helics_filter filt, const char* dest, nullptr);
    //auto res3=helicsFilterAddDestinationTarget(evil_filt, const char* dest, &err);
}

TEST(evil_filter_test, helicsFilterAddSourceTarget)
{
    //void helicsFilterAddSourceTarget(helics_filter filt, const char* source, helics_error* err);
    char rdata[256];
    helics_filter evil_filt = reinterpret_cast<helics_filter>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFilterAddSourceTarget(helics_filter filt, const char* source, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFilterAddSourceTarget(helics_filter filt, const char* source, nullptr);
    //auto res3=helicsFilterAddSourceTarget(evil_filt, const char* source, &err);
}

TEST(evil_filter_test, helicsFilterAddDeliveryEndpoint)
{
    //void helicsFilterAddDeliveryEndpoint(helics_filter filt, const char* deliveryEndpoint, helics_error* err);
    char rdata[256];
    helics_filter evil_filt = reinterpret_cast<helics_filter>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFilterAddDeliveryEndpoint(helics_filter filt, const char* deliveryEndpoint, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFilterAddDeliveryEndpoint(helics_filter filt, const char* deliveryEndpoint, nullptr);
    //auto res3=helicsFilterAddDeliveryEndpoint(evil_filt, const char* deliveryEndpoint, &err);
}

TEST(evil_filter_test, helicsFilterRemoveTarget)
{
    //void helicsFilterRemoveTarget(helics_filter filt, const char* target, helics_error* err);
    char rdata[256];
    helics_filter evil_filt = reinterpret_cast<helics_filter>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFilterRemoveTarget(helics_filter filt, const char* target, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFilterRemoveTarget(helics_filter filt, const char* target, nullptr);
    //auto res3=helicsFilterRemoveTarget(evil_filt, const char* target, &err);
}

TEST(evil_filter_test, helicsFilterRemoveDeliveryEndpoint)
{
    //void helicsFilterRemoveDeliveryEndpoint(helics_filter filt, const char* deliveryEndpoint, helics_error* err);
    char rdata[256];
    helics_filter evil_filt = reinterpret_cast<helics_filter>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFilterRemoveDeliveryEndpoint(helics_filter filt, const char* deliveryEndpoint, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFilterRemoveDeliveryEndpoint(helics_filter filt, const char* deliveryEndpoint, nullptr);
    //auto res3=helicsFilterRemoveDeliveryEndpoint(evil_filt, const char* deliveryEndpoint, &err);
}

TEST(evil_filter_test, helicsFilterGetInfo)
{
    //const char*  helicsFilterGetInfo(helics_filter filt);
    char rdata[256];
    helics_filter evil_filt = reinterpret_cast<helics_filter>(rdata);
    //auto res1=helicsFilterGetInfo(evil_filt);
}

TEST(evil_filter_test, helicsFilterSetInfo)
{
    //void helicsFilterSetInfo(helics_filter filt, const char* info, helics_error* err);
    char rdata[256];
    helics_filter evil_filt = reinterpret_cast<helics_filter>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFilterSetInfo(helics_filter filt, const char* info, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFilterSetInfo(helics_filter filt, const char* info, nullptr);
    //auto res3=helicsFilterSetInfo(evil_filt, const char* info, &err);
}

TEST(evil_filter_test, helicsFilterSetOption)
{
    //void helicsFilterSetOption(helics_filter filt, int option, helics_bool value, helics_error* err);
    char rdata[256];
    helics_filter evil_filt = reinterpret_cast<helics_filter>(rdata);
    auto err = helicsErrorInitialize();
    err.error_code = 45;
    //auto res1=helicsFilterSetOption(helics_filter filt, int option, helics_bool value, &err);
    helicsErrorClear(&err);
    //auto res2=helicsFilterSetOption(helics_filter filt, int option, helics_bool value, nullptr);
    //auto res3=helicsFilterSetOption(evil_filt, int option, helics_bool value, &err);
}

TEST(evil_filter_test, helicsFilterGetOption)
{
    //helics_bool helicsFilterGetOption(helics_filter filt, int option);
    char rdata[256];
    helics_filter evil_filt = reinterpret_cast<helics_filter>(rdata);
    //auto res1=helicsFilterGetOption(evil_filt, int option);
}

//section Query Functions
//functions applying to a \ref helics_query object
TEST(evil_query_test, helicsQueryExecute)
{
    //const char*  helicsQueryExecute(helics_query query, helics_federate fed, helics_error* err);
}

TEST(evil_query_test, helicsQueryCoreExecute)
{
    //const char*  helicsQueryCoreExecute(helics_query query, helics_core core, helics_error* err);
}

TEST(evil_query_test, helicsQueryBrokerExecute)
{
    //const char*  helicsQueryBrokerExecute(helics_query query, helics_broker broker, helics_error* err);
}

TEST(evil_query_test, helicsQueryExecuteAsync)
{
    //void helicsQueryExecuteAsync(helics_query query, helics_federate fed, helics_error* err);
}

TEST(evil_query_test, helicsQueryExecuteComplete)
{
    //const char*  helicsQueryExecuteComplete(helics_query query, helics_error* err);
}

TEST(evil_query_test, helicsQueryIsCompleted)
{
    //helics_bool helicsQueryIsCompleted(helics_query query);
}

TEST(evil_query_test, helicsQueryFree)
{
    //void helicsQueryFree(helics_query query);
}

// end generated code
