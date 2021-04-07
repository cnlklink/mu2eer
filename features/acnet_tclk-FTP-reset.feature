Feature: TCLK FTP reset event counter (x:SLWXFTPRESETS)

  The Fast Time Plot Facility depends on a TCLK decoder to timestamp the data to be plotted.  An ACNET device called
  x:SLWXFTPRESETS is provided that counts FTP reset events as they are decoded.  This device can be used to
  verify that the TCLK decoder is operating at a basic level.

  @acnet
  Scenario: I want to observe that FTP reset events are being received
    Given the mu2eerd daemon process is running
    When I request the reading property for the ACNET device "Z:SLWXFTPRESETS"
      And I wait 5 seconds
      And I request the reading property again
    Then the difference between the requests is within 1 or 2
