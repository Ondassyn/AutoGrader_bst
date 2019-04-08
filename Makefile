
# Automatically generated Makefile

Flags = -Wreturn-type -pedantic -pedantic-errors -Wundef -std=c++17  -O5 
CPP = g++


main : Makefile   main.o set.o mset.o
	$(CPP) $(Flags) -o main   main.o set.o mset.o 

main.o : Makefile   main.cpp set.h mset.h 
	$(CPP) -c $(Flags) main.cpp -o  main.o


set.o : Makefile   set.cpp   set.h 
	$(CPP) -c $(Flags) set.cpp -o  set.o

mset.o : Makefile mset.cpp mset.h
	$(CPP) -c $(Flags) mset.cpp -o mset.o
