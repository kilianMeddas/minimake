#!/bin/sh

mkdir -p tests/script_results

./minimake -f tests/test_integration/input/missing_dependency.syntax-test all > tests/script_results/missing_dependency.output 2> tests/script_results/missing_dependency.error

status=$?

if [ "$status" -eq 2 ] && [ -s tests/script_results/missing_dependency.error ] && [ ! -s tests/script_results/missing_dependency.output ]; then
    echo "PASS: Missing dependency"
else
    echo "FAIL: Missing dependency"
    exit 1
fi
