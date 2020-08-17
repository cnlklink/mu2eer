#!/usr/bin/ruby
#
# mu2eercli-pid.rb
#
# The file contains the Cucumber step definitions for testing the mu2eercli "pid" command.
#
# @author jdiamond

require 'rspec/expectations'

After do
  pid = `pidof mu2eerd`
  if pid != ""
    `kill #{pid}`
  end
end

Given(/mu2eerd (is|is not) running/) do |running|
  if running == "is"
    if !system( "./bin/host/mu2eerd/mu2eerd" )
      puts "Failed to start mu2eerd!"
    end
    
    @expectedPID = `pidof mu2eerd`
  else
    # Do not start mu2eerd...
    expect( `pidof mu2eerd` ).to eq ""
  end
end

Given("the spill state machine has been started") do
   `./bin/host/mu2eercli/mu2eercli start`
end

When("I pass the {string} command to mu2eercli") do |command|
  @result = `./bin/host/mu2eercli/mu2eercli #{command}`
  @rc = $?.exitstatus
end

Then("the PID for mu2eerd should be displayed") do
  expect( @result ).to eq @expectedPID
end

Then("an exit code of {int} is returned") do |returnCode|
  expect( @rc ).to eq returnCode
end

Then(/(a confirmation|an error) message is displayed/) do |phrase|
  expect( @result != "" )
end

Then("within {int} seconds the mu2eerd process terminates") do |timeout|
  expect( `pidof mu2eerd` ).to eq ""
end 

Then("{string} is in the output") do |needle|
  expect( @result ).to match needle
end

Then("the uptime for mu2eerd is displayed") do
  # Expect the uptime to be present in the output
  expect( @result ).to match /uptime: \d+/

  # Extract uptime
  uptime = @result[/uptime: \d+/][/\d+/].strip.to_i

  # Get the runtime with `ps` and compare to what mu2ercli gives
  # allow the two to be within +/- 2 seconds
  pid=`pidof mu2eerd`.strip.to_i
  expect( uptime ).to be_within( 2 ).of( `ps -p #{pid} -o etimes | tail -1`.strip.to_i )
end

Then("the start time for mu2eerd is displayed") do
  # Expect the start time to be present in the output
  expect( @result ).to match /running since (\d{4})-(\d{2})-(\d{2}) (\d{2}):(\d{2}):(\d{2})/

  # Extract date and time
  dAndT = @result.match(/running since (\d{4})-(\d{2})-(\d{2}) (\d{2}):(\d{2}):(\d{2})/)

  # Get the start time with `ps` and compare to what mu2eercli gives
  pid=`pidof mu2eerd`.strip.to_i
  expectedDAndT = `ps -p #{pid} -o lstart | tail -1`.match(/(\d{2}):(\d{2}):(\d{2}) (\d{4})/)
  expect( dAndT[1] ).to eq expectedDAndT[4]
  expect( dAndT[4] ).to eq expectedDAndT[1]
  expect( dAndT[5] ).to eq expectedDAndT[2]
  expect( dAndT[6].to_i ).to be_within( 2 ).of( expectedDAndT[3].to_i )
end

Then("the PID for mu2eerd is displayed") do
  # Expect the PID to be present in the output
  expect( @result ).to match /pid: \d+/

  # Extract the PID
  pid = @result.match /pid: (\d+)/
  
  # Use `pidof` to verify
  expect( pid[1].to_i ).to eq `pidof mu2eerd`.strip.to_i
end

Then("the SSM state displayed is {string}") do |expected_state_string| 
  # Expect the SSM state to be present in the output
  expect( @result ).to match /SSM state: /

  # Extract the SSM state
  state = @result.match /SSM state: (.*)/

  # Expect the state to match
  expect( state[1] ).to eq expected_state_string
end

Then("the spill counter displayed is {int}") do |expected_spills|
  # Expect the spill counter to be present in the output
  expect( @result ).to match /Spill counter: /

  # Extract the spill count
  spills = @result.match /Spill counter: (\d+)/

  # Expect the spills to be equal
  expect( spills[1].to_i ).to eq expected_spills
end

Then("the time-in-spill displayed is {int}") do |expected_tis|
  # Expect the time-in-spill to be present in the output
  expect( @result ).to match /Time-in-spill \(from last cycle\): /
end

Then("the SSM thread state displayed is {string}") do |expected_thread_state|
  # Expect the thread state to be present in the output
  expect( @result ).to match /SSM thread is/

  # Extract the thread state
  thread_state = @result.match /SSM thread is (.*)/

  # Expect the thread state to match
  expect( thread_state[1] ).to eq expected_thread_state
end

Then("{string} is displayed") do |message|
  # Expect the message to be present in the output
  expect( @result ).to match message
end

