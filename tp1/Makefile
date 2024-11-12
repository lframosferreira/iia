CXX = g++
CFLAGS = -std=c++17 -Wall -Werror
LIBS = -lm

build:
	$(CXX) $(CFLAGS) main.cpp -o pathfinder $(LIBS)

run:
	./pathfinder mapas/mapa_teste.map Greedy 2 2 2 4 

clean:
	rm -rf *.o
	rm -rf pathfinder 

