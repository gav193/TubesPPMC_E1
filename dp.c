#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 256
#define MAX_PATHS 100
#define MAX_PATH_LENGTH 100

char maze[MAX][MAX];
int memo[MAX][MAX];
char foundPaths[MAX_PATHS][MAX_PATH_LENGTH]; // To store paths
int pathCount = 0; // To count paths
int shortestPathLength = MAX_PATH_LENGTH; // Initialize with a large value
int longestPathLength = 0;

// Function to check if a cell is within the bounds of the maze
bool isValid(int x, int y, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

// Dynamic programming function to find all paths from (x, y) to 'E' and their lengths
void dp(int x, int y, int rows, int cols, char currentPath[], int step) {
    // Base case
    if (!isValid(x, y, rows, cols) || maze[x][y] == '#') {
        return; // Return for invalid cells or obstacles
    }
    if (maze[x][y] == 'E') {
        currentPath[step] = '\0'; // Null terminate the path
        strcpy(foundPaths[pathCount], currentPath); // Store the path
        int pathLength = strlen(currentPath);
        if (pathLength < shortestPathLength) {
            shortestPathLength = pathLength; // Update shortest path length
        }
        if (pathLength > longestPathLength) {
            longestPathLength = pathLength;
        }
        pathCount++; // Increment path count
        return;
    }

    // Mark current cell as visited
    char original = maze[x][y];
    maze[x][y] = '#'; // Use a different character to avoid extra boolean array for visited

    // Explore adjacent cells
    currentPath[step] = 'D';
    dp(x + 1, y, rows, cols, currentPath, step + 1);
    currentPath[step] = 'U';
    dp(x - 1, y, rows, cols, currentPath, step + 1);
    currentPath[step] = 'R';
    dp(x, y + 1, rows, cols, currentPath, step + 1);
    currentPath[step] = 'L';
    dp(x, y - 1, rows, cols, currentPath, step + 1);

    // Unmark current cell
    maze[x][y] = original; // Restore the original character
}

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

    while (fgets(line, sizeof(line), stream)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        strcpy(maze[row], line);
        row++;
        col = strlen(line); // Assuming all rows are of the same length
    }

    fclose(stream);

    // Print maze for debugging
    printf("Maze contents:\n");
    for (int i = 0; i < row; i++) {
        printf("%s\n", maze[i]);
    }

    // Find the start 'S' position
    int startX = -1, startY = -1;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (maze[i][j] == 'S') {
                startX = i;
                startY = j;
                break;
            }
        }
        if (startX != -1) break;
    }

    printf("Start position: (%d, %d)\n", startX, startY);

    // If start position found, find all paths
    if (startX != -1) {
        char currentPath[MAX_PATH_LENGTH] = {0};
        dp(startX, startY, row, col, currentPath, 0);

        // Print paths and their lengths
        printf("Total number of paths from 'S' to 'E': %d\n", pathCount);
        printf("Paths:\n");
        for (int i = 0; i < pathCount; i++) {
            printf("Path %d: %s\n", i + 1, foundPaths[i]);
            printf("Length: %lu\n", strlen(foundPaths[i]));
        }
        for (int i = 0; i < pathCount; i++) {
            if (strlen(foundPaths[i])==shortestPathLength) {
                printf("\nShortest path length: %d\n", shortestPathLength);
                printf("Path : %s\n", foundPaths[i]);
            }
            if (strlen(foundPaths[i]) == longestPathLength) {
                printf("\nLongest path length: %d\n", longestPathLength);
                printf("Path : %s\n", foundPaths[i]);
            }
        }

    } else {
        printf("Start 'S' position not found.\n");
    }

    return 0;
}
