import time
import pytest as pt
import helics as h

@pt.fixture
def vFed():

    initstring = "1 --name=mainbroker"
    fedinitstring = "--broker=mainbroker --federates=1"
    deltat = 0.01
    #TODO: should add an assert here about helicsGetVersion
    h.helicsGetVersion()

    # Create broker #
    broker = h.helicsCreateBroker("zmq", "", initstring)

    isconnected = h.helicsBrokerIsConnected(broker)

    if isconnected == 1:
        pass

    # Create Federate Info object that describes the federate properties #
    fedinfo = h.helicsFederateInfoCreate()

    # Set Federate name #
    h.helicsFederateInfoSetFederateName(fedinfo, "TestA Federate")

    # Set core type from string #
    h.helicsFederateInfoSetCoreTypeFromString(fedinfo, "zmq")

    # Federate init string #
    h.helicsFederateInfoSetCoreInitString(fedinfo, fedinitstring)

    # Set the message interval (timedelta) for federate. Note th#
    # HELICS minimum message time interval is 1 ns and by default
    # it uses a time delta of 1 second. What is provided to the
    # setTimedelta routine is a multiplier for the default timedelta.

    # Set one second message interval #
    h.helicsFederateInfoSetTimeDelta(fedinfo, deltat)

    h.helicsFederateInfoSetLoggingLevel(fedinfo, 1)

    vFed = h.helicsCreateValueFederate(fedinfo)

    yield vFed

    h.helicsFederateFinalize(vFed)

    state = h.helicsFederateGetState(vFed)
    assert state == 3

    while (h.helicsBrokerIsConnected(broker)):
        time.sleep(1)

    h.helicsFederateFree(vFed)
    h.helicsCloseLibrary()


def test_value_federate_initialize(vFed):
    state = h.helicsFederateGetState(vFed)
    assert state == 0

    h.helicsFederateEnterExecutingMode(vFed)

    state = h.helicsFederateGetState(vFed)
    assert state == 2

def test_value_federate_publication_registration(vFed):
    pubid1 = h.helicsFederateRegisterPublication(vFed, "pub1", "string", "")
    pubid2 = h.helicsFederateRegisterGlobalPublication(vFed, "pub2", "int", "")
    pubid3 = h.helicsFederateRegisterPublication(vFed, "pub3", "double", "V")
    h.helicsFederateEnterExecutingMode(vFed)

    publication_key = h.helicsPublicationGetKey(pubid1)
    assert publication_key == 'TestA Federate/pub1'
    publication_key = h.helicsPublicationGetKey(pubid2)
    assert publication_key == 'pub2'
    publication_key = h.helicsPublicationGetKey(pubid3)
    assert publication_key == 'TestA Federate/pub3'
    publication_type = h.helicsPublicationGetType(pubid3)
    assert publication_type == 'double'
    publication_units = h.helicsPublicationGetUnits(pubid3)
    assert publication_units == 'V'

def test_value_federate_runFederateTestNamedPoint(vFed):
    defaultValue = "start of a longer string in place of the shorter one and now this should be very long"
    defVal = 5.3
    #testValue1 = "inside of the functional relationship of helics"
    testValue1 = "short string"
    testVal1 = 45.7823
    testValue2 = "I am a string"
    testVal2 = 0.0

    pubid = h.helicsFederateRegisterGlobalTypePublication(vFed, "pub1", h.HELICS_DATA_TYPE_NAMEDPOINT, "")
    subid = h.helicsFederateRegisterSubscription(vFed, "pub1", "named_point", "")

    h.helicsSubscriptionSetDefaultNamedPoint(subid, defaultValue, defVal)

    h.helicsFederateEnterExecutingMode(vFed)

    # publish string1 at time=0.0;
    h.helicsPublicationPublishNamedPoint(pubid, testValue1, testVal1)

    # double val;
    value, val = h.helicsSubscriptionGetNamedPoint(subid)
    assert value == defaultValue
    assert val == defVal

    grantedtime = h.helicsFederateRequestTime(vFed, 1.0)

    assert grantedtime == 0.01

    # get the value
    value2, val2 = h.helicsSubscriptionGetNamedPoint(subid)
    # make sure the string is what we expect
    assert value2 == testValue1
    assert val2 == testVal1

    # publish a second string
    h.helicsPublicationPublishNamedPoint(pubid, testValue2, testVal2)

    # make sure the value is still what we expect
    value3, val3 = h.helicsSubscriptionGetNamedPoint(subid)
    # make sure the string is what we expect
    assert value3 == testValue1
    assert val3 == testVal1

    # advance time
    grantedtime = h.helicsFederateRequestTime(vFed, 2.0)
    assert grantedtime == 0.02

    # make sure the value was updated
    value4, val4 = h.helicsSubscriptionGetNamedPoint(subid)
    # make sure the string is what we expect
    assert value4 == testValue2
    assert val4 == testVal2


