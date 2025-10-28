all: main tests

main: *.o
	g++ main.o rb_tree.o node.o -o main

tests: *.o
	g++ tests.o rb_tree.o node.o -o tests

*.o: modules/*.h modules/*.cpp *.cpp
	g++ -c modules/*.cpp -Imodules
	g++ -c *.cpp -Imodules

clean:
	rm -f *.o

cleanAll: clean
	rm -f main tests