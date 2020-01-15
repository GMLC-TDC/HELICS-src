/*
Copyright (c) 2017-2019,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See
the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "../core/Core.hpp"
#include "Federate.hpp"
#include "helics/helics_enums.h"

#include <memory>
#include <string>

namespace helics {
class FilterOperations;
class FilterOperator;
class CoreApp;
/** a set of common defined filters*/
enum class filter_types {
    custom = helics_filter_type_custom,
    delay = helics_filter_type_delay,
    random_delay = helics_filter_type_random_delay,
    random_drop = helics_filter_type_random_drop,
    reroute = helics_filter_type_reroute,
    clone = helics_filter_type_clone,
    firewall = helics_filter_type_firewall,
    unrecognized = 7

};

#define EMPTY_STRING std::string()

/** get the filter type from a string*/
HELICS_CXX_EXPORT filter_types filterTypeFromString(const std::string& filterType) noexcept;

/** class for managing a particular filter*/
class HELICS_CXX_EXPORT Filter {
  protected:
    Core* corePtr = nullptr; //!< the Core to use
    Federate* fed = nullptr; //!< pointer to fed
    interface_handle handle; //!< the handle as generated by the Federate
    bool cloning = false;
    bool disableAssign = false; //!< disable assignment for the object
    std::string name; //!< The name of the filter
  private:
    std::shared_ptr<FilterOperations>
        filtOp; //!< a class running any specific operation of the Filter
  public:
    /** default constructor*/
    Filter() = default;
    /** construct through a federate*/
    explicit Filter(Federate* ffed, const std::string& filtName = EMPTY_STRING);
    /** construct from handle*/
    Filter(Federate* ffed, const std::string& filtName, interface_handle ihandle);
    /** construct through a federate*/
    Filter(
        interface_visibility locality,
        Federate* ffed,
        const std::string& filtName = EMPTY_STRING);
    /** construct through a core object*/
    explicit Filter(Core* cr, const std::string& filtName = EMPTY_STRING);
    /** virtual destructor*/
    virtual ~Filter() = default;

    Filter(Filter&& filt) = default;
    /** copy the filter, a copied filter will point to the same object*/
    Filter(const Filter& filt) = default;
    Filter& operator=(Filter&& filt) = default;
    /** copy the filter, a copied filter will point to the same object as the original*/
    Filter& operator=(const Filter& filt) = default;
    /** check if the Filter links to a valid filter*/
    bool isValid() const { return handle.isValid(); }
    /** check if the filter is a cloning filter*/
    bool isCloningFilter() const { return cloning; }
    /** set a message operator to process the message*/
    void setOperator(std::shared_ptr<FilterOperator> mo);

    /** get the underlying core handle for use with a core*/
    interface_handle getHandle() const { return handle; }
    /** implicit conversion operator for extracting the handle*/
    operator interface_handle() const { return handle; }
    /** get the name for the filter*/
    const std::string& getName() const { return name; }
    /** get the full global key for the filter*/
    const std::string& getKey() const;
    /** get the specified input type of the filter*/
    const std::string& getInjectionType() const;
    /** get the specified output type of the filter*/
    const std::string& getExtractionType() const;

    /** get the interface information field of the publication*/
    const std::string& getInfo() const;
    /** set the interface information field of the publication*/
    void setInfo(const std::string& info);

    /** set a property on a filter
    @param property the name of the property of the filter to change
    @param val the numerical value of the property
    */
    virtual void set(const std::string& property, double val);

    /** set a string property on a filter
    @param property the name of the property of the filter to change
    @param val the numerical value of the property
    */
    virtual void setString(const std::string& property, const std::string& val);
    /** add a sourceEndpoint to the list of endpoint to clone*/
    virtual void addSourceTarget(const std::string& sourceName);
    /** add a destination endpoint to the list of endpoints to clone*/
    virtual void addDestinationTarget(const std::string& destinationName);
    /** alias for addSourceTarget*/
    void addTarget(const std::string& target) { addSourceTarget(target); }

    /** remove a sourceEndpoint to the list of endpoint to clone*/
    virtual void removeTarget(const std::string& sourceName);
    /** set an interface option for a filter*/
    void setOption(int32_t option, bool value);
    /** close a filter during an active simulation
    @details it is not necessary to call this function unless you are continuing the simulation after the close*/
    void close();

    /** get the current value of a flag for the handle*/
    bool getOption(int32_t option) const;

  protected:
    /** set a filter operations object */
    void setFilterOperations(std::shared_ptr<FilterOperations> filterOps);
    /** add a defined operation to a filter*/
    friend void addOperations(Filter* filt, filter_types type, Core* cptr);
};

