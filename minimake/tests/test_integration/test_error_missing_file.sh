#!/bin/sh

mkdir -p tests/script_results

./minimake -p > tests/script_results/missing_file.output 2> tests/script_results/missing_file.error

status=$?

if [ "$status" -eq 2 ] && [ -s tests/script_results/missing_file.error ]; then
    echo "PASS: Missing file option"
else
    echo "FAIL: Missing file option"
fi
