#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Task struct
typedef struct {
    int id;
    char *description;
} Task;

// Each file can only have a maximum of 50 tasks
#define MAX_TASKS 50
Task tasks[MAX_TASKS];

int task_count = 0; // Track the number of tasks

// Functions for every command
// next_id helper function
int next_id(char *filename) {
    int id;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        id = 1;
        return 1;
    }
    else {
        int count = 0;
        char line [256];
        fgets(line, sizeof(line), file); // Skip header
        while (fgets(line, sizeof(line), file) != NULL) {
            count++;
        }
        fclose(file);
        id = count + 1;
    }
    return id;
}

// Add command:
int add_task(char *description, char *filename) {
    const int id = next_id(filename);

    // Check if file has headers
    int has_header = 0;
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        char line [256];
        if (fgets(line, sizeof(line), file) != NULL) {
            has_header = 1;
        }
        fclose(file);
    }

    file = fopen(filename, "a");
    if (file != NULL) {
        // If file is empty, add headers
        if (!has_header) {
            fprintf(file,"Task Number\tTask Description\n");
        }
        fprintf(file,"%i\t%s\n", id, description);
        fclose(file);
        return 0;
    }
    else {
        printf("Error while opening/creating file! Terminating program...\n");
        return 1;
    }
}

// List command
// Store the tasks into the "tasks" array using the helper function "store_tasks"
void store_tasks(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }

    char line [256];
    fgets(line, sizeof(line), file); // Skip the headers

    for (int i = 0; fgets(line, sizeof(line), file) != NULL && i < MAX_TASKS; i++) {
        char description[256];
        sscanf(line, "%d\t%[^\n]", &tasks[i].id, description);

        tasks[i].description = malloc(strlen(description) + 1); // Allocate memory for the task description with the size of the temporary description used above + the NUL character (\0)
        if (tasks[i].description != NULL) {
            strcpy(tasks[i].description, description);
            task_count++;
        }
    }
    fclose(file);
}

// Free dynamically allocated memory
void free_tasks() {
    // Free old memory
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].description != NULL) {
            free(tasks[i].description);
            tasks[i].description = NULL;
        }
    }
}

void list_tasks(char *filename) {
    store_tasks(filename);

    if (task_count == 0) {
        printf("No tasks yet!\n");
        printf("Add tasks using this command: `./todo add \"Task Description\" [file-name.tsv (keep empty if you want to write to the default file [todo.tsv])]\n");
    }
    else {
        printf("Your tasks\n");
        printf("==========\n");
        for (int i = 0; i < task_count; i++) {
            printf("%d. %s\n", tasks[i].id, tasks[i].description);
        }
    }
}

int save_tasks(char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return 1;
    }

    fprintf(file,"Task Number\tTask Description\n");

    for (int i = 0; i < task_count; i++) {
       fprintf(file,"%d\t%s\n", tasks[i].id, tasks[i].description);
    }

    return 0;
}

int remove_task(int id, char *filename) {

    // Load tasks
    store_tasks(filename);

    if (id < 1 || id > task_count) {
        printf("Invalid task number!\n");
        printf("There are %d tasks in this list.\n", task_count);
        return 1;
    }

    free(tasks[id - 1].description);

    // Shift the array
    for (int i = id - 1; i < task_count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    task_count--;

    tasks[task_count].id = 0;
    tasks[task_count].description = NULL;

    // Update IDs
    for (int i = 0; i < task_count; i++) {
        tasks[i].id = i + 1;
    }

    // Save to the file again
    save_tasks(filename);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./todo [command] {Description if available for command} (File name unless default [todo.tsv])\n");
        return 1;
    }

    if (strcmp(argv[1], "add") != 0 && strcmp(argv[1], "list") != 0 && strcmp(argv[1], "done") != 0 && strcmp(argv[1], "remove") != 0) {
        printf("\"%s\" is not a valid command!\n", argv[1]);
        printf("The only valid commands are: \"add\", \"list\", \"done\", and \"remove\"\n");
        return 2;
    }

    if (argc == 2) {
        if (strcmp(argv[1], "list") == 0) {
            list_tasks("todo.tsv");
        }
    }

    else if (argc == 3) {
        if (strcmp(argv[1], "add") == 0) {
            add_task(argv[2], "todo.tsv");
        }

        else if (strcmp(argv[1], "list") == 0) {
            list_tasks(argv[2]);
        }

        else if (strcmp(argv[1], "remove") == 0) {
            remove_task(atoi(argv[2]), "todo.tsv");
        }
    }
    else if (argc == 4) {
        if (strcmp(argv[1], "add") == 0) {
            add_task(argv[2], argv[3]);
        }

        else if (strcmp(argv[1], "remove") == 0) {
            remove_task(atoi(argv[2]), argv[3]);
        }
    }

    free_tasks();

    return 0;
}