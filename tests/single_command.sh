#!/bin/sh

#tests exit and commands with exit
make all
echo -e "cd .. \nexit\n" | bin/rshell
echo
echo -e "echo A\nexit\n" | bin/rshell
echo
echo -e "ls -l\n" | bin/rshell
echo
echo -e "mkdir a\n" | bin/rshell
echo
echo -e "mkdir a\n" | bin/rshell
echo
echo -e "git status\n" | bin/rshell
echo
