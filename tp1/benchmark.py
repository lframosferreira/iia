import subprocess
import time
import random
import sys

ALGORITHMS: list[str] = ["BFS", "IDS", "UCS", "Greedy", "Astar"]
NUMBER_OF_RUNS: int = 1


def parse_input_and_retrieve_non_wall_coordinates(
    inputfile: str,
) -> list[tuple[int, int]]:
    non_wall_coordinates: list[tuple[int, int]] = []
    return non_wall_coordinates


input_file: str = sys.argv[1]
parse_input_and_retrieve_non_wall_coordinates(input_file)

H, W = 256, 256  # hardcoded do cidades.map
for _ in range(NUMBER_OF_RUNS):
    x_i: int = random.randint(1, 256)
    x_f: int = random.randint(1, 256)
    y_i: int = random.randint(1, 256)
    y_f: int = random.randint(1, 256)
    print(x_i, x_f, y_i, y_f)
    for algorithm in ALGORITHMS:
        start: float = time.perf_counter()
        output: str = subprocess.run(
            [
                "./pathfinder",
                "mapas/cidade.map",
                algorithm,
                str(x_i),
                str(x_f),
                str(y_i),
                str(y_f),
            ],
            capture_output=True,
            text=True,
        ).stdout
        total_time: float = time.perf_counter() - start
        cost: str = output.split(" ")[0]
        print(algorithm, end=" ")
        print(cost, end=" ")
        print(total_time)
