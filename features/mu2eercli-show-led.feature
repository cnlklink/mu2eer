Feature: LED

Verifies the state of the LED in different scenarios.
@led
Scenario: mu2eerd process is running
  Given mu2eerd is running
    And the spill state machine is in the fault state
  When I pass the "show" command to mu2eercli
  Then the LED state displayed is 0
    And an exit code of 0 is returned

@led
Scenario: mu2eerd process is running
  Given mu2eerd is running
  When I pass the "show" command to mu2eercli
  Then the LED state displayed is 1
  And an exit code of 0 is returned
