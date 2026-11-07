#!/bin/sh

mkdir -p tests/script_results

./minimake --u > tests/script_results/unknown_option.output 2> tests/script_results/unknown_option.error

status=$?

if [ "$status" -eq 2 ] && [ -s tests/script_results/unknown_option.error ]; then
    echo "PASS: Unknown option"
else
    echo "FAIL: Unknown option"
fi
