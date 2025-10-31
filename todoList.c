#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BLUE   "\033[1;34m"
#define GREEN  "\033[1;32m"
#define YELLOW "\033[1;33m"
#define RED    "\033[1;31m"
#define RESET  "\033[0m"

#define TASK_SIZE 64

typedef struct {
    char task[TASK_SIZE];
    int completed;
} Todo;

void makeTODO(void);
void getTODO(void);
void addTODO(void);
void removeTODO(void);
void markTODO(void);

int main(int argc, char *argv[]) {
    if (argc > 1 && argc < 3) {
        goto args;
    } else if (argc > 2) {
        printf("Invalid input!\n");
        return 1;
    }

    while(1) {
        int op;
        puts(GREEN
            "_____________\n"
            "|1. Show\n"
            "|2. Add\n"
            "|3. Remove\n"
            "|4. Mark\n"
            "|5. Exit"
        );
        printf("|Option: " RESET);
        if (scanf("%d", &op) != 1) {
            printf(RED "Invalid Input!\n" RESET);
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch(op) {
            case 1: getTODO(); break;
            case 2: addTODO(); break;
            case 3: removeTODO(); break;
            case 4: markTODO(); break;
            case 5: 
                printf(BLUE "Goodbye...\n" RESET);
                return 0;
            default: printf(RED "Invalid Input!\n" RESET);
        }
    }

args:
    if (strcmp(argv[1], "get") == 0) getTODO();
    else if (strcmp(argv[1], "add") == 0) addTODO();
    else if (strcmp(argv[1], "remove") == 0) removeTODO();
    else if (strcmp(argv[1], "mark") == 0) markTODO();
    else printf("Invalid Argument: %s\n", argv[1]);
    
    return 0;
}

void makeTODO() {
    printf(GREEN "Looking for TODO list...\n" RESET);
    const char *filename = "todo.dat";
    struct stat st;
    if (stat(filename, &st) == 0) {
        printf(GREEN "TODO list found...\n" RESET);
    } else {
        printf(YELLOW "TODO list not found...\nCreating TODO list...\n" RESET);
        FILE* file = fopen(filename, "wb");
        if (file) {
            printf(GREEN "TODO created successfully!\n" RESET);
            fclose(file);
        } else {
            perror(RED "fopen" RESET);
            exit(1);
        }
    }
}

void getTODO(void) {
    makeTODO();
    const char *filename = "todo.dat";
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror(RED "fopen" RESET);
        exit(1);
    }

    printf("______________________\n");
    printf(GREEN "TODO LIST: %s\n", filename);

    Todo t;
    int index = 1;
    while (fread(&t, sizeof(Todo), 1, file) == 1) {
        printf("%d. %s [%s]\n", index++, t.task, t.completed ? "DONE" : "TODO");
    }

    printf(RESET "______________________\n");
    fclose(file);
}

void addTODO(void) {
    makeTODO();
    const char *filename = "todo.dat";
    FILE* file = fopen(filename, "ab");
    if (!file) {
        perror(RED "fopen" RESET);
        exit(1);
    }

    Todo t;
    t.completed = 0;
    char buffer[TASK_SIZE];

    printf(GREEN "Enter tasks (end with a single '.' on a line):\n" RESET);
    while (1) {
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        if (buffer[0] == '.' && buffer[1] == '\n') break;

        buffer[strcspn(buffer, "\n")] = '\0'; // remove newline
        strncpy(t.task, buffer, TASK_SIZE - 1);
        t.task[TASK_SIZE - 1] = '\0';

        if (fwrite(&t, sizeof(Todo), 1, file) != 1) {
            perror(RED "fwrite" RESET);
        }
    }

    fclose(file);
}

void removeTODO(void) {
    makeTODO();
    const char *filename = "todo.dat";

    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror(RED "fopen" RESET);
        exit(EXIT_FAILURE);
    }

    Todo *tasks = NULL;
    Todo t;
    size_t count = 0, capacity = 10;
    tasks = malloc(capacity * sizeof(Todo));
    if (!tasks) { perror("malloc"); fclose(file); exit(1); }

    while (fread(&t, sizeof(Todo), 1, file) == 1) {
        if (count >= capacity) {
            capacity *= 2;
            tasks = realloc(tasks, capacity * sizeof(Todo));
            if (!tasks) { perror("realloc"); fclose(file); exit(1); }
        }
        tasks[count++] = t;
    }
    fclose(file);

    if (count == 0) {
        printf(YELLOW "No tasks to remove.\n" RESET);
        free(tasks);
        return;
    }

    printf("Current tasks:\n");
    for (size_t i = 0; i < count; i++)
        printf("%zu. %s [%s]\n", i + 1, tasks[i].task, tasks[i].completed ? "DONE" : "TODO");

    int remove_index;
    printf("Enter task number to remove: ");
    if (scanf("%d", &remove_index) != 1 || remove_index < 1 || remove_index > count) {
        printf(RED "Invalid task number.\n" RESET);
        while (getchar() != '\n');
        free(tasks);
        return;
    }
    getchar();
    remove_index--;

    FILE *newfile = fopen("todo_temp.dat", "wb");
    if (!newfile) { perror(RED "fopen" RESET); free(tasks); exit(1); }

    for (size_t i = 0; i < count; i++)
        if ((int)i != remove_index)
            fwrite(&tasks[i], sizeof(Todo), 1, newfile);

    fclose(newfile);

    if (remove(filename) != 0) perror(RED "remove" RESET);
    if (rename("todo_temp.dat", filename) != 0) perror(RED "rename" RESET);

    printf(GREEN "Task removed successfully.\n" RESET);
    free(tasks);
}

void markTODO(void) {
    makeTODO();
    const char *filename = "todo.dat";
    FILE *file = fopen(filename, "rb+");
    if (!file) { perror(RED "fopen" RESET); exit(1); }

    Todo *tasks = NULL;
    long *positions = NULL;
    Todo t;
    size_t count = 0, capacity = 10;
    tasks = malloc(capacity * sizeof(Todo));
    positions = malloc(capacity * sizeof(long));
    if (!tasks || !positions) { perror("malloc"); fclose(file); exit(1); }

    printf(GREEN "TODO LIST:\n" RESET);
    while (fread(&t, sizeof(Todo), 1, file) == 1) {
        if (count >= capacity) {
            capacity *= 2;
            tasks = realloc(tasks, capacity * sizeof(Todo));
            positions = realloc(positions, capacity * sizeof(long));
            if (!tasks || !positions) { perror("realloc"); fclose(file); exit(1); }
        }
        tasks[count] = t;
        positions[count] = ftell(file) - sizeof(Todo);
        printf("%zu. %s [%s]\n", count + 1, t.task, t.completed ? "DONE" : "TODO");
        count++;
    }

    if (count == 0) { printf(YELLOW "No tasks to mark.\n" RESET); free(tasks); free(positions); fclose(file); return; }

    int mark_index;
    printf("Enter task number to mark as done: ");
    if (scanf("%d", &mark_index) != 1 || mark_index < 1 || mark_index > count) {
        printf(RED "Invalid input!\n" RESET);
        while (getchar() != '\n');
        free(tasks); free(positions); fclose(file);
        return;
    }
    getchar();
    mark_index--;

    fseek(file, positions[mark_index], SEEK_SET);
    t.completed = 1;
    if (fwrite(&t, sizeof(Todo), 1, file) != 1) perror(RED "fwrite" RESET);
    else printf(GREEN "Task marked as done!\n" RESET);

    free(tasks);
    free(positions);
    fclose(file);
}