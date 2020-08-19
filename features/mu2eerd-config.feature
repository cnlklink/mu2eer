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

  Scenario: Could not find the configuration file passed with the -c option

  Scenario: Could not parse the configuration file