def test_value_federate_runFederateTestBool(vFed):
    defaultValue = True
    testValue1 = True
    testValue2 = False

    # register the publications
    pubid = h.helicsFederateRegisterGlobalTypePublication(vFed, "pub1", h.HELICS_DATA_TYPE_BOOLEAN, "")
    subid = h.helicsFederateRegisterSubscription(vFed, "pub1", "bool", "")

    h.helicsSubscriptionSetDefaultBoolean(subid, h.helics_true if defaultValue else h.helics_false)

    h.helicsFederateEnterExecutingMode(vFed)

    # publish string1 at time=0.0;
    h.helicsPublicationPublishBoolean(pubid, h.helics_true if testValue1 else h.helics_false)
    val = h.helicsSubscriptionGetBoolean(subid)

    assert val == h.helics_true if defaultValue else h.helics_false

    grantedtime = h.helicsFederateRequestTime (vFed, 1.0)
    assert grantedtime == 0.01

    # get the value
    val = h.helicsSubscriptionGetBoolean(subid)

    # make sure the string is what we expect
    assert val == h.helics_true if testValue1 else h.helics_false

    # publish a second string
    h.helicsPublicationPublishBoolean(pubid, h.helics_true if testValue2 else h.helics_false)

    # make sure the value is still what we expect
    val = h.helicsSubscriptionGetBoolean(subid)
    assert val == h.helics_true if testValue1 else h.helics_false
    # advance time
    grantedtime = h.helicsFederateRequestTime (vFed, 2.0)
    # make sure the value was updated
    assert grantedtime == 0.02

    val = h.helicsSubscriptionGetBoolean(subid)
    assert val == h.helics_false if testValue2 else h.helics_true


def test_value_federate_publisher_registration(vFed):
    pubid1 = h.helicsFederateRegisterTypePublication(vFed, "pub1", h.HELICS_DATA_TYPE_STRING, "")
    pubid2 = h.helicsFederateRegisterGlobalTypePublication(vFed, "pub2", h.HELICS_DATA_TYPE_INT, "")
    pubid3 = h.helicsFederateRegisterTypePublication(vFed, "pub3", h.HELICS_DATA_TYPE_DOUBLE, "V")
    h.helicsFederateEnterExecutingMode(vFed)

    publication_key = h.helicsPublicationGetKey(pubid1)
    assert publication_key == 'TestA Federate/pub1'
    publication_type = h.helicsPublicationGetType(pubid1)
    assert publication_type == 'string'
    publication_key = h.helicsPublicationGetKey(pubid2)
    assert publication_key == 'pub2'
    publication_key = h.helicsPublicationGetKey(pubid3)
    assert publication_key == 'TestA Federate/pub3'
    publication_type = h.helicsPublicationGetType(pubid3)
    assert publication_type == 'double'
    publication_units = h.helicsPublicationGetUnits(pubid3)
    assert publication_units == 'V'
    publication_type = h.helicsPublicationGetType(pubid2)
    assert publication_type == 'int64'


def test_value_federate_subscription_and_publication_registration(vFed):
    pubid3 = h.helicsFederateRegisterPublication(vFed, "pub3", "double", "V")

    subid1 = h.helicsFederateRegisterOptionalSubscription(vFed, "sub1", "int64", "V")
    subid2 = h.helicsFederateRegisterOptionalTypeSubscription(vFed, "sub2", h.HELICS_DATA_TYPE_INT, "")

    subid3 = h.helicsFederateRegisterOptionalSubscription(vFed, "sub3", "vector", "V")
    h.helicsFederateEnterExecutingMode(vFed)
    publication_type = h.helicsPublicationGetType(pubid3)
    assert publication_type == 'double'

    sub_key = h.helicsSubscriptionGetKey(subid1)
    assert sub_key == 'sub1'
    sub_type = h.helicsSubscriptionGetType(subid1)
    assert sub_type == 'int64'
    sub_key = h.helicsSubscriptionGetKey(subid2)
    assert sub_key == 'sub2'
    sub_key = h.helicsSubscriptionGetKey(subid3)
    assert sub_key == 'sub3'
    sub_type = h.helicsSubscriptionGetType(subid3)
    assert sub_type == 'double_vector'
    sub_units = h.helicsSubscriptionGetUnits(subid3)
    assert sub_units == 'V'
    sub_type = h.helicsSubscriptionGetType(subid2)
    assert sub_type == 'int64'


def test_value_federate_single_transfer(vFed):

    pubid = h.helicsFederateRegisterGlobalTypePublication (vFed, "pub1", h.HELICS_DATA_TYPE_STRING, "");
    subid = h.helicsFederateRegisterSubscription (vFed, "pub1", "string", "");

    h.helicsFederateEnterExecutingMode(vFed)

    h.helicsPublicationPublishString(pubid, "string1")

    grantedtime = h.helicsFederateRequestTime(vFed, 1.0)
    assert grantedtime == 0.01

    s = h.helicsSubscriptionGetString(subid)
    assert s == "string1"

