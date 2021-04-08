#!/usr/bin/ruby
#
# mu2eercli-tclk.rb
#
# The file contains the Cucumber step definitions for testing the mu2eercli "show" command.
#
# @author jdiamond and rtadkins

require 'rspec/expectations'

Then("{string} is displayed as the TCLK Driver") do |expectedDriver|
  driver = @result.match(/Device driver: (.*)/)
  expect( driver[1] ).to eq expectedDriver
end

Then("{string} is displayed as the TCLK signal status") do |expectedStatus|
  status = @result.match(/TCLK signal status: (.*)/)
  expect( status[1] ).to eq expectedStatus
end

Then("a value less than or equal to {int} is displayed for the FTP reset event counter") do |count|
  expect( @result ).to match(/FTP reset event counter: (\d+)/)
  eventCounterMatch = @result.match(/FTP reset event counter: (\d+)/)
  expect( eventCounterMatch[1].to_i ).to be <= count
end
