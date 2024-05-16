#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ROWS 100
#define MAX_COLS 100

typedef struct {
    int x, y;
} Point;

typedef struct Node {
    Point point;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Queue;

// Initialize the queue
void initQueue(Queue *q) {
    q->front = NULL;
    q->rear = NULL;
}

// Check if the queue is empty
int isEmpty(Queue *q) {
    return q->front == NULL;
}

// Add an element to the queue
void enqueue(Queue *q, Point p) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newNode->point = p;
    newNode->next = NULL;
    if (isEmpty(q)) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

// Remove and return the front element from the queue
Point dequeue(Queue *q) {
    if (isEmpty(q)) {
        printf("Queue is empty.\n");
        exit(1);
    }
    Node* temp = q->front;
    Point p = temp->point;
    q->front = q->front->next;
    free(temp);
    if (q->front == NULL) {
        q->rear = NULL;
    }
    return p;
}

// Check if a move is valid
int isValidMove(int x, int y, int rows, int cols, char maze[][MAX_COLS], int visited[][MAX_COLS]) {
    return (x >= 0 && x < rows && y >= 0 && y < cols && (maze[x][y] == '.'|| maze[x][y] == 'E') && !visited[x][y]);
}

// Print the shortest path
void printShortestPath(Point start, Point end, Point parent[][MAX_COLS]) {
    Point path[MAX_ROWS * MAX_COLS];
    int pathLength = 0;
    Point current = end;
    while (!(current.x == start.x && current.y == start.y)) {
        path[pathLength++] = current;
        current = parent[current.x][current.y];
    }
    path[pathLength++] = start;
    printf("Shortest path: ");
    for (int i = pathLength - 1; i > 0; i--) {
        printf("(%d, %d) -> ", path[i].x, path[i].y);
    }
    printf("(%d, %d)", path[0].x, path[0].y);
    printf("\n");
}

// Breadth-first search algorithm to find the shortest path
void bfs(Point start, Point end, int rows, int cols, char maze[][MAX_COLS]) {
    int visited[MAX_ROWS][MAX_COLS] = {0}; // Initialize visited array to keep track of visited cells
    Point parent[MAX_ROWS][MAX_COLS]; // Array to store parent of each cell
    Queue q;
    initQueue(&q);
    enqueue(&q, start);
    visited[start.x][start.y] = 1;
    parent[start.x][start.y] = start;
    int dx[] = {-1, 1, 0, 0}; // Possible moves: up, down, left, right
    int dy[] = {0, 0, -1, 1};

    while (!isEmpty(&q)) {
        Point current = dequeue(&q);
        if (current.x == end.x && current.y == end.y) {
            printShortestPath(start, end, parent);
            return;
        }
        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            if (isValidMove(newX, newY, rows, cols, maze, visited)) {
                enqueue(&q, (Point){newX, newY});
                visited[newX][newY] = 1;
                parent[newX][newY] = current;
            }
        }
    }
    printf("No path found.\n");
}

int main() {
    char filename[MAX_COLS];
    printf("Enter file name: ");
    scanf("%s", filename);

    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {
        printf("File not found!\n");
        return 1;
    }

    int row = 0;
    int col = 0;
    char line[100];
    char maze[100][100];

    while (fgets(line, sizeof(line), stream))
    {
        line[strcspn(line, "\n")] = '\0';
        strcpy(maze[row], line);
        row++;
        col = strlen(line);
    }

    fclose(stream);

    Point start, end;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (maze[i][j] == 'S') {
                start.x = i;
                start.y = j;
            } else if (maze[i][j] == 'E') {
                end.x = i;
                end.y = j;
            }
        }
    }
    clock_t startclk, endclk;
    double cpu_time_used;

    startclk = clock();
    bfs(start, end, row, col, maze);
    endclk = clock();
    cpu_time_used = ((double) (endclk - startclk)) / CLOCKS_PER_SEC;
    printf("Waktu yang diperlukan: %f",cpu_time_used);

    return 0;
}
