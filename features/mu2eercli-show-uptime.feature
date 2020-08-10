Feature: mu2eercli show command / uptime display

  Provides the up-time for mu2eerd via the show command.

  Scenario: display the mu2eerd uptime to the user
    Given mu2eerd is running
    When I pass the "show" command to mu2eercli
    Then the uptime for mu2eerd is displayed
    And the start time for mu2eerd is displayed
    And an exit code of 0 is returned
