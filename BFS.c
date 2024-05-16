#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // For INT_MAX

#define MAX 256

// Define the path structure
typedef struct path
{
    int x;
    int y;
    struct path *next;
    struct path *neigh;
} Path;

// Function to insert a new cell into the path list
void insertnextpath(Path *prev_node, Path *new_data)
{
    Path *new_node = (Path *)malloc(sizeof(Path));
    new_node->x = new_data->x;
    new_node->y = new_data->y;
    new_node->neigh = NULL;
    new_node->next = prev_node->next;
    prev_node->next = new_node;
}

// Function to insert a new cell into the neighboring list
void insertnextneigh(Path *prev_node, Path *new_data)
{
    Path *new_node = (Path *)malloc(sizeof(Path));
    new_node->x = new_data->x;
    new_node->y = new_data->y;
    new_node->next = NULL;
    new_node->neigh = prev_node->neigh;
    prev_node->neigh = new_node;
}

// Function to print the path
void print_path(Path *start)
{
    Path *curr = start;
    while (curr != NULL)
    {
        printf("(%d, %d) -> ", curr->x, curr->y);
        curr = curr->next;
    }
    printf("\n");
}

// Function to find paths using BFS
void find_paths(int startX, int startY, char map[][MAX], int row, int col)
{
    Path *jalan = (Path *)malloc(sizeof(Path));
    jalan->x = startX;
    jalan->y = startY;
    jalan->next = NULL;
    jalan->neigh = NULL;
    struct path *jalan2 = (struct path *)malloc(sizeof(struct path));

    // Initialize variables for longest and shortest paths
    int longest_length = 0;
    int shortest_length = INT_MAX;
    Path *longest_path = NULL;
    Path *shortest_path = NULL;

    while (jalan != NULL)
    {
        int r = jalan->x;
        int c = jalan->y;

        if (r == row - 1 && c == col - 1)
        {
            // Found a path
            int path_length = 0;
            Path *temp = jalan;
            while (temp != NULL)
            {
                path_length++;
                temp = temp->next;
            }

            // Update longest and shortest paths
            if (path_length > longest_length)
            {
                longest_length = path_length;
                longest_path = jalan;
            }
            if (path_length < shortest_length)
            {
                shortest_length = path_length;
                shortest_path = jalan;
            }
        }

        // Check neighboring cells and add them to the lists
        // Cek arah kanan
        if (c + 1 < col && map[r][c + 1] == '.'){   
            
            jalan2->x = r;
            jalan2->y = c + 1;
            jalan2->next = NULL;
            jalan2->neigh = NULL;
            insertnextneigh(jalan, jalan2);
            jalan = jalan->neigh;
        }

        // Cek arah bawah
        if (r + 1 < row && map[r + 1][c] == '.'){   
            jalan2->x = r + 1;
            jalan2->y = c;
            jalan2->next = NULL;
            jalan2->neigh = NULL;
            insertnextpath(jalan, jalan2);
            jalan = jalan->next;
        }

        // Cek arah kiri
        if (c - 1 >= 0 && map[r][c - 1] == '.'){   
            jalan2->x = r;
            jalan2->y = c - 1;
            jalan2->next = NULL;
            jalan2->neigh = NULL;
            insertnextneigh(jalan, jalan2);
            jalan = jalan->neigh;
        }

        // Cek arah atas
        if (r - 1 >= 0 && map[r - 1][c] == '.'){   
            jalan2->x = r - 1;
            jalan2->y = c;
            jalan2->next = NULL;
            jalan2->neigh = NULL;
            insertnextpath(jalan, jalan2);
            jalan = jalan->next;
        }

        // Move to the next node
        jalan = jalan->next;
    }

    // Print all paths
    printf("All possible paths from start to end:\n");
    int path_number = 1;
    Path *curr = jalan;
    while (curr != NULL)
    {
        printf("Path %d: ", path_number);
        print_path(curr);
        path_number++;
        curr = curr->neigh;
    }

    // Print total number of paths
    printf("Total number of paths: %d\n", path_number - 1);

    // Print longest path
    printf("Longest path from start to end:\n");
    print_path(longest_path);

    // Print shortest path
    printf("Shortest path from start to end:\n");
    print_path(shortest_path);
}

int main()
{
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

    int awal[2], akhir[2];
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (map[i][j] == 'S')
            {
                awal[0] = i;
                awal[1] = j;
            } else if (map[i][j] == 'E')
            {
                akhir[0] = i;
                akhir[1] = j;
            }
        }
    }

    find_paths(awal[0], awal[1], map, akhir[0], akhir[1]);
    return 0;
}
