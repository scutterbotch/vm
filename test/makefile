test: test_call test_arithmetic

test_call: test_call.cpp ../src/vm.hpp
	g++ -I ../src test_call.cpp -o test_call

test_arithmetic: ../src/vm.hpp test_arithmetic.cpp
	g++ -I ../src test_arithmetic.cpp -o test_arithmetic

clean:
	rm -f test_call test_arithmetic

