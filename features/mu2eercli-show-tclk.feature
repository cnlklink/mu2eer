Feature: mu2eercli show command / tclk status display

  Provides the mu2eerd TCLK Decoder status via the show command.

  Scenario: display the TCLK decoder status when using the mock driver
    Given mu2eerd is running
    When I pass the "show" command to mu2eercli
    Then "Mock" is displayed as the TCLK Driver
     And "Present" is displayed as the TCLK signal status
