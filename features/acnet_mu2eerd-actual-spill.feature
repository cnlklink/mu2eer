Feature: Mu2eerd ideal spill device (x:SLWXASPILL)

  Provides control and status for the slow extraction daemon process and the front end.

  @acnet
  Scenario: I want to know if the mu2eerd daemon process is running.
    Given the mu2eerd daemon process is running
    When I request the reading property for the ACNET device "Z:SLWXASPILL"
    Then I receive a value of "RUNNING" for the "mu2eerd process" bit
      And I receive a value of "YES" for the "shared memory connected" bit
      And I receive a value of "YES" for the "control queue connected" bit

  @acnet
  Scenario: I want to know if the mu2eerd daemon process is not running.
    Given the mu2eerd daemon process is not running
    When I request the reading property for the ACNET device "Z:SLWXASPILL"
    Then I receive a value of "NOT_RUN" for the "mu2eerd process" bit
      And I receive a value of "NO" for the "shared memory connected" bit
      And I receive a value of "NO" for the "control queue connected" bit

  @acnet
  Scenario: I want to start the mu2eerd daemon process.
    Given the mu2eerd daemon process is not running
    When I send the "start" command to the ACNET device "Z:SLWXASPILL"
      And I request the status property for the ACNET device "Z:SLWXASPILL"
    Then I receive a value of "RUNNING" for the "mu2eerd process" bit

  @acnet
  Scenario: I want to stop the mu2eerd daemon process.
    Given the mu2eerd daemon process is running
    When I send the "stop" command to the ACNET device "Z:SLWXASPILL"
      And I request the status property for the ACNET device "Z:SLWXASPILL"
    Then I receive a value of "NOT_RUN" for the "mu2eerd process" bit

  @acnet
  @actual-spill
  Scenario: I want to read the actual spill memory block
    Given the mu2eerd daemon process is running
    When I request the reading property for the ACNET device "Z:SLWXASPILL[]"
    Then I receive an array of actual spill values like the following:
      |index|value  |
      |0    |15999  |
      |1    |15998  |
      |2    |15997  |
