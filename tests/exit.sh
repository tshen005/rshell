#!/bin/sh

#tests exit and commands with exit
make all
echo -e "cd .. || exit\nexit\n" | bin/rshell
echo
echo -e "echo A#exit\nexit\n" | bin/rshell
echo
echo -e "exit\n" | bin/rshell
echo
echo -e "ls && exit\n" | bin/rshell
echo
echo -e "echo A && exit\n" | bin/rshell
echo
echo -e "echo A || exit\n" | bin/rshell
echo
echo -e "echo A; exit\n" | bin/rshell
echo