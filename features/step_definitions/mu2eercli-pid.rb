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

When("I pass the pid command to mu2eercli") do
  @result = `./bin/host/mu2eercli/mu2eercli pid`
  @rc = $?
end

Then("the PID for mu2eerd should be displayed") do
  expect( @result ).to eq @expectedPID
end

Then("an exit code of {int} is returned") do |returnCode|
  expect( @rc ).to eq returnCode
end

Then("{string} is displayed") do |err|
  expect( @result ).to eq err
end
