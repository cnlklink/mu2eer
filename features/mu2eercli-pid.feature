Feature: mu2ecli pid command

  Provides an easy way for a command-line user to find the PID for the mu2eerd process.

  Scenario: mu2eerd process is running
    Given mu2eerd is running
    When I pass the pid command to mu2eercli
    Then the PID for mu2eerd should be displayed
    And an exit code of 0 is returned

  Scenario: mu2eerd process is not running
    Given mu2eerd is not running
    When I pass the pid command to mu2eercli
    Then "mu2eerd is not running." is displayed
    And an exit code of 1 is returned