def test_value_federate_runFederateTestDouble(vFed):
    defaultValue = 1.0
    testValue = 2.0
    pubid = h.helicsFederateRegisterGlobalTypePublication (vFed, "pub1", h.HELICS_DATA_TYPE_DOUBLE, "")
    subid = h.helicsFederateRegisterSubscription (vFed, "pub1", "double", "")
    h.helicsSubscriptionSetDefaultDouble(subid, defaultValue)

    h.helicsFederateEnterExecutingMode (vFed)

    # publish string1 at time=0.0;
    h.helicsPublicationPublishDouble(pubid, testValue)

    value = h.helicsSubscriptionGetDouble(subid)
    assert value == defaultValue

    grantedtime = h.helicsFederateRequestTime (vFed, 1.0)
    assert grantedtime == 0.01

    value = h.helicsSubscriptionGetDouble(subid)
    assert value == testValue

    # publish string1 at time=0.0;
    h.helicsPublicationPublishDouble(pubid, testValue + 1)

    grantedtime = h.helicsFederateRequestTime (vFed, 2.0)
    assert grantedtime == 0.02

    value = h.helicsSubscriptionGetDouble(subid)
    assert value == testValue + 1

def test_value_federate_runFederateTestComplex(vFed):
    rDefaultValue = 1.0
    iDefaultValue = 1.0
    rTestValue = 2.0
    iTestValue = 2.0
    pubid = h.helicsFederateRegisterGlobalTypePublication (vFed, "pub1", h.HELICS_DATA_TYPE_COMPLEX, "")
    subid = h.helicsFederateRegisterSubscription (vFed, "pub1", "complex", "")
    h.helicsSubscriptionSetDefaultComplex(subid, rDefaultValue, iDefaultValue)

    h.helicsFederateEnterExecutingMode (vFed)

    # publish string1 at time=0.0;
    h.helicsPublicationPublishComplex(pubid, rTestValue, iTestValue)

    value1, value2 = h.helicsSubscriptionGetComplex(subid)
    assert value1 == rDefaultValue
    assert value2 == iDefaultValue

    grantedtime = h.helicsFederateRequestTime (vFed, 1.0)
    assert grantedtime == 0.01

    value1, value2 = h.helicsSubscriptionGetComplex(subid)
    assert value1 == rTestValue
    assert value2 == iTestValue


def test_value_federate_runFederateTestInteger(vFed):
    defaultValue = 1
    testValue = 2
    pubid = h.helicsFederateRegisterGlobalTypePublication (vFed, "pub1", h.HELICS_DATA_TYPE_INT, "")
    subid = h.helicsFederateRegisterSubscription (vFed, "pub1", "int", "")
    h.helicsSubscriptionSetDefaultInteger(subid, defaultValue)

    h.helicsFederateEnterExecutingMode (vFed)

    h.helicsPublicationPublishInteger(pubid, testValue)

    value = h.helicsSubscriptionGetInteger(subid)
    assert value == defaultValue

    grantedtime = h.helicsFederateRequestTime(vFed, 1.0)
    assert grantedtime == 0.01

    value = h.helicsSubscriptionGetInteger(subid)
    assert value == testValue

    h.helicsPublicationPublishInteger(pubid, testValue + 1)

    grantedtime = h.helicsFederateRequestTime (vFed, 2.0)
    assert grantedtime == 0.02

    value = h.helicsSubscriptionGetInteger(subid)
    assert value == testValue + 1


def test_value_federate_runFederateTestString(vFed):
    defaultValue = "String1"
    testValue = "String2"
    pubid = h.helicsFederateRegisterGlobalTypePublication (vFed, "pub1", h.HELICS_DATA_TYPE_STRING, "")
    subid = h.helicsFederateRegisterSubscription (vFed, "pub1", "string", "")
    h.helicsSubscriptionSetDefaultString(subid, defaultValue)

    h.helicsFederateEnterExecutingMode(vFed)

    h.helicsPublicationPublishString(pubid, testValue)

    value = h.helicsSubscriptionGetString(subid)
    assert value == defaultValue

    grantedtime = h.helicsFederateRequestTime (vFed, 1.0)
    assert grantedtime == 0.01

    value = h.helicsSubscriptionGetString(subid)
    assert value == testValue

def test_value_federate_runFederateTestVectorD(vFed):
    defaultValue = [0, 1, 2]
    testValue = [3, 4, 5]
    pubid = h.helicsFederateRegisterGlobalTypePublication (vFed, "pub1", h.HELICS_DATA_TYPE_VECTOR, "")
    subid = h.helicsFederateRegisterSubscription (vFed, "pub1", "vector", "")
    h.helicsSubscriptionSetDefaultVector(subid, defaultValue)

    h.helicsFederateEnterExecutingMode(vFed)

    h.helicsPublicationPublishVector(pubid, testValue)

    value = h.helicsSubscriptionGetVector(subid)
    assert value == [0, 1, 2]

    grantedtime = h.helicsFederateRequestTime(vFed, 1.0)
    assert grantedtime == 0.01

    value = h.helicsSubscriptionGetVector(subid)
    assert value == [3, 4, 5]
