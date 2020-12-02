Feature: Mu2eerd actual spill device (x:SLWXASPILL)

  Allows for the reading of the actual spill memory block

  @acnet
  @actual-spill
  Scenario: I want to read the actual spill memory block
    Given the mu2eerd daemon process is running
    When I request the reading property for the ACNET device "Z:SLWXASPILL[]"
    Then I receive an array of actual spill values

