/*
Copyright (c) 2017-2020,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See
the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

#include "BenchmarkFederate.hpp"
#include "helics/application_api/Endpoints.hpp"
#include "helics/core/ActionMessage.hpp"

using helics::operator"" _t;
/** class implementing the hub for an echo test*/
class EchoMessageHub : public BenchmarkFederate {
  private:
    helics::Endpoint ept;

  public:
    EchoMessageHub() : BenchmarkFederate("echo message hub benchmark federate") {}

    void setupArgumentParsing() override
    {
        // default final time for this benchmark
        finalTime = helics::Time(100, time_units::ms);
    }

    std::string getName() override
    {
        return "echohub";
    }

    void doFedInit() override
    {
        ept = fed->registerGlobalEndpoint("echo");
    }

    void doMainLoop() override
    {
        auto cTime = 0.0_t;
        while (cTime <= finalTime) {
            while (ept.hasMessage()) {
                auto m = ept.getMessage();
                std::swap(m->source, m->dest);
                std::swap(m->original_source, m->original_dest);
                ept.send(std::move(m));
            }

            cTime = fed->requestTime(finalTime + 0.05);
        }
    }
};
