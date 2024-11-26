import re
import matplotlib.pyplot as plt
import numpy as np


def parse_file(file_path):
    grid = []
    with open(file_path, "r") as file:
        for line in file:
            matches = re.findall(r"\((\d+), ([^)]+)\)", line)
            row = [(int(key), value) for key, value in matches]
            grid.append(row)
    return grid


def plot_grid(grid):
    # Determine the size of the grid
    n_rows = len(grid)
    n_cols = max(len(row) for row in grid)

    # Create a matrix for visualization
    matrix = np.zeros((n_rows, n_cols), dtype=int)

    # Fill the matrix with 1 for cells that are not (0, "0")
    for i, row in enumerate(grid):
        for j, cell in enumerate(row):
            if cell != (0, "0"):
                matrix[i, j] = 1

    # Plot the grid
    fig, ax = plt.subplots(figsize=(n_cols, n_rows))
    cmap = plt.cm.Blues
    ax.imshow(matrix, cmap=cmap, aspect="equal")

    # Add gridlines for clarity
    ax.set_xticks(np.arange(-0.5, n_cols, 1), minor=True)
    ax.set_yticks(np.arange(-0.5, n_rows, 1), minor=True)
    ax.grid(which="minor", color="black", linestyle="-", linewidth=0.5)
    ax.tick_params(which="minor", size=0)
    ax.set_xticks([])
    ax.set_yticks([])

    plt.show()


def main():
    input_file = "output.txt"  # Replace with your file name
    grid = parse_file(input_file)
    plot_grid(grid)


if __name__ == "__main__":
    main()
