Feature: SSM State device (x:SLWXSSM)

  Provides status and control for the Spill State Machine via ACNET.

  @acnet
  Scenario: Read the initial state of the spill state machine
    Given the "RESET" command has been sent to ACNET device "Z:SLWXSSM"
    When I request the reading property for the ACNET device "Z:SLWXSSM"
    Then I receive a value of "0"

  @acnet
  Scenario: Read the spill state machine state after running a couple of spills
    Given the "RESET" command has been sent to ACNET device "Z:SLWXSSM"
    When I send the "START" command to the ACNET device "Z:SLWXSSM"
      And I request the reading property for the ACNET device "Z:SLWXSSM"
    Then I receive a value of "254"

  @acnet
  Scenario: Reset the spill state machine
    Given the spill state machine is in the FAULT state
    When I send the "RESET" command to the ACNET device "Z:SLWXSSM"
      And I request the reading property for the ACNET device "Z:SLWXSPILLS"
    Then I receive a value of "0"
