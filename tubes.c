#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 256

int pathCount = 0;
bool visited[MAX][MAX]; // Track visited cells

bool isValid(int x, int y, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

// DFS function to find all unique paths from start to end
void dfs(char map[MAX][MAX], int x, int y, int endX, int endY, int rows, int cols) {
    if (x == endX && y == endY) {
        pathCount++;
        return;
    }

    visited[x][y] = true;

    // Possible moves: Up, Down, Left, Right
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int newX = x + dx[i];
        int newY = y + dy[i];

        if (isValid(newX, newY, rows, cols) && !visited[newX][newY] && map[newX][newY] != '#') {
            dfs(map, newX, newY, endX, endY, rows, cols);
        }
    }

    visited[x][y] = false; // Backtrack
}

int main() {
    char filename[MAX];
    // input filename from user
    printf("Enter file name : ");
    scanf("%s", filename);

    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {
        printf("File not found!\n");
        return 1;
    }

    int row = 0;
    int col = MAX;
    char line[MAX]; // assumption: no lines have varying lengths
    while (fgets(line, MAX, stream)) {
        printf("%s \n", line);
        row++; // save number of rows (lines)
        int cur = strlen(line);
        if (cur < col) {
            col = cur; // to save the number of cols with no newline char
        }
    }

    rewind(stream);

    char map[MAX][MAX];
    int track = 0;

    while (fgets(line, MAX, stream)) {
        line[strcspn(line,"\n")] = '\0';
        strncpy(map[track], line, col-1);
        map[track][col-1] = '\0';
        track++;
    }
    // parse into matrix of char complete
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

    // Perform DFS to find all unique paths from 'S' to 'E'
    dfs(map, startX, startY, endX, endY, row, col);

    printf("Total number of unique paths from 'S' to 'E': %d\n", pathCount);

    return 0;
}
