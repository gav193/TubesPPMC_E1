#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

typedef struct path
{
    int x;
    int y;
    struct path *next;
    struct path *neigh;
} path;

void insertnextpath(path *prev_node, path *new_data)
{
    path *new_node = (path *)malloc(sizeof(path));
    new_node->x = new_data->x;
    new_node->y = new_data->y;
    new_node->neigh = NULL;
    new_node->next = prev_node->next;
    prev_node->next = new_node;
}

void insertnextneigh(path *prev_node, path *new_data)
{
    path *new_node = (path *)malloc(sizeof(path));
    new_node->x = new_data->x;
    new_node->y = new_data->y;
    new_node->next = NULL;
    new_node->neigh = prev_node->neigh;
    prev_node->neigh = new_node;
}

void print_path(path *start)
{
    if (start == NULL)
    {
        printf("Tidak ada jalur yang ditemukan.\n");
        return;
    }

    path *curr = start;
    while (curr != NULL)
    {
        printf("(%d, %d) -> ", curr->x, curr->y);
        curr = curr->next;
    }
    printf("\n");
}

void bfs(int startX, int startY, char map[][MAX], int row, int col)
{
    path *jalan = (path *)malloc(sizeof(path));
    jalan->x = startX;
    jalan->y = startY;
    jalan->next = NULL;
    jalan->neigh = NULL;

    while (jalan != NULL)
    {
        int r = jalan->x;
        int c = jalan->y;

        if (r == row-1 && c == col-1)
        {
            printf("Jalur terpendek:\n");
            print_path(jalan);
            break;
        }

        // Mark the current position as visited
        map[r][c] = 'X';

        // Cek arah kanan
        if (c + 1 < col && map[r][c + 1] == '.')
        {
            path *jalan2 = (path *)malloc(sizeof(path));
            jalan2->x = r;
            jalan2->y = c + 1;
            jalan2->next = jalan->next;
            jalan2->neigh = NULL;
            insertnextpath(jalan, jalan2);
        }

        // Cek arah bawah
        if (r + 1 < row && map[r + 1][c] == '.')
        {
            path *jalan2 = (path *)malloc(sizeof(path));
            jalan2->x = r + 1;
            jalan2->y = c;
            jalan2->next = jalan->next;
            jalan2->neigh = NULL;
            insertnextpath(jalan, jalan2);
        }

        // Cek arah kiri
        if (c - 1 >= 0 && map[r][c - 1] == '.')
        {
            path *jalan2 = (path *)malloc(sizeof(path));
            jalan2->x = r;
            jalan2->y = c - 1;
            jalan2->next = jalan->next;
            jalan2->neigh = NULL;
            insertnextpath(jalan, jalan2);
        }

        // Cek arah atas
        if (r - 1 >= 0 && map[r - 1][c] == '.')
        {
            path *jalan2 = (path *)malloc(sizeof(path));
            jalan2->x = r - 1;
            jalan2->y = c;
            jalan2->next = jalan->next;
            jalan2->neigh = NULL;
            insertnextpath(jalan, jalan2);
        }

        // Move to the next node in the current level
        jalan = jalan->next;
    }
}

int main()
{
    char filename[MAX];
    // input filename from user
    printf("Enter file name: ");
    scanf("%s", filename);

    FILE *stream = fopen(filename, "r");
    if (stream == NULL)
    {
        printf("File not found!\n");
        return 1;
    }

    int row = 0;
    int col = 0;
    char line[MAX];
    char map[MAX][MAX];

    while (fgets(line, sizeof(line), stream))
    {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        strcpy(map[row], line);
        row++;
        col = strlen(line); // Assuming all rows are of same length
    }

    fclose(stream);

    int awal[2], akhir[2];
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (map[i][j] == 'S')
            {
                awal[0] = i;
                awal[1] = j;
            }
            else if (map[i][j] == 'E')
            {
                akhir[0] = i;
                akhir[1] = j;
            }
        }
    }
    int startX = awal[0];
    int startY = awal[1];
    bfs(startX, startY, map, akhir[0], akhir[1]);
    return 0;
}
