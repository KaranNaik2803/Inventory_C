gcc -c client.c
gcc -c readfile.c
gcc -c bill.c
gcc readfile.o client.o bill.o
a.exe
del client.o
del readfile.o
del bill.o
del a.exe