FLAGS=$(CXXFLAGS) -Wall -Wextra -pedantic -std=c++17 -O3 -Werror -march=native

validate: validate.cpp avx2.cpp avx2.inl.cpp
	$(CXX) $(FLAGS) validate.cpp -o $@

avx2.inl.cpp: pop_heap.py
	python3 pop_heap.py