/** class used to clone message for delivery to other endpoints*/
class HELICS_CXX_EXPORT CloningFilter: public Filter {
  public:
    /** default constructor*/
    CloningFilter() = default;
    /** construct from a core object
     */
    explicit CloningFilter(Core* cr, const std::string& filtName = EMPTY_STRING);
    /** construct from a Federate
     */
    explicit CloningFilter(Federate* ffed, const std::string& filtName = EMPTY_STRING);
    /** construct from a Federate
     */
    CloningFilter(
        interface_visibility locality,
        Federate* ffed,
        const std::string& filtName = EMPTY_STRING);

    /** constructor used by FilterFederateManager*/
    CloningFilter(Federate* ffed, const std::string& filtName, interface_handle handle);
    /** move the filter to a new cloning filter*/
    CloningFilter(CloningFilter&& filt) = default;
    /** copy the filter, a copied filter will point to the same object*/
    CloningFilter(const CloningFilter& filt) = default;
    /** move assign the cloning filter*/
    CloningFilter& operator=(CloningFilter&& filt) = default;
    /** copy the filter, a copied filter will point to the same object as the original*/
    CloningFilter& operator=(const CloningFilter& filt) = default;
    /** destructor */
    ~CloningFilter() = default;
    /** add a delivery address this is the name of an endpoint to deliver the message to*/
    void addDeliveryEndpoint(const std::string& endpoint);

    /** remove a delivery address this is the name of an endpoint to deliver the message to*/
    void removeDeliveryEndpoint(const std::string& endpoint);

    virtual void setString(const std::string& property, const std::string& val) override;

  private:
    friend class FilterFederateManager;
};

/** create a  filter
@param type the type of filter to create
@param fed the federate to create the filter through
@param name the name of the filter (optional)
@return a unique pointer to a destination Filter object,  note destroying the object does not deactivate the filter
*/
HELICS_CXX_EXPORT Filter&
    make_filter(filter_types type, Federate* fed, const std::string& name = EMPTY_STRING);

/** create a  filter
@param locality the visibility of the filter global or local
@param type the type of filter to create
@param fed the federate to create the filter through
@param name the name of the filter (optional)
@return a unique pointer to a destination Filter object,  note destroying the object does not deactivate the filter
*/
HELICS_CXX_EXPORT Filter& make_filter(
    interface_visibility locality,
    filter_types type,
    Federate* fed,
    const std::string& name = EMPTY_STRING);

/** create a filter
@param type the type of filter to create
@param cr the core to create the filter through
@param name the name of the filter (optional)
@return a unique pointer to a source Filter object,  note destroying the object does not deactivate the filter
*/
HELICS_CXX_EXPORT std::unique_ptr<Filter>
    make_filter(filter_types type, Core* cr, const std::string& name = EMPTY_STRING);

/** create a filter
@param type the type of filter to create
@param cr the core to create the filter through
@param name the name of the filter (optional)
@return a unique pointer to a source Filter object,  note destroying the object does not deactivate the filter
*/
HELICS_CXX_EXPORT std::unique_ptr<Filter>
    make_filter(filter_types type, CoreApp& cr, const std::string& name = EMPTY_STRING);

/** create a  filter
@param type the type of filter to create
@param fed the federate to create the filter through
@param delivery the endpoint to deliver the cloned message to
@param name the name of the filter (optional)
@return a unique pointer to a destination Filter object,  note destroying the object does not deactivate the filter
*/
HELICS_CXX_EXPORT CloningFilter& make_cloning_filter(
    filter_types type,
    Federate* fed,
    const std::string& delivery,
    const std::string& name = EMPTY_STRING);

/** create a cloning filter with a specified visibility
@param locality can be global or local
@param type the type of filter to create
@param fed the federate to create the filter through
@param delivery the endpoint to deliver the cloned message to
@param name the name of the filter (optional)
@return a unique pointer to a destination Filter object,  note destroying the object does not deactivate the filter
*/
HELICS_CXX_EXPORT CloningFilter& make_cloning_filter(
    interface_visibility locality,
    filter_types type,
    Federate* fed,
    const std::string& delivery,
    const std::string& name = EMPTY_STRING);

/** create a cloning filter with a delivery location
@param type the type of filter to create
@param cr the core to create the filter through
@param delivery the endpoint to deliver the cloned message to
@param name the name of the filter (optional)
@return a unique pointer to a source Filter object,  note destroying the object does not deactivate the filter
*/
HELICS_CXX_EXPORT std::unique_ptr<CloningFilter> make_cloning_filter(
    filter_types type,
    Core* cr,
    const std::string& delivery,
    const std::string& name = EMPTY_STRING);

/** create a cloning filter with a delivery location
@param type the type of filter to create
@param cr the core to create the filter through
@param delivery the endpoint to deliver the cloned message to
@param name the name of the filter (optional)
@return a unique pointer to a source Filter object,  note destroying the object does not deactivate the filter
*/
HELICS_CXX_EXPORT std::unique_ptr<CloningFilter> make_cloning_filter(
    filter_types type,
    CoreApp& cr,
    const std::string& delivery,
    const std::string& name = EMPTY_STRING);

} // namespace helics
