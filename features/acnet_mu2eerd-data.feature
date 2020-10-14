Feature: Mu2eerd status & control device (x:SLWXSPILL)

  Provides control and status for the slow extraction daemon process and the front end.

  @acnet_read_only
  Scenario: I want to read the daemon status block
    Given the mu2eerd daemon process is running
    When I request the reading property for the ACNET device "Z:SLWXSPILL[]"
    Then I receive an array of linear structured data like the following:
      |entry|value  |
      |0    |16000  |
      |1    |15999  |
      |2    |15998  |
