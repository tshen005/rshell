make all
echo -e "#do not print\nexit\n" | bin/rshell
echo
echo -e "echo a#hello\nexit\n" | bin/rshell
echo
echo -e "#exit\nexit\n" | bin/rshell
echo
echo -e "echo a #;\nexit\n" | bin/rshell
echo
echo -e "ls #-a\nexit\n" | bin/rshell
echo
echo -e " \nexit\n" | bin/rshell
echo
echo -e "\t\nexit\n" | bin/rshell 
echo
