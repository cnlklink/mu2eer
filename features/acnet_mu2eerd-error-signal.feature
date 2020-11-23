Feature: Mu2eerd error signal device (x:SLWXESPILL)

  Allows for the reading of the error signal memory block

  @acnet
  @error-signal
  Scenario: I want to read the error spill memory block
    Given the mu2eerd daemon process is running
    When I request the reading property for the ACNET device "Z:SLWXESPILL[]"
    Then I receive an array of error signal values like the following:
      |index|value |
      |0    |0     |
      |1    |0     |
      |2    |0     |
