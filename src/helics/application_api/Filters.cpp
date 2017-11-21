/*

Copyright (C) 2017, Battelle Memorial Institute
All rights reserved.

This software was co-developed by Pacific Northwest National Laboratory, operated by the Battelle Memorial
Institute; the National Renewable Energy Laboratory, operated by the Alliance for Sustainable Energy, LLC; and the
Lawrence Livermore National Laboratory, operated by Lawrence Livermore National Security, LLC.

*/

#include "Filters.hpp"
#include "MessageOperators.h"

#include <map>
#include <memory>
#include <random>
#include <thread>

namespace helics
{
static void addOperations (Filter *filt, defined_filter_types type)
{
    switch (type)
    {
    case custom:
        break;
    case randomDelay:
    {
        auto op = std::make_shared<randomDelayFilterOperation> ();
        filt->setFilterOperations (std::move (op));
    }
    break;
    case delay:
    {
        auto op = std::make_shared<delayFilterOperation> ();
        filt->setFilterOperations (std::move (op));
    }
    break;
    case randomDrop:
    {
        auto op = std::make_shared<randomDropFilterOperation> ();
        filt->setFilterOperations (std::move (op));
    }
    case reroute:
    {
        auto op = std::make_shared<randomDropFilterOperation> ();
        filt->setFilterOperations (std::move (op));
    }
    break;
    case redirect:
    {
    }
    break;
    }
}

std::unique_ptr<DestinationFilter> make_destination_filter (defined_filter_types type,
                                                            MessageFilterFederate *mFed,
                                                            const std::string &target,
                                                            const std::string &name)

{
    auto dfilt = std::make_unique<DestinationFilter> (mFed, target, name);
    addOperations (dfilt.get (), type);
    return dfilt;
}

std::unique_ptr<SourceFilter> make_Source_filter (defined_filter_types type,
                                                  MessageFilterFederate *mFed,
                                                  const std::string &target,
                                                  const std::string &name)
{
    auto sfilt = std::make_unique<SourceFilter> (mFed, target, name);
    addOperations (sfilt.get (), type);
    return sfilt;
}

void FilterOperations::set (const std::string & /*property*/, double /*val*/) {}
void FilterOperations::setString (const std::string & /*property*/, const std::string & /*val*/) {}

delayFilterOperation::delayFilterOperation (Time delayTime) : delay (delayTime)
{
    if (delayTime < timeZero)
    {
        delay = timeZero;
    }
    td = std::make_shared<MessageTimeOperator> ([this](Time messageTime) { return messageTime + delay; });
}

void delayFilterOperation::set (const std::string &property, double val)
{
    if (property == "delay")
    {
        if (val >= timeZero)
        {
            delay = Time (val);
        }
    }
}

std::shared_ptr<MessageOperator> delayFilterOperation::getOperator ()
{
    return std::static_pointer_cast<MessageOperator> (td);
}

// enumeration of possible random number generator distributions
enum class random_dists_t : int
{
    constant,
    uniform,
    bernoulli,
    binomial,
    geometric,
    poisson,
    exponential,
    gamma,
    weibull,
    extreme_value,
    normal,
    lognormal,
    chi_squared,
    cauchy,
    fisher_f,
    student_t
};

static const std::map<std::string, random_dists_t> distMap{
  {"constant", constant},           {"uniform", uniform},     {"bernoulli", bernoulli},
  {"binomial", binomial},           {"geometric", geometric}, {"poisson", poisson},
  {"exponential", exponential},     {"gamma", gamma},         {"weibull", weibull},
  {"extreme_value", extreme_value}, {"normal", normal},       {"lognormal", lognormal},
  {"chi_squared", chi_squared},     {"cauchy", cauchy},       {"fisher_f", fisher_f},
  {"student_t", student_t}};

double randDouble (random_dists_t dist, double p1, double p2)
{
#ifndef __apple_build_version__
    static thread_local std::mt19937 generator (
      std::random_device{}() +
      static_cast<unsigned int> (std::hash<std::thread::id>{}(std::this_thread::get_id ())));
#else
#if __clang_major__ >= 8
    static thread_local std::mt19937 generator (
      std::random_device{}() +
      static_cast<unsigned int> (std::hash<std::thread::id>{}(std::this_thread::get_id ())));
#else
    // this will leak on thread termination,  older apple clang does not have proper thread_local variables so
    // there really isn't any option
    //  static __thread std::mt19937 *genPtr =
    //    new std::mt19937(std::random_device{}() +
    //        static_cast<unsigned int> (std::hash<std::thread::id>{}(std::this_thread::get_id())));

    static __thread std::mt19937 *genPtr = nullptr;
    if (genPtr == nullptr)
    {
        genPtr =
          new std::mt19937 (std::random_device{}() +
                            static_cast<unsigned int> (std::hash<std::thread::id>{}(std::this_thread::get_id ())));
    }


    auto &generator = *genPtr;

#endif
#endif
    switch (dist)
    {
    case random_dists_t::constant:
        return p1;
    case random_dists_t::uniform:
    {
        std::uniform_real_distribution<double> distribution (p1, p2);
        return distribution (generator);
    }
    case random_dists_t::normal:
    {
        std::normal_distribution<double> distribution (p1, p2);
        return distribution (generator);
    }
    case random_dists_t::lognormal:
    {
        std::lognormal_distribution<double> distribution (p1, p2);
        return distribution (generator);
    }
    case random_dists_t::cauchy:
    {
        std::cauchy_distribution<double> distribution (p1, p2);
        return distribution (generator);
    }
    case random_dists_t::chi_squared:
    {
        std::chi_squared_distribution<double> distribution (p1);
        return distribution (generator);
    }
    case random_dists_t::exponential:
    {
        std::exponential_distribution<double> distribution (p1);
        return distribution (generator);
    }
    case random_dists_t::extreme_value:
    {
        std::extreme_value_distribution<double> distribution (p1, p2);
        return distribution (generator);
    }
    case random_dists_t::fisher_f:
    {
        std::fisher_f_distribution<double> distribution (p1, p2);
        return distribution (generator);
    }
    case random_dists_t::weibull:
    {
        std::weibull_distribution<double> distribution (p1, p2);
        return distribution (generator);
    }
    case random_dists_t::student_t:
    {
        std::student_t_distribution<double> distribution (p1);
        return distribution (generator);
    }
    case random_dists_t::geometric:
    {  // integer multiples of some period
        std::geometric_distribution<int> distribution (p1);
        return distribution (generator) * p2;
    }
    case random_dists_t::poisson:
    {  // integer multiples of some period
        std::poisson_distribution<int> distribution (p1);
        return distribution (generator) * p2;
    }
    case random_dists_t::bernoulli:
    {
        std::bernoulli_distribution distribution (p1);
        return distribution (generator) ? p2 : 0.0;
    }
    case random_dists_t::binomial:
    {
        std::binomial_distribution<int> distribution (static_cast<int> (p1), p2);
        return static_cast<double> (distribution (generator));
    }
    case random_dists_t::gamma:
    {
        std::gamma_distribution<double> distribution (p1, p2);
        return distribution (generator);
    }
    break;
    }

    // return 0.0;
}

/** class wrapping the distribution generation functions and parameters*/
class randomDelayGenerator
{
  public:
    std::atomic<random_dists_t> dist;  //!< the distribution
    std::atomic<double> param1{0.0};  //!< parameter 1 typically mean or min
    std::atomic<double> param2{0.0};  //!< parameter 2 typically stddev or max

