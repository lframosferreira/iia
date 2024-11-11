#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

enum GroundType { Grass, HighGrass, Water, Fire, Wall };

std::vector<std::vector<GroundType>>
parse_input_file(const std::string &filename) {
  std::ifstream input_stream(filename);
  int height, width;
  input_stream >> height >> width;
  std::vector<std::vector<GroundType>> map_(height,
                                            std::vector<GroundType>(width));
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      char c;
      input_stream >> c;
      switch (c) {
      case '@':
        map_.at(i).at(j) = GroundType::Fire;
        break;
      default:
        std::cerr << "Unkown ground type in input file" << std::endl;
        break;
      }
    }
  }
  return map_;
}

int main(int argc, char *argv[]) { exit(EXIT_SUCCESS); }
