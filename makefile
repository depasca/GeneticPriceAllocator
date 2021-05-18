GOOGLE_TEST_LIB = gtest

CPPFLAGS=-g

pa: solution.o priceallocator.o main.o test.o
	g++ -o pa solution.o priceallocator.o main.o test.o -l $(GOOGLE_TEST_LIB) -l pthread

solution.o: solution.cpp
	g++  -c solution.cpp $(CPPFLAGS)

priceallocator.o: priceallocator.cpp
	g++ -c priceallocator.cpp $(CPPFLAGS)

test.o: test.cpp
	g++ -c test.cpp $(CPPFLAGS)

main.o: main.cpp
	g++ -c main.cpp $(CPPFLAGS)

clean: pa main.o test.o priceallocator.o solution.o
	rm -f pa main.o priceallocator.o solution.o test.o