/**
 * ConfigurationManager.C
 *
 * This file contains unit tests (CppUTest) for the ConfigurationManager class.
 *
 * @author jdiamond
 */

#include "CppUTest/TestHarness.h"

#include "ConfigurationManager.H"

using namespace Mu2eER;
using namespace std;

/**
 * A global ConfigurationManager object used for all tests
 */
static ConfigurationManager* _cm;

/**
 * Core Group
 *
 * Tests related to loading and querying configurations.
 */
TEST_GROUP( CoreGroup )
{
  void setup()
  {
    _cm = new ConfigurationManager();
  }

  void teardown()
  {
    delete _cm;
  }
};

/**
 * Test Default Configuration
 *
 * Verify that the default configuration is loaded by default and is what we expect.
 */
TEST( CoreGroup, DefaultConfig )
{
  STRCMP_EQUAL( DEFAULT_CONFIG_FILE_PATH, _cm->configFileGet().c_str() );

  STRCMP_EQUAL( "mock", _cm->tclkDriverGet().c_str() );

  STRCMP_EQUAL( "mock", _cm->ssmGet().driverGet().c_str() );
  CHECK_EQUAL( false, _cm->ssmGet().autoInitGet() );
  CHECK_EQUAL( 0, _cm->ssmGet().mockSpillsGet() );
  CHECK_EQUAL( 0, _cm->ssmGet().mockDelayGet() );
}

/**
 * Test Get Host Config File
 *
 * Verify that the static hostConfigFileGet() method returns a file name in the form
 * /etc/mu2eer.d/<hostname>-mu2eerd.conf.
 *
 * Note: this test will fail if the host it's running on is not added to the test conditional.
 */
TEST( CoreGroup, HostConfigFile )
{
  auto filename = ConfigurationManager::hostConfigFileGet();

  CHECK( filename == "/etc/mu2eer.d/adlinux-mu2eerd.conf" ||
         filename == "/etc/mu2eer.d/srsd-mu2eerd.conf" );
}

/**
 * Test Bad Config
 *
 * Verify that excpetions are thrown for bad configuration files.
 */
TEST( CoreGroup, LoadBadConfig )
{
  CHECK_THROWS( config_error, _cm->load( "../etc/mu2eer.d/parse-error.conf" ) );
}

/**
 * Test Load Configuration
 *
 * Verify that we can load the reference configuration.
 */
TEST( CoreGroup, LoadReferenceConfig )
{
  CHECK_THROWS( config_error, _cm->load( "file_does_not_exist" ) );

  _cm->load( "../etc/mu2eer.d/reference.conf" );
  
  STRCMP_EQUAL( "../etc/mu2eer.d/reference.conf", _cm->configFileGet().c_str() );

  STRCMP_EQUAL( "mock", _cm->tclkDriverGet().c_str() );

  STRCMP_EQUAL( "mock", _cm->ssmGet().driverGet().c_str() );
  CHECK_EQUAL( true, _cm->ssmGet().autoInitGet() );
  CHECK_EQUAL( 5, _cm->ssmGet().mockSpillsGet() );
  CHECK_EQUAL( 5, _cm->ssmGet().mockDelayGet() );
}

/**
 * Test Reload Configuration
 *
 * Verify that we can re-load a configuration.
 */
TEST( CoreGroup, ReloadConfig )
{
  _cm->load( "../etc/mu2eer.d/reference.conf" );
  
  STRCMP_EQUAL( "../etc/mu2eer.d/reference.conf", _cm->configFileGet().c_str() );

  STRCMP_EQUAL( "mock", _cm->tclkDriverGet().c_str() );

  _cm->tclkDriverSet( "real" );
  STRCMP_EQUAL( "real", _cm->tclkDriverGet().c_str() );

  _cm->reload();
  STRCMP_EQUAL( "mock", _cm->tclkDriverGet().c_str() );
}
