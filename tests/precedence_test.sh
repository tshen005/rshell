make all 
echo -e "echo A && (echo B || echo C)\nexit\n" | bin/rshell
echo
echo -e "(echo A && echo B) || (echo C && echo D)\nexit\n" | bin/rshell
echo
echo -e "ls && (pwd || exit)\nexit\n" | bin/rshell
echo
echo -e "( )\nexit\n" | bin/rshell	#should be syntax error
echo
echo -e "(\t)\nexit\n" | bin/rshell	#should be command not found
echo
echo -e "ls -j || (echo t && echo f) && exit\n" | bin/rshell
echo
echo -e "ls -j || (echo t && echo f) && (|| exit)\nexit\n" | bin/rshell		#should be syntax error on ||
echo
