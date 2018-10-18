/*
Copyright © 2017-2018,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC
All rights reserved. See LICENSE file and DISCLAIMER for more details.
*/

#include "CoreBroker.hpp"
#include "../common/stringToCmdLine.h"
#include "BrokerFactory.hpp"

#include "../common/argParser.h"
#include "../common/fmt_format.h"
#include <boost/filesystem.hpp>

#include "../common/JsonProcessingFunctions.hpp"
#include "../common/logger.h"
#include "ForwardingTimeCoordinator.hpp"
#include "loggingHelper.hpp"
#include "queryHelpers.hpp"
#include <fstream>

namespace helics
{
using namespace std::string_literals;

static const ArgDescriptors extraArgs{
  {"root"s, ArgDescriptor::arg_type_t::flag_type, "specify whether the broker is a root"s},
};

void CoreBroker::displayHelp ()
{
    std::cout << "Broker Specific options:\n";
    variable_map vm;
    const char *const argV[] = {"", "--help"};
    argumentParser (2, argV, vm, extraArgs);
    BrokerBase::displayHelp ();
}

CoreBroker::~CoreBroker ()
{
    std::lock_guard<std::mutex> lock (name_mutex_);
    // make sure everything is synchronized
}

void CoreBroker::setIdentifier (const std::string &name)
{
    if (brokerState <= broker_state_t::connecting)  // can't be changed after initialization
    {
        std::lock_guard<std::mutex> lock (name_mutex_);
        identifier = name;
    }
}

const std::string &CoreBroker::getAddress () const
{
    if ((brokerState != broker_state_t::connected) || (address.empty ()))
    {
        address = generateLocalAddressString ();
    }
    return address;
}

route_id_t CoreBroker::getRoute (global_federate_id_t fedid) const
{
    if ((fedid == parent_broker_id) || (fedid == higher_broker_id))
    {
        return parent_route_id;
    }
    auto fnd = routing_table.find (fedid);
    return (fnd != routing_table.end ()) ? fnd->second :parent_route_id;  // zero is the default route
}

BasicBrokerInfo *CoreBroker::getBrokerById (global_broker_id_t brokerid)
{
    if (isRootc)
    {
        auto brkNum = brokerid.localIndex ();
        return (isValidIndex (brkNum, _brokers) ? &_brokers[brkNum] : nullptr);
    }

    auto fnd = _brokers.find (brokerid);
    return (fnd != _brokers.end ()) ? &(*fnd) : nullptr;
}

const BasicBrokerInfo *CoreBroker::getBrokerById (global_broker_id_t brokerid) const
{
    if (isRootc)
    {
        auto brkNum = brokerid.localIndex ();
        return (isValidIndex (brkNum, _brokers)) ? &_brokers[brkNum] : nullptr;
    }

    auto fnd = _brokers.find (brokerid);
    return (fnd != _brokers.end ()) ? &(*fnd) : nullptr;
}

void CoreBroker::setLoggingCallback (
  const std::function<void(int, const std::string &, const std::string &)> &logFunction)
{
	ActionMessage loggerUpdate(CMD_BROKER_CONFIGURE);
	loggerUpdate.messageID = UPDATE_LOGGING_CALLBACK;
	loggerUpdate.source_id = global_broker_id.load();
	if (logFunction)
	{
		auto ii = getNextAirlockIndex();
		dataAirlocks[ii].load(std::move(logFunction));
		loggerUpdate.counter = ii;
	}
	else
	{
		setActionFlag(loggerUpdate, empty_flag);
	}

	actionQueue.push(loggerUpdate);
}

uint16_t CoreBroker::getNextAirlockIndex()
{
	uint16_t index = nextAirLock++;
	if (index > 2)
	{  // this is an atomic operation if the nextAirLock was not adjusted this could result in an out of bounds
	   // exception if this check were not done
		index %= 2;
	}
	if (index == 2)
	{
		decltype (index) exp = 3;

		while (exp > 2)
		{  // doing a lock free modulus we need to make sure the nextAirLock<4
			if (nextAirLock.compare_exchange_weak(exp, exp % 3))
			{
				break;
			}
		}
	}
	return index;
}

void CoreBroker::dataLink (const std::string &publication, const std::string &input)
{
    ActionMessage M (CMD_DATA_LINK);
    M.name = publication;
    M.setStringData (input);
    addActionMessage (std::move (M));
}

void CoreBroker::addSourceFilterToEndpoint (const std::string &filter, const std::string &endpoint)
{
    ActionMessage M (CMD_FILTER_LINK);
    M.name = filter;
    M.setStringData (endpoint);
    addActionMessage (std::move (M));
}

void CoreBroker::addDestinationFilterToEndpoint (const std::string &filter, const std::string &endpoint)
{
    ActionMessage M (CMD_FILTER_LINK);
    M.name = filter;
    M.setStringData (endpoint);
    setActionFlag (M, destination_target);
    addActionMessage (std::move (M));
}

route_id_t CoreBroker::fillMessageRouteInformation (ActionMessage &mess)
{
    auto &endpointName = mess.getString (targetStringLoc);
    auto eptInfo = handles.getEndpoint (endpointName);
    if (eptInfo != nullptr)
    {
        mess.setDestination (eptInfo->handle);
        return getRoute (eptInfo->handle.fed_id);
    }
    auto fnd2 = knownExternalEndpoints.find (endpointName);
    if (fnd2 != knownExternalEndpoints.end ())
    {
        return fnd2->second;
    }
    return parent_route_id;
}

bool CoreBroker::isOpenToNewFederates () const { return ((brokerState != created) && (brokerState < operating)); }

void CoreBroker::processPriorityCommand (ActionMessage &&command)
{
    // deal with a few types of message immediately
    LOG_TRACE (global_broker_id_local, getIdentifier (),
               fmt::format ("|| priority_cmd:{} from {}", prettyPrintString (command), command.source_id.baseValue()));
    switch (command.action ())
    {
	case CMD_BROKER_SETUP:
	{
		global_broker_id_local = global_broker_id.load();
		isRootc = _isRoot.load();
		timeCoord->source_id = global_broker_id_local;
		break;
	}
    case CMD_REG_FED:
    {
        if (brokerState != operating)
        {
            if (allInitReady ())
            {
                ActionMessage noInit (CMD_INIT_NOT_READY);
                noInit.source_id = global_broker_id_local;
                transmit (parent_route_id, noInit);
            }
        }
        else  // we are initialized already
        {
            ActionMessage badInit (CMD_FED_ACK);
            setActionFlag (badInit, error_flag);
            badInit.source_id = global_broker_id_local;
            badInit.messageID = 5;
            badInit.name = command.name;
            transmit (getRoute (global_federate_id_t (command.source_id)), badInit);
            return;
        }
        // this checks for duplicate federate names
        if (_federates.find (command.name) != _federates.end ())
        {
            ActionMessage badName (CMD_FED_ACK);
            setActionFlag (badName, error_flag);
            badName.source_id = global_broker_id_local;
            badName.messageID = 6;
            badName.name = command.name;
            transmit (getRoute (global_federate_id_t (command.source_id)), badName);
            return;
        }
        _federates.insert (command.name,
                           global_federate_id_t (
                             static_cast<global_federate_id_t::base_type> (_federates.size ())),
                           command.name);
        _federates.back ().route_id = getRoute (command.source_id);
        if (!isRootc)
        {
            if (global_broker_id_local.isValid ())
            {
                command.source_id = global_broker_id_local;
                transmit (parent_route_id, command);
            }
            else
            {
                // delay the response if we are not fully registered yet
                delayTransmitQueue.push (command);
            }
        }
        else
        {
            _federates.back ().global_id = global_federate_id_t (
              static_cast<global_federate_id_t::base_type> (_federates.size ()) - 1 + global_federate_id_shift);
            auto route_id = _federates.back ().route_id;
            auto global_id = _federates.back ().global_id;
            routing_table.emplace (global_id, route_id);
            // don't bother with the federate_table
            // transmit the response
            ActionMessage fedReply (CMD_FED_ACK);
            fedReply.source_id = global_broker_id_local;
            fedReply.dest_id = global_id;
            fedReply.name = command.name;
            transmit (route_id, fedReply);
            LOG_CONNECTIONS (global_broker_id_local, getIdentifier (),
                             fmt::format ("registering federate {}({}) on route {}", command.name, global_id.baseValue(),
                                          route_id.baseValue()));
        }
    }
    break;
    case CMD_REG_BROKER:
    {
        if (brokerState != operating)
        {
            if (allInitReady ())
            {
                // send an init not ready as we were ready now we are not
                ActionMessage noInit (CMD_INIT_NOT_READY);
                noInit.source_id = global_broker_id_local;
                transmit (parent_route_id, noInit);
            }
        }
        else  // we are initialized already
        {
            ActionMessage badInit (CMD_BROKER_ACK);
            setActionFlag (badInit, error_flag);
            badInit.source_id = global_broker_id_local;
            badInit.name = command.name;
            transmit (getRoute(command.source_id), badInit);
            return;
        }
        _brokers.insert (command.name,
                         global_broker_id_t (static_cast<global_broker_id_t::base_type> (_brokers.size ())),
                         command.name);
        if (!command.source_id.isValid())
        {
			//TODO:: this will need to be updated when we enable mesh routing
            _brokers.back ().route_id = route_id_t(static_cast<route_id_t::base_type>(_brokers.size ()));
            addRoute (_brokers.back ().route_id, command.getString (targetStringLoc));
        }
        else
        {
            _brokers.back ().route_id = getRoute (command.source_id);
            _brokers.back ()._nonLocal = true;
        }
        _brokers.back ()._core = checkActionFlag (command, core_flag);
        if (!isRootc)
        {
            if (global_broker_id_local.isValid())
            {
                command.source_id = global_broker_id_local;
                transmit (parent_route_id, command);
            }
            else
            {
                // delay the response if we are not fully registered yet
                delayTransmitQueue.push (command);
            }
        }
        else
        {
            _brokers.back ().global_id = global_broker_id_t (
              static_cast<global_broker_id_t::base_type> (_brokers.size ()) - 1 + global_broker_id_shift);
            auto global_id = _brokers.back ().global_id;
            auto route_id = _brokers.back ().route_id;
            routing_table.emplace (global_id, route_id);
            // don't bother with the broker_table for root broker

            // sending the response message
            ActionMessage brokerReply (CMD_BROKER_ACK);
            brokerReply.source_id = global_broker_id_local;  // source is global root
            brokerReply.dest_id = global_id;  // the new id
            brokerReply.name = command.name;  // the identifier of the broker
            transmit (route_id, brokerReply);
            LOG_CONNECTIONS (global_broker_id_local, getIdentifier (),
                             fmt::format ("registering broker {}({}) on route {}", command.name, global_id.baseValue(),
                                          route_id.baseValue()));
        }
    }
    break;
    case CMD_FED_ACK:
    {  // we can't be root if we got one of these
        auto fed = _federates.find (command.name);
        if (fed != _federates.end ())
        {
            fed->global_id = global_federate_id_t (command.dest_id);
            auto route = fed->route_id;
            _federates.addSearchTerm (global_federate_id_t (command.dest_id), fed->name);
            transmit (route, command);
            routing_table.emplace (fed->global_id, route);
        }
        else
        {
            // this means we haven't seen this federate before for some reason
            _federates.insert (command.name, global_federate_id_t (command.dest_id), command.name);
            _federates.back ().route_id = getRoute (command.source_id);
            _federates.back ().global_id = global_federate_id_t (command.dest_id);
            routing_table.emplace (fed->global_id, _federates.back ().route_id);
            // it also means we don't forward it
        }
    }
    break;
    case CMD_BROKER_ACK:
    {  // we can't be root if we got one of these
        if (command.name == identifier)
        {
            if (checkActionFlag (command, error_flag))
            {
                // generate an error message
				LOG_ERROR(global_broker_id_local, identifier, fmt::format("unable to register broker {}",command.payload));
                return;
            }

			global_broker_id_local = global_broker_id_t (command.dest_id);
            global_broker_id.store(global_broker_id_local);
            higher_broker_id = global_broker_id_t (command.source_id);
            timeCoord->source_id = global_federate_id_t (global_broker_id_local);
            transmitDelayedMessages ();

            return;
        }
        auto broker = _brokers.find (command.name);
        if (broker != _brokers.end ())
        {
            broker->global_id = global_broker_id_t (command.dest_id);
            auto route = broker->route_id;
            _brokers.addSearchTerm (global_broker_id_t (command.dest_id), broker->name);
            routing_table.emplace (broker->global_id, route);
            command.source_id =
              global_broker_id_local;  // we want the intermediate broker to change the source_id
            transmit (route, command);
        }
        else
        {
            _brokers.insert (command.name, global_broker_id_t (command.dest_id), command.name);
            _brokers.back ().route_id = getRoute (command.source_id);
            _brokers.back ().global_id = global_broker_id_t (command.dest_id);
            routing_table.emplace (broker->global_id, _brokers.back ().route_id);
        }
    }
    break;
    case CMD_PRIORITY_DISCONNECT:
    {
        auto brk = getBrokerById (global_broker_id_t (command.source_id));
        if (brk != nullptr)
        {
            brk->_disconnected = true;
        }
        if (allDisconnected ())
        {
            if (!isRootc)
            {
                ActionMessage dis (CMD_PRIORITY_DISCONNECT);
                dis.source_id = global_broker_id_local;
                transmit (parent_route_id, dis);
            }
            addActionMessage (CMD_STOP);
        }
    }
    break;
    case CMD_REG_ROUTE:
        break;
    case CMD_BROKER_QUERY:
        if (command.dest_id == global_broker_id_local)
        {
            processLocalQuery (command);
        }
        else if ((isRootc) && (command.dest_id == parent_broker_id))
        {
            processLocalQuery (command);
        }
        else
        {
            routeMessage (command);
        }
        break;
    case CMD_QUERY:
        processQuery (command);
        break;
    case CMD_QUERY_REPLY:
        if (command.dest_id == global_broker_id_local)
        {
            processQueryResponse (command);
        }
        else
        {
            transmit (getRoute (command.dest_id), command);
        }
        break;
    default:
        // must not have been a priority command
        break;
    }
}

std::string CoreBroker::generateFederationSummary () const
{
    int pubs = 0;
    int epts = 0;
    int ipts = 0;
    int filt = 0;
    for (auto &hand : handles)
    {
        switch (hand.handle_type)
        {
        case handle_type_t::publication:
            ++pubs;
            break;
        case handle_type_t::input:
            ++ipts;
            break;
        case handle_type_t::endpoint:
            ++epts;
            break;
        default:
            ++filt;
            break;
        }
    }
    std::string output = fmt::format ("Federation Summary> \n\t{} federates\n\t{} brokers/cores\n\t{} "
                                      "publications\n\t{} inputs\n\t{} endpoints\n\t{} filters\n<<<<<<<<<",
                                      _federates.size (), _brokers.size (), pubs, ipts, epts, filt);
    return output;
}

void CoreBroker::transmitDelayedMessages ()
{
    auto msg = delayTransmitQueue.pop ();
    while (msg)
    {
        msg->source_id = global_broker_id_local;
        transmit (parent_route_id, *msg);
        msg = delayTransmitQueue.pop ();
    }
}

void CoreBroker::sendDisconnect ()
{
    ActionMessage bye (CMD_DISCONNECT);
    bye.source_id = global_broker_id_local;
    for (auto &brk : _brokers)
    {
        if (!brk._nonLocal)
        {
            routeMessage (bye, brk.global_id);
            brk._disconnected = true;
        }
        if (hasTimeDependency)
        {
            timeCoord->removeDependency (brk.global_id);
            timeCoord->removeDependent (brk.global_id);
        }
    }
    if (hasTimeDependency)
    {
        timeCoord->disconnect ();
    }
}

void CoreBroker::sendErrorToImmediateBrokers (int error_code)
{
    ActionMessage errorCom (CMD_ERROR);
    errorCom.messageID = error_code;
    for (auto &brk : _brokers)
    {
        if (!brk._nonLocal)
        {
            routeMessage (errorCom, brk.global_id);
        }
    }
}

void CoreBroker::processCommand (ActionMessage &&command)
{
    LOG_TRACE (global_broker_id_local, getIdentifier (),
               fmt::format ("|| cmd:{} from {} to ", prettyPrintString (command), command.source_id.baseValue(),
                            command.dest_id.baseValue()));
    switch (command.action ())
    {
    case CMD_IGNORE:
    case CMD_PROTOCOL:
        break;

    case CMD_TICK:
        if (!isRootc)
        {
            if (waitingForServerPingReply)
            {
                // try to reset the connection to the broker
                // brokerReconnect()
                LOG_ERROR (global_broker_id_local, getIdentifier (), "lost connection with server");
                sendErrorToImmediateBrokers (-5);
                disconnect ();
                brokerState = broker_state_t::errored;
                addActionMessage (CMD_STOP);
            }
            else
            {
                // if (allFedWaiting())
                //{
                ActionMessage png (CMD_PING);
                png.source_id = global_broker_id_local;
                png.dest_id = higher_broker_id;
                transmit (parent_route_id, png);
                waitingForServerPingReply = true;
                //}
            }
        }
        break;
    case CMD_PING:
        if (command.dest_id == global_broker_id_local)
        {
            ActionMessage pngrep (CMD_PING_REPLY);
            pngrep.dest_id = command.source_id;
            pngrep.source_id = global_broker_id_local;
            routeMessage (pngrep);
        }
        else
        {
            routeMessage (command);
        }
        break;
    case CMD_PING_REPLY:
        if (command.dest_id == global_broker_id_local)
        {
            waitingForServerPingReply = false;
        }
        else
        {
            routeMessage (command);
        }
        break;
    case CMD_INIT:
    {
        auto brk = getBrokerById (static_cast<global_broker_id_t>(command.source_id));
        if (brk != nullptr)
        {
            brk->_initRequested = true;
        }
        if (allInitReady ())
        {
            if (isRootc)
            {
                LOG_TIMING (global_broker_id_local, "root", "entering initialization mode");
                LOG_SUMMARY (global_broker_id_local, "root", generateFederationSummary ());
                executeInitializationOperations ();
            }
            else
            {
                LOG_TIMING (global_broker_id_local, getIdentifier (), "entering initialization mode");
                checkDependencies ();
                command.source_id = global_broker_id_local;
                transmit (parent_route_id, command);
            }
        }
    }
    break;
    case CMD_INIT_NOT_READY:
    {
        if (allInitReady ())
        {
            transmit (parent_route_id, command);
        }
        auto brk = getBrokerById (global_broker_id_t (command.source_id));
        if (brk != nullptr)
        {
            brk->_initRequested = false;
        }
    }
    break;
    case CMD_INIT_GRANT:
        brokerState = broker_state_t::operating;
        for (auto &brk : _brokers)
        {
            transmit (brk.route_id, command);
        }
        {
            timeCoord->enteringExecMode ();
            auto res = timeCoord->checkExecEntry ();
            if (res == message_processing_result::next_step)
            {
                enteredExecutionMode = true;
            }
        }
        break;
    case CMD_SEARCH_DEPENDENCY:
    {
        auto fed = _federates.find (command.name);
        if (fed != _federates.end ())
        {
            if (fed->global_id.isValid ())
            {
                ActionMessage dep (CMD_ADD_DEPENDENCY, fed->global_id, command.source_id);
                routeMessage (dep);
                dep = ActionMessage (CMD_ADD_DEPENDENT, command.source_id, fed->global_id);
                routeMessage (dep);
                break;
            }
        }
        if (isRootc)
        {
            delayedDependencies.emplace_back (command.name, command.source_id);
        }
        else
        {
            routeMessage (command);
        }
        break;
    }
    case CMD_DATA_LINK:
    {
        auto pub = handles.getPublication (command.name);
        if (pub != nullptr)
        {
            command.name = command.getString (targetStringLoc);
            command.setAction (CMD_ADD_NAMED_INPUT);
            command.setSource (pub->handle);
            checkForNamedInterface (command);
        }
        else
        {
            auto input = handles.getInput (command.getString (targetStringLoc));
            if (input == nullptr)
            {
                if (isRootc)
                {
                    unknownHandles.addDataLink (command.name, command.getString (targetStringLoc));
                }
                else
                {
                    routeMessage (command);
                }
            }
            else
            {
                command.setAction (CMD_ADD_NAMED_PUBLICATION);
                command.setSource (input->handle);
                checkForNamedInterface (command);
            }
        }
    }
    break;
    case CMD_FILTER_LINK:
    {
        auto filt = handles.getFilter (command.name);
        if (filt != nullptr)
        {
            command.name = command.getString (targetStringLoc);
            command.setAction (CMD_ADD_NAMED_ENDPOINT);
            command.setSource (filt->handle);
            checkForNamedInterface (command);
        }
        else
        {
            auto ept = handles.getEndpoint (command.getString (targetStringLoc));
            if (ept == nullptr)
            {
                if (isRootc)
                {
					if (checkActionFlag(command, destination_target))
					{
                        unknownHandles.addDestinationFilterLink (command.name,
                                                                 command.getString (targetStringLoc));
					}
					else
					{
                        unknownHandles.addSourceFilterLink (command.name,
                                                                 command.getString (targetStringLoc));
					}
                }
                else
                {
                    routeMessage (command);
                }
            }
            else
            {
                command.setAction (CMD_ADD_NAMED_FILTER);
                command.setSource (ept->handle);
                checkForNamedInterface (command);
            }
        }
    }
    break;
    case CMD_DISCONNECT_NAME:
        if (command.dest_id == parent_broker_id)
        {
            auto brk = _brokers.find (command.payload);
            if (brk != _brokers.end ())
            {
                command.source_id = brk->global_id;
            }
        }
        FALLTHROUGH
        /* FALLTHROUGH */
    case CMD_DISCONNECT:
    {
        if ((command.dest_id == parent_broker_id) || (command.dest_id == global_broker_id_local))
        {
            if (!isRootc)
            {
                if (command.source_id == higher_broker_id)
                {
                    sendDisconnect ();
                    addActionMessage (CMD_STOP);
                    return;
                }
            }

            auto brk = getBrokerById (global_broker_id_t (command.source_id));
            if (brk != nullptr)
            {
                brk->_disconnected = true;
            }
            if (hasTimeDependency)
            {
                if (!enteredExecutionMode)
                {
                    timeCoord->processTimeMessage (command);
                    auto res = timeCoord->checkExecEntry ();
                    if (res == message_processing_result::next_step)
                    {
                        enteredExecutionMode = true;
                    }
                }
                else
                {
                    if (timeCoord->processTimeMessage (command))
                    {
                        timeCoord->updateTimeFactors ();
                    }
                }
            }

            if (allDisconnected ())
            {
                timeCoord->disconnect ();
                if (!isRootc)
                {
                    ActionMessage dis (CMD_DISCONNECT);
                    dis.source_id = global_broker_id_local;
                    transmit (parent_route_id, dis);
                }
                addActionMessage (CMD_STOP);
            }
        }
        else
        {
            transmit (getRoute (command.dest_id), command);
        }
    }
    break;
    case CMD_USER_DISCONNECT:
        sendDisconnect ();
        addActionMessage (CMD_STOP);
        break;
    case CMD_STOP:
        if (!allDisconnected ())
        {  // only send a disconnect message if we haven't done so already
            timeCoord->disconnect ();
            if (!isRootc)
            {
                ActionMessage m (CMD_DISCONNECT);
                m.source_id = global_broker_id_local;
                transmit (parent_route_id, m);
            }
        }
        break;
    case CMD_EXEC_REQUEST:
    case CMD_EXEC_GRANT:
        if (command.dest_id == global_broker_id_local)
        {
            timeCoord->processTimeMessage (command);
            if (!enteredExecutionMode)
            {
                auto res = timeCoord->checkExecEntry ();
                if (res == message_processing_result::next_step)
                {
                    enteredExecutionMode = true;
                    LOG_TIMING (global_broker_id_local, getIdentifier (), "entering Exec Mode");
                }
            }
        }
        else if (command.source_id == global_broker_id_local)
        {
            for (auto dep : timeCoord->getDependents ())
            {
                routeMessage (command, dep);
            }
        }
        else
        {
            transmit (getRoute (global_federate_id_t (command.dest_id)), command);
        }

        break;
    case CMD_TIME_REQUEST:
    case CMD_TIME_GRANT:
        if ((command.source_id == global_broker_id_local) && (command.dest_id == parent_broker_id))
        {
            LOG_TIMING (global_broker_id_local, getIdentifier (),
                        fmt::format ("time request update {}", prettyPrintString (command)));
            for (auto dep : timeCoord->getDependents ())
            {
                routeMessage (command, dep);
            }
        }
        else if (command.dest_id == global_broker_id_local)
        {
            if (timeCoord->processTimeMessage (command))
            {
                timeCoord->updateTimeFactors ();
            }
        }
        else
        {
            routeMessage (command);
        }

        break;
    case CMD_SEND_MESSAGE:
    case CMD_SEND_FOR_FILTER:
    case CMD_SEND_FOR_FILTER_AND_RETURN:
    case CMD_FILTER_RESULT:
    case CMD_NULL_MESSAGE:
        if (command.dest_id == parent_broker_id)
        {
            auto route = fillMessageRouteInformation (command);
            transmit (route, command);
        }
        else
        {
            transmit (getRoute (command.dest_id), command);
        }
        break;
    case CMD_PUB:
        transmit (getRoute (command.dest_id), command);
        break;

    case CMD_LOG:
        if (isRootc)
        {
			sendToLogger(command.source_id, command.counter, std::string(), command.payload);
        }
        else
        {
            transmit (parent_route_id, command);
        }
        break;
    case CMD_ERROR:
        if (isRootc)
        {
			sendToLogger(global_federate_id_t(command.source_id),HELICS_LOG_LEVEL_ERROR, std::string(), command.payload);
        }
        else
        {
            transmit (parent_route_id, command);
        }
        break;
    case CMD_REG_PUB:
        if ((!isRootc) && (command.dest_id != parent_broker_id))
        {
            routeMessage (command);
            break;
        }
        addPublication (command);
        break;
    case CMD_REG_INPUT:
        if ((!isRootc) && (command.dest_id != parent_broker_id))
        {
            routeMessage (command);
            break;
        }
        addInput (command);
        break;
    case CMD_REG_ENDPOINT:
        if ((!isRootc) && (command.dest_id != parent_broker_id))
        {
            routeMessage (command);
            break;
        }
        addEndpoint (command);
        break;
    case CMD_REG_FILTER:
        if ((!isRootc) && (command.dest_id != parent_broker_id))
        {
            routeMessage (command);
            break;
        }
        addFilter (command);
        break;
    case CMD_ADD_DEPENDENCY:
    case CMD_REMOVE_DEPENDENCY:
    case CMD_ADD_DEPENDENT:
    case CMD_REMOVE_DEPENDENT:
    case CMD_ADD_INTERDEPENDENCY:
    case CMD_REMOVE_INTERDEPENDENCY:
        if (command.dest_id != global_broker_id_local)
        {
            routeMessage (command);
        }
        else
        {
            timeCoord->processDependencyUpdateMessage (command);
            if (!hasTimeDependency)
            {
                hasTimeDependency = true;
            }
        }
        break;
    case CMD_ADD_NAMED_ENDPOINT:
    case CMD_ADD_NAMED_PUBLICATION:
    case CMD_ADD_NAMED_INPUT:
    case CMD_ADD_NAMED_FILTER:
        checkForNamedInterface (command);
        break;
	case CMD_BROKER_CONFIGURE:
		processBrokerConfigureCommands(command);
		break;
    default:
        if (command.dest_id != global_broker_id_local)
        {
            routeMessage (command);
        }
    }
}

void CoreBroker::processBrokerConfigureCommands(ActionMessage &cmd)
{
	switch (cmd.messageID)
	{
	case ENABLE_INIT_ENTRY:
		/*if (delayInitCounter <= 1)
		{
			delayInitCounter = 0;
			if (allInitReady())
			{
				broker_state_t exp = connected;
				if (brokerState.compare_exchange_strong(exp, broker_state_t::initializing))
				{  // make sure we only do this once
					checkDependencies();
					cmd.setAction(CMD_INIT);
					cmd.source_id = global_broker_id_local;
					cmd.dest_id = 0;
					transmit(0, cmd);
				}
			}
		}
		else
		{
			--delayInitCounter;
		}
		break;
		*/
	case LOG_LEVEL_PROPERTY:
		setLogLevel(cmd.counter);
		break;
	case UPDATE_LOGGING_CALLBACK:
		if (checkActionFlag(cmd, empty_flag))
		{
			setLoggerFunction(nullptr);
		}
		else
		{
			auto op = dataAirlocks[cmd.counter].try_unload();
			if (op)
			{
				auto M = stx::any_cast<std::function<void(int, const std::string &, const std::string &)>> (
					std::move(*op));
				setLoggerFunction(std::move(M));
			}
		}
		break;
	default:
		break;
	}
}

void CoreBroker::checkForNamedInterface (ActionMessage &command)
{
    bool foundInterface = false;
    switch (command.action ())
    {
    case CMD_ADD_NAMED_PUBLICATION:
    {
        auto pub = handles.getPublication (command.name);
        if (pub != nullptr)
        {
            command.setAction (CMD_ADD_SUBSCRIBER);
            command.setDestination (pub->handle);
            command.name.clear ();
            routeMessage (command);
            command.setAction (CMD_ADD_PUBLISHER);
            command.swapSourceDest ();
            command.setStringData (pub->type, pub->units);
            routeMessage (command);
            foundInterface = true;
        }
    }
    break;
    case CMD_ADD_NAMED_INPUT:
    {
        auto inp = handles.getInput (command.name);
        if (inp != nullptr)
        {
            command.setAction (CMD_ADD_PUBLISHER);
            command.setDestination (inp->handle);
            auto pub = handles.findHandle (command.getSource ());
            if (pub != nullptr)
            {
                command.setStringData (pub->type, pub->units);
            }
            command.name.clear ();
            routeMessage (command);
            command.setAction (CMD_ADD_SUBSCRIBER);
            command.swapSourceDest ();
            command.clearStringData ();
            routeMessage (command);
            foundInterface = true;
        }
    }
    break;
    case CMD_ADD_NAMED_FILTER:
    {
        auto filt = handles.getFilter (command.name);
        if (filt != nullptr)
        {
            command.setAction (CMD_ADD_ENDPOINT);
            command.setDestination (filt->handle);
            command.name.clear ();
            routeMessage (command);
            command.setAction (CMD_ADD_FILTER);
            command.swapSourceDest ();
            if ((!filt->type_in.empty ()) || (!filt->type_out.empty ()))
            {
                command.setStringData (filt->type_in, filt->type_out);
            }
            routeMessage (command);
            foundInterface = true;
        }
    }
    break;
    case CMD_ADD_NAMED_ENDPOINT:
    {
        auto ept = handles.getEndpoint (command.name);
        if (ept != nullptr)
        {
            command.setAction (CMD_ADD_FILTER);
            command.setDestination (ept->handle);
            command.name.clear ();
            auto filt = handles.findHandle (command.getSource ());
            if (filt != nullptr)
            {
                if ((!filt->type_in.empty ()) || (!filt->type_out.empty ()))
                {
                    command.setStringData (filt->type_in, filt->type_out);
                }
            }
            routeMessage (command);
            command.setAction (CMD_ADD_ENDPOINT);
            command.swapSourceDest ();
            command.clearStringData ();
            routeMessage (command);
            foundInterface = true;
        }
    }
    break;
    default:
        break;
    }
    if (!foundInterface)
    {
        if (isRootc)
        {
            switch (command.action ())
            {
            case CMD_ADD_NAMED_PUBLICATION:
                unknownHandles.addUnknownPublication (command.name, command.getSource (), command.flags);
                break;
            case CMD_ADD_NAMED_INPUT:
                unknownHandles.addUnknownInput (command.name, command.getSource (), command.flags);
                if (!command.getStringData ().empty ())
                {
                    auto pub = handles.findHandle (command.getSource ());
                    if (pub == nullptr)
                    {
                        // an anonymous publisher is adding an input
                        auto &apub =
                          handles.addHandle (global_federate_id_t (command.source_id),
                                             interface_handle (command.source_handle), handle_type_t::publication,
                                             std::string (), command.getString (typeStringLoc),
                                             command.getString (unitStringLoc));

                        addLocalInfo (apub, command);
                    }
                }
                break;
            case CMD_ADD_NAMED_ENDPOINT:
                unknownHandles.addUnknownEndpoint (command.name, command.getSource (), command.flags);
                if (!command.getStringData ().empty ())
                {
                    auto filt = handles.findHandle (command.getSource ());
                    if (filt == nullptr)
                    {
                        // an anonymous filter is adding and endpoint
                        auto &afilt =
                          handles.addHandle (global_federate_id_t (command.source_id),
                                             interface_handle (command.source_handle), handle_type_t::filter,
                                             std::string (), command.getString (typeStringLoc),
                                             command.getString (typeOutStringLoc));

                        addLocalInfo (afilt, command);
                    }
                }
                break;
            case CMD_ADD_NAMED_FILTER:
                unknownHandles.addUnknownFilter (command.name, command.getSource (), command.flags);
                break;
            default:
                LOG_WARNING (global_broker_id_local, getIdentifier (),
                             "unknown command in interface addition code section\n");
                break;
            }
        }
        else
        {
            routeMessage (command);
        }
    }
}

void CoreBroker::addLocalInfo (BasicHandleInfo &handleInfo, const ActionMessage &m)
{
    auto res = global_id_translation.find (global_federate_id_t (m.source_id));
    if (res != global_id_translation.end ())
    {
        handleInfo.local_fed_id = res->second;
    }
    handleInfo.flags = m.flags;
}

void CoreBroker::addPublication (ActionMessage &m)
{
    // detect duplicate publications
    if (handles.getPublication (m.name) != nullptr)
    {
        ActionMessage eret (CMD_ERROR, global_broker_id_local, m.source_id);
        eret.dest_handle = m.source_handle;
        eret.messageID = ERROR_CODE_REGISTRATION_FAILURE;
        eret.payload = "Duplicate publication names (" + m.name + ")";
        routeMessage (eret);
        return;
    }
    auto &pub = handles.addHandle (global_federate_id_t (m.source_id), interface_handle (m.source_handle),
                                   handle_type_t::publication, m.name, m.getString (0), m.getString (1));

    addLocalInfo (pub, m);
    if (!isRootc)
    {
        transmit (parent_route_id, m);
    }
    else
    {
        FindandNotifyPublicationTargets (pub);
    }
}
void CoreBroker::addInput (ActionMessage &m)
{
    // detect duplicate publications
    if (handles.getInput (m.name) != nullptr)
    {
        ActionMessage eret (CMD_ERROR, global_broker_id_local, m.source_id);
        eret.dest_handle = m.source_handle;
        eret.messageID = ERROR_CODE_REGISTRATION_FAILURE;
        eret.payload = "Duplicate input names (" + m.name + ")";
        routeMessage (eret);
        return;
    }
    auto &inp = handles.addHandle (global_federate_id_t (m.source_id), interface_handle (m.source_handle),
                                   handle_type_t::input, m.name, m.getString (0), m.getString (1));

    addLocalInfo (inp, m);
    if (!isRootc)
    {
        transmit (parent_route_id, m);
    }
    else
    {
        FindandNotifyInputTargets (inp);
    }
}

void CoreBroker::addEndpoint (ActionMessage &m)
{
    // detect duplicate endpoints
    if (handles.getEndpoint (m.name) != nullptr)
    {
        ActionMessage eret (CMD_ERROR, global_broker_id_local, m.source_id);
        eret.dest_handle = m.source_handle;
        eret.messageID = ERROR_CODE_REGISTRATION_FAILURE;
        eret.payload = "Duplicate endpoint names (" + m.name + ")";
        routeMessage (eret);
        return;
    }
    auto &ept = handles.addHandle (global_federate_id_t (m.source_id), interface_handle (m.source_handle),
                                   handle_type_t::endpoint, m.name, m.getString (typeStringLoc),
                                   m.getString (unitStringLoc));

    addLocalInfo (ept, m);

    if (!isRootc)
    {
        transmit (parent_route_id, m);
        if (!hasTimeDependency)
        {
            if (timeCoord->addDependency (higher_broker_id))
            {
                hasTimeDependency = true;
                ActionMessage add (CMD_ADD_INTERDEPENDENCY, global_broker_id_local, higher_broker_id);
                transmit (parent_route_id, add);

                timeCoord->addDependent (higher_broker_id);
            }
        }
    }
    else
    {
        FindandNotifyEndpointTargets (ept);
    }
}
void CoreBroker::addFilter (ActionMessage &m)
{
    // detect duplicate endpoints
    if (handles.getFilter (m.name) != nullptr)
    {
        ActionMessage eret (CMD_ERROR, global_broker_id_local, m.source_id);
        eret.dest_handle = m.source_handle;
        eret.messageID = ERROR_CODE_REGISTRATION_FAILURE;
        eret.payload = "Duplicate filter names (" + m.name + ")";
        routeMessage (eret);
        return;
    }

    auto &filt = handles.addHandle (global_federate_id_t (m.source_id), interface_handle (m.source_handle),
                                    handle_type_t::filter, m.name, m.getString (typeStringLoc),
                                    m.getString (typeOutStringLoc));
    addLocalInfo (filt, m);

    if (!isRootc)
    {
        transmit (parent_route_id, m);
        if (!hasFilters)
        {
            hasFilters = true;
            if (timeCoord->addDependent (higher_broker_id))
            {
                hasTimeDependency = true;
                ActionMessage add (CMD_ADD_DEPENDENCY, global_broker_id_local, higher_broker_id);
                transmit (parent_route_id, add);
            }
        }
    }
    else
    {
        FindandNotifyFilterTargets (filt);
    }
}

CoreBroker::CoreBroker (bool setAsRootBroker) noexcept : _isRoot (setAsRootBroker),isRootc(setAsRootBroker) {}

CoreBroker::CoreBroker (const std::string &broker_name) : BrokerBase (broker_name) {}

void CoreBroker::initialize (const std::string &initializationString)
{
    if (brokerState == broker_state_t::created)
    {
        StringToCmdLine cmdline (initializationString);
        initializeFromArgs (cmdline.getArgCount (), cmdline.getArgV ());
    }
}

void CoreBroker::initializeFromArgs (int argc, const char *const *argv)
{
    broker_state_t exp = broker_state_t::created;
    if (brokerState.compare_exchange_strong (exp, broker_state_t::initialized))
    {
        variable_map vm;
        argumentParser (argc, argv, vm, extraArgs);
        // Initialize the brokerBase component
        initializeFromCmdArgs (argc, argv);

        if (vm.count ("root") > 0)
        {
            setAsRoot ();
        }
    }
}

void CoreBroker::setAsRoot ()
{
    if (brokerState < broker_state_t::connected)
    {
        _isRoot = true;
        global_broker_id = global_broker_id_t (1);
    }
}

bool CoreBroker::connect ()
{
    if (brokerState < broker_state_t::connected)
    {
        broker_state_t exp = broker_state_t::initialized;
        if (brokerState.compare_exchange_strong (exp, broker_state_t::connecting))
        {
            LOG_CONNECTIONS (parent_broker_id, getIdentifier (), "connecting");
            auto res = brokerConnect ();
            if (res)
            {
                LOG_CONNECTIONS (parent_broker_id, getIdentifier (),
                                 fmt::format ("||connected on {}", getAddress ()));
               
                disconnection.activate ();
                brokerState = broker_state_t::connected;
				ActionMessage setup(CMD_BROKER_SETUP);
				addActionMessage(setup);
				if (!_isRoot)
				{
					ActionMessage m(CMD_REG_BROKER);
                    m.source_id = global_federate_id_t ();
					m.name = getIdentifier();
					m.setStringData(getAddress());
					transmit(parent_route_id, m);
				}
            }
            else
            {
                brokerState = broker_state_t::initialized;
            }
            return res;
        }
        if (brokerState == broker_state_t::connecting)
        {
            while (brokerState == broker_state_t::connecting)
            {
                std::this_thread::sleep_for (std::chrono::milliseconds (20));
            }
        }
    }
    return isConnected ();
}

bool CoreBroker::isConnected () const { return ((brokerState == operating) || (brokerState == connected)); }

void CoreBroker::waitForDisconnect (int msToWait) const
{
    if (msToWait <= 0)
    {
        disconnection.wait ();
    }
    else
    {
        disconnection.wait_for (std::chrono::milliseconds (msToWait));
    }
}

void CoreBroker::processDisconnect (bool skipUnregister)
{
    LOG_CONNECTIONS (parent_broker_id, getIdentifier (), "||disconnecting");
    if (brokerState > broker_state_t::initialized)
    {
        brokerState = broker_state_t::terminating;
        brokerDisconnect ();
    }
    brokerState = broker_state_t::terminated;

    if (!skipUnregister)
    {
        unregister ();
    }
    disconnection.trigger ();
}

void CoreBroker::unregister ()
{
    /*We need to ensure that the destructor is not called immediately upon calling unregister
    otherwise this would be a mess and probably cause segmentation faults so we capture it in a local variable
    that will be destroyed on function exit
    */
    auto keepBrokerAlive = BrokerFactory::findBroker (identifier);
    if (keepBrokerAlive)
    {
        BrokerFactory::unregisterBroker (identifier);
    }
    if (!previous_local_broker_identifier.empty ())
    {
        auto keepBrokerAlive2 = BrokerFactory::findBroker (previous_local_broker_identifier);
        if (keepBrokerAlive2)
        {
            BrokerFactory::unregisterBroker (previous_local_broker_identifier);
        }
    }
}

void CoreBroker::disconnect ()
{
    ActionMessage udisconnect (CMD_USER_DISCONNECT);
    addActionMessage (udisconnect);
    waitForDisconnect ();
}

void CoreBroker::routeMessage (ActionMessage &cmd, global_federate_id_t dest)
{
    if (dest == global_federate_id_t ())
    {
        return;
    }
    cmd.dest_id = dest;
    if ((dest == parent_broker_id) || (dest == higher_broker_id))
    {
        transmit (parent_route_id, cmd);
    }
    else
    {
        auto route = getRoute (dest);
        transmit (route, cmd);
    }
}

void CoreBroker::routeMessage (const ActionMessage &cmd)
{
    if ((cmd.dest_id == parent_broker_id) || (cmd.dest_id == higher_broker_id))
    {
        transmit (parent_route_id, cmd);
    }
    else
    {
        auto route = getRoute (cmd.dest_id);
        transmit (route, cmd);
    }
}

void CoreBroker::routeMessage (ActionMessage &&cmd, global_federate_id_t dest)
{
    if (dest == global_federate_id_t ())
    {
        return;
    }
    cmd.dest_id = dest;
    if ((dest == parent_broker_id) || (dest == higher_broker_id))
    {
        transmit (parent_route_id, std::move(cmd));
    }
    else
    {
        auto route = getRoute (dest);
        transmit (route, std::move(cmd));
    }
}

void CoreBroker::routeMessage (const ActionMessage &&cmd)
{
    if ((cmd.dest_id == parent_broker_id) || (cmd.dest_id == higher_broker_id))
    {
        transmit (parent_route_id, std::move(cmd));
    }
    else
    {
        auto route = getRoute (cmd.dest_id);
        transmit (route, std::move(cmd));
    }
}

void CoreBroker::executeInitializationOperations ()
{
    checkDependencies ();
    ActionMessage m (CMD_INIT_GRANT);
    m.source_id = global_broker_id_local;
    brokerState = broker_state_t::operating;
    for (auto &broker : _brokers)
    {
        if (!broker._nonLocal)
        {
            m.dest_id = broker.global_id;
            transmit (broker.route_id, m);
        }
    }
    timeCoord->enteringExecMode ();
    auto res = timeCoord->checkExecEntry ();
    if (res == message_processing_result::next_step)
    {
        enteredExecutionMode = true;
    }
    if (unknownHandles.hasUnknowns ())
    {
        LOG_WARNING (parent_broker_id, getIdentifier (), "Some requested targets are unresolved");
    }
}

void CoreBroker::FindandNotifyInputTargets (BasicHandleInfo &handleInfo)
{
    auto Handles = unknownHandles.checkForInputs (handleInfo.key);
    for (auto target : Handles)
    {
        // notify the publication about its subscriber
        ActionMessage m (CMD_ADD_SUBSCRIBER);

        m.setDestination (target.first);
        m.setSource (handleInfo.handle);
        m.payload = handleInfo.type;
        m.flags = handleInfo.flags;
        transmit (getRoute (m.dest_id), m);

        // notify the subscriber about its publisher
        m.setAction (CMD_ADD_PUBLISHER);
        m.setSource (target.first);
        m.setDestination (handleInfo.handle);
        m.flags = target.second;
        auto pub = handles.findHandle (target.first);
        if (pub != nullptr)
        {
            m.setStringData (pub->type, pub->units);
        }

        transmit (getRoute (m.dest_id), std::move (m));
    }
    if (!Handles.empty ())
    {
        unknownHandles.clearPublication (handleInfo.key);
    }
}

void CoreBroker::FindandNotifyPublicationTargets (BasicHandleInfo &handleInfo)
{
    auto subHandles = unknownHandles.checkForPublications (handleInfo.key);
    for (auto sub : subHandles)
    {
        // notify the publication about its subscriber
        ActionMessage m (CMD_ADD_SUBSCRIBER);
        m.setSource (sub.first);
        m.setDestination (handleInfo.handle);
        m.flags = sub.second;

        transmit (getRoute( m.dest_id), m);

        // notify the subscriber about its publisher
        m.setAction (CMD_ADD_PUBLISHER);
        m.setDestination (sub.first);
        m.setSource (handleInfo.handle);
        m.payload = handleInfo.type;
        m.flags = handleInfo.flags;
        m.setStringData (handleInfo.type, handleInfo.units);
        transmit (getRoute (m.dest_id), std::move (m));
    }
   
    auto Pubtargets = unknownHandles.checkForLinks (handleInfo.key);
    for (auto sub : Pubtargets)
    {
        ActionMessage m (CMD_ADD_NAMED_INPUT);
        m.name = sub;
        m.setSource (handleInfo.handle);
        checkForNamedInterface (m);
    }
    if (!(subHandles.empty () && Pubtargets.empty()))
    {
        unknownHandles.clearPublication (handleInfo.key);
    }
}

void CoreBroker::FindandNotifyEndpointTargets (BasicHandleInfo &handleInfo)
{
    auto Handles = unknownHandles.checkForEndpoints (handleInfo.key);
    for (auto target : Handles)
    {
        // notify the filter about its endpoint
        ActionMessage m (CMD_ADD_ENDPOINT);
        m.setSource (handleInfo.handle);
        m.setDestination (target.first);
        m.flags = target.second;
        transmit (getRoute (m.dest_id), m);

        // notify the endpoint about its filter
        m.setAction (CMD_ADD_FILTER);
        m.swapSourceDest ();

        m.flags = target.second;
        transmit (getRoute (m.dest_id), m);
    }

    if (!Handles.empty ())
    {
        unknownHandles.clearEndpoint (handleInfo.key);
    }
}

void CoreBroker::FindandNotifyFilterTargets (BasicHandleInfo &handleInfo)
{
    auto Handles = unknownHandles.checkForFilters (handleInfo.key);
    for (auto target : Handles)
    {
        // notify the endpoint about a filter
        ActionMessage m (CMD_ADD_FILTER);
        m.setSource (handleInfo.handle);
        m.flags = target.second;
        m.setDestination (target.first);
        if ((!handleInfo.type_in.empty ()) || (!handleInfo.type_out.empty ()))
        {
            m.setStringData (handleInfo.type_in, handleInfo.type_out);
        }
        transmit (getRoute (m.dest_id), m);

        // notify the filter about an endpoint
        m.setAction (CMD_ADD_ENDPOINT);
        m.swapSourceDest ();
        m.clearStringData ();
        transmit (getRoute (m.dest_id), m);
    }

	
    auto FiltDestTargets = unknownHandles.checkForFilterDestTargets (handleInfo.key);
    for (auto target : FiltDestTargets)
    {
        ActionMessage m (CMD_ADD_NAMED_ENDPOINT);
        m.name = target;
        m.setSource (handleInfo.handle);
        setActionFlag (m, destination_target);
        checkForNamedInterface (m);
    }

	auto FiltSourceTargets = unknownHandles.checkForFilterSourceTargets (handleInfo.key);
    for (auto target : FiltSourceTargets)
    {
        ActionMessage m (CMD_ADD_NAMED_ENDPOINT);
        m.name = target;
        m.setSource (handleInfo.handle);
        checkForNamedInterface (m);
    }
    if (!(Handles.empty () && FiltDestTargets.empty () && FiltSourceTargets.empty ()))
    {
        unknownHandles.clearFilter (handleInfo.key);
    }
}

void CoreBroker::setLoggingLevel (int logLevel) 
{ 
	ActionMessage cmd(CMD_BROKER_CONFIGURE);
	cmd.dest_id = global_broker_id.load();
	cmd.messageID = LOG_LEVEL_PROPERTY;
	cmd.counter = logLevel;
	addActionMessage(cmd);
}

// public query function
std::string CoreBroker::query (const std::string &target, const std::string &queryStr)
{
	auto gid = global_broker_id.load();
    if ((target == "broker") || (target == getIdentifier ()))
    {
        ActionMessage querycmd (CMD_BROKER_QUERY);
        querycmd.source_id = querycmd.dest_id=gid;
        auto index = ++queryCounter;
        querycmd.messageID = index;
        querycmd.payload = queryStr;
        auto queryResult = ActiveQueries.getFuture (index);
        addActionMessage (std::move (querycmd));
        auto ret = queryResult.get ();
        ActiveQueries.finishedWithValue (index);
        return ret;
    }
    else if (target == "parent")
    {
        if (isRootc)
        {
            return "#na";
        }
        ActionMessage querycmd (CMD_BROKER_QUERY);
        querycmd.source_id = gid;
        querycmd.messageID = ++queryCounter;
        querycmd.payload = queryStr;
        auto queryResult = ActiveQueries.getFuture (querycmd.messageID);
        addActionMessage (querycmd);
        auto ret = queryResult.get ();
        ActiveQueries.finishedWithValue (querycmd.messageID);
        return ret;
    }
    else if ((target == "root") || (target == "rootbroker"))
    {
        ActionMessage querycmd (CMD_BROKER_QUERY);
        querycmd.source_id = gid;
        auto index = ++queryCounter;
        querycmd.messageID = index;
        querycmd.payload = queryStr;
        auto queryResult = ActiveQueries.getFuture (querycmd.messageID);
        if (!gid.isValid ())
        {
			//TODO:: this has potential for deadlock
            delayTransmitQueue.push (std::move (querycmd));
        }
        else
        {
            transmit (parent_route_id, std::move(querycmd));
        }
        auto ret = queryResult.get ();
        ActiveQueries.finishedWithValue (index);
        return ret;
    }
    else
    {
        ActionMessage querycmd (CMD_QUERY);
        querycmd.source_id = gid;
        auto index = ++queryCounter;
        querycmd.messageID = index;
        querycmd.payload = queryStr;
        querycmd.setStringData (target);
        auto queryResult = ActiveQueries.getFuture (querycmd.messageID);
        if (!gid.isValid ())
        {
            delayTransmitQueue.push (std::move (querycmd));
        }
        else
        {
            transmit (parent_route_id, std::move(querycmd));
        }

        auto ret = queryResult.get ();
        ActiveQueries.finishedWithValue (index);
        return ret;
    }
    //  return "#invalid";
}

std::string CoreBroker::generateQueryAnswer (const std::string &request)
{
    if (request == "isinit")
    {
        return (brokerState >= broker_state_t::operating) ? std::string ("true") : std::string ("false");
    }
    if (request == "isconnected")
    {
        return (isConnected ()) ? std::string ("true") : std::string ("false");
    }
    if (request == "name")
    {
        return getIdentifier ();
    }
    if (request == "address")
    {
        return getAddress ();
    }
    if (request == "counts")
    {
        std::string cnts = "{\"brokers\":";
        cnts += std::to_string (_brokers.size ());
        cnts += ",\n";
        cnts += "\"federates\":";
        cnts += std::to_string (_federates.size ());
        cnts += ",\n";
        cnts += "\"handles\":";
        cnts += std::to_string (handles.size ());
        cnts += '}';
        return cnts;
    }
    if (request == "summary")
    {
        return generateFederationSummary ();
    }
    if (request == "federates")
    {
        return generateStringVector (_federates, [](auto &fed) { return fed.name; });
    }
    if (request == "brokers")
    {
        return generateStringVector (_brokers, [](auto &brk) { return brk.name; });
    }
    if (request == "inputs")
    {
        return generateStringVector_if (handles, [](auto &handle) { return handle.key; },
                                        [](auto &handle) { return (handle.handle_type == handle_type_t::input); });
    }
    if (request == "publications")
    {
        return generateStringVector_if (handles, [](auto &handle) { return handle.key; },
                                        [](auto &handle) {
                                            return (handle.handle_type == handle_type_t::publication);
                                        });
    }
    if (request == "filters")
    {
        return generateStringVector_if (handles, [](auto &handle) { return handle.key; },
                                        [](auto &handle) {
                                            return (handle.handle_type == handle_type_t::filter);
                                        });
    }
    if (request == "endpoints")
    {
        return generateStringVector_if (handles, [](auto &handle) { return handle.key; },
                                        [](auto &handle) {
                                            return (handle.handle_type == handle_type_t::endpoint);
                                        });
    }
    if (request == "federate_map")
    {
        if (fedMap.isCompleted ())
        {
            return fedMap.generate ();
        }
        if (fedMap.isActive ())
        {
            return "#wait";
        }
        initializeFederateMap ();
        if (fedMap.isCompleted ())
        {
            return fedMap.generate ();
        }
        return "#wait";
    }
    if (request == "dependency_graph")
    {
        if (depMap.isCompleted ())
        {
            return depMap.generate ();
        }
        if (depMap.isActive ())
        {
            return "#wait";
        }
        initializeDependencyGraph ();
        if (depMap.isCompleted ())
        {
            return depMap.generate ();
        }
        return "#wait";
    }
    if (request == "dependson")
    {
        return generateStringVector (timeCoord->getDependencies (),
                                     [](const auto &dep) { return std::to_string (dep.baseValue()); });
    }
    if (request == "dependents")
    {
        return generateStringVector (timeCoord->getDependents (),
                                     [](const auto &dep) { return std::to_string (dep.baseValue()); });
    }
    if (request == "dependencies")
    {
        Json_helics::Value base;
        base["name"] = getIdentifier ();
        base["id"] = global_broker_id_local.baseValue();
        if (!isRootc)
        {
            base["parent"] = higher_broker_id.baseValue();
        }
        base["dependents"] = Json_helics::arrayValue;
        for (auto &dep : timeCoord->getDependents ())
        {
            base["dependents"].append (dep.baseValue());
        }
        base["dependencies"] = Json_helics::arrayValue;
        for (auto &dep : timeCoord->getDependencies ())
        {
            base["dependencies"].append (dep.baseValue());
        }
        return generateJsonString (base);
    }
    return "#invalid";
}

void CoreBroker::initializeFederateMap ()
{
    Json_helics::Value &base = fedMap.getJValue ();
    base["name"] = getIdentifier ();
    base["id"] = global_broker_id_local.baseValue();
    if (!isRootc)
    {
        base["parent"] = higher_broker_id.baseValue();
    }
    base["brokers"] = Json_helics::arrayValue;
    ActionMessage queryReq (CMD_BROKER_QUERY);
    queryReq.payload = "federate_map";
    queryReq.source_id = global_broker_id_local;
    queryReq.counter = 2;  // indicating which processing to use
    bool hasCores = false;
    for (auto &broker : _brokers)
    {
        if (!broker._nonLocal)
        {
            int index;
            if (broker._core)
            {
                if (!hasCores)
                {
                    hasCores = true;
                    base["cores"] = Json_helics::arrayValue;
                }
                index = fedMap.generatePlaceHolder ("cores");
            }
            else
            {
                index = fedMap.generatePlaceHolder ("brokers");
            }
            queryReq.messageID = index;
            queryReq.dest_id = broker.global_id;
            transmit (broker.route_id, queryReq);
        }
    }
}

void CoreBroker::initializeDependencyGraph ()
{
    Json_helics::Value &base = depMap.getJValue ();
    base["name"] = getIdentifier ();
    base["id"] = global_broker_id_local.baseValue();
    if (!isRootc)
    {
        base["parent"] = higher_broker_id.baseValue();
    }
    base["brokers"] = Json_helics::arrayValue;
    ActionMessage queryReq (CMD_BROKER_QUERY);
    queryReq.payload = "dependency_graph";
    queryReq.source_id = global_broker_id_local;
    queryReq.counter = 4;  // indicating which processing to use
    bool hasCores = false;
    for (auto &broker : _brokers)
    {
        int index;
        if (broker._core)
        {
            if (!hasCores)
            {
                hasCores = true;
                base["cores"] = Json_helics::arrayValue;
            }
            index = depMap.generatePlaceHolder ("cores");
        }
        else
        {
            index = depMap.generatePlaceHolder ("brokers");
        }
        queryReq.messageID = index;
        queryReq.dest_id = broker.global_id;
        transmit (broker.route_id, queryReq);
    }

    base["dependents"] = Json_helics::arrayValue;
    for (auto &dep : timeCoord->getDependents ())
    {
        base["dependents"].append (dep.baseValue());
    }
    base["dependencies"] = Json_helics::arrayValue;
    for (auto &dep : timeCoord->getDependencies ())
    {
        base["dependencies"].append (dep.baseValue());
    }
}

void CoreBroker::initializeDataFlowGraph ()
{
    Json_helics::Value &base = depMap.getJValue ();
    base["name"] = getIdentifier ();
    base["id"] = global_broker_id_local.baseValue();
    if (!isRootc)
    {
        base["parent"] = higher_broker_id.baseValue();
    }
    base["brokers"] = Json_helics::arrayValue;
    ActionMessage queryReq (CMD_BROKER_QUERY);
    queryReq.payload = "dependency_graph";
    queryReq.source_id = global_broker_id_local;
    queryReq.counter = 4;  // indicating which processing to use
    bool hasCores = false;
    for (auto &broker : _brokers)
    {
        int index;
        if (broker._core)
        {
            if (!hasCores)
            {
                hasCores = true;
                base["cores"] = Json_helics::arrayValue;
            }
            index = depMap.generatePlaceHolder ("cores");
        }
        else
        {
            index = depMap.generatePlaceHolder ("brokers");
        }
        queryReq.messageID = index;
        queryReq.dest_id = broker.global_id;
        transmit (broker.route_id, queryReq);
    }

    base["dependents"] = Json_helics::arrayValue;
    for (auto &dep : timeCoord->getDependents ())
    {
        base["dependents"].append (dep.baseValue());
    }
    base["dependencies"] = Json_helics::arrayValue;
    for (auto &dep : timeCoord->getDependencies ())
    {
        base["dependencies"].append (dep.baseValue());
    }
}

void CoreBroker::processLocalQuery (const ActionMessage &m)
{
    ActionMessage queryRep (CMD_QUERY_REPLY);
    queryRep.source_id = global_broker_id_local;
    queryRep.dest_id = m.source_id;
    queryRep.messageID = m.messageID;
    queryRep.payload = generateQueryAnswer (m.payload);
    queryRep.counter = m.counter;
    if (queryRep.payload == "#wait")
    {
        if (m.payload == "dependency_graph")
        {
            depMapRequestors.push_back (queryRep);
        }
        else if (m.payload == "federate_map")
        {
            fedMapRequestors.push_back (queryRep);
        }
        else if (m.payload == "data_flow_graph")
        {
            dataflowMapRequestors.push_back (queryRep);
        }
    }
    else if (queryRep.dest_id == global_broker_id_local)
    {
        ActiveQueries.setDelayedValue (m.messageID, queryRep.payload);
    }
    else
    {
        routeMessage (std::move(queryRep), global_federate_id_t (m.source_id));
    }
}

void CoreBroker::processQuery (const ActionMessage &m)
{
    auto &target = m.getString (targetStringLoc);
    if ((target == getIdentifier ()) || (target == "broker"))
    {
        processLocalQuery (m);
    }
    else if ((isRootc) && ((target == "root") || (target == "federation")))
    {
        processLocalQuery (m);
    }
    else
    {
        route_id_t route = parent_route_id;
        auto fed = _federates.find (target);
        if (fed != _federates.end ())
        {
            route = fed->route_id;
        }
        else
        {
            auto broker = _brokers.find (target);
            if (broker != _brokers.end ())
            {
                route = broker->route_id;
            }
        }
        if ((route == parent_route_id) && (isRootc))
        {
            ActionMessage queryResp (CMD_QUERY_REPLY);
            queryResp.dest_id = m.source_id;
            queryResp.source_id = global_broker_id_local;
            queryResp.messageID = m.messageID;

            queryResp.payload = "#invalid";
            transmit (getRoute (queryResp.dest_id), queryResp);
        }
        else
        {
            transmit (route, m);
        }
    }
}

void CoreBroker::processQueryResponse (const ActionMessage &m)
{
    switch (m.counter)
    {
    case 0:
    default:
        ActiveQueries.setDelayedValue (m.messageID, m.payload);
        break;
    case 2:
        if (fedMap.addComponent (m.payload, m.messageID))
        {
            if (fedMapRequestors.size () == 1)
            {
                if (fedMapRequestors.front ().dest_id == global_broker_id_local)
                {
                    ActiveQueries.setDelayedValue (fedMapRequestors.front ().messageID, fedMap.generate ());
                }
                else
                {
                    fedMapRequestors.front ().payload = fedMap.generate ();
                    routeMessage (fedMapRequestors.front ());
                }
            }
            else
            {
                auto str = fedMap.generate ();
                for (auto &resp : fedMapRequestors)
                {
                    if (resp.dest_id == global_broker_id_local)
                    {
                        ActiveQueries.setDelayedValue (resp.messageID, str);
                    }
                    else
                    {
                        resp.payload = str;
                        routeMessage (resp);
                    }
                }
            }
            fedMapRequestors.clear ();
        }
        break;
    case 4:
        if (depMap.addComponent (m.payload, m.messageID))
        {
            if (depMapRequestors.size () == 1)
            {
                if (depMapRequestors.front ().dest_id == global_broker_id_local)
                {
                    ActiveQueries.setDelayedValue (depMapRequestors.front ().messageID, depMap.generate ());
                }
                else
                {
                    depMapRequestors.front ().payload = depMap.generate ();
                    routeMessage (std::move(depMapRequestors.front ()));
                }
            }
            else
            {
                auto str = depMap.generate ();
                for (auto &resp : depMapRequestors)
                {
                    if (resp.dest_id == global_broker_id_local)
                    {
                        ActiveQueries.setDelayedValue (resp.messageID, str);
                    }
                    else
                    {
                        resp.payload = str;
                        routeMessage (std::move(resp));
                    }
                }
            }
            depMapRequestors.clear ();
        }
        break;
    }
}

void CoreBroker::checkDependencies ()
{
    if (isRootc)
    {
        for (const auto &newdep : delayedDependencies)
        {
            auto depfed = _federates.find (newdep.first);
            if (depfed != _federates.end ())
            {
                ActionMessage addDep (CMD_ADD_DEPENDENCY, newdep.second, depfed->global_id);
                routeMessage (addDep);
                addDep = ActionMessage (CMD_ADD_DEPENDENT, depfed->global_id, newdep.second);
                routeMessage (addDep);
            }
            else
            {
                ActionMessage logWarning (CMD_LOG, parent_broker_id, newdep.second);
                logWarning.messageID = warning;
                logWarning.payload = "unable to locate " + newdep.first + " to establish dependency";
                routeMessage (logWarning);
            }
        }

        if (timeCoord->getDependents ().size () == 1)
        {  // if there is just one dependency remove it
            auto depid = timeCoord->getDependents ()[0];
            auto dependencies = timeCoord->getDependencies ();
            if (dependencies.size () == 1)
            {
                if (dependencies.front () != depid)
                {
                    ActionMessage adddep (CMD_ADD_DEPENDENT);
                    adddep.source_id = depid;
                    ActionMessage rmdep (CMD_REMOVE_DEPENDENT);
                    rmdep.source_id = global_broker_id_local;
                    routeMessage (adddep, dependencies.front ());
                    routeMessage (rmdep, dependencies.front ());

                    adddep.setAction (CMD_ADD_DEPENDENCY);
                    adddep.source_id = dependencies.front ();
                    rmdep.setAction (CMD_REMOVE_DEPENDENCY);
                    routeMessage (adddep, depid);
                    routeMessage (rmdep, depid);

                    timeCoord->removeDependency (dependencies.front ());
                    timeCoord->removeDependent (depid);
                }
                else
                {
                    ActionMessage rmdep (CMD_REMOVE_INTERDEPENDENCY);
                    rmdep.source_id = global_broker_id_local;

                    routeMessage (rmdep, depid);
                    timeCoord->removeDependency (depid);
                    timeCoord->removeDependent (depid);
                }
            }
        }
    }
    else
    {
        // if there is more than 2 dependents(higher broker + 2 or more other objects then we need to be a
        // timeCoordinator
        if (timeCoord->getDependents ().size () > 2)
        {
            return;
        }

        global_federate_id_t fedid;
        int localcnt = 0;
        for (auto &dep : timeCoord->getDependents ())
        {
            if (dep != higher_broker_id)
            {
                ++localcnt;
                fedid = dep;
            }
        }
        if (localcnt != 1)
        {
            return;
        }
        // remove the core from the time dependency chain
        timeCoord->removeDependency (higher_broker_id);
        timeCoord->removeDependency (fedid);
        timeCoord->removeDependent (higher_broker_id);
        timeCoord->removeDependent (fedid);

        ActionMessage rmdep (CMD_REMOVE_INTERDEPENDENCY);

        rmdep.source_id = global_broker_id_local;
        routeMessage (rmdep, higher_broker_id);
        routeMessage (rmdep, fedid);

        ActionMessage adddep (CMD_ADD_INTERDEPENDENCY);
        adddep.source_id = fedid;
        routeMessage (adddep, higher_broker_id);
        adddep.source_id = higher_broker_id;
        routeMessage (adddep, fedid);
    }
}
bool CoreBroker::allInitReady () const
{
    // the federate count must be greater than the min size
    if (static_cast<decltype (minFederateCount)> (_federates.size ()) < minFederateCount)
    {
        return false;
    }
    if (static_cast<decltype (minBrokerCount)> (_brokers.size ()) < minBrokerCount)
    {
        return false;
    }

    return std::all_of (_brokers.begin (), _brokers.end (),
                        [](const auto &brk) { return ((brk._nonLocal) || (brk._initRequested)); });
}

bool CoreBroker::allDisconnected () const
{
    return std::all_of (_brokers.begin (), _brokers.end (),
                        [](const auto &brk) { return ((brk._nonLocal) || (brk._disconnected)); });
}

}  // namespace helics
