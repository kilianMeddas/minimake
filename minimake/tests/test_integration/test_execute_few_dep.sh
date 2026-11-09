#!/bin/sh

mkdir -p tests/script_results

./minimake -f tests/test_integration/input/few_dep.syntax-test all > tests/script_results/few_dep.output 2> tests/script_results/few_dep.error

status=$?

if [ "$status" -eq 0 ] && [ ! -s tests/script_results/few_dep.error ] && diff -u tests/test_integration/expected/few_dep.expected tests/script_results/few_dep.output; then
    echo "PASS: Multiple dependencies"
else
    echo "FAIL: Multiple dependencies"
    exit 1
fi
