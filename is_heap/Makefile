FLAGS=$(CXXFLAGS) -Wall -Wextra -pedantic -std=c++17 -O3 -Werror

DEPS=is_heap_scalar.h \
     is_heap_sse.cpp

test: validate
	./validate

bench: benchmark
	./benchmark 256 512 1024 4096 8192 16384 32768 65536

validate: validate.cpp $(DEPS)
	$(CXX) $(FLAGS) validate.cpp -o $@

benchmark: benchmark.cpp $(DEPS)
	$(CXX) $(FLAGS) benchmark.cpp -o $@
