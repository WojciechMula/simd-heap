FLAGS=$(CXXFLAGS) -Wall -Wextra -pedantic -std=c++17 -O2 -Werror

DEPS=is_heap_scalar.h \
     is_heap_sse.cpp

test: validate
	./validate

validate: validate.cpp $(DEPS)
	$(CXX) $(FLAGS) validate.cpp -o $@
