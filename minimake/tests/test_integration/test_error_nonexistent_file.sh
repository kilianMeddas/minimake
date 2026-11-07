#!/bin/sh

mkdir -p tests/script_results

./minimake -p -f tests/nonexistent_file  > tests/script_results/nonexistent_file.output 2> tests/script_results/nonexistent_file.error

status=$?

if [ "$status" -eq 2 ] && [ -s tests/script_results/nonexistent_file.error ]; then
    echo "PASS: Nonexistent file"
else
    echo "FAIL: Nonexistent file"

fi
