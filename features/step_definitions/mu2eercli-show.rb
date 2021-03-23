#!/usr/bin/ruby
#
# mu2eercli-tclk.rb
#
# The file contains the Cucumber step definitions for testing the mu2eercli "show" command.
#
# @author jdiamond and rtadkins

require 'rspec/expectations'

Then("{string} is displayed as the TCLK Driver") do |expectedDriver|
  driver = @result.match(/TCLK device: (.*)/)
  expect( driver[1] ).to eq expectedDriver
end

Then("{string} is displayed as the TCLK signal status") do |expectedStatus|
  status = @result.match(/TCLK signal status: (.*)/)
  expect( status[1] ).to eq expectedStatus
end
