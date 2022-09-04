#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./sh1mcc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo -e "$input\t=>\t$actual"
    else
        echo -e "$input\t=>\t$expected expected, but got $actual"
        exit 1
    fi
}

assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 41 " 12 + 34- 5"
assert 5 "(2 + 3) * 1"
assert 10 "(1 + 2)*3 + 4- 6 /2"

echo OK