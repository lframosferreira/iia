import matplotlib.pyplot as plt
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


def random_benchmark() -> None:
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


# x_i, y_i = 1, 1
# x_f, y_f = k, k for k in 2..H (assuming square matrix map, will work for cidade and floresta)
def iterative_benchmark() -> None:
    x_i, y_i = 1, 1
    JUMP: int = 4
    for kind in ["time", "state", "cost"]:
        for algorithm in ALGORITHMS:
            # if algorithm == "IDS":
            #     continue
            print(algorithm)
            times: list[float] = []
            states: list[int] = []
            costs: list[float] = []
            for k in range(2, H + 1, JUMP):
                start: float = time.perf_counter()
                x_f, y_f = k, k
                if (x_f, y_f) not in non_wall_coordinates:
                    times.append(0)
                    states.append(0)
                    costs.append(0)
                    continue
                output: str = subprocess.run(
                    [
                        "./pathfinder",
                        input_file,
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
                number_of_expanded_states: int = int(output.split(" ")[-1])
                cost: float = float(output.split(" ")[0])
                times.append(total_time)
                states.append(number_of_expanded_states)
                costs.append(cost)
            if kind == "time":
                plt.plot(list(range(2, H + 1, JUMP)), times, label=algorithm)
            elif kind == "state":
                plt.plot(list(range(2, H + 1, JUMP)), states, label=algorithm)
            else:
                plt.plot(list(range(2, H + 1, JUMP)), costs, label=algorithm)
        plt.legend()
        plt.ylabel(
            "Tempo de execução (s)"
            if kind == "time"
            else "Número de estados expandidos"
            if kind == "state"
            else "Custo do caminho encontrado"
        )
        plt.xlabel("Diagonal")
        map: str = input_file.split(".")[0].split("/")[1]
        plt.title(
            f"Comparação entre tempo por algoritmo: {map} "
            if kind == "time"
            else f"Comparação entre número de estados expandidos por algoritmo: {map}"
            if kind == "state"
            else f"Comparação entre custo dos caminhos encontrados: {map}"
        )
        plt.savefig(f"{map}_{kind}_benchmark.png")
        plt.clf()


iterative_benchmark()
