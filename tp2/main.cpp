#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#define dbg(x) std::cout << #x << " = " << x << std::endl;
#define LEARNING_RATE 0.1
#define DISCOUNT_RATE 0.9
#define EPSILON 0.1

const void print(const std::string &txt) { std::cout << txt << std::endl; }

// Vetor com os possíveis movimentos do agente durante a busca
static const std::vector<std::pair<int, int>> moves = {
    {-1, 0}, {1, 0}, {0, 1}, {0, -1}};

// Função auxiliar para checar se uma coordenada estará dentro dos limites do
// mapa
inline bool out_of_bounds(int x, int y, int dx, int dy, int map_width,
                          int map_height) {
  return x + dx < 0 or x + dx >= map_width or y + dy < 0 or
         y + dy >= map_height;
}

enum GroundType { Grass, HighGrass, Water, Fire, Objective, Wall };
enum Action { Left, Right, Down, Up };

inline char ground_type_to_char(const GroundType &ground_type) {
  char ret;

  switch (ground_type) {

  case GroundType::Grass: {
    ret = '.';
  } break;
  case GroundType::HighGrass: {
    ret = ';';
  } break;
  case GroundType::Water: {
    ret = '+';
  } break;
  case GroundType::Fire: {
    ret = 'x';
  } break;
  case GroundType::Objective: {
    ret = 'O';
  } break;
  case GroundType::Wall: {
    ret = '@';
  } break;
  default: {
    std::cerr << "Error getting char from ground type\n";
    exit(EXIT_FAILURE);
  }
  }

  return ret;
}

static const std::map<GroundType, long double> base_ground_type_rewards{
    {GroundType::Grass, -0.1},
    {GroundType::HighGrass, -0.3},
    {GroundType::Water, -1.0},
    {GroundType::Fire, -10.0},
    {GroundType::Objective, 10.0},
    {GroundType::Wall, std::numeric_limits<long double>::min()},

};

static const std::map<GroundType, long double> positive_ground_type_rewards{
    {GroundType::Grass, 3.0},
    {GroundType::HighGrass, 1.5},
    {GroundType::Water, 1.0},
    {GroundType::Fire, 0.0},
    {GroundType::Objective, 10.0},
    {GroundType::Wall, std::numeric_limits<long double>::min()},
};

static std::vector<std::vector<GroundType>> map_;
static std::vector<std::vector<std::vector<long double>>> weights;

void print_map() {
  for (auto &e1 : map_) {
    for (auto &e2 : e1) {
      std::cout << ground_type_to_char(e2) << " ";
    }
    std::cout << "\n";
  }
}

std::vector<std::vector<GroundType>>
parse_input_file(const std::string &filename) {
  std::ifstream input_stream(filename);
  int height, width;
  input_stream >> width >> height;
  map_.resize(height);
  for (auto &e : map_)
    e.resize(width);
  weights.resize(height);
  for (auto &e : weights) {
    e.resize(width);
    for (auto &e2 : e)
      e2.resize(4, 0.0);
  }

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
      case 'O':
        map_.at(i).at(j) = GroundType::Objective;
        break;
      default:
        std::cerr << "Unkown ground type in input file" << std::endl;
        break;
      }
    }
  }
  return map_;
}

inline Action get_action_from_move(const int &dx, const int &dy) {
  if (dx == -1)
    return Action::Left;
  if (dx == 1)
    return Action::Right;
  if (dy == 1)
    return Action::Down;
  if (dy == -1)
    return Action::Up;
  std::cerr << "Unknown action from move\n";
  exit(EXIT_FAILURE);
}

typedef struct {
  long double reward;
  Action action;
  int64_t x;
  int64_t y;
} NextStateConfig;

inline NextStateConfig get_next_state_config(const uint64_t x,
                                             const uint64_t y) {
  long double curr_max = std::numeric_limits<long double>::min();
  Action best_action = Action::Right;
  int64_t bx = 1, by = 0;
  for (const auto &[dx, dy] : moves) {
    Action action = get_action_from_move(dx, dy);
    if (out_of_bounds(x, y, dx, dy, map_.at(0).size(), map_.size()))
      continue;
    if (weights[x + dx][y + dy][action] > curr_max) {
      curr_max = weights[x + dx][y + dy][action];
      best_action = action;
      bx = x + dx;
      by = y + dy;
    }
  }
  return {curr_max, best_action, bx, by};
}

inline std::pair<long double, std::pair<int, int>>
get_reward_and_next_state(const uint64_t x, const uint64_t y,
                          const Action action) {
  const auto [dx, dy] = moves[action];
  return {base_ground_type_rewards.at(map_.at(x + dx).at(y + dy)),
          {x + dx, y + dy}};
}

void standard(const uint64_t xi, const uint64_t yi,
              const uint64_t number_of_steps) {

  uint64_t sx = xi;
  uint64_t sy = yi;
  for (auto i = 0; i < number_of_steps; i++) {
    auto [reward, best_action, newx, newy] = get_next_state_config(sx, sy);
    auto [ignorar1, best_action_new, ignorar2, ignorar3] =
        get_next_state_config(sx, sy);
    weights[sx][sy][best_action] +=
        LEARNING_RATE * (reward + DISCOUNT_RATE * weights[newx][newx][newy] -
                         weights[sx][sy][best_action]);

    sx = newx;
    sy = newy;
  }
}

void print_policy() {
  for (uint64_t i = 0; i < map_.size(); i++) {
    for (uint64_t j = 0; j < map_.at(0).size(); j++) {
      auto e2 = map_.at(i).at(j);
      if (e2 == GroundType::Fire or e2 == GroundType::Wall or
          e2 == GroundType::Objective) {
        std::cout << ground_type_to_char(e2);
      } else {
        auto action = std::max_element(weights.at(i).at(j).begin(),
                                       weights.at(i).at(j).end()) -
                      weights.at(i).at(j).begin();
        switch (action) {
        case Action::Left:
          std::cout << "<";
          break;
        case Action::Right:
          std::cout << ">";
          break;
        case Action::Up:
          std::cout << "^";
          break;
        case Action::Down:
          std::cout << "v";
          break;
        }
      }
      std::cout << " ";
    }
    std::cout << "\n";
  }
}

int main(int argc, char **argv) {

  if (argc != 6) {
    std::cout << "Usage: ./main [input_file_path] [modifier_id] x_i y_i n"
              << std::endl;
    exit(EXIT_FAILURE);
  }
  const std::string input_file_path = argv[1];
  parse_input_file(input_file_path);

  const std::string modifier_id = argv[2];
  uint64_t xi = std::stoi(argv[3]);
  uint64_t yi = std::stoi(argv[4]);
  uint64_t number_of_steps = std::stoi(argv[5]);
  print_map();

  if (modifier_id == "standard") {
    standard(xi, yi, number_of_steps);
  } else if (modifier_id == "positive") {
  } else if (modifier_id == "stochastic") {
  } else {
    std::cerr << "Unknown modifier id" << std::endl;
    exit(EXIT_FAILURE);
  }

  print_policy();

  exit(EXIT_SUCCESS);
}
