Feature: SSM Time-in-Spill device (x:SLWXTIS)

  Provide the ability to read the duration of the last spill cycle.

  @acnet
  Scenario: I want to read the initial time-in-spill from ACNET
    Given the "RESET" command has been sent to ACNET device "Z:SLWXSSM"
    When I request the reading property for the ACNET device "Z:SLWXTIS"
    Then I receive a value of "0"

  @acnet
  Scenario: I want to read the time-in-spill after processing a couple of spills
    Given the "RESET" command has been sent to ACNET device "Z:SLWXSSM"
      And the "START" command has been sent to ACNET device "Z:SLWXSSM"
    When I request the reading property for the ACNET device "Z:SLWXTIS"
    Then I receive a value of "107"
