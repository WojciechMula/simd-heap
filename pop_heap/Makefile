FLAGS=$(CXXFLAGS) -Wall -Wextra -pedantic -std=c++17 -O3 -Werror -march=native

validate: validate.cpp avx2.cpp
	$(CXX) $(FLAGS) validate.cpp -o $@
