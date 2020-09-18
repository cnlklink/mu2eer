Feature: mu2eercli dump command / shared memory display

  Provides the contents of the shared memory for mu2eerd via the dump command.

  Scenario: display the shared memory to the user
    Given mu2eerd is running
    When I pass the "dump" command to mu2eercli
    Then the contents of the shared memory for mu2eerd are displayed
    And an exit code of 0 is returned
