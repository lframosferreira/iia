#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

enum GroundType { Grass, HighGrass, Water, Fire, Wall };

std::vector<std::vector<GroundType>>
parse_input_file(const std::string &filename) {
  std::ifstream input_stream = std::ifstream(filename, "r");
  std::vector<std::vector<GroundType>> map_;
}

int main(int argc, char *argv[]) { exit(EXIT_SUCCESS); }
