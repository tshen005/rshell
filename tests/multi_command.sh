#!/bin/sh

#tests commands with comments
make all
echo -e "ls -a; echo hello && mkdir test || echo world; git status\nexit\n" | bin/rshell
echo
echo -e "echo a || echo hello\nexit\n" | bin/rshell
echo
echo -e "ls -a; echo hello; mkdir test\nexit\n" | bin/rshell
echo
echo -e "echo hello || mkdir test\nexit\n" | bin/rshell
echo
echo -e "echo hello && mkdir test\nexit\n" | bin/rshell
echo
echo -e "ech o && echo a \nexit\n" | bin/rshell
echo
echo -e "echo hello && \nexit\n" | bin/rshell
echo