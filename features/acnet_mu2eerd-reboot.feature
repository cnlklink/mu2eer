Feature: Mu2eerd status & control device (x:SLWXSTATUS) reboot command

  Provides an ACNET command to remotely reboot the front end.

  @acnet
  Scenario: I want to reboot the front end.
    Given the mu2eerd daemon process is running
    When I send the "soft reboot" command to the ACNET device "Z:SLWXSTATUS"
      And I wait 65 seconds
      And I request the reading property for the ACNET device "Z:SLWXSTATUS[0]"
    Then I receive a value less than 65
