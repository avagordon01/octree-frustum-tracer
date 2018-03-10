all: bin/render 

bin/render: src/render.cpp
	$(CXX) -o $@ $< -O3 -std=c++17 -lOpenMeshCore

clean:
	git clean -Xfd

.PHONY: all clean
