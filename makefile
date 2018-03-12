CFLAGS := -O3 -std=c++1z \
    -Wall -Wextra -Wpedantic -Werror \
    -L/usr/include/lib -lOpenMeshCore \
    `pkg-config --libs --cflags eigen3` -Wno-int-in-bool-context

all: bin/render 

bin/render: src/render.cpp
	@mkdir -p bin
	$(CXX) $< -o $@ $(CFLAGS)

clean:
	git clean -Xfd

.PHONY: all clean
