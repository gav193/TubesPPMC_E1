#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

typedef struct pos {
    int x;
    int y;
} pos;

int main() {
    char filename[MAX];
    // Input filename from user
    printf("Enter file name: ");
    scanf("%s", filename);

    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {
        printf("File not found!\n");
        return 1;
    }

    int row = 0;
    int col = 0;
    char line[MAX];
    char map[MAX][MAX];

    // Read the file and store it in the map
    while (fgets(line, sizeof(line), stream)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        strcpy(map[row], line);
        row++;
        col = strlen(line); // Assuming all rows are of same length
    }

    fclose(stream);

    // Clone map to solution matrix
    char map2[row][col];
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            map2[i][j] = map[i][j];
        }
    }

    // Create solution matrix
    pos sol[MAX];
    for (int i = 0; i < MAX; i++) {
        sol[i].x = -1;
        sol[i].y = -1;
    }

    // Find location of start and end
    pos start = { -1, -1 };
    pos end = { -1, -1 };
    pos curr;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (map2[i][j] == 'S') {
                start.x = j;
                start.y = i;
            }
            if (map2[i][j] == 'E') {
                end.x = j;
                end.y = i;
            }
        }
    }

    curr.x = start.x;
    curr.y = start.y;

    // Algorithm will always prioritize vertical movement
    int sol_counter = 0;
    int finish = -1;

    // Start position in solution path
    sol[sol_counter++] = start;

    // Function to find solution of maze using backtracking algorithm
    while (sol_counter > 0) {
        curr = sol[--sol_counter];

        // Case finished
        if (curr.x == end.x && curr.y == end.y) {
            sol[sol_counter++] = curr; // Push current position back to stack
            finish = 1;
            break;
        }

        // Mark current position as part of the path
        map2[curr.y][curr.x] = '+';

        // Try moving in all four directions
        pos directions[4] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

        for (int i = 0; i < 4; i++) {
            pos next = { curr.x + directions[i].x, curr.y + directions[i].y };
            if (next.x >= 0 && next.x < col && next.y >=0 && next.y < row && (map2[next.y][next.x] == '.' || map2[next.y][next.x] == 'E')) {
                sol[sol_counter++] = curr; // Push current position back to stack
                sol[sol_counter++] = next; // Push next position to stack
                break;
            }
        }
    }

    // Output
    if (finish) {
        printf("Maze successfully solved\n");
        printf("Path taken is:\n");
        for (int i = 0; i < sol_counter; i++) {
            printf("%d. (%d,%d)\n", i, sol[i].x, sol[i].y);
        }
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                printf("%c", map2[i][j]);
            }
            printf("\n");
        }
    } else {
        printf("Failed to solve the maze\n");
    }

    return 0;
}
