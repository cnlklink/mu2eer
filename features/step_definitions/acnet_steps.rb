#!/usr/bin/ruby
#
# acnet_steps.rb
#
# This file contains Cucumber step definitions that handle ACNET requests.
#
# @author jdiamond

require 'rspec/expectations'

###
# acnet_request_read
#
# Make an ACNET reading request.
#
# @param request DRF2 request string
# @return result
##
def acnet_request_read ( request )
  acl_command = "acl 'read/no_name/no_units/column #{request}'"
  @prev_result_str = @result_str
  @result_str = `#{acl_command}`

  # It seems ACL has a bug where it leaves a trailing single-quote
  # around string data when using read/no_name/no_units/column.
  @result_str.chomp! "'\n"

  @prev_acl_command = acl_command
end

###
# acnet_request_status
#
# Make an ACNET reading request for the device status property.
#
# @param request DRF2 request string
# @return result
##
def acnet_request_status ( request )
  acl_command = "acl 'read/bit_status #{request}'"

  @prev_result_str = @result_str
  @prev_result_bit_hash = @result_bit_hash

  @result_str = `#{acl_command}`
  @result_bit_hash = acnet_extract_bits @result_str

  @prev_acl_command = acl_command
end

###
# acnet_extract_bits
#
# Extract bit names and values from status property response.
#
# @param response ACL response string for the status property with the bit_status modifier
# @return A hash with bit names as key and bit values as value
##
def acnet_extract_bits ( response )
  bitHash = Hash.new
  @result_str.each_line do |line|
    # Ignore blank lines
    if line.strip == ""
      next
    end

    # Make sure the line matches the format that we expect from ACL's bit_status modifier
    unless line.match /^[a-zA-Z0-9: ]{16}[0-9 ]{2}[ ].{24}[a-zA-Z0-9:_ ]{9}[01]$/
      puts line
      raise "ACNET error OR bit_status response did not match expectation"
    end

    # Place the bit name and value into the hash
    name = line[19, 24]
    value = line[44, 8]
    unless name.nil?
      bitHash[name.strip] = value.strip
    end
  end

  bitHash
end

###
# acnet_write_control
#
# Send the given command to the given ACNET device's control property.
#
# @param device ACNET device
# @param command Command string
##
def acnet_write_control ( device, command )
  # Replace the ":" with "&" (for basic control property)
  expect( device.sub!( ":", "&" ) ).not_to be nil

  # Invoke acl
  @result_str = `acl "enable settings; set #{device} '#{command}'"`

  # Settings should return nothing on success
  expect( @result_str ).to eq ""
end

###
# acnet_write_setting
#
# Send the given setting to the given ACNET device's setting property.
#
# @param device ACNET device
# @param setting Setting
##
def acnet_write_setting ( device, setting )
  # Replace the ":" with "_" (for setting property)
  expect( device.sub!( ":", "_" ) ).not_to be nil

  # Invoke acl
  @result_str = `acl "enable settings; set #{device} '#{setting}'"`

  # Settings should return nothing on success
  expect( @result_str ).to eq ""
end


###
# Expect a valid PID
#
# Validates the given PID.
#
# @param value Expected to be a PID
##
def expect_valid_pid ( value )
  # Init is 1 and everything after is a valid PID
  expect( value ).to be > 1

  # Max PID value for Linux
  expect( value ).to be <= 4194304
end

Given("the mu2eerd daemon process is not running") do
  acnet_request_status "Z|SLWXSTATUS"
  if @result_bit_hash["mu2eerd process"] == "RUNNING"
    acnet_write_control "Z:SLWXSTATUS", "stop"
  end

  acnet_request_status "Z|SLWXSTATUS"
  expect( @result_bit_hash["mu2eerd process"] ).to eq "NOT_RUN"
end

Given("the mu2eerd daemon process is running") do
  acnet_request_status "Z|SLWXSTATUS"
  if @result_bit_hash["mu2eerd process"] == "NOT_RUN"
    acnet_write_control "Z:SLWXSTATUS", "start"
  end

  acnet_request_status "Z|SLWXSTATUS"
  expect( @result_bit_hash["mu2eerd process"] ).to eq "RUNNING"
