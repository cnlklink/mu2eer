Feature: Green LED

Verifies the state of the LED in different scenarios.

Scenario: I want to check that the LED is off
  Given mu2eerd is running
    And the spill state machine is in the fault state
  When I query the state of the LED
  Then the I receive a value of "false"
    And an exit code of 0 is returned

Scenario: I want to check that the LED is on
  Given mu2eerd is running
  When I pass the "start" command to mu2eercli
    And I query the state of the LED
  Then the I receive a value of "true"
  And an exit code of 0 is returned

Scenario: I want to check that the LED is on
  Given mu2eerd is running
  When I pass the "shutdown" command to mu2eercli
    And I query the state of the LED
  Then I receive a value of "false"
  And an exit code of 0 is returned