    double generate () { return randDouble (dist.load (), param1.load (), param2.load ()); }
};

randomDelayFilterOperation::randomDelayFilterOperation ()
{
    rdelayGen = std::make_unique<randomDelayGenerator> ();
    td = std::make_shared<MessageTimeOperator> (
      [this](Time messageTime) { return messageTime + rdelayGen->generate (); });
}
randomDelayFilterOperation::~randomDelayFilterOperation () = default;

void randomDelayFilterOperation::set (const std::string &property, double val)
{
    if ((property == "param1") || (property == "mean") || (property == "min") || (property == "alpha"))
    {
        rdelayGen->param1.store (val);
    }
    else if ((property == "param2") || (property == "stddev") || (property == "max") || (property == "beta"))
    {
        rdelayGen->param2.store (val);
    }
}
void randomDelayFilterOperation::setString (const std::string &property, const std::string &val)
{
    if ((property == "dist") || (property == "distribution"))
    {
        auto res = distMap.find (val);
        if (res != distMap.end ())
        {
            rdelayGen->dist.store (res->second);
        }
    }
}

std::shared_ptr<MessageOperator> randomDelayFilterOperation::getOperator ()
{
    return std::static_pointer_cast<MessageOperator> (td);
}

randomDropFilterOperation::randomDropFilterOperation ()
{
    tcond = std::make_shared<MessageConditionalOperator> (
      [this](const Message *) { return (randDouble (random_dists_t::bernoulli, dropProb, 1.0) > 0.1); });
}

randomDropFilterOperation::~randomDropFilterOperation () = default;
void randomDropFilterOperation::set (const std::string &property, double val)
{
    if ((property == "dropprob") || (property == "prob"))
    {
        dropProb = val;
    }
}
void randomDropFilterOperation::setString (const std::string &property, const std::string &val) {}

std::shared_ptr<MessageOperator> randomDropFilterOperation::getOperator ()
{
    return std::static_pointer_cast<MessageOperator> (tcond);
}

rerouteFilterOperation::rerouteFilterOperation ()
{
    op = std::make_shared<MessageDestOperator> ([this](const std::string &dest) { return newTarget.load (); });
}
rerouteFilterOperation::~rerouteFilterOperation () = default;

void rerouteFilterOperation::set (const std::string &property, double val) {}

void rerouteFilterOperation::setString (const std::string &property, const std::string &val)
{
    if (property == "target")
    {
        newTarget = val;
    }
}

std::shared_ptr<MessageOperator> rerouteFilterOperation::getOperator ()
{
    return std::static_pointer_cast<MessageOperator> (op);
}

}  // namespace helics