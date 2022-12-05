gcc mappyal.c -O2 -Wall -c -o mappyal.o
gcc pbdemo1.c mappyal.o -O2 -Wall -o pbdemo1.exe -lalleg
gcc pbdemo2.c mappyal.o -O2 -Wall -o pbdemo2.exe -lalleg
gcc pbdemo3.c mappyal.o -O2 -Wall -o pbdemo3.exe -lalleg
gcc mpgame.c mappyal.o -O2 -Wall -o mpgame.exe -lalleg
gcc mousecrd.c mappyal.o -O2 -Wall -o mousecrd.exe -lalleg
gcc proja.c mappyal.o -O2 -Wall -o proja.exe -lalleg
gcc isodemo.c mappyal.o -O2 -Wall -o isodemo.exe -lalleg
