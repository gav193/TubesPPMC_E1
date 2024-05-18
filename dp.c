#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX 256
#define MAX_PATHS 10000
#define MAX_PATH_LENGTH 100
#define INF 10000 // A large value to represent infinity

char maze[MAX][MAX];
char foundPaths[MAX_PATHS][MAX_PATH_LENGTH]; // To store paths
int pathCount = 0; // To count paths

bool visited[MAX][MAX];

// Function to check if a cell is within the bounds of the maze
bool isValid(int x, int y, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

// Function to find all paths from (x, y) to 'E' and their lengths
void findPaths(int x, int y, int rows, int cols, char currentPath[], int step, bool* found) {
    // Base case
    if (!isValid(x, y, rows, cols) || maze[x][y] == '#' || visited[x][y]) {
        return; // Return for invalid cells, obstacles, or already visited cells
    }
    if (maze[x][y] == 'E') {
        currentPath[step] = '\0'; // Null terminate the path
        strcpy(foundPaths[pathCount], currentPath); // Store the path
        pathCount++; // Increment path count
        *found = true; // Set found flag to true
        return;
    }

    // Mark current cell as visited
    visited[x][y] = true;

    // Explore adjacent cells
    currentPath[step] = 'D';
    findPaths(x + 1, y, rows, cols, currentPath, step + 1, found);
    currentPath[step] = 'U';
    findPaths(x - 1, y, rows, cols, currentPath, step + 1, found);
    currentPath[step] = 'R';
    findPaths(x, y + 1, rows, cols, currentPath, step + 1, found);
    currentPath[step] = 'L';
    findPaths(x, y - 1, rows, cols, currentPath, step + 1, found);

    // Unmark current cell
    visited[x][y] = false; // Restore the original character
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
    char line[MAX];
    char map[MAX][MAX];

    fgets(line, sizeof(line), stream);
    int col = strlen(line) -1 ; // initialize column value and delete newline char
    rewind(stream);

    while (fgets(line, sizeof(line), stream)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        strcpy(maze[row], line);
        row++;
        if (col != strlen(line)) {
            printf("Found uneven row of line at %d\n", row);
            exit(1);
        }
    }
    fclose(stream);

    // Print maze for debugging
    printf("Maze contents:\n");
    for (int i = 0; i < row; i++) {
        printf("%s\n", maze[i]);
    }

    // Initialize visited table with false
    memset(visited, false, sizeof(visited));

    // Find the 'S' position
    int startX = -1, startY = -1;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (maze[i][j] == 'S') {
                startX = i;
                startY = j;
                break;
            }
        }  
    }

    printf("Start position: (%d, %d)\n", startX, startY);

    if (startX != -1) {
        char currentPath[MAX_PATH_LENGTH] = {0};
        int shortestPathLength = INF;
        int longestPathLength = 0;
        clock_t startclk, endclk;
        double cpu_time_used;

        startclk = clock();
        bool found = false; // Initialize found flag
        findPaths(startX, startY, row, col, currentPath, 0, &found);

        if (!found) {
            printf("No path found from 'S' to 'E'.\n");
            return 0; // Exit if no path is found
        }

        // Print all paths and length
        printf("Total number of paths from 'S' to 'E': %d\n", pathCount);
        printf("Paths:\n");
        for (int i = 0; i < pathCount; i++) {
            int tempx = startX;
            int tempy = startY;
            printf("Path %d: (%d, %d)", i + 1, startX, startY);
            for (int j = 0; j < strlen(foundPaths[i]); j++) {
                switch(foundPaths[i][j]) {
                    case 'D' :
                        tempx++;
                        break;
                    case 'U' :
                        tempx--;
                        break;
                    case 'L' :
                        tempy--;
                        break;
                    case 'R' :
                        tempy++;
                        break;
                }
                printf("-> (%d, %d) ", tempx, tempy);
            }
            printf("\nLength: %lu\n", strlen(foundPaths[i]));
        }

        // Determine the shortest and longest paths
        for (int i = 0; i < pathCount; i++) {
            int pathLength = strlen(foundPaths[i]);
            if (pathLength < shortestPathLength) {
                shortestPathLength = pathLength;
            }
            if (pathLength > longestPathLength) {
                longestPathLength = pathLength;
            }
        }

        // Print shortest paths
        printf("\nShortest path length: %d\n", shortestPathLength);
        for (int i = 0; i < pathCount; i++) {
            if (strlen(foundPaths[i]) == shortestPathLength) {
                printf("Path %d: (%d, %d)", i + 1, startX, startY);
                int tempx = startX;
                int tempy = startY;
                for (int j = 0; j < strlen(foundPaths[i]); j++) {
                    switch(foundPaths[i][j]) {
                        case 'D' :
                            tempx++;
                            break;
                        case 'U' :
                            tempx--;
                            break;
                        case 'L' :
                            tempy--;
                            break;
                        case 'R' :
                            tempy++;
                            break;
                    }
                    printf("-> (%d, %d) ", tempx, tempy);
                }
                printf("\n");
            }
        }

        // Print longest paths
        printf("\nLongest path length: %d\n", longestPathLength);
        for (int i = 0; i < pathCount; i++) {
            if (strlen(foundPaths[i]) == longestPathLength) {
                printf("Path %d: (%d, %d)", i + 1, startX, startY);
                int tempx = startX;
                int tempy = startY;
                for (int j = 0; j < strlen(foundPaths[i]); j++) {
                    switch(foundPaths[i][j]) {
                        case 'D' :
                            tempx++;
                            break;
                        case 'U' :
                            tempx--;
                            break;
                        case 'L' :
                            tempy--;
                            break;
                        case 'R' :
                            tempy++;
                            break;
                    }
                    printf("-> (%d, %d) ", tempx, tempy);
                }
                printf("\n");
            }
        }
        endclk = clock();
        cpu_time_used = ((double) (endclk - startclk)) / CLOCKS_PER_SEC;
        printf("Waktu yang diperlukan: %f",cpu_time_used);
    } else {
        printf("Start 'S' position not found.\n");
    }

    return 0;
}
