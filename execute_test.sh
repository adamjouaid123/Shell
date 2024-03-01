#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <test_number format 01>"
    exit 1
fi

TEST_DIR="tests"

TEST_NUMBER=$1

TEST_FILE="${TEST_DIR}/test${TEST_NUMBER}.txt"

if [ ! -f "$TEST_FILE" ]; then
    echo "Test file not found: $TEST_FILE"
    exit 1
fi

./sdriver.pl -t "$TEST_FILE" -s ./shell 
