#!/bin/sh

mkdir -p "tests/script_results"

./minimake -p -f tests/test_integration/input/complete.syntax-test > tests/script_results/complete.output

if diff -u tests/script_results/complete.output tests/test_integration/expected/complete.expected; then
    echo "PASS: Pretty-print complete"
else
    echo "FAIL: Pretty-print complete"
fi
