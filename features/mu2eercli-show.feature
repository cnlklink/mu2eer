Feature: mu2eercli show command

  Provides a command to show a system status overview.

  Scenario: mu2eerd process is not running
    Given mu2eerd is not running
    When I pass the "show" command to mu2eercli
    Then an error message is displayed
    And an exit code of 1 is returned

  Scenario: mu2eerd process is running
    Given mu2eerd is running
    When I pass the "show" command to mu2eercli
    Then "mu2eerd running since" is in the output
    And "uptime:" is in the output
    And an exit code of 0 is returned
