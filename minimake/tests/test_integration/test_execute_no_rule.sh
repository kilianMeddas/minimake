#!/bin/sh

mkdir -p "tests/script_results"

./minimake -f tests/test_integration/input/no_dependencies.syntax-test > tests/script_results/no_dependencies.output

if diff -u tests/script_results/no_dependencies.output tests/test_integration/expected/no_dependencies.expected; then
    echo "PASS: Execute without dependencies"
else
    echo "FAIL: Execute without dependencies"
	exit 1
fi

