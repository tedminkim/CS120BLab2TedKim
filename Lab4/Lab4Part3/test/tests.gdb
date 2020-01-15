# Test file for Lab2_introToAVR


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly,
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

# Example test:
#test "PINA: 0x00, PINB: 0x00 => PORTC: 0"
# Set inputs
#setPINA 0x00
#setPINB 0x00
# Continue for several ticks
#continue 2
# Set expect values
#expectPORTC 0
# Check pass/fail
#checkResult

#Add tests below
test “PINA: 0x01 => PORTC: 0x60”
setPINA 0x01
continue 5
expectPORTC 0x60
checkResult

test “PINA: 0x12 => PORTC: 0x60”
setPINA 0x12
continue 5
expectPORTC 0x60
checkResult

test “PINA: 0x33 => PORTC: 0x70”
setPINA 0x33
continue 5
expectPORTC 0x70
checkResult

test “PINA: 0x44 => PORTC: 0x70”
setPINA 0x44
continue 5
expectPORTC 0x70
checkResult

test “PINA: 0x55 => PORTC: 0x28”
setPINA 0x55
continue 5
expectPORTC 0x28
checkResult

test “PINA: 0x66 => PORTC: 0x28”
setPINA 0x66
continue 5
expectPORTC 0x28
checkResult

test “PINA: 0x77 => PORTC: 0x2C”
setPINA 0x07
continue 5
expectPORTC 0x2C
checkResult

test “PINA: 0x08 => PORTC: 0x2C”
setPINA 0x08
continue 5
expectPORTC 0x2C
checkResult

test “PINA: 0x09 => PORTC: 0x2C”
setPINA 0x09
continue 5
expectPORTC 0x2C
checkResult

test “PINA: 0x0A => PORTC: 0x2E”
setPINA 0x0A
continue 5
expectPORTC 0x2E
checkResult

test “PINA: 0x0B => PORTC: 0x2E”
setPINA 0x0B
continue 5
expectPORTC 0x2E
checkResult

test “PINA: 0x0C => PORTC: 0x2E”
setPINA 0x0C
continue 5
expectPORTC 0x2E
checkResult

test “PINA: 0x0D => PORTC: 0x2F”
setPINA 0x0D
continue 5
expectPORTC 0x2F
checkResult

test “PINA: 0x0E => PORTC: 0x2F”
setPINA 0x0E
continue 5
expectPORTC 0x2F
checkResult

test “PINA: 0x0F => PORTC: 0x2F”
setPINA 0x0F
continue 5
expectPORTC 0x2F
checkResult


# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
