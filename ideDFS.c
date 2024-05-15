#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 256

bool visited[MAX][MAX]; // Track visited cells
char direction[] = "UDLR";

bool isValid(int x, int y, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

// DFS function to find all unique paths from start to end
void dfs(char map[MAX][MAX], int x, int y, int endX, int endY, int rows, int cols, char currentPath[], char ans[][100], int* count) {
    if (x == endX && y == endY) {
        strcpy(ans[*count], currentPath); // after finding end point, copy current path to ans
        (*count)++; // paths found ++
        return;
    }

    visited[x][y] = true; // mark true for current traversed point 

    // Possible moves: Up, Down, Left, Right
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int newX = x + dx[i];
        int newY = y + dy[i];

        if (isValid(newX, newY, rows, cols) && !visited[newX][newY] && map[newX][newY] != '#') {
            int len = strlen(currentPath);
            currentPath[len] = direction[i]; // save current path in directions ex: "DDULRDD"
            currentPath[len + 1] = '\0'; // extend path by 1
            dfs(map, newX, newY, endX, endY, rows, cols, currentPath, ans, count); // traverse next point if valid
            currentPath[len] = '\0'; // Backtrack
        }
    }

    visited[x][y] = false; // Backtrack
}

void findPath(char map[MAX][MAX], int sx, int sy, int ex, int ey, int row, int col) {
    if (map[sx][sy] == 'S') {
        char currentPath[100]; // set as 100 for path length
        char ans[100][100]; // ans = array of paths
        int count = 0;
        memset(currentPath, 0, sizeof(currentPath)); // set all paths to empty
        memset(visited, false, sizeof(visited)); // set all visited to false
        dfs(map, sx, sy, ex, ey, row, col, currentPath, ans, &count); // apply dfs
        if (count == 0) {
            printf("-1\n"); // no paths found
        } else {
            for (int i = 0; i < count; i++) {
                printf("Path %d : \n", i+1);
                printf("(%d, %d)", sx,sy);
                //printf("%s\n", ans[i]); // to print or debug directions
                int tempx = sx;
                int tempy = sy;
                for (int j = 0; j < strlen(ans[i]); j++) { // convert directions to coordinates
                    if (ans[i][j] == 'D') {
                        tempy++;
                    } else if (ans[i][j] == 'U') {
                        tempy--;
                    } else if (ans[i][j] == 'R') {
                        tempx++;
                    } else if (ans[i][j] == 'L') {
                        tempx--;
                    }
                    printf(" -> (%d, %d)", tempx,tempy);
                }
                printf("\n");
                printf("Distance : %d", strlen(ans[i])); // final distance
                printf("\n");
            }
        }
        printf("Total number of unique paths from 'S' to 'E': %d\n", count);
    }
}

int main() {
    char filename[MAX];
    // input filename from user
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

    while (fgets(line, sizeof(line), stream)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        strcpy(map[row], line);
        row++;
        col = strlen(line); // Assuming all rows are of same length
    }

    fclose(stream);

    // Find the starting point 'S' and ending point 'E'
    int startX = -1, startY = -1, endX = -1, endY = -1;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (map[i][j] == 'S') {
                startX = i;
                startY = j;
            } else if (map[i][j] == 'E') {
                endX = i;
                endY = j;
            }
        }
    }

    if (startX == -1 || startY == -1 || endX == -1 || endY == -1) {
        printf("Invalid map: Start or End point not found.\n");
        return 1;
    }

    // Perform DFS to find all unique paths from 'S' to 'E'
    findPath(map, startX, startY, endX, endY, row, col);
    return 0;
}
