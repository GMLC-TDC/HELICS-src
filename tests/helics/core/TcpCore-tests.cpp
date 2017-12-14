/*
Copyright (C) 2017, Battelle Memorial Institute
All rights reserved.

This software was co-developed by Pacific Northwest National Laboratory, operated by the Battelle Memorial
Institute; the National Renewable Energy Laboratory, operated by the Alliance for Sustainable Energy, LLC; and the
Lawrence Livermore National Laboratory, operated by Lawrence Livermore National Security, LLC.

*/
#include <boost/test/unit_test.hpp>

#include "helics/common/AsioServiceManager.h"
#include "helics/core/ActionMessage.h"
#include "helics/core/BrokerFactory.h"
#include "helics/core/CoreFactory.h"
#include "helics/core/core-types.h"
#include "helics/core/core.h"
#include "helics/core/tcp/TcpBroker.h"
#include "helics/core/tcp/TcpComms.h"
#include "helics/core/tcp/TcpCore.h"
#include "helics/core/tcp/TcpHelperClasses.h"

//#include "boost/process.hpp"
#include <future>

BOOST_AUTO_TEST_SUITE (TcpCore_tests)

using boost::asio::ip::tcp;
using helics::Core;


BOOST_AUTO_TEST_CASE (tcpComms_broker_test)
{
    std::atomic<int> counter{0};
    std::string host = "localhost";
    helics::TcpComms comm (host, host);

    auto srv = AsioServiceManager::getServicePointer ();

    tcp_server server(srv->getBaseService(), 24160);
    srv->runServiceLoop();
    std::vector<char> data(1024);
    server.setDataCall([&counter](const char *, size_t) {++counter; });
    server.start_accept();
    
    comm.setCallback ([&counter](helics::ActionMessage m) { ++counter; });
    comm.setBrokerPort (24160);
    comm.setName ("tests");
    comm.setTimeout(1000);
    auto confut = std::async (std::launch::async, [&comm]() { return comm.connect (); });

   
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    int cnt = 0;
    while (counter != 1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ++cnt;
        if (cnt > 30)
        {
            break;
        }
    }
    BOOST_CHECK_EQUAL(counter, 1);


    server.haltServer();
    comm.disconnect();
    srv->haltServiceLoop();
}

BOOST_AUTO_TEST_CASE (tcpComms_broker_test_transmit)
{
    std::atomic<int> counter{0};
    std::atomic<size_t> len{ 0 };
    std::string host = "localhost";
    helics::TcpComms comm (host, host);

    auto srv = AsioServiceManager::getServicePointer ();
    tcp_server server(srv->getBaseService(), 24160);
    srv->runServiceLoop();
    std::vector<char> data(1024);
    server.setDataCall([&data, &counter,&len](const char *data_rec, size_t data_Size) {std::copy(data_rec, data_rec + data_Size, data.begin()); len = data_Size; ++counter; });
    server.start_accept();

    comm.setCallback([](helics::ActionMessage m) { });
    comm.setBrokerPort (24160);
    comm.setPortNumber (24180);
    comm.setName ("tests");
    bool connected = comm.connect ();
    BOOST_REQUIRE (connected);
    comm.transmit (0, helics::CMD_IGNORE);

    boost::system::error_code error;
    int cnt = 0;
    while (counter != 1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ++cnt;
        if (cnt > 30)
        {
            break;
        }
    }
    BOOST_CHECK_EQUAL(counter, 1);

    BOOST_CHECK_GT (len, 32);
    helics::ActionMessage rM (data.data (), len);
    BOOST_CHECK (rM.action () == helics::action_message_def::action_t::cmd_ignore);
    server.haltServer();
    comm.disconnect ();
    std::this_thread::sleep_for (std::chrono::milliseconds (100));
}


