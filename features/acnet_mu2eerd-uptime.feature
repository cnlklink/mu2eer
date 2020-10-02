Feature: Mu2eerd status & control device (x:SLWXSTATUS)

  Provides control and status for the slow extraction daemon process and the front end.
  
  @acnet
  Scenario: I want to know if the mu2eerd daemon process is running.
    Given the mu2eerd daemon process is running
    When I request the status property for the ACNET device "Z:SLWXSTATUS"
    Then I receive a value of "RUNNING" for the "mu2eerd process" bit
      And I receive a value of "YES" for the "shared memory connected" bit
      And I receive a value of "YES" for the "control queue connected" bit

  @acnet
  Scenario: I want to know if the mu2eerd daemon process is not running.
    Given the mu2eerd daemon process is not running
    When I request the status property for the ACNET device "Z:SLWXSTATUS"
    Then I receive a value of "NOT_RUN" for the "mu2eerd process" bit
      And I receive a value of "NO" for the "shared memory connected" bit
      And I receive a value of "NO" for the "control queue connected" bit

  @acnet
  Scenario: I want to start the mu2eerd daemon process.
    Given the mu2eerd daemon process is not running
    When I send the "start" command to the ACNET device "Z:SLWXSTATUS"
      And I request the status property for the ACNET device "Z:SLWXSTATUS"
    Then I receive a value of "RUNNING" for the "mu2eerd process" bit

  @acnet
  Scenario: I want to stop the mu2eerd daemon process.
    Given the mu2eerd daemon process is running
    When I send the "stop" command to the ACNET device "Z:SLWXSTATUS"
      And I request the status property for the ACNET device "Z:SLWXSTATUS"
    Then I receive a value of "NOT_RUN" for the "mu2eerd process" bit

  @acnet
  Scenario: I want to read the uptime for the mu2eerd daemon process.
    Given the mu2eerd daemon process is running
    When I request the reading property for the ACNET device "Z:SLWXSTATUS[0]"
      And I wait 2 seconds
      And I request the reading property again
    Then the difference between the requests is within 1 of 2

  @acnet
  Scenario: I want to read the PID for the mu2eerd daemon process.
    Given the mu2eerd daemon process is running
    When I request the reading property for the ACNET device "Z:SLWXSTATUS[1]"
    Then I receive a valid PID

  @acnet
  Scenario: I want to read the Jenkins build number for the target's Buildroot image
    Given the mu2eerd daemon process is running
    When I request the reading property for the ACNET device "Z:SLWXSTATUS[2]"
    Then I receive a value greater than 100

  @acnet
  Scenario: I want to read the daemon status block
    Given the mu2eerd daemon process is running
    When I request the reading property for the ACNET device "Z:SLWXSTATUS[]"
    Then I receive an array of structured data like the following:
      |index|value           |
      |0    |greater than 0  |
      |1    |a valid PID     |
      |2    |greater than 100|
