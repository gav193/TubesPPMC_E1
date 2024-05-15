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
        //printf("%s \n", line);
        row++; // save number of rows (lines)
        int cur = strlen(line);
        if (cur < col) {
            col = cur; // to save the number of cols with no newline char
        }
    }

    rewind(stream);

    char map[MAX][MAX];
    int track = 0;
    int height = row;
    if (row%2 == 0) { 
        height = row / 2;

    } else { 
        height = (row+1)/2;
    }

    char **top = (char **)malloc(height * sizeof(char *));
    char **bottom = (char **)malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++) {
        top[i] = (char *)malloc(col * sizeof(char));
        bottom[i] = (char *)malloc(col * sizeof(char));
    }

    while (fgets(line, MAX, stream)) {
        line[strcspn(line,"\n")] = '\0';
        strncpy(map[track], line, col-1);
        map[track][col-1] = '\0';
        track++;
    }
    // parse into matrix of char complete

    for (int i = 0; i < height; i++) {
        strncpy(top[i], map[i], col);
        strncpy(bottom[i], map[i + height], col);
    }

    // Find the starting point 'S' and ending point 'E'
    int startX_top = -1, startY_top = -1, endX_top = -1, endY_top = -1;
    int startX_bottom = -1, startY_bottom = -1, endX_bottom = -1, endY_bottom = -1;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (top[i][j] == 'S') {
                startX_top = i;
                startY_top = j;
            } else if (top[i][j] == 'E') {
                endX_top = i;
                endY_top = j;
            } 
            if (bottom[i][j] == 'S') {
                startX_bottom = i;
                startY_bottom = j;
            } else if (bottom[i][j] == 'E') {
                endX_bottom = i;
                endY_bottom = i;
            }
        }
    }

    // Perform DFS to find all unique paths from 'S' to 'E'
    // case : S and E in the same half
    if (startX_top != -1 && startY_top != -1 && endX_top!= -1 && endY_top!= -1) {
        dfs(top, startX_top, startY_top, endX_top, endY_top, height, col);
    } else if (startX_bottom != -1 && startY_bottom != -1 && endX_bottom!=-1 && endY_bottom!= -1){
        dfs(bottom, startX_bottom,startY_bottom,endX_bottom,endY_bottom,height,col);
    }
    
    // to be added : 
    /*
    find all '.' on border where row is shared between top and bottom
    set '.' as 'T' marking temporary flagpoints
    find path from S to 'T':
        if path exists, continue traversing from 'T' to 'E' and pathcount++
        if path doesn't exist, set 'T' as '.' and pathcount stays
    */

    printf("Total number of unique paths from 'S' to 'E': %d\n", pathCount);

    for (int i = 0; i < height; i++) {
        free(top[i]);
        free(bottom[i]);
    }
    free(top);
    free(bottom);
    fclose(stream);
    return 0;
}