BOOST_AUTO_TEST_CASE (tcpComms_rx_test)
{
    std::atomic<int> counter{0};
    std::atomic<size_t> len{ 0 };
    helics::ActionMessage act;
    std::string host = "localhost";
    helics::TcpComms comm (host, host);

    auto srv = AsioServiceManager::getServicePointer ();

    tcp_server server(srv->getBaseService(), 24160);
    srv->runServiceLoop();
    std::vector<char> data(1024);
    server.setDataCall([&data, &counter, &len](const char *data_rec, size_t data_Size) {std::copy(data_rec, data_rec + data_Size, data.begin()); len = data_Size; ++counter; });
    server.start_accept();

    comm.setCallback ([&counter, &act](helics::ActionMessage m) {
        ++counter;
        act = m;
    });
    comm.setBrokerPort (24160);
    comm.setPortNumber (24163);
    comm.setName ("tests");

    bool connected = comm.connect ();
    BOOST_REQUIRE (connected);

    auto txconn=tcp_connection::create(srv->getBaseService(), host, "24163", 1024);
    int cnt = 0;
    while (!txconn->isConnected())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        ++cnt;
        if (cnt > 20)
        {
            break;
        }
    }

    BOOST_REQUIRE(txconn->isConnected());

    helics::ActionMessage cmd (helics::CMD_ACK);
    std::string buffer = cmd.to_string ();

    txconn->send(buffer);
    
    std::this_thread::sleep_for (std::chrono::milliseconds (200));
    BOOST_REQUIRE_EQUAL (counter, 1);
    BOOST_CHECK (act.action () == helics::action_message_def::action_t::cmd_ack);
    txconn->close();
    comm.disconnect ();
    std::this_thread::sleep_for (std::chrono::milliseconds (100));
}

/*
BOOST_AUTO_TEST_CASE (zmqComm_transmit_through)
{
    std::atomic<int> counter{0};
    std::atomic<int> counter2{0};
    helics::ActionMessage act;
    helics::ActionMessage act2;

    std::string host = "localhost";
    helics::UdpComms comm (host, host);
    helics::UdpComms comm2 (host, "");

    comm.setBrokerPort (23901);
    comm.setName ("tests");
    comm2.setName ("test2");
    comm2.setPortNumber (23901);
    comm.setPortNumber (23908);

    comm.setCallback ([&counter, &act](helics::ActionMessage m) {
        ++counter;
        act = m;
    });
    comm2.setCallback ([&counter2, &act2](helics::ActionMessage m) {
        ++counter2;
        act2 = m;
    });

    // need to launch the connection commands at the same time since they depend on eachother in this case
    auto connected_fut = std::async (std::launch::async, [&comm] { return comm.connect (); });

    bool connected = comm2.connect ();
    BOOST_REQUIRE (connected);
    connected = connected_fut.get ();
    BOOST_REQUIRE (connected);

    comm.transmit (0, helics::CMD_ACK);

    std::this_thread::sleep_for (std::chrono::milliseconds (250));
    if (counter2 != 1)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds (500));
    }
    BOOST_REQUIRE_EQUAL (counter2, 1);
    BOOST_CHECK (act2.action () == helics::action_message_def::action_t::cmd_ack);

    comm.disconnect ();
    comm2.disconnect ();
    std::this_thread::sleep_for (std::chrono::milliseconds (100));
}

*/
/*
BOOST_AUTO_TEST_CASE (zmqComm_transmit_add_route)
{
    std::atomic<int> counter{0};
    std::atomic<int> counter2{0};
    std::atomic<int> counter3{0};

    std::string host = "localhost";
    helics::UdpComms comm (host, host);
    helics::UdpComms comm2 (host, "");
    helics::UdpComms comm3 (host, host);

    comm.setBrokerPort (23901);
    comm.setName ("tests");
    comm2.setName ("broker");
    comm3.setName ("test3");
    comm3.setBrokerPort (23901);

    comm2.setPortNumber (23901);
    comm.setPortNumber (23905);
    comm3.setPortNumber (23920);

    helics::ActionMessage act;
    helics::ActionMessage act2;
    helics::ActionMessage act3;

    comm.setCallback ([&counter, &act](helics::ActionMessage m) {
        ++counter;
        act = m;
    });
    comm2.setCallback ([&counter2, &act2](helics::ActionMessage m) {
        ++counter2;
        act2 = m;
    });
    comm3.setCallback ([&counter3, &act3](helics::ActionMessage m) {
        ++counter3;
        act3 = m;
    });

    // need to launch the connection commands at the same time since they depend on eachother in this case
    // auto connected_fut = std::async(std::launch::async, [&comm] {return comm.connect(); });

    bool connected = comm2.connect ();
    BOOST_REQUIRE (connected);
    // connected = connected_fut.get();
    connected = comm.connect ();
    BOOST_REQUIRE (connected);
    connected = comm3.connect ();

    comm.transmit (0, helics::CMD_ACK);

    std::this_thread::sleep_for (std::chrono::milliseconds (250));
    BOOST_REQUIRE_EQUAL (counter2, 1);
    BOOST_CHECK (act2.action () == helics::action_message_def::action_t::cmd_ack);

    comm3.transmit (0, helics::CMD_ACK);

    std::this_thread::sleep_for (std::chrono::milliseconds (250));
    BOOST_REQUIRE_EQUAL (counter2, 2);
    BOOST_CHECK (act2.action () == helics::action_message_def::action_t::cmd_ack);

    comm2.addRoute (3, comm3.getAddress ());

    comm2.transmit (3, helics::CMD_ACK);

    std::this_thread::sleep_for (std::chrono::milliseconds (250));
    if (counter3 != 1)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds (250));
    }
    BOOST_REQUIRE_EQUAL (counter3, 1);
    BOOST_CHECK (act3.action () == helics::action_message_def::action_t::cmd_ack);

    comm2.addRoute (4, comm.getAddress ());

    comm2.transmit (4, helics::CMD_ACK);

    std::this_thread::sleep_for (std::chrono::milliseconds (250));
    BOOST_REQUIRE_EQUAL (counter, 1);
    BOOST_CHECK (act.action () == helics::action_message_def::action_t::cmd_ack);

    comm.disconnect ();
    comm2.disconnect ();
    comm3.disconnect ();
    std::this_thread::sleep_for (std::chrono::milliseconds (100));
}

*/

