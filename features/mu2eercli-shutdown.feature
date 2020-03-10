Feature: mu2eercli shutdown command

  Provides a command to properly terminate the mu2eerd process from the command-line.

  Scenario: mu2eerd process is running
    Given mu2eerd is running
    When I pass the "shutdown" command to mu2eercli
    Then within 3 seconds the mu2eerd process terminates
    And a confirmation message is displayed
    And an exit code of 0 is returned

  Scenario: mu2eerd process is not running
    Given mu2eerd is not running
    When I pass the "shutdown" command to mu2eercli
    Then an error message is displayed
    And an exit code of 1 is returned
