CFLAGS := -O3 \
    -Wall -Wextra -Wpedantic -Werror \
    -L/usr/include/lib -lOpenMeshCore

all: bin/render 

bin/render: src/render.cpp
	@mkdir -p bin
	$(CXX) $< -o $@ $(CFLAGS)

clean:
	git clean -Xfd

.PHONY: all clean
