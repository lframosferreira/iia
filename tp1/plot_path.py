import sys

import matplotlib.pyplot as plt
import numpy as np

H, W = int(sys.argv[2]) + 1, int(sys.argv[3]) + 1


def create_grid(H, W, positions):
    # Create an empty grid with all values set to 0 (white)
    grid = np.zeros((H, W))

    # Set specified positions to 1 (blue)
    for row, col in positions:
        if 0 <= row < H and 0 <= col < W:  # Ensure the position is within bounds
            grid[row, col] = 1

    # Plotting
    _, ax = plt.subplots()
    cmap = plt.cm.get_cmap(
        "Reds", 2
    )  # Use 'Blues' colormap with 2 levels (white and blue)
    _ = ax.matshow(grid, cmap=cmap)

    # Customize grid lines and ticks
    ax.set_xticks(np.arange(-0.5, W, 1), minor=True)
    ax.set_yticks(np.arange(-0.5, H, 1), minor=True)
    ax.grid(which="minor", color="#d3d3d3", linestyle="-", linewidth=1)
    ax.tick_params(
        which="both", bottom=False, left=False, labelbottom=False, labelleft=False
    )

    plt.show()


with open(sys.argv[1], "r") as f:
    content: list[str] = f.read().split(" ")
    cost: float = float(content[0])
    indices: list[tuple[int, int]] = []
    for pos in content[1:]:
        pos = pos[1:]
        pos = pos[:-1]
        tup = pos.split(",")
        # work around to avoid problems with \n at end of output.txt file
        if len(tup) == 1:
            continue
        indices.append((int(tup[0]), int(tup[1])))
    create_grid(H, W, indices)
