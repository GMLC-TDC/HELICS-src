/*
Copyright (c) 2017-2019,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See
the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

#include "helics/application_api/Endpoints.hpp"
#include "helics/application_api/MessageFederate.hpp"
#include "helics/core/ActionMessage.hpp"
#include "helics/helics-config.h"
#include <random>

using namespace helics;
/** class implementing a federate for the PHOLD benchmark*/
class PholdFederate
{
  public:
    helics::Time deltaTime = helics::Time (10, time_units::ns);  // sampling rate
    helics::Time finalTime = helics::Time (10000, time_units::ns);  // final time
    int evCount = 0; // number of events handled by this federate

  private:
    std::unique_ptr<helics::MessageFederate> mFed;
    helics::Endpoint* ept;

    int index_ = 0;
    int maxIndex_ = 0;
    int initEvCount_ = 16; // starting number of events
    double localProbability_ = .1; // probability of local events
    double randTimeMean_ = deltaTime * 2; // mean for the exponential distribution used when picking event times

    // classes related to the exponential and uniform distribution random number generator
    bool generateRandomSeed = true; 
    unsigned int seed = 0;
    std::mt19937 rand_gen;
    std::exponential_distribution<double> rand_exp;
    std::uniform_real_distribution<double> rand_uniform;

    bool initialized{false};
    bool readyToRun{false};

  public:
    PholdFederate () = default;

    void setGenerateRandomSeed (bool b) { generateRandomSeed = b;  };
    void setRandomSeed (unsigned int s) { seed = s; };

    void run (std::function<void ()> callOnReady = nullptr)
    {
        makeReady ();
        if (callOnReady)
        {
            callOnReady ();
        }
        mainLoop ();
    };

    void initialize (const std::string &coreName, int index, int maxIndex)
    {
        std::string name = "phold_" + std::to_string (index);
        index_ = index;
        maxIndex_ = maxIndex;
        helics::FederateInfo fi;
        fi.coreName = coreName;
        mFed = std::make_unique<helics::MessageFederate> (name, fi);
        ept = &mFed->registerEndpoint ("ept");

        // en.cppreference.com/w/cpp/numeric/random/exponential_distribution
        if (generateRandomSeed)
        {
            std::random_device rd;
            rand_gen.seed(rd());
        }
        else
        {
            rand_gen.seed(seed);
        }
        rand_exp = std::exponential_distribution<double> (1.0/randTimeMean_);
        rand_uniform = std::uniform_real_distribution<double> (0.0, 1.0);
        initialized = true;
    }

    void makeReady ()
    {
        if (!initialized)
        {
            throw ("must initialize first");
        }
        mFed->enterExecutingMode ();

        // create initial events (not included in benchmark time)
        for (int i = 0; i < initEvCount_; i++)
        {
            createNewEvent ();
        }

        readyToRun = true;
    }

    void createNewEvent ()
    {
        // decide if the event is local or remote
        auto destIndex = index_;
        if (rand_uniform(rand_gen) > localProbability_)
        {
            destIndex = rand() % maxIndex_;
        }

        // set the event time to current time + lookahead + rand exponential (mean >= lookahead or ~2x lookahead)
        helics::Time evTime = mFed->getCurrentTime() + deltaTime + helics::Time(rand_exp(rand_gen));
        std::string data = "ev";
        std::string dest = "phold_" + std::to_string(destIndex) + "/ept";
        ept->send(dest, data, evTime);
    }

    void mainLoop ()
    {
        auto nextTime = deltaTime;

        while (nextTime < finalTime)
        {
            nextTime = mFed->requestTime (finalTime);
            // for each event message received, create a new event
            while (ept->hasMessage ())
            {
                auto m = ept->getMessage ();
                evCount++;
                createNewEvent();
            }

        }
        mFed->finalize ();
    }
};