/*
BOOST_AUTO_TEST_CASE (udpCore_initialization_test)
{
    std::string initializationString =
      "1 --brokerport=23901  --port=23950 --local_interface=localhost --name=core1";
    auto core = helics::CoreFactory::create (helics::core_type::UDP, initializationString);

    BOOST_REQUIRE (core != nullptr);
    BOOST_CHECK (core->isInitialized ());
    auto srv = AsioServiceManager::getServicePointer ();
    udp::socket rxSocket (AsioServiceManager::getService (), udp::endpoint (udp::v4 (), 23901));

    BOOST_CHECK (rxSocket.is_open ());

    bool connected = core->connect ();
    BOOST_REQUIRE (connected);

    std::vector<char> data (1024);

    udp::endpoint remote_endpoint;
    boost::system::error_code error;

    auto len = rxSocket.receive_from (boost::asio::buffer (data), remote_endpoint, 0, error);
    BOOST_REQUIRE (!error);
    BOOST_CHECK_GT (len, 32);
    helics::ActionMessage rM (data.data (), len);

    BOOST_CHECK_EQUAL (rM.name, "core1");
    BOOST_CHECK (rM.action () == helics::action_message_def::action_t::cmd_reg_broker);
    helics::ActionMessage resp (helics::CMD_PRIORITY_ACK);
    rxSocket.send_to (boost::asio::buffer (resp.to_string ()), remote_endpoint, 0, error);
    BOOST_CHECK (!error);
    core->disconnect ();
    core = nullptr;
    helics::CoreFactory::cleanUpCores (100);
}
*/
/** test case checks default values and makes sure they all mesh together
also tests the automatic port determination for cores
*/

/*
BOOST_AUTO_TEST_CASE (udpCore_core_broker_default_test)
{
    std::string initializationString = "1";

    auto broker = helics::BrokerFactory::create (helics::core_type::UDP, initializationString);

    auto core = helics::CoreFactory::create (helics::core_type::UDP, initializationString);
    bool connected = broker->isConnected ();
    BOOST_CHECK (connected);
    connected = core->connect ();
    BOOST_CHECK (connected);

    auto ccore = static_cast<helics::UdpCore *> (core.get ());
    // this will test the automatic port allocation
    BOOST_CHECK_EQUAL (ccore->getAddress (), "localhost:23964");
    core->disconnect ();
    broker->disconnect ();
    core = nullptr;
    broker = nullptr;
    helics::CoreFactory::cleanUpCores (100);
    helics::BrokerFactory::cleanUpBrokers (100);
}

*/
BOOST_AUTO_TEST_SUITE_END ()