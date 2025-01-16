#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#define dbg(x) std::cout << #x << " = " << x << std::endl;
#define LEARNING_RATE 0.1
#define DISCOUNT_RATE 0.9
#define EPSILON 0.1

using std::string;

const void print(const std::string &txt) { std::cout << txt << std::endl; }

// Vetor com os possíveis movimentos do agente durante a busca
static const std::vector<std::pair<int, int>> moves = {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1}};

// Função auxiliar para checar se uma coordenada estará dentro dos limites do
// mapa
inline bool out_of_bounds(int x, int y, int dx, int dy, int map_width,
                          int map_height) {
  return x + dx < 0 or x + dx >= map_width or y + dy < 0 or
         y + dy >= map_height;
}

enum GroundType { Grass, HighGrass, Water, Fire, Objective, Wall };

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

std::vector<std::vector<GroundType>> map_;

std::vector<std::vector<GroundType>>
parse_input_file(const std::string &filename) {
  std::ifstream input_stream(filename);
  int height, width;
  input_stream >> width >> height;
  std::vector<std::vector<GroundType>> map_(height,
                                            std::vector<GroundType>(width));
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
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

int main(int argc, char **argv) {

  print("oi");
  if (argc != 6) {
    std::cout << "Usage: ./main [input_file_path] [modifier_id] x_i y_i n"
              << std::endl;
    exit(EXIT_FAILURE);
  }
  const std::string input_file_path = argv[1];
  parse_input_file(input_file_path);

  const std::string modifier_id = argv[2];
  int xi = std::stoi(argv[3]);
  int yi = std::stoi(argv[4]);
  int n = std::stoi(argv[5]);

  exit(EXIT_SUCCESS);
}
