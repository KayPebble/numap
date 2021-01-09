gcc -g -I. -o build\numap.c.o -c numap.c
gcc -g -I. -o build\test test.c build\numap.c.o
gcc -g -I. -o build\test2 test2.c build\numap.c.o
