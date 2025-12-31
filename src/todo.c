#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Task struct (will be used later)
typedef struct {
    int id;
    char *description;
} Task;

// Each file can only have a maximum of 50 tasks (used later)
#define MAX_TASKS 50
Task tasks[MAX_TASKS];

// Functions for every command
// next_id helper function
int next_id(char *filename) {
    int id;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        id = 1;
    }
    else {
        int count = 0;
        char line [256];
        while (fgets(line, sizeof(line), file) != NULL) {
            count++;
        }
        fclose(file);
        id = count;
    }
    return id;
}

// Add command:
int add_task(char *description, char *filename) {
    int id = next_id(filename);

    FILE *file = fopen(filename, "a");
    if (file != NULL) {
        // if file is empty, add headers
        if (id == 1) {
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
int list_tasks(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        char *line = NULL;
        size_t len = 0;
        while (getline(&line, &len, file) != -1) {
            printf("%s", line);
        }
        free(line);
        fclose(file);
        return 0;
    }
    else {
        printf("Error reading from file! Terminating...\n");
        return 1;
    }
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

        if (strcmp(argv[1], "list") == 0) {
            list_tasks(argv[2]);
        }
    }
    else if (argc == 4) {
        if (strcmp(argv[1], "add") == 0) {
            add_task(argv[2], argv[3]);
        }
    }

    return 0;
}