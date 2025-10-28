all: main

main: *.o
	g++ main.o rb_tree.o node.o -o main

*.o: ../src/modules/*.h ../src/modules/*.cpp *.cpp
	g++ -c ../src/modules/*.cpp -I../src/modules
	g++ -c *.cpp -I../src/modules

clean:
	rm -f *.o

cleanAll: clean
	rm -f main