#!/bin/sh

mkdir -p "tests/script_results"

./minimake -p -f tests/test_integration/input/no_rules.syntax-test > tests/script_results/no_rules.output

if diff -u tests/script_results/no_rules.output tests/test_integration/expected/no_rules.expected; then
	echo "PASS: Pretty-print no rule"
else
	echo "FAIL: Pretty-print no rule"
	exit 1
fi

