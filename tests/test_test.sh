make all 

echo -e "test -e /src/parse.cpp\nexit\n" | bin/rshell
echo
echo -e "[test -e /src/parse.cpp]\nexit\n" | bin/rshell
echo
echo -e "test -e /src/parse.h\nexit\n" | bin/rshell
echo
echo -e "[-e /src/parse.h]\nexit\n" | bin/rshell
echo
echo -e "test -f /src/shell.h && echo true; exit\n" | bin/rshell
echo
echo -e "test -d /src && echo true; exit\n" | bin/rshell
echo
echo -e "test /CS100/rshell/tests && echo true && exit\n" | bin/rshell
echo
echo -e "test /CS100/rshell/makefile && echo true && exit\n" | bin/rshell
echo
echo -e "test exit\n" | bin/rshell	#should be true
echo
echo -e "test || echo false; exit\n" | bin/rshell #should be false
echo 
echo -e "test -e && echo true; exit\n" | bin/rshell # should be true
echo
