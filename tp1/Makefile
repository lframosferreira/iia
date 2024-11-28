CXX = g++
CFLAGS = -std=c++17 -Wall -Werror
LIBS = -lm

build:
	$(CXX) $(CFLAGS) main.cpp -o pathfinder $(LIBS)

run:
	./pathfinder mapas/cidade.map Greedy 2 2 83 98

test:
	./pathfinder mapas/mapa_teste.map UCS 1 1 3 1

clean:
	rm -rf *.o
	rm -rf pathfinder 

