#!/bin/bash
assert() {
    mkdir -p tmp
    cd tmp
    expected="$1"
    input="$2"

    ../bin/sh1mcc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo -e "$input\t=>\t$actual"
    else
        echo -e "$input\t=>\t$expected expected, but got $actual"
        exit 1
    fi
    cd ../
}

assert 0 "0;"
assert 42 "42;"
assert 21 "5+20-4;"
assert 41 " 12 + 34- 5;"
assert 5 "(2 + 3) * 1;"
assert 10 "(1 + 2)*3 + 4- 6 /2;"
assert 5 "-1* +5-(-10);"
assert 1 "1!=2;"
assert 1 "-2*-3==6;"
assert 0 "1 + 2 > 3 + 4;"
assert 1 "1 + 2 >= 3;"
assert 0 "3 + 4 < 1 + 2;"
assert 1 "3 <= 1 + 2;"
assert 2 "(1 + 2 <= 3)+(7/2 == 3);"
assert 7 "a = 12; a;"

echo OK