#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define MAX_L 256
#define abs(a) ((a > 0) ? (a) : (-a))

typedef struct {
    int x_pos;
    int y_pos;
} Pos;

typedef struct {
    int open;
    int closed;
    int f, g, h;
    Pos parent;
} Cell;

int isValid(int ROW, int COL, Pos pos) {
    int row = pos.y_pos;
    int col = pos.x_pos;
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

int isObstacle(Pos pos, char map[MAX_L][MAX_L]) {
    return (map[pos.y_pos][pos.x_pos] == '#');
}

int isSame(Pos pos_a, Pos pos_b) {
    return (pos_a.x_pos == pos_b.x_pos) && (pos_a.y_pos == pos_b.y_pos);
}

int hValue(Pos curr, Pos dest) {
    int D = 1;
    return D * (abs((curr.y_pos - dest.y_pos)) + abs((curr.x_pos - dest.x_pos)));
}

int findStartEnd(char map[MAX_L][MAX_L], int ROW, int COL, Pos* start, Pos* end) {
    int count = 0;
    for (int y = 0; y < ROW; y++) {
        for (int x = 0; x < COL; x++) {
            if (map[y][x] == 'E') {
                end->x_pos = x;
                end->y_pos = y;
                count++;
            } else if (map[y][x] == 'S') {
                start->x_pos = x;
                start->y_pos = y;
                count++;
            }
        }
    }
    return (count == 2);
}

int findminF(Cell** celldetails, int ROW, int COL, Pos* min) {
    int min_f = 999999;
    Pos temp;
    int found = 0;
    for (int y = 0; y < ROW; y++) {
        for (int x = 0; x < COL; x++) {
            if (celldetails[y][x].open == 1 && celldetails[y][x].f < min_f) {
                temp.x_pos = x;
                temp.y_pos = y;
                min_f = celldetails[y][x].f;
                found = 1;
            }
        }
    }
    *min = temp;
    return found;
}

int A_star(char map[MAX_L][MAX_L], int ROW, int COL, Pos dest, Cell** cellDetails);
void tracePath(Cell** cellDetails, Pos start, Pos end, char map[MAX_L][MAX_L],int ROW, int COL);

void start_Astar(char map[MAX_L][MAX_L], const int ROW, const int COL) {
    Pos start;
    Pos end;
    if (findStartEnd(map, ROW, COL, &start, &end) == 0) {
        printf("Map Invalid, Start and/or END not found\n");
        return;
    } else {
        printf("MAP VALID\n S: %d,%d\n E: %d,%d\n", start.x_pos, start.y_pos, end.x_pos, end.y_pos);
    }
    Cell** cellDetails = (Cell**)malloc(sizeof(Cell*) * ROW);
    for (int i = 0; i < ROW; i++) {
        cellDetails[i] = (Cell*)calloc(COL, sizeof(Cell));
    }

    cellDetails[start.y_pos][start.x_pos].f = 0;
    cellDetails[start.y_pos][start.x_pos].g = 0;
    cellDetails[start.y_pos][start.x_pos].h = 0;
    cellDetails[start.y_pos][start.x_pos].open = 1;
    cellDetails[start.y_pos][start.x_pos].parent.x_pos = -1;
    cellDetails[start.y_pos][start.x_pos].parent.y_pos = -1;

    if (A_star(map, ROW, COL, end, cellDetails) == 0) {
        printf("No Path Found\n");
    } else {
        printf("PATH FOUND\n");
        tracePath(cellDetails, start, end, map, ROW, COL);
    }

    for (int i = 0; i < ROW; i++) {
        free(cellDetails[i]);
    }
    free(cellDetails);
}

int addSuccessor(Cell* tempCell, Cell* currCell, Pos temp, Pos curr, Pos dest, char map[MAX_L][MAX_L], const int ROW, const int COL) {
    int gNew, hNew, fNew;
    if (isValid(ROW, COL, temp)) {
        if (isSame(temp, dest)) {
            tempCell->parent = curr;
            return 1;
        } else if (tempCell->closed == 0 && isObstacle(temp, map) == 0) {
            hNew = hValue(temp, dest);
            gNew = currCell->g + 1;
            fNew = hNew + gNew;

            if (tempCell->open == 0) {
                tempCell->open = 1;
                tempCell->f = fNew;
                tempCell->g = gNew;
                tempCell->h = hNew;
                tempCell->parent = curr;
            } else if (tempCell->f > fNew) {
                tempCell->f = fNew;
                tempCell->g = gNew;
                tempCell->h = hNew;
                tempCell->parent = curr;
            }
        }
    }
    return 0;
}

int A_star(char map[MAX_L][MAX_L], int ROW, int COL, Pos dest, Cell** cellDetails) {
    while (1) {
        Pos curr;
        if (findminF(cellDetails, ROW, COL, &curr) == 0) {
            return 0;
        }
        Cell* currCell = &(cellDetails[curr.y_pos][curr.x_pos]);
        currCell->open = 0;
        currCell->closed = 1;

        // Successor UP
        Pos temp = curr;
        temp.y_pos -= 1;
        if (isValid(ROW, COL, temp)) {
            Cell* tempCell = &(cellDetails[temp.y_pos][temp.x_pos]);
            if (addSuccessor(tempCell, currCell, temp, curr, dest, map, ROW, COL)) {
                return 1;
            }
        }

        // Successor DOWN
        temp = curr;
        temp.y_pos += 1;
        if (isValid(ROW, COL, temp)) {
            Cell* tempCell = &(cellDetails[temp.y_pos][temp.x_pos]);
            if (addSuccessor(tempCell, currCell, temp, curr, dest, map, ROW, COL)) {
                return 1;
            }
        }

        // Successor RIGHT
        temp = curr;
        temp.x_pos += 1;
        if (isValid(ROW, COL, temp)) {
            Cell* tempCell = &(cellDetails[temp.y_pos][temp.x_pos]);
            if (addSuccessor(tempCell, currCell, temp, curr, dest, map, ROW, COL)) {
                return 1;
            }
        }

        // Successor LEFT
        temp = curr;
        temp.x_pos -= 1;
        if (isValid(ROW, COL, temp)) {
            Cell* tempCell = &(cellDetails[temp.y_pos][temp.x_pos]);
            if (addSuccessor(tempCell, currCell, temp, curr, dest, map, ROW, COL)) {
                return 1;
            }
        }
    }
}

void tracePath(Cell** cellDetails, Pos start, Pos end, char map[MAX_L][MAX_L], int ROW, int COL) {
    Pos curr = end;
    while (!(isSame(curr, start))) {
        map[curr.y_pos][curr.x_pos] = '*';
        curr = cellDetails[curr.y_pos][curr.x_pos].parent;
    }
    map[start.y_pos][start.x_pos] = 'S';
    map[end.y_pos][end.x_pos] = 'E';

    printf("Traced Path:\n");
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

#define MAX 256

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
    
    clock_t t; 
    t = clock();
    double time_taken; 
    start_Astar(map,row,col);
    t = clock() - t; 
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Time Used : %lf sec", time_taken);

    return 0;
}
