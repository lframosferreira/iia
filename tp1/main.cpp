#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

enum GroundType { Grass, HighGrass, Water, Fire, Wall };
enum SearchMethod { BFS, IDS, UCS, Greedy, Astar, Unkown };

inline SearchMethod
get_search_method_from_identifier(const std::string &search_method_identifier) {
  if (search_method_identifier == "BFS") {
    return SearchMethod::BFS;
  } else if (search_method_identifier == "IDS") {
    return SearchMethod::IDS;
  } else if (search_method_identifier == "UCS") {
    return SearchMethod::UCS;
  } else if (search_method_identifier == "Greedy") {
    return SearchMethod::Greedy;
  } else if (search_method_identifier == "Astar") {
    return SearchMethod::Astar;
  }
  return SearchMethod::Unkown;
}

inline double get_ground_type_cost(const GroundType &ground_type) {

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
  return -1.0;
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

typedef struct SearchMethodOutput {
  double cost;
  std::vector<std::pair<unsigned int, unsigned int>> path;

  SearchMethodOutput() {
    this->cost = 0.0;
    // I suppose std::vector has a default constructor for empty vectors being
    // used here by the compiler
  }
} SearchMethodOutput;

std::ostream &operator<<(std::ostream &os,
                         const SearchMethodOutput &search_method_output) {
  os << search_method_output.cost << " ";
  for (auto &[x, y] : search_method_output.path) {
    os << "(" << x << "," << y << ")" << " ";
  }
  os << std::endl;
  return os;
}

SearchMethodOutput
breadth_first_search(const std::vector<std::vector<GroundType>> &map_,
                     unsigned int x_i, unsigned int y_i, unsigned int x_f,
                     unsigned int y_f) {
  SearchMethodOutput output;

  return output;
}

int main(int argc, char *argv[]) {
  if (argc != 7) {
    std::cerr
        << "Usage: \n\t ./pathfinder input_file_path method_identifier x_i y_i "
           "x_f y_f"
        << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string input_file_path = argv[1];
  SearchMethod search_method = get_search_method_from_identifier(argv[2]);

  unsigned int x_i = std::stoi(argv[3]);
  unsigned int y_i = std::stoi(argv[4]);
  unsigned int x_f = std::stoi(argv[5]);
  unsigned int y_f = std::stoi(argv[6]);

  std::vector<std::vector<GroundType>> map_ = parse_input_file(input_file_path);

  switch (search_method) {

  case SearchMethod::BFS:
    std::cout << breadth_first_search(map_, x_i, y_i, x_f, y_f) << std::endl;
    break;
  case SearchMethod::IDS:
    std::cout << "Not implemented yet" << std::endl;
    break;
  case SearchMethod::UCS:
    std::cout << "Not implemented yet" << std::endl;
    break;
  case SearchMethod::Greedy:
    std::cout << "Not implemented yet" << std::endl;
    break;
  case SearchMethod::Astar:
    std::cout << "Not implemented yet" << std::endl;
    break;
  case SearchMethod::Unkown:
    std::cerr << "Unknow search method" << std::endl;
    exit(EXIT_FAILURE);
    break;
  default:
    break;
  }
  exit(EXIT_SUCCESS);
}
