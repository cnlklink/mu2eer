Feature: mu2eercli start command

  Provides a command to startup the spill state machine from the command-line.

  Scenario: Start the spill state machine
    Given mu2eerd is running
    When I pass the "start" command to mu2eercli
    Then "Spill state machine successfully started." is displayed
    And an exit code of 0 is returned

  Scenario: Warn the user if the spill state machine is already running
    Given mu2eerd is running
    And the spill state machine has been started
    When I pass the "start" command to mu2eercli
    Then "The spill state machine is already running." is displayed
    And an exit code of 1 is returned

  Scenario: Inspect spill state machine status with the show command
    Given mu2eerd is running
    And the spill state machine has been started
    When I pass the "show" command to mu2eercli
    Then the SSM thread state displayed is "running"
