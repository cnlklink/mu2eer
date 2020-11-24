Feature: mu2eercli show command / Spill State Machine display

  Provides status and diagnostics for the Spill State Machine (SSM) via the show command.

  Scenario: display initial SSM state to the user
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

  Scenario: display the initial SSM thread state to the user
    Given mu2eerd is running
    When I pass the "show" command to mu2eercli
    Then the SSM thread state displayed is "not running"
    And an exit code of 0 is returned

  Scenario: display the LED state to the user
    Given mu2eerd is running
    When I pass the "show" command to mu2eercli
    Then the LED state displayed is true
    And an exit code of 0 is returned

  Scenario: Display SSM state after 5 spills
    Given mu2eerd is running with the "-c etc/mu2eer.d/reference.conf" flags
      And the spill state machine has been started
    When I pass the "show" command to mu2eercli
    Then the SSM state displayed is "fault"
      And the spill counter displayed is 5
      And the time-in-spill displayed is 107
      And the SSM thread state displayed is "running"
      And the LED state displayed is true
      And an exit code of 0 is returned
