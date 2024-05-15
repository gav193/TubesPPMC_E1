#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX 256
#define INF 1000000

typedef struct {
    int x, y;
} Point;

int n, m; // for rows and cols
char grid[MAX][MAX];
int distance[MAX][MAX];

int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

bool isValid(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < m && grid[x][y] != '#';
}

int dfs(Point p) {
    if (!isValid(p.x, p.y))
        return INF;

    if (distance[p.x][p.y] != -1)
        return distance[p.x][p.y];

    if (grid[p.x][p.y] == 'E')
        return distance[p.x][p.y] = 0;

    grid[p.x][p.y] = '#';  // Mark as visited

    int minDist = INF;
    for (int i = 0; i < 4; i++) { // to find the shortest path
        int nx = p.x + dx[i];
        int ny = p.y + dy[i];
        minDist = fmin(minDist, 1 + dfs((Point){nx, ny}));
    }

    grid[p.x][p.y] = '.';  // Unmark as visited

    return distance[p.x][p.y] = minDist;
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
    while (fgets(grid[row], MAX, stream)) {
        grid[row][strcspn(grid[row], "\n")] = '\0';
        row++;
    }

    fclose(stream);

    if (row == 0) {
        printf("Empty file or no valid data found!\n");
        return 1;
    }

    n = row;
    m = strlen(grid[0]);

    Point start = {-1, -1}, end = {-1, -1};
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            distance[i][j] = -1; // Initialize distance to -1
            if (grid[i][j] == 'S') {
                start = (Point){i, j};
                grid[i][j] = '.';
            }
            if (grid[i][j] == 'E') {
                end = (Point){i, j};
            }
        }
    }

    if (start.x == -1 || start.y == -1 || end.x == -1 || end.y == -1) {
        printf("Start or end point not found in the map!\n");
        return 1;
    }

    int result = dfs(start);

    if (result >= INF) {
        printf("No path from 'S' to 'E'.\n");
    } else {
        printf("Shortest path from 'S' to 'E': %d\n", result);
    }

    return 0;
}
