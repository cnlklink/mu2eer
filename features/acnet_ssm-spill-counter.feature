Feature: SSM Spill Counter ACNET device (x:SLWXSPILLS)

  Provide the ability to read the spill counter from ACNET

  @acnet
  Scenario: I want to read the initial spill counter from ACNET
    Given the "RESET" command has been sent to ACNET device "Z:SLWXSSM"
    When I request the reading property for the ACNET device "Z:SLWXSPILLS"
    Then I receive a value of "0"

  @acnet
  Scenario: I want to read the spill counter from ACNET after processing a couple of spills
    Given the "RESET" command has been sent to ACNET device "Z:SLWXSSM"
      And the "START" command has been sent to ACNET device "Z:SLWXSSM"
    When I request the reading property for the ACNET device "Z:SLWXSPILLS"
    Then I receive a value of "5"
