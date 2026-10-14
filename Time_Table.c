
#include <stdio.h>

int graph[20][20], color[20];
int N, M, R;

// Check if a course can be assigned to a slot
int safe(int v, int c) {
    int count = 0;

    for (int i = 0; i < N; i++) {
        if (color[i] == c)
            count++;

        if (graph[v][i] && color[i] == c)
            return 0;
    }

    if (count >= R)
        return 0;

    return 1;
}

// Greedy algorithm
int greedy() {
    int maxColor = -1;

    for (int i = 0; i < N; i++) {
        color[i] = -1;

        for (int c = 0; c < N; c++) {
            if (safe(i, c)) {
                color[i] = c;
                if (c > maxColor)
                    maxColor = c;
                break;
            }
        }

        if (color[i] == -1)
            return -1;
    }

    return maxColor + 1;
}

// Exact backtracking
int solve(int v, int slots) {
    if (v == N)
        return 1;

    for (int c = 0; c < slots; c++) {
        if (safe(v, c)) {
            color[v] = c;

            if (solve(v + 1, slots))
                return 1;

            color[v] = -1;
        }
    }

    return 0;
}

int exact() {
    for (int slots = 1; slots <= N; slots++) {
        for (int i = 0; i < N; i++)
            color[i] = -1;

        if (solve(0, slots))
            return slots;
    }

    return -1;
}

// Print timetable
void display(int slots) {
    if (slots == -1) {
        printf("No timetable found.\n");
        return;
    }

    for (int c = 0; c < slots; c++) {
        printf("Slot %d: ", c + 1);

        for (int i = 0; i < N; i++) {
            if (color[i] == c)
                printf("%d ", i + 1);
        }

        printf("\n");
    }
}

int main() {
    printf("Enter N (courses), M (conflicts), R (halls): ");
    scanf("%d %d %d", &N, &M, &R);

    if (N < 1 || N > 20 || M < 0 || R < 1) {
        printf("Invalid input.\n");
        return 0;
    }

    printf("Enter conflicting course pairs:\n");

    for (int i = 0; i < M; i++) {
        int u, v;
        scanf("%d %d", &u, &v);

        if (u < 1 || u > N || v < 1 || v > N || u == v) {
            printf("Invalid course pair.\n");
            return 0;
        }

        graph[u - 1][v - 1] = 1;
        graph[v - 1][u - 1] = 1;
    }

    int g = greedy();

    printf("\nGREEDY RESULT\n");
    printf("Slots: %d\n", g);
    printf("Optimality proven: No\n");
    display(g);

    int e = exact();

    printf("\nEXACT BACKTRACKING RESULT\n");
    printf("Minimum slots: %d\n", e);
    printf("Optimality proven: Yes\n");
    display(e);

    return 0;
}
