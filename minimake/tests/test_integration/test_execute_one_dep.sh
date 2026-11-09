#!/bin/sh

mkdir -p tests/script_results

./minimake -f tests/test_integration/input/one_dep.syntax-test all > tests/script_results/one_dep.output 2> tests/script_results/one_dep.error

status=$?

if [ "$status" -eq 0 ] && [ ! -s tests/script_results/one_dep.error ] && diff -u tests/test_integration/expected/one_dep.expected tests/script_results/one_dep.output; then
    echo "PASS: One dependency"
else
    echo "FAIL: One dependency"
    exit 1
fi
