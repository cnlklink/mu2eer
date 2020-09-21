#!/usr/bin/ruby
#
# mu2eercli-pid.rb
#
# The file contains the Cucumber step definitions for testing the mu2eercli "pid" command.
#
# @author jdiamond

require 'rspec/expectations'
require 'csv'

After do
  # mu2eerd should not be left running!
  pid = `pidof mu2eerd`
  if pid != ""
    `kill #{pid}`
  end

  # And the /etc/mu2eer.d directory should be cleared
  if File.directory? "/etc/mu2eer.d"
    FileUtils.rm Dir.glob "/etc/mu2eer.d/*"
  end
end

Given("the hostname of this machine is <hostname>") do
  # Save the hostname for use by other step definitions (-s cuts off at the first dot)
  @hostname = `hostname -s`
  @hostname.strip!
end

Given(/^mu2eerd (is|is not) running$/) do |running|
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

Given(/mu2eerd is running with the "(.*)" flags/) do |flags|
  @result = `./bin/host/mu2eerd/mu2eerd #{flags}`
  @expectedPID = `pidof mu2eerd`
  expect( `pidof mu2eerd` ).not_to eq ""
end

Given("the spill state machine has been started") do
   `./bin/host/mu2eercli/mu2eercli start`
end

Given("the spill state machine is in the fault state" ) do
  @result = `./bin/host/mu2eercli/mu2eercli fault`
  expect( $?.exitstatus ).to eq 0
end

Given(/(.*) (does|does not) exist/) do |file_name, does_or_doesnt|
  # replace <hostname> with this node's hostname
  expect( @hostname ).not_to be_nil
  file_name.sub! /<hostname>/, @hostname

  if does_or_doesnt == "does"
    # First, expect that the file does not already exist on the system
    expect( File.file? file_name ).not_to be_truthy

    # Second, expect the file to exist in our source tree
    source_file = "." + file_name
    expect( File.file? source_file ).to be_truthy

    # And copy it to the system location
    FileUtils.cp source_file, file_name
  else
    expect( File.file? file_name ).not_to be_truthy
  end
end

Given(/there is a copy of (.*) at (.*)/) do |src_file_name, dest_file_name|
  # Expect src_file_name to exist
  expect( File.file? src_file_name ).to be_truthy

  # And the dest_file_name does not exist
  expect( File.file? dest_file_name ).not_to be_truthy

  # Then copy it there
  FileUtils.cp src_file_name, dest_file_name
  expect( File.file? dest_file_name ).to be_truthy
end

When("I pass the {string} command to mu2eercli") do |command|
  # redirect stderr to stdout so error messages are captured in @result
  @result = `./bin/host/mu2eercli/mu2eercli #{command} 2>&1`
  @rc = $?.exitstatus
end

When("I start mu2eerd with no flags") do
  # redirect stderr to stdout so error messages are captured in @result
  @result = `./bin/host/mu2eerd/mu2eerd 2>&1`
  @rc = $?.exitstatus
end

When("I start mu2eerd with the {string} flags") do |flags|
  # redirect stderr to stdout so error messages are captured in @result
  @result = `./bin/host/mu2eerd/mu2eerd #{flags} 2>&1`
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

Then("the configuration file displayed is {string}") do |expected_config_file_name|
  # Expect the config file to be present in the output
  expect( @result ).to match /with configuration: /

  # Extract the configuration file name
  config_file_name = @result.match /with configuration: (.*)/

  # Replace <hostname> in the expected file name
  if expected_config_file_name.match /<hostname>/ then
    expected_config_file_name.sub! /<hostname>/, @hostname
  end

  # Expect the configuration file to match
  expect( config_file_name[1] ).to eq expected_config_file_name
end

Then("the contents of the shared memory displayed are {string}") do |shared_memory|
  # Expect the array contents to be present in the output
  table = CSV.parse(@result)
  table.delete(0)
  table do |row|
    expect( row ).to match /i: \d+/
  end
end
