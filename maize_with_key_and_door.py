
from collections import deque

def shortest_path_to_exit(grid):
    if not grid or not grid[0]:
        return -1

    rows, cols = len(grid), len(grid[0])

    start_r, start_c = -1, -1
    for r in range(rows):
        for c in range(cols):
            if grid[r][c] == 'S':
                start_r, start_c = r, c
                break
        if start_r != -1:
            break

    if start_r == -1:
        return -1

    queue = deque([(start_r, start_c, 0, 0)])
    visited = {(start_r, start_c, 0)}

    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]

    while queue:
        r, c, mask, dist = queue.popleft()

        if grid[r][c] == 'E':
            return dist

        for dr, dc in directions:
            nr, nc = r + dr, c + dc

            if 0 <= nr < rows and 0 <= nc < cols:
                cell = grid[nr][nc]

                if cell == '#':
                    continue

                if 'A' <= cell <= 'F' and cell != 'E':
                    key_bit = ord(cell.lower()) - ord('a')
                    if not (mask & (1 << key_bit)):
                        continue

                new_mask = mask

                if 'a' <= cell <= 'f':
                    key_bit = ord(cell) - ord('a')
                    new_mask |= (1 << key_bit)

                next_state = (nr, nc, new_mask)

                if next_state not in visited:
                    visited.add(next_state)
                    queue.append((nr, nc, new_mask, dist + 1))

    return -1


if __name__ == "__main__":
    n = int(input("Enter grid size N: "))

    grid = []
    print("Enter each row of the maze (e.g., S..#):")

    for i in range(n):
        row = input(f"Row {i + 1}: ").strip()

        if len(row) != n:
            print("Error: Each row must contain exactly N characters.")
            exit()

        grid.append(list(row))

    print("\nMaze Layout:")
    for row in grid:
        print("".join(row))

    result = shortest_path_to_exit(grid)
    print("\nShortest Path Length:", result)
