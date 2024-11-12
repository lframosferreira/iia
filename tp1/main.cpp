#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

#define dbg(x) std::cout << #x << " = " << x << std::endl;

inline bool out_of_bounds(int x, int y, int dx, int dy, int map_width,
                          int map_height) {
  return (x + dx < 1) or (x + dx > map_height - 1) or (y + dy < 1) or
         (y + dy > map_width - 1);
}

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
  std::vector<std::vector<GroundType>> map_(height + 1,
                                            std::vector<GroundType>(width + 1));
  for (int i = 1; i <= height; i++) {
    for (int j = 1; j <= width; j++) {
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
  std::vector<std::pair<int, int>> path;

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

inline std::vector<std::pair<int, int>>
get_path(int x_f, int y_f,
         const std::vector<std::vector<std::pair<int, int>>> &parent) {
  std::vector<std::pair<int, int>> path;
  int x = x_f;
  int y = y_f;
  while (x != -1 and y != -1) {
    path.push_back(std::make_pair(x, y));
    auto &[new_x, new_y] = parent.at(x).at(y);
    x = new_x;
    y = new_y;
  }
  std::reverse(path.begin(), path.end());
  return path;
}

SearchMethodOutput
breadth_first_search(const std::vector<std::vector<GroundType>> &map_, int x_i,
                     int y_i, int x_f, int y_f) {
  SearchMethodOutput output;
  int map_height = map_.size();
  int map_width = map_.at(0).size();
  std::queue<std::tuple<int, int, double, int, int>> states_to_process;
  std::vector<std::vector<std::pair<int, int>>> parent(
      map_height, std::vector<std::pair<int, int>>(map_width));
  GroundType initial_pos_ground_type = map_.at(x_i).at(y_i);
  states_to_process.push(std::make_tuple(
      x_i, y_i, get_ground_type_cost(initial_pos_ground_type), -1, -1));
  std::vector<std::pair<int, int>> moves = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  std::pair<int, int> not_visited_position_flag = std::make_pair(0, 0);
  while (!states_to_process.empty()) {
    auto [x, y, cost, parent_x, parent_y] = states_to_process.front();
    states_to_process.pop();
    if (parent.at(x).at(y) != not_visited_position_flag) {
      continue;
    }
    parent.at(x).at(y) = std::make_pair(parent_x, parent_y);

    for (const auto &[dx, dy] : moves) {
      if (out_of_bounds(x, y, dx, dy, map_width, map_height)) {
        continue;
      }
      if (map_.at(x + dx).at(y + dy) == GroundType::Wall) {
        continue;
      }
      if (parent.at(x + dx).at(y + dy) != not_visited_position_flag) {
        continue;
      }
      // early return
      if (x + dx == x_f and y + dy == y_f) {
        output.cost = cost + get_ground_type_cost(map_.at(x + dx).at(y + dy));
        parent.at(x + dx).at(y + dy) = std::make_pair(x, y);
        output.path = get_path(x_f, y_f, parent);
        return output;
      }
      states_to_process.push(std::make_tuple(
          x + dx, y + dy,
          cost + get_ground_type_cost(map_.at(x + dx).at(y + dy)), x, y));
    }
  }
  return output;
}

// dijkstra
SearchMethodOutput
uniform_cost_search(const std::vector<std::vector<GroundType>> &map_, int x_i,
                    int y_i, int x_f, int y_f) {
  SearchMethodOutput output;
  int map_height = map_.size();
  int map_width = map_.at(0).size();
  typedef std::tuple<int, int, double, int, int> priority_queue_element_type;
  class Compare {
  public:
    bool operator()(const priority_queue_element_type &a,
                    const priority_queue_element_type &b) {
      double cost_a, cost_b;
      std::tie(std::ignore, std::ignore, cost_a, std::ignore, std::ignore) = a;
      std::tie(std::ignore, std::ignore, cost_b, std::ignore, std::ignore) = b;
      return cost_a > cost_b;
    }
  };

  std::priority_queue<priority_queue_element_type,
                      std::vector<priority_queue_element_type>, Compare>
      states_to_process;
  std::vector<std::vector<std::pair<int, int>>> parent(
      map_height, std::vector<std::pair<int, int>>(map_width));
  GroundType initial_pos_ground_type = map_.at(x_i).at(y_i);
  states_to_process.push(std::make_tuple(
      x_i, y_i, get_ground_type_cost(initial_pos_ground_type), -1, -1));
  std::vector<std::pair<int, int>> moves = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  std::pair<int, int> not_visited_position_flag = std::make_pair(0, 0);
  while (!states_to_process.empty()) {
    auto [x, y, cost, parent_x, parent_y] = states_to_process.top();
    states_to_process.pop();

    if (parent.at(x).at(y) != not_visited_position_flag) {
      continue;
    }
    parent.at(x).at(y) = std::make_pair(parent_x, parent_y);

    // early return
    if (x == x_f and y == y_f) {
      output.cost = cost;
      output.path = get_path(x_f, y_f, parent);
      return output;
    }

    for (const auto &[dx, dy] : moves) {
      if (out_of_bounds(x, y, dx, dy, map_width, map_height)) {
        continue;
      }
      if (map_.at(x + dx).at(y + dy) == GroundType::Wall) {
        continue;
      }
      if (parent.at(x + dx).at(y + dy) != not_visited_position_flag) {
        continue;
      }
      states_to_process.push(std::make_tuple(
          x + dx, y + dy,
          cost + get_ground_type_cost(map_.at(x + dx).at(y + dy)), x, y));
    }
  }
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

  int x_i = std::stoi(argv[3]);
  int y_i = std::stoi(argv[4]);
  int x_f = std::stoi(argv[5]);
  int y_f = std::stoi(argv[6]);

  std::vector<std::vector<GroundType>> map_ = parse_input_file(input_file_path);

  switch (search_method) {

  case SearchMethod::BFS:
    std::cout << breadth_first_search(map_, x_i, y_i, x_f, y_f) << std::endl;
    break;
  case SearchMethod::IDS:
    std::cout << "Not implemented yet" << std::endl;
    break;
  case SearchMethod::UCS:
    std::cout << uniform_cost_search(map_, x_i, y_i, x_f, y_f) << std::endl;
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
