#!/bin/sh

mkdir -p tests/script_results

./minimake "" > tests/script_results/empty_argument.output  2> tests/script_results/empty_argument.error

status=$?

if [ "$status" -eq 2 ] && [ -s tests/script_results/empty_argument.error ]; then
    echo "PASS: Empty argument"
else
    echo "FAIL: Empty argument" 
fi
