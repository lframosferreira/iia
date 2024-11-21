import random
import subprocess
import sys
import time

ALGORITHMS: list[str] = ["BFS", "IDS", "UCS", "Greedy", "Astar"]
NUMBER_OF_RUNS: int = 10


def parse_input_and_retrieve_non_wall_coordinates(
    input_file: str,
) -> tuple[list[tuple[int, int]], int, int]:
    non_wall_coordinates: list[tuple[int, int]] = []
    H, W = None, None
    with open(input_file, "r") as f:
        lines = f.readlines()
        H, W = tuple(map(lambda x: int(x), lines[0].split(" ")))
        for i, line in enumerate(lines[1:]):
            i += 1
            for j, ground in enumerate(line):
                j += 1
                if ground != "@":
                    non_wall_coordinates.append((i, j))

    return non_wall_coordinates, H, W


input_file: str = sys.argv[1]
non_wall_coordinates, H, W = parse_input_and_retrieve_non_wall_coordinates(input_file)

for _ in range(NUMBER_OF_RUNS):
    x_i, y_i = random.choice(non_wall_coordinates)
    non_wall_coordinates.sort(key=lambda x: abs(x_i - x[0]) + abs(y_i - x[1]))
    x_f, y_f = random.choice(non_wall_coordinates[150:])
    for algorithm in ALGORITHMS:
        start: float = time.perf_counter()
        print(x_i, y_i, x_f, y_f)
        output: str = subprocess.run(
            [
                "./pathfinder",
                "mapas/cidade.map",
                algorithm,
                str(x_i),
                str(y_i),
                str(x_f),
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
    print("--------------------------------------")
