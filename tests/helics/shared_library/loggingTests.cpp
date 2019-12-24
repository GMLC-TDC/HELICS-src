/*
Copyright (c) 2017-2019,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See
the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

#include "../ThirdParty/concurrency/gmlc/libguarded/guarded.hpp"

#include <complex>
#include <gtest/gtest.h>

/** these test cases test out the value converters
 */
#include "ctestFixtures.hpp"
#include "helics/shared_api_library/helicsCallbacks.h"

using logblocktype = gmlc::libguarded::guarded<std::vector<std::pair<int, std::string>>>;
TEST(logging_tests, check_log_message)
{
    auto fi = helicsCreateFederateInfo();
    auto err = helicsErrorInitialize();
    helicsFederateInfoSetCoreType(fi, helics_core_type_test, &err);
    helicsFederateInfoSetCoreInitString(fi, "--autobroker", &err);
    helicsFederateInfoSetIntegerProperty(fi, helics_property_int_log_level, 5, &err);

    auto fed = helicsCreateValueFederate("test1", fi, &err);

    logblocktype mlog;

    auto logg = [](int level, const char*, const char* message, void* udata) {
        auto* mp = reinterpret_cast<logblocktype*>(udata);
        mp->lock()->emplace_back(level, message);
    };

    helicsFederateSetLoggingCallback(fed, logg, &mlog, &err);

    EXPECT_EQ(err.error_code, 0);

    helicsFederateEnterExecutingMode(fed, &err);
    helicsFederateLogInfoMessage(fed, "test MEXAGE", &err);
    helicsFederateRequestNextStep(fed, &err);
    helicsFederateFinalize(fed, &err);
    EXPECT_EQ(err.error_code, 0);
    auto llock = mlog.lock();
    bool found = false;
    for (auto& m : llock) {
        if (m.second.find("MEXAGE") != std::string::npos) {
            found = true;
        }
    }
    EXPECT_TRUE(found);
    helicsFederateFree(fed);
    helicsFederateInfoFree(fi);
}

TEST(logging_tests, check_log_message_levels)
{
    auto fi = helicsCreateFederateInfo();
    auto err = helicsErrorInitialize();
    helicsFederateInfoSetCoreType(fi, helics_core_type_test, &err);
    helicsFederateInfoSetCoreInitString(fi, "--autobroker", &err);
    helicsFederateInfoSetIntegerProperty(fi, helics_property_int_log_level, 5, &err);

    auto fed = helicsCreateValueFederate("test1", fi, &err);

    logblocktype mlog;

    auto logg = [](int level, const char*, const char* message, void* udata) {
        auto* mp = reinterpret_cast<logblocktype*>(udata);
        mp->lock()->emplace_back(level, message);
    };
    helicsFederateSetLoggingCallback(fed, nullptr, &mlog, &err);

    helicsFederateSetLoggingCallback(fed, logg, &mlog, &err);

    EXPECT_EQ(err.error_code, 0);

    helicsFederateEnterExecutingMode(fed, &err);
    helicsFederateLogLevelMessage(fed, 3, "test MEXAGE1", &err);
    helicsFederateLogLevelMessage(fed, 8, "test MEXAGE2", &err);
    helicsFederateRequestNextStep(fed, &err);
    helicsFederateFinalize(fed, &err);
    EXPECT_EQ(err.error_code, 0);

    auto llock = mlog.lock();
    bool found_low = false;
    bool found_high = false;
    for (auto& m : llock) {
        if (m.second.find("MEXAGE1") != std::string::npos) {
            found_low = true;
        }
        if (m.second.find("MEXAGE2") != std::string::npos) {
            found_high = true;
        }
    }
    EXPECT_TRUE(found_low);
    EXPECT_FALSE(found_high);

    helicsFederateFree(fed);
    helicsFederateInfoFree(fi);
}

TEST(logging_tests, check_log_message_levels_high)
{
    auto fi = helicsCreateFederateInfo();
    auto err = helicsErrorInitialize();
    helicsFederateInfoSetCoreType(fi, helics_core_type_test, &err);
    helicsFederateInfoSetCoreInitString(fi, "--autobroker", &err);
    helicsFederateInfoSetIntegerProperty(fi, helics_property_int_log_level, 9, &err);

    auto fed = helicsCreateValueFederate("test1", fi, &err);

    logblocktype mlog;

    auto logg = [](int level, const char*, const char* message, void* udata) {
        auto* mp = reinterpret_cast<logblocktype*>(udata);
        mp->lock()->emplace_back(level, message);
    };

    helicsFederateSetLoggingCallback(fed, logg, &mlog, &err);

    EXPECT_EQ(err.error_code, 0);

    helicsFederateEnterExecutingMode(fed, &err);
    helicsFederateLogLevelMessage(fed, 3, "test MEXAGE1", &err);
    helicsFederateLogLevelMessage(fed, 8, "test MEXAGE2", &err);
    helicsFederateRequestNextStep(fed, &err);
    helicsFederateFinalize(fed, &err);
    EXPECT_EQ(err.error_code, 0);

    auto llock = mlog.lock();
    bool found_low = false;
    bool found_high = false;
    for (auto& m : llock) {
        if (m.second.find("MEXAGE1") != std::string::npos) {
            found_low = true;
        }
        if (m.second.find("MEXAGE2") != std::string::npos) {
            found_high = true;
        }
    }
    EXPECT_TRUE(found_low && found_high);
    helicsFederateFree(fed);
    helicsFederateInfoFree(fi);
}

TEST(logging_tests, core_logging)
{
    auto core = helicsCreateCore("inproc", "ctype", "--autobroker --log_level=trace", nullptr);

    helicsCoreSetLoggingCallback(core, nullptr, nullptr, nullptr);

    logblocktype mlog;

    auto logg = [](int level, const char*, const char* message, void* udata) {
        auto* mp = reinterpret_cast<logblocktype*>(udata);
        mp->lock()->emplace_back(level, message);
    };

    helicsCoreSetLoggingCallback(core, logg, &mlog, nullptr);
    helicsCoreDisconnect(core, nullptr);
    helicsCleanupLibrary();
    EXPECT_FALSE(mlog.lock()->empty());
}

TEST(logging_tests, broker_logging)
{
    auto broker = helicsCreateBroker("inproc", "btype", "--log_level=trace", nullptr);

    helicsBrokerSetLoggingCallback(broker, nullptr, nullptr, nullptr);

    logblocktype mlog;

    auto logg = [](int level, const char*, const char* message, void* udata) {
        auto* mp = reinterpret_cast<logblocktype*>(udata);
        mp->lock()->emplace_back(level, message);
    };

    helicsBrokerSetLoggingCallback(broker, logg, &mlog, nullptr);
    helicsBrokerDisconnect(broker, nullptr);
    helicsCleanupLibrary();
    EXPECT_FALSE(mlog.lock()->empty());
}
