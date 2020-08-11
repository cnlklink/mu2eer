Feature: mu2eercli show command / pid display

  Provides the PID for mu2eerd via the show command.

  Scenario: display the mu2eerd PID to the user
    Given mu2eerd is running
    When I pass the "show" command to mu2eercli
    Then the PID for mu2eerd is displayed
    And an exit code of 0 is returned
