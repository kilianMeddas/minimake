#!/bin/sh

mkdir -p tests/script_results

./minimake -f tests/test_integration/input/missing_rule.syntax-test all > tests/script_results/missing_rule.output 2> tests/script_results/missing_rule.error

status=$?

if [ "$status" -eq 2 ] && [ -s tests/script_results/missing_rule.error ] && [ ! -s tests/script_results/missing_rule.output ]; then
    echo "PASS: Missing rule needed by another"
else
    echo "FAIL: Missing rule needed by another"
    exit 1
fi
