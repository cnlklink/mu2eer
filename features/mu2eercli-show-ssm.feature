Feature: mu2eercli show command / Spill State Machine display

  Provides status and diagnostics for the Spill State Machine (SSM) via the show command.

  Scenario: display SSM state to the user
    Given mu2eerd is running
    When I pass the "show" command to mu2eercli
    Then the SSM state displayed is "idle"
    And an exit code of 0 is returned

  Scenario: display the initial spill counter to the user
    Given mu2eerd is running
    When I pass the "show" command to mu2eercli
    Then the spill counter displayed is 0
    And an exit code of 0 is returned

  Scenario: display the initial time-in-spill to the user
    Given mu2eerd is running
    When I pass the "show" command to mu2eercli
    Then the time-in-spill displayed is 0
    And an exit code of 0 is returned
