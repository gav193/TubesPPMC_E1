/** EL2208 Praktikum Pemecahan Masalah dengan C 2023/2024
 *   Modul               : 9 - Tugas Besar
 *   Hari dan Tanggal    : Senin, 20 Mei 2024
 *   Nama (NIM)          : Kennard Benaya Manli (13222098)
 *   Nama File           : backtracking.c
 *   Deskripsi           : mencari jalur dari input file txt dengan metode backtracking
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
        return 0;
    }
    
    int row = 0;
    char line[MAX];
    char map[MAX][MAX];

    fgets(line, sizeof(line), stream);
    int col = strlen(line) -1; // initialize column value as line length (-1 because of newline char)
    rewind(stream);

    while (fgets(line, sizeof(line), stream)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        strcpy(map[row], line);
        row++;
        if (col != strlen(line)) {
            printf("Found uneven row of line at %d\n", row);
            exit(1);
        }
    }
    fclose(stream);
    clock_t startclk,endclk;
    double cpu_time_used;
    startclk =clock();

    // Create solution stack
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
            if (map[i][j] == 'S') {
                start.x = j;
                start.y = i;
            }
            if (map[i][j] == 'E') {
                end.x = j;
                end.y = i;
            }
        }
    }

    curr.x = start.x;
    curr.y = start.y;

    // Algorithm will always prioritize vertical movement
    int sol_counter = 0;
    int finish = 0;

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
        if (map[curr.y][curr.x] == '.'){
            map[curr.y][curr.x] = '+';
        }

        // Try moving in all four directions
        pos directions[4] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

        for (int i = 0; i < 4; i++) {
            pos next = { curr.x + directions[i].x, curr.y + directions[i].y };
            if (next.x >= 0 && next.x < col && next.y >=0 && next.y < row && (map[next.y][next.x] == '.' || map[next.y][next.x] == 'E')) {
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

        // Print the maze with path

        printf("\n");
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (map[i][j] == '#') {
                    printf("█");
                }else if (map[i][j] == '.') {
                    printf("░");
                }else if (map[i][j] == '+') {
                    printf("+");
                }
                else{printf("%c", map[i][j]);
                }
                if (j == col - 1) {
                }
            }
            printf("\n");   
        }

        printf("\n");
        endclk=clock();
        cpu_time_used = ((double) (endclk-startclk))/CLOCKS_PER_SEC;
        printf("Time taken: %f\n", cpu_time_used);
    } else {
        printf("Failed to solve the maze\n");

                printf("\n");
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (map[i][j] == '#') {
                    printf("█");
                }else if (map[i][j] == '.') {
                    printf("░");
                }else if (map[i][j] == '+') {
                    printf("+");
                }
                else{printf("%c", map[i][j]);
                }
                if (j == col - 1) {
                }
            }
            printf("\n");   
        }

        printf("\n");
        endclk=clock();
        cpu_time_used = ((double) (endclk-startclk))/CLOCKS_PER_SEC;
        printf("Time taken: %f\n", cpu_time_used);
    }

    return 0;
}
