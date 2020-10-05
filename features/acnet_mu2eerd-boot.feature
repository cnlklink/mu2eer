Feature: Mu2eerd startup device (x:SLWXSTARTUP)

  Provides control over the mu2eerd daemon boot procedure.

  @acnet
  Scenario: I do not want the mu2eerd daemon process to start on boot-up.
    Given the mu2eerd daemon process is not running
    When I set the ACNET device "Z:SLWXSTARTUP" to "OFF"
      And I request the status property for the ACNET device "Z:SLWXSTATUS"
    Then I receive a value of "NOT_RUN" for the "mu2eerd process" bit

  @acnet
  Scenario: mu2eerd is already running, but I do not want it to start on the next boot
    Given the mu2eerd daemon process is running
    When I set the ACNET device "Z:SLWXSTARTUP" to "OFF"
      And I request the status property for the ACNET device "Z:SLWXSTATUS"
    Then I receive a value of "RUNNING" for the "mu2eerd process" bit

  @acnet
  Scenario: If mu2eerd is already running, setting ON_OPER does nothing
    Given the mu2eerd daemon process is running
    When I set the ACNET device "Z:SLWXSTARTUP" to "ON_OPER"
      And I request the status property for the ACNET device "Z:SLWXSTATUS"
    Then I receive a value of "RUNNING" for the "mu2eerd process" bit

  @acnet
  Scenario: If mu2eerd is already running, setting ON_TEST does nothing
    Given the mu2eerd daemon process is running
    When I set the ACNET device "Z:SLWXSTARTUP" to "ON_TEST"
      And I request the status property for the ACNET device "Z:SLWXSTATUS"
    Then I receive a value of "RUNNING" for the "mu2eerd process" bit

  @acnet
  Scenario: If mu2eerd is not running, setting ON_OPER starts mu2eerd with the operational configuration
    Given the mu2eerd daemon process is not running
    When I set the ACNET device "Z:SLWXSTARTUP" to "ON_OPER"
      And I request the reading property for the ACNET device "Z:SLWXCONFIG"
    Then I receive a value of "/etc/mu2eer.d/srsd-mu2eerd.conf"

  @acnet
  Scenario: If mu2eerd is not running, setting ON_TEST starts mu2eerd with the test configuration
    Given the mu2eerd daemon process is not running
    When I set the ACNET device "Z:SLWXSTARTUP" to "ON_TEST"
      And I request the reading property for the ACNET device "Z:SLWXCONFIG"
    Then I receive a value of "/etc/mu2eer.d/reference.conf"
