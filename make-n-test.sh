rm *.o
rm program
gcc -c mathwork.c -o mathwork.o
gcc -c cstr.c -o cstr.o
gcc -c main.c -o main.o
gcc main.o cstr.o mathwork.o -o program -lm
rm *.o
./program
