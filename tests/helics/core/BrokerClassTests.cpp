/*
Copyright (c) 2017-2021,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable
Energy, LLC.  See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/
#include "helics/core/BrokerFactory.hpp"
#include "helics/core/CoreBroker.hpp"
#include "helics/core/CoreFactory.hpp"
#include "helics/helics-config.h"
#include "helics/core/CoreFederateInfo.hpp"

#include "gtest/gtest.h"

#include <future>

/** test the assignment and retrieval of global value from a broker object*/
TEST(broker_tests, global_value_test)
{
    auto brk = helics::BrokerFactory::create(helics::CoreType::TEST, "gbroker", "-f2 --root");
    std::string globalVal = "this is a string constant that functions as a global";
    std::string globalVal2 = "this is a second string constant that functions as a global";
    brk->setGlobal("testglobal", globalVal);
    auto res = brk->query("global_value", "testglobal");
    EXPECT_EQ(res, globalVal);
    brk->setGlobal("testglobal2", globalVal2);

    res = brk->query("global_value", "testglobal");
    EXPECT_EQ(res, globalVal);
    res = brk->query("global_value", "testglobal2");
    EXPECT_EQ(res, globalVal2);
    brk->disconnect();
    EXPECT_FALSE(brk->isConnected());
}

/** test the assignment and retrieval of global value from a broker object*/
TEST(broker_tests, subbroker_min_test) {
    auto brk = helics::BrokerFactory::create(helics::CoreType::TEST, "gbroker2", "--subbrokers=2 -f 2 --root");

    auto brk2 = helics::BrokerFactory::create(helics::CoreType::TEST, "gb2", "--broker=gbroker2");

    auto cr1 = helics::CoreFactory::create(helics::CoreType::TEST, "c1", "--broker=gb2");

    helics::CoreFederateInfo cf1;
    auto fid1 = cr1->registerFederate("fed1", cf1);
    auto fid2 = cr1->registerFederate("fed2", cf1);

    auto fut1 =
        std::async(std::launch::async, [fid1, &cr1]() { cr1->enterInitializingMode(fid1); });

     auto fut2 =
        std::async(std::launch::async, [fid2, &cr1]() { cr1->enterInitializingMode(fid2); });

     auto res = fut1.wait_for(std::chrono::milliseconds(100));
     // this should not allow initilizingMode entry since only 1 subbroker
     EXPECT_EQ(res, std::future_status::timeout);

     auto cr2 = helics::CoreFactory::create(helics::CoreType::TEST, "c2", "--broker=gb2");
     auto fid3 = cr2->registerFederate("fed3", cf1);

     auto fut3 =
         std::async(std::launch::async, [fid3, &cr2]() { cr2->enterInitializingMode(fid3); });

     res = fut1.wait_for(std::chrono::milliseconds(100));
     // this should still not allow initilizingMode entry since still only 1 subbroker
     EXPECT_EQ(res, std::future_status::timeout);


     auto brk3 = helics::BrokerFactory::create(helics::CoreType::TEST, "gb3", "--broker=gbroker2");

     auto cr3 = helics::CoreFactory::create(helics::CoreType::TEST, "c3", "--broker=gb3");

     auto fid4 = cr3->registerFederate("fed4", cf1);

     auto fut4 =
         std::async(std::launch::async, [fid4, &cr3]() { cr3->enterInitializingMode(fid4); });

     // now it should grant
     fut1.get();
     fut2.get();
     fut3.get();
     fut4.get();

     EXPECT_FALSE(brk->isOpenToNewFederates());
     EXPECT_FALSE(cr3->isOpenToNewFederates());

     brk->disconnect();

     EXPECT_TRUE(brk3->waitForDisconnect());
}
