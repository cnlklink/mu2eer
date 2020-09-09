Feature: mu2eercli reset command

  Provides a command to reset the spill state machine from the command-line.

  Scenario: I want to reset the spill state machine
    Given mu2eerd is running
      And the spill state machine is in the fault state
    When I pass the "reset" command to mu2eercli
    Then "Spill state machine has been reset." is displayed
      And an exit code of 0 is returned

  Scenario: I want to run the test spills twice
    Given mu2eerd is running
    When I pass the "start" command to mu2eercli
      And I pass the "reset" command to mu2eercli
      And I pass the "start" command to mu2eercli
      And I pass the "show" command to mu2eercli
    Then the SSM state displayed is "fault"
      And the spill counter displayed is 5
      And the time-in-spill displayed is 107
      And an exit code of 0 is returned
