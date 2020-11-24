Feature: Green LED

Verifies the state of the LED in different scenarios.

Scenario: mu2eerd process is running
  Given mu2eerd is running
    And the spill state machine is in the fault state
  When I pass the "show" command to mu2eercli
  Then the LED state displayed is false
    And an exit code of 0 is returned

Scenario: mu2eerd process is running
  Given mu2eerd is running
  When I pass the "show" command to mu2eercli
  Then the LED state displayed is true
  And an exit code of 0 is returned
