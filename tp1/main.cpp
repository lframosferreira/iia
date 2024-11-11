#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

enum GroundType { Grass, HighGrass, Water, Fire, Wall };

inline double get_ground_type_cost(GroundType ground_type) {

  switch (ground_type) {
  case GroundType::Grass:
    return 1.0;
  case GroundType::HighGrass:
    return 1.5;
  case GroundType::Water:
    return 2.5;
  case GroundType::Fire:
    return 6.0;
  case GroundType::Wall:
    return std::numeric_limits<double>::max();
  default:
    break;
  }
  return -1;
}

std::vector<std::vector<GroundType>>
parse_input_file(const std::string &filename) {
  std::ifstream input_stream(filename);
  int height, width;
  input_stream >> width >> height;
  std::vector<std::vector<GroundType>> map_(height,
                                            std::vector<GroundType>(width));
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      char c;
      input_stream >> c;
      switch (c) {
      case '.':
        map_.at(i).at(j) = GroundType::Grass;
        break;
      case ';':
        map_.at(i).at(j) = GroundType::HighGrass;
        break;
      case '+':
        map_.at(i).at(j) = GroundType::Water;
        break;
      case 'x':
        map_.at(i).at(j) = GroundType::Fire;
        break;
      case '@':
        map_.at(i).at(j) = GroundType::Wall;
        break;
      default:
        std::cerr << "Unkown ground type in input file" << std::endl;
        break;
      }
    }
  }
  return map_;
}

int main(int argc, char *argv[]) {
  if (argc != 7) {
    std::cerr << "Usage: " << std::endl;
    exit(EXIT_FAILURE);
  }
  std::vector<std::vector<GroundType>> map_ = parse_input_file(argv[1]);
  for (auto e1 : map_) {
    for (auto e2 : e1) {
      std::cout << e2 << " ";
    }
    std::cout << std::endl;
  }
  exit(EXIT_SUCCESS);
}