end

Given("the {string} command has been sent to ACNET device {string}") do |command, device|
  # Replace the ":" with "&" (for basic control property)
  expect( device.sub!( ":", "&" ) ).not_to be nil

  # Invoke acl
  @result_str = `acl "enable settings; set #{device} '#{command}'"`

  # Settings should return nothing on success
  expect( @result_str ).to eq ""
end

Given("the spill state machine is in the FAULT state") do
  expect( `acl "enable settings; set Z&SLWXSSM 'FAULT'"` ).to eq ""
end

When("I request the reading property again") do
  # There should have been a previous request
  expect( @prev_acl_command ).not_to be nil

  # Run the request again
  @prev_result_str = @result_str
  @result_str = `#{@prev_acl_command}`
end

When(/^I wait (\d+) second|seconds$/) do |seconds|
  print "Waiting for #{seconds} second(s)..."
  seconds.to_i.downto(0) do |i|
    sleep 1
    print "."
    STDOUT.flush
  end
  print "\n"
end

When("I send the {string} command to the ACNET device {string}") do |command, device|
  acnet_write_control device, command
end

When("I set the ACNET device {string} to {string}") do |device, setting|
  acnet_write_setting device, setting
end

When("I request the reading property for the ACNET device {string}") do |device|
  acnet_request_read device
end

When("I request the status property for the ACNET device {string}") do |device|
  acnet_request_status device.sub( ":", "|" )
end

Then("I receive a value of {string}") do |expected_value|
  expect( @result_str.strip ).to eq expected_value
end

Then(/I receive a value greater than (\d+)/) do |expected_min|
  expect( @result_str.to_f ).to be > expected_min.to_f
end

Then(/I receive a value less than (\d+)/) do |expected_max|
  expect( @result_str.to_f ).to be < expected_max.to_f
end

Then(/the difference between the requests is within (\d+) of (\d+)/) do |tolerance, expected_diff|
  # Find difference in the results
  expect( @result_str.to_f - @prev_result_str.to_f ).to be_within( tolerance.to_f).of( expected_diff.to_f )
end

Then("I receive a valid PID") do
  result_int = @result_str.to_i

  expect_valid_pid result_int
end

Then("I receive an array of structured data like the following:") do |expected_table|
  # Convert the ACL output into an array
  result_arr = @result_str.split(/\n+/)

  # Run through the table of expected values
  expected_table.hashes.each do |expected_row|
    index = expected_row["index"].strip.to_i
    expected = expected_row["value"].strip

    # The result should contain data at "index"
    expect( result_arr[index] ).not_to be nil

    # Match the expression in the "value" column and
    # test it against the element received from ACNET in result_arr
    result_val = result_arr[index].to_f
    case expected

      # Static value
      when /^\d+/
        expect( result_val ).to eq expected.to_f

      # "a valid PID"
      when /^a valid PID/
        expect_valid_pid result_val.to_f

      # "greater than X"
      when /^greater than \d+/
        args = expected.match /^greater than (\d+)/
        expect( result_val.to_f ).to be > args[1].to_f

      # "less than X"
      when /^less than \d+/
        args = expected.match /^less than (\d+)/
        expect( result_val.to_f ).to be < args[1].to_f

      # Don't know what to do
      else
        raise "I don't know what to do with this value expression:\n\t#{expected}\n"

    end
  end
end

Then("I receive a value of {string} for the {string} bit") do |expected_value, bit_name|
  expect( @result_bit_hash[bit_name] ).to eq expected_value
end

Then("I receive an array of linear structured data like the following:") do |device|
  acnet_request_read device
  if @result_str.match(/[E|e]rror/)
    raise @result_str
  end

  arr = @result_str.split(/\n+/)
  counter = 15999
  arr.each do |entry|
    contents = entry.match /(\d+)/
    expect(contents).to eq(counter)
    counter = counter - 1
  end
end
