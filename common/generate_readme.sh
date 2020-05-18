#!/bin/sh
TEMPLATE=README.md.template
NAME=$(basename $PWD)

cat $TEMPLATE | sed '/NOTE-BEGIN/,/NOTE-READ-CODE/!d' | grep -v "^NOTE-"
cat main.cpp | \
    grep -v "define DOC" | \
    sed -e 's/DOC(\"\(.*\)\"); *$/\/\/ \1/g'
cat $TEMPLATE | sed '/NOTE-READ-CODE/,/NOTE-BUILD-CODE/!d' | grep -v "^NOTE-"
echo "cd $NAME"
make clean all
echo "./example"
cat $TEMPLATE | sed '/NOTE-BUILD-CODE/,/NOTE-RUN-CODE/!d' | grep -v "^NOTE-"
./example
cat $TEMPLATE | sed '/NOTE-RUN-CODE/,/NOTE-END/!d' | grep -v "^NOTE-"

