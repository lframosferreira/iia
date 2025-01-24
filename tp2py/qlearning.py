from random import random, randint, choice
import sys
import numpy as np
from enum import IntEnum

Grass = "."
HighGrass = ";"
Water = "+"
Fire = "x"
Objective = "O"
Wall = "@"

xi = int(sys.argv[3])
yi = int(sys.argv[4])
n = int(sys.argv[5])

EPSILON = 0.1
LEARNING_RATE = 0.1
DISCOUNT_RATE = 0.9


class Actions(IntEnum):
    Left = 0
    Right = 1
    Down = 2
    Up = 3


actions_moves: list[tuple[int, int]] = [(0, -1), (0, 1), (1, 0), (-1, 0)]
chars = ["<", ">", "v", "^"]


def get_ground_type_reward(gt, modifier):
    if modifier == "standard":
        if gt == Grass:
            return -0.1
        elif gt == HighGrass:
            return -0.3
        elif gt == Water:
            return -1.0
        elif gt == Fire:
            return -10.0
        elif gt == Objective:
            return 10.0
        else:  # Wall
            return -1e9
    elif modifier == "positive":
        if gt == Grass:
            return 3.0
        elif gt == HighGrass:
            return 1.5
        elif gt == Water:
            return 1.0
        elif gt == Fire:
            return 0.0
        elif gt == Objective:
            return 10.0
        else:  # Wall
            return -1e9


def parse_input(path):
    mapa = None
    with open(path, "r") as f:
        WIDTH, HEIGHT = list(map(int, f.readline().split(" ")))
        mapa = np.full((HEIGHT, WIDTH), "L", dtype="<U1")
        for i, line in enumerate(f.readlines()):
            if line[-1] == "\n":
                line = line[:-1]
            for j, c in enumerate(line):
                mapa[i][j] = c
    return mapa


def qlearning(mapa, pesos):
    h, w = mapa.shape
    for idx in range(n):
        cx = xi
        cy = yi
        while mapa[cx][cy] not in [Fire, Objective]:
            # print(idx)
            action = None
            if random() <= EPSILON:
                action = randint(0, 3)
            else:
                action = np.argmax(pesos[cx, cy])
            dx, dy = actions_moves[action]
            nx = cx
            ny = cy
            reward = -1e9
            if cx + dx < 0 or cx + dx >= h or cy + dy < 0 or cy + dy >= w:
                pass
            elif mapa[cx + dx][cy + dy] == Wall:
                pass
            else:
                nx += dx
                ny += dy
                reward = get_ground_type_reward(mapa[nx][ny], sys.argv[2])
            pesos[cx, cy, action] += LEARNING_RATE * (
                reward + DISCOUNT_RATE * np.max(pesos[nx, ny]) - pesos[cx, cy, action]
            )
            cx = nx
            cy = ny


def stochastic(mapa, pesos):
    h, w = mapa.shape
    for _ in range(n):
        cx = xi
        cy = yi
        while mapa[cx][cy] not in [Fire, Objective]:
            # print(idx)
            original_action = np.argmax(pesos[cx, cy])
            action = original_action.copy()
            if random() <= 0.2:
                if action in [Actions.Up, Actions.Down]:
                    action = int(choice([Actions.Left, Actions.Right]))
                else:
                    action = int(choice([Actions.Up, Actions.Down]))
            dx, dy = actions_moves[action]
            nx = cx
            ny = cy
            reward = -1e9
            if cx + dx < 0 or cx + dx >= h or cy + dy < 0 or cy + dy >= w:
                pass
            elif mapa[cx + dx][cy + dy] == Wall:
                pass
            else:
                nx += dx
                ny += dy
                reward = get_ground_type_reward(mapa[nx][ny], "standard")
            pesos[cx, cy, original_action] += LEARNING_RATE * (
                reward
                + DISCOUNT_RATE * np.max(pesos[nx, ny])
                - pesos[cx, cy, original_action]
            )
            cx = nx
            cy = ny


def main() -> None:
    mapa = parse_input(sys.argv[1])
    h, w = mapa.shape
    pesos = np.random.uniform(-1, 1, (h, w, 4))

    if sys.argv[2] == "stochastic":
        stochastic(mapa, pesos)
    else:
        qlearning(mapa, pesos)

    for i, line in enumerate(pesos):
        for j, col in enumerate(line):
            if mapa[i][j] in [Wall, Fire, Objective]:
                print(mapa[i][j], end="")
            else:
                print(chars[np.argmax(col)], end="")
        print()


if __name__ == "__main__":
    main()
