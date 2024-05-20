#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_SIZE 100
#define INF INT_MAX

typedef struct {
    int x, y;
} Point;

typedef struct {
    char grid[MAX_SIZE][MAX_SIZE];
    int dist[MAX_SIZE][MAX_SIZE];
    int visited[MAX_SIZE][MAX_SIZE];
    Point parent[MAX_SIZE][MAX_SIZE];
    int width, height;
    Point start, end;
} Maze;

void readMaze(const char *filename, Maze *maze) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file.");
        exit(EXIT_FAILURE);
    }

    maze->width = 0;
    maze->height = 0;
    char line[MAX_SIZE];
    int row = 0;
    int firstLine = 1;  

    while (fgets(line, sizeof(line), file)) {
        int col = 0;
        while (line[col] != '\n' && line[col] != '\0') {
            maze->grid[row][col] = line[col];
            if (line[col] == 'S') {
                maze->start.x = row;
                maze->start.y = col;
            } else if (line[col] == 'E') {
                maze->end.x = row;
                maze->end.y = col;
            }
            col++;
        }

        if (firstLine) {
            maze->width = col;
            firstLine = 0; 
        } else if (col != maze->width) {
            fprintf(stderr, "Kolom maze memiliki panjang yang berbeda!\n");
            fclose(file);
            exit(EXIT_FAILURE);
        }

        row++;
    }
    maze->height = row;

    fclose(file);
}

void dijkstra(Maze *maze) {
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            maze->dist[i][j] = INF;
            maze->visited[i][j] = 0;
            maze->parent[i][j] = (Point) {-1, -1};
        }
    }

    maze->dist[maze->start.x][maze->start.y] = 0;

    for (int k = 0; k < maze->height * maze->width; k++) {
        int minDist = INF;
        Point u = { -1, -1 };
        for (int i = 0; i < maze->height; i++) {
            for (int j = 0; j < maze->width; j++) {
                if (!maze->visited[i][j] && maze->dist[i][j] < minDist) {
                    minDist = maze->dist[i][j];
                    u.x = i; 
                    u.y = j;
                }
            }
        }

        if (u.x == -1 && u.y == -1) break;
        maze->visited[u.x][u.y] = 1;

        for (int i = 0; i < 4; i++) {
            int nx = u.x + dx[i];
            int ny = u.y + dy[i];
            if (nx >= 0 && ny >= 0 && nx < maze->height && ny < maze->width && maze->grid[nx][ny] != '#') {
                int newDist = maze->dist[u.x][u.y] + 1;
                if (newDist < maze->dist[nx][ny]) {
                    maze->dist[nx][ny] = newDist;
                    maze->parent[nx][ny] = u;
                }
            }
        }
    }
}

void printShortestPath(Maze *maze) {
    if (maze->dist[maze->end.x][maze->end.y] == INF) {
        printf("No path found.\n");
        return;
    }

    printf("Shortest path distance: %d\n", maze->dist[maze->end.x][maze->end.y]);

    Point path[MAX_SIZE * MAX_SIZE];
    int pathLength = 0;
    for (Point at = maze->end; at.x != -1 && at.y != -1; at = maze->parent[at.x][at.y]) {
        path[pathLength++] = at;
    }

    printf("Path: ");
    for (int i = pathLength - 1; i >= 0; i--) {
        printf("(%d, %d)", path[i].y, path[i].x);
        if (i > 0) {
            printf(" -> ");
        }
    }
    printf("\n");
}

int main() {
    Maze maze;
    char file[100];
    clock_t startclk, endclk;
    double cpu_time_used;

    printf("Masukkan File Txt Struktur Maze : ");
    scanf("%s", file);

    readMaze(file, &maze);
    startclk = clock();
    dijkstra(&maze);
    printShortestPath(&maze);
    endclk = clock();
    cpu_time_used = ((double) (endclk-startclk)) / CLOCKS_PER_SEC;
    printf("Waktu yang diperlukan: %f", cpu_time_used);
    return 0;
}
