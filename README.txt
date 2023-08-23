to compile all source code with ptest ((testbed)) programm
run below command in shell  assuming having make installed

~$make

NOTE:compiling command inside the makefile

to run testing application with required three conditions in task paper
please copy and paste every of those trhee commands below in the shell.

./ptest -p $((1024*1024*2))    -n10 -s $((5*1024))
./ptest -p $((1024*1024*1024)) -n10 -s $((5*1024*1024))
./ptest -p $((1024*1024*1024)) -n10 -r

you can change the number of chunks by modifing -n option.
and  -r option is for random sizes.
please sea -h option for help and more information.

ALAA ZAKARIYA 
2023-08-23
