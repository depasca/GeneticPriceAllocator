CPPFLAGS=-g

pa: solution.o priceallocator.o main.o
	g++ -o pa solution.o priceallocator.o main.o

solution.o: solution.cpp solution.h
	g++ -c solution.cpp

priceallocator.o: priceallocator.cpp priceallocator.h solution.h
	g++ -c priceallocator.cpp

main.o: main.cpp priceallocator.h solution.h
	g++ -c main.cpp

clean: pa main.o priceallocator.o solution.o
	rm -f pa main.o priceallocator.o solution.o