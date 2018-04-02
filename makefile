CXX := clang++

CFLAGS := -O3 -std=c++17 \
    -Werror -Wall -Wextra -Wpedantic \
    -L/usr/include/lib -lOpenMeshCore \
    `pkg-config --libs --cflags eigen3`

.PHONY: all
all: bin/screen-space bin/world-space

bin/%: src/%.cpp | bin
	$(CXX) $< -o $@ $(CFLAGS)
bin:
	mkdir bin

.PHONY: clean
clean:
	rm -r bin
