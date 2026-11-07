#!/bin/sh

mkdir -p tests/script_results

./minimake -f > tests/script_results/missing_filename.output 2> tests/script_results/missing_filename.error

status=$?

if [ "$status" -eq 2 ] && [ -s tests/script_results/missing_filename.error ]; then
    echo "PASS: Missing filename"
else
    echo "FAIL: Missing filename"
    exit 1
fi
