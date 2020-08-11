Feature: mu2eercli show command / Spill State Machine display

  Provides status and diagnostics for the Spill State Machine (SSM) via the show command.

  Scenario: display SSM state to the user
    Given mu2eerd is running
    When I pass the "show" command to mu2eercli
    Then the SSM state displayed is "idle"
    And an exit code of 0 is returned
