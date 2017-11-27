#!/bin/sh

# Galen Helfter
# run.sh

LISPC=sbcl

COREFILE=./composite.core
first=""
second=""
matte=""

assert_exists()
{
    if ! test -e $1
    then
        echo "Error - image $1 does not exist."
        exit 1
    fi
}

if  test -e $COREFILE
then
    if [ $# -gt 2 ]
    then
        first=$1
        assert_exists $first
        second=$2
        assert_exists $second
        matte=$3
        assert_exists $matte
    elif [ $# -gt 1 ]
    then
        first=$1
        assert_exists $first
        second=$2
        assert_exists $second
    elif [ $# -gt 0 ]
    then
        first=$1
        assert_exists $first
    fi
    # Run command
    $LISPC --core $COREFILE $first $second $matte
else
    echo "The corefile, $COREFILE, does not exist."
    echo "Compile the code using make to run it."
fi
