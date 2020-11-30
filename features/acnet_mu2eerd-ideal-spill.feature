Feature: Mu2eerd ideal spill device (x:SLWXSPILL)

  Allows for the reading of the ideal spill memory block	  

  @acnet
  @ideal-spill
  Scenario: I want to read the ideal spill memory block
    Given the mu2eerd daemon process is running
    When I request the reading property for the ACNET device "Z:SLWXSPILL[]"
    Then I receive an array of ideal spill values