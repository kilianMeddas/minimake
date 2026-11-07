#!/bin/sh

mkdir -p "tests/script_results"

./minimake -p -f tests/test_integration/input/no_variables.syntax-test > tests/script_results/no_variables.output

if diff -u tests/script_results/no_variables.output tests/test_integration/expected/no_variables.expected; then
	echo "PASS: Pretty-print no variables"
else
	echo "FAIL: Pretty-print no variables"
	exit 1
fi

