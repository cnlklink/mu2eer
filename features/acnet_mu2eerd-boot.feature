Feature: Mu2eerd startup device (x:SLWXSTARTUP)

  Provides control over the mu2eerd daemon boot procedure.

  @acnet
  Scenario: I do not want the mu2eerd daemon process to start on boot-up.
    Given the mu2eerd daemon process is not running
    When I set the ACNET device "Z:SLWXSTARTUP" to "OFF"
      And I request the status property for the ACNET device "Z:SLWXSTATUS"
    Then I receive a value of "NOT_RUN" for the "mu2eerd process" bit
