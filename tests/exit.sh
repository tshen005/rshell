#!/bin/sh

#tests exit and commands with exit

cd .. || exit
echo A#exit
exit
ls && exit
echo A && exit
echo A || exit
echo A; exit
