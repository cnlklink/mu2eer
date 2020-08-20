Feature: mu2eerd configuration file

  Load the mu2eerd configuration from a file.

  Scenario: Use the default configuration
    Given mu2eerd is not running
      And the hostname of this machine is <hostname>
      And /etc/mu2eer.d/<hostname>-mu2eerd.conf does not exist
    When I start mu2eerd with no flags
      And I pass the "show" command to mu2eercli
    Then the configuration file displayed is "none (default configuration loaded)"

  Scenario: Load configuration from /etc/mu2eer.d/hostname-mu2eerd.conf
    Given mu2eerd is not running
      And the hostname of this machine is <hostname>
      And /etc/mu2eer.d/<hostname>-mu2eerd.conf does exist
    When I start mu2eerd with no flags
      And I pass the "show" command to mu2eercli
    Then the configuration file displayed is "/etc/mu2eer.d/<hostname>-mu2eerd.conf"

  Scenario: Specify a configuration file with the -c option
    Given mu2eerd is not running
      And there is a copy of etc/mu2eer.d/reference.conf at /etc/mu2eer.d/example.conf
    When I start mu2eerd with the "-c /etc/mu2eer.d/example.conf" flags
      And I pass the "show" command to mu2eercli
    Then the configuration file displayed is "/etc/mu2eer.d/example.conf"

  Scenario: Could not find the configuration file passed with the -c option
    Given mu2eerd is not running
    When I start mu2eerd with the "-c /etc/mu2eer.d/does_not_exist.conf" flags
    Then an exit code of 2 is returned
      And "The configuration file specified with -c option was not found or could not be accessed." is displayed

  Scenario: Could not parse the configuration file
    Given mu2eerd is not running
    When I start mu2eerd with the "-c etc/mu2eer.d/parse-error.conf" flags
    Then an exit code of 2 is returned
      And "Failed to parse configuration file" is displayed
