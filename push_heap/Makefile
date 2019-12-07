FLAGS=$(CXXFLAGS) -Wall -Wextra -pedantic -std=c++17 -O3 -Werror -march=icelake-server

DEPS=push_heap.h \
     push_heap_avx512.h

test: validate
	./validate

validate: validate.cpp $(DEPS)
	$(CXX) $(FLAGS) validate.cpp -o $@
