#include <stdlib.h>
#include <stdio.h>
// #include <string.h>

// Enumeration
enum Color {
    RED, GREEN, BLUE
};

// Union
union DataUnion {
    int i;
    float f;
    char str[20];
};

// Structure
struct Node {
    int data;
    struct Node* next;
};

// Function prototypes
void basic_variables();
void array_example();
void enum_example();
void union_example();
void linked_list_example();
void factorial_example();
void file_io_example();
void switch_case_example(int value);
void free_linked_list(struct Node* head);

// Function to demonstrate basic variable usage
void basic_variables() {
    int integer = 10;
    float floating_point = 3.14;
    char character = 'A';
    char string[20];

    strcpy(string, "Hello, C!");

    printf("Integer: %d\n", integer);
    printf("Floating point: %.2f\n", floating_point);
    printf("Character: %c\n", character);
    printf("String: %s\n", string);

    // New condition: Check if integer > 5
    if (integer > 5) {
        printf("Integer is greater than 5\n");
    } else {
        printf("Integer is 5 or less\n");
    }
}

// Function to demonstrate array usage
void array_example() {
    int numbers[5];
    int i;

    numbers[0] = 1;
    numbers[1] = 2;
    numbers[2] = 3;
    numbers[3] = 4;
    numbers[4] = 5;

    printf("Array: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
}

// Function to demonstrate enumeration usage
void enum_example() {
    enum Color color;
    color = GREEN;
    printf("Enumeration (GREEN): %d\n", color);
}

// Function to demonstrate union usage
void union_example() {
    union DataUnion data;

    data.i = 42;
    printf("Union (int): %d\n", data.i);
    data.f = 3.14;
    printf("Union (float): %.2f\n", data.f);
    strcpy(data.str, "C Language");
    printf("Union (string): %s\n", data.str);
}

// Function to create a new node for the linked list
struct Node* create_node(int data) {
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        printf("Memory allocation error!\n");
        exit(1);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

// Function to print a linked list
void print_list(struct Node* head) {
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

// Function to demonstrate linked list usage
void linked_list_example() {
    struct Node* head;

    head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);

    printf("Linked list: ");
    print_list(head);

    // Freeing memory
    free_linked_list(head);
}

// Recursive function to calculate the factorial of a number
int factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Function to demonstrate factorial calculation
void factorial_example() {
    int num = 5;  // Moving the declaration inside a block
    printf("Factorial of %d: %d\n", num, factorial(num));
}

// File I/O example using fgetc() in a for loop
void file_io_example() {
    FILE *file;
    char ch;

    // Open file for writing
    file = fopen("example.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        exit(1);
    }
    fprintf(file, "Hello, this is a test file!\n");
    fclose(file);

    // Open file for reading
    file = fopen("example.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading!\n");
        exit(1);
    }

    // Using a for loop to read one character at a time
    for (ch = fgetc(file); ch != EOF; ch = fgetc(file)) {
        putchar(ch);  // Print the character to the console
    }
    fclose(file);
}

// Example of switch-case
void switch_case_example(int value) {
    switch (value) {
        case 1:
            printf("The value is ONE.\n");
            break;
        case 2:
            printf("The value is TWO.\n");
            break;
        case 3:
            printf("The value is THREE.\n");
            break;
        default:
            printf("The value is unknown.\n");
            break;
    }
}

// Pilha (Stack) - LIFO
struct Stack {
    int *arr;
    int top;
    int capacity;
};

void init_stack(struct Stack* stack, int capacity) {
    stack->arr = (int*)malloc(sizeof(int) * capacity);
    stack->top = -1;
    stack->capacity = capacity;
}

int is_full(struct Stack* stack) {
    return stack->top == stack->capacity - 1;
}

int is_empty(struct Stack* stack) {
    return stack->top == -1;
}

void push(struct Stack* stack, int value) {
    if (is_full(stack)) {
        printf("Stack Overflow!\n");
        return;
    }
    stack->arr[++stack->top] = value;
}

int pop(struct Stack* stack) {
    if (is_empty(stack)) {
        printf("Stack Underflow!\n");
        return -1;
    }
    return stack->arr[stack->top--];
}

void stack_example() {
    struct Stack stack;
    init_stack(&stack, 5);
    push(&stack, 10);
    push(&stack, 20);
    push(&stack, 30);
    printf("Pop: %d\n", pop(&stack));
}

// Fila (Queue) - FIFO
struct Queue {
    int *arr;
    int front;
    int rear;
    int capacity;
};

void init_queue(struct Queue* queue, int capacity) {
    queue->arr = (int*)malloc(sizeof(int) * capacity);
    queue->front = 0;
    queue->rear = -1;
    queue->capacity = capacity;
}

int is_queue_full(struct Queue* queue) {
    return queue->rear == queue->capacity - 1;
}

int is_queue_empty(struct Queue* queue) {
    return queue->front > queue->rear;
}

void enqueue(struct Queue* queue, int value) {
    if (is_queue_full(queue)) {
        printf("Queue Overflow!\n");
        return;
    }
    queue->arr[++queue->rear] = value;
}

int dequeue(struct Queue* queue) {
    if (is_queue_empty(queue)) {
        printf("Queue Underflow!\n");
        return -1;
    }
    return queue->arr[queue->front++];
}

void queue_example() {
    struct Queue queue;
    init_queue(&queue, 5);
    enqueue(&queue, 10);
    enqueue(&queue, 20);
    enqueue(&queue, 30);
    printf("Dequeue: %d\n", dequeue(&queue));
}

// Árvore Binária
struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
};

struct TreeNode* new_node(int data) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

void inorder(struct TreeNode* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

void tree_example() {
    struct TreeNode* root = new_node(10);
    root->left = new_node(5);
    root->right = new_node(15);
    root->left->left = new_node(3);
    root->left->right = new_node(7);
    printf("In-order traversal: ");
    inorder(root);
    printf("\n");
}

// Function to free the linked list
void free_linked_list(struct Node* head) {
    struct Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Main function
int main() {
    // Calling all the function examples
    basic_variables();
    array_example();
    enum_example();
    union_example();
    linked_list_example();
    factorial_example();
    file_io_example();
    stack_example();
    queue_example();
    tree_example();

    // Switch-case example
    printf("\nSwitch-case Example:\n");
    switch_case_example(2);

    return 0;
}
