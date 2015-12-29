test: linalg-tests
	./linalg-tests

clean:
	rm -f ./linalg-tests
	
linalg-tests: ../linalg.h *.cpp *.h 3rdparty/*
	g++ -std=c++14 -I.. -I3rdparty *.cpp -o linalg-tests