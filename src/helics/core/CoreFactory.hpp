/*
Copyright © 2017-2018,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC
All rights reserved. See LICENSE file and DISCLAIMER for more details.
*/
#pragma once

#include "core-types.hpp"
#include <memory>
#include <string>

namespace helics
{
class Core;
class CommonCore;
/**
 * Factory for building Core API instances.
 */

/** available core types*/

namespace CoreFactory
{
/**
 * Creates a Core API object of the specified type.
 *
 * Invokes initialize() on the instantiated Core object.
 */
std::shared_ptr<Core> create (core_type type, const std::string &initializationString);

/** create a core from a type and command line arguments
@param type the type of core to create
@param argc the number of command line arguments
@param argv the actual string parameters for the command line
*/
std::shared_ptr<Core> create (core_type type, int argc, const char *const *argv);

/** create a core from arguments
@details an argument of '--coretype' must be specified to define the type,  otherwise the default type is used
@param argc the number of arguments
@param argv the actual argument parameters
@return a pointer to the created core
*/
std::shared_ptr<Core> create(int argc, const char *const *argv);

/** create a core from a type, name, and initializationString
@param type the type of core to create
@param core_name the name for the core
@param initializationString a string containing arguments for the core
*/
std::shared_ptr<Core> create (core_type type, const std::string &core_name, std::string &initializationString);

/** create a core from a type, name, and arguments
@param type the type of core to create
@param core_name the name for the core
@param argc the number of arguments
@param argv the actual argument parameters
@return a pointer to the created core
*/
std::shared_ptr<Core> create (core_type type, const std::string &core_name, int argc, const char *const *argv);
/** tries to find a named core if it fails it creates a new one
 */
std::shared_ptr<Core>
FindOrCreate (core_type type, const std::string &coreName, const std::string &initializationString);

/** tries to find a named core if it fails it creates a new one
 */
std::shared_ptr<Core>
FindOrCreate (core_type type, const std::string &coreName, int argc, const char *const *argv);
/** try to find a joinable core of a specific type*/
std::shared_ptr<Core> findJoinableCoreOfType (core_type type);

/** locate a registered Core by name
@param name the name of the core to find
@return a shared_ptr to the testCore*/
std::shared_ptr<Core> findCore (const std::string &name);

/** register a testCore so it can be found by others
@details also cleans up any leftover bCoresrokers that were previously unregistered this can be controlled by
calling cleanUpBrokers earlier if desired
@param core a pointer to a testCore object that should be found globally
@return true if the registration was successful false otherwise*/
bool registerCore (std::shared_ptr<Core> core);

/** remove a Core from the registry
@param name the name of the Core to unregister
*/
void unregisterCore (const std::string &name);
/** clean up unused cores
@details when Cores are unregistered they get put in a holding area that gets cleaned up when a new Core is
registered or when the clean up function is called this prevents some odd threading issues
@return the number of cores still operating
*/
size_t cleanUpCores ();
/** clean up unused cores
@details when Cores are unregistered they get put in a holding area that gets cleaned up when a new Core is
registered or when the clean up function is called this prevents some odd threading issues
@param delay the delay time in milliseconds to wait for the cores to finish before destroying
@return the number of cores still operating
*/
size_t cleanUpCores (int delay);

/** make a copy of the broker pointer to allow access to the new name
 */
void copyCoreIdentifier (const std::string &copyFromName, const std::string &copyToName);
}  // namespace CoreFactory

}  // namespace helics

