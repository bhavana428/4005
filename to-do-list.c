#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_DESC_LEN 200
#define FILENAME "todo_list.txt"

// Task structure
typedef struct {
    char description[MAX_DESC_LEN];
    int day, month, year;
    int isCompleted;
} Task;

// Global array to hold tasks
Task tasks[MAX_TASKS];
int taskCount = 0;

// Clear input buffer
void clearBuffer() {
    while (getchar() != '\n');
}

// Print a single task
void printTask(Task t, int index) {
    printf("%d. [%s] Due: %02d/%02d/%d | Status: %s\n",
        index + 1,
        t.description,
        t.day,
        t.month,
        t.year,
        t.isCompleted ? "Completed" : "Pending"
    );
}

// Add a new task
void addTask() {
    if (taskCount >= MAX_TASKS) {
        printf("Task limit reached!\n");
        return;
    }

    Task newTask;

    printf("Enter task description: ");
    fgets(newTask.description, MAX_DESC_LEN, stdin);
    newTask.description[strcspn(newTask.description, "\n")] = 0;

    printf("Enter due date (DD MM YYYY): ");
    scanf("%d %d %d", &newTask.day, &newTask.month, &newTask.year);

    clearBuffer();

    newTask.isCompleted = 0;

    tasks[taskCount] = newTask;
    taskCount++;

    printf("Task added!\n");
}

// View all tasks
void listTasks() {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }

    printf("\n-- Task List --\n");

    for (int i = 0; i < taskCount; i++) {
        printTask(tasks[i], i);
    }
}

// Mark a task as completed
void markTaskComplete() {
    int num;

    listTasks();

    printf("Enter task number to mark as completed: ");
    scanf("%d", &num);

    clearBuffer();

    if (num < 1 || num > taskCount) {
        printf("Invalid task number.\n");
        return;
    }

    tasks[num - 1].isCompleted = 1;

    printf("Task marked as completed.\n");
}

// Delete a task from the list
void deleteTask() 
{
    int num;

    listTasks();

    printf("Enter task number to delete: ");
    scanf("%d", &num);

    clearBuffer();

    if (num < 1 || num > taskCount) {
        printf("Invalid task number.\n");
        return;
    }

    for (int i = num - 1; i < taskCount - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    taskCount--;

    printf("Task deleted.\n");
}

// Save tasks to a file
void saveTasks() {
    FILE *fp = fopen(FILENAME, "w");

    if (!fp) {
        printf("Error saving tasks.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        Task t = tasks[i];

        fprintf(fp,
                "%s|%d|%d|%d|%d\n",
                t.description,
                t.day,
                t.month,
                t.year,
                t.isCompleted
        );
    }

    fclose(fp);

    printf("Tasks saved to %s\n", FILENAME);
}

// Load tasks from a file
void loadTasks() {
    FILE *fp = fopen(FILENAME, "r");

    if (!fp) {
        printf("No saved tasks found.\n");
        return;
    }

    taskCount = 0;

    Task t;

    while (fscanf(fp,
                  " %[^|]|%d|%d|%d|%d\n",
                  t.description,
                  &t.day,
                  &t.month,
                  &t.year,
                  &t.isCompleted
          ) == 5) {

        tasks[taskCount] = t;
        taskCount++;

        if (taskCount >= MAX_TASKS)
            break;
    }

    fclose(fp);

    printf("Tasks loaded from %s\n", FILENAME);
}

// Menu for user interaction
void menu() 
{
    int choice;

    do {
        printf("\n--- To-Do List Manager ---\n");

        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Mark Task as Completed\n");
        printf("4. Delete Task\n");
        printf("5. Save Tasks\n");
        printf("6. Load Tasks\n");
        printf("0. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
            case 1:
                addTask();
                break;

            case 2:
                listTasks();
                break;

            case 3:
                markTaskComplete();
                break;

            case 4:
                deleteTask();
                break;

            case 5:
                saveTasks();
                break;

            case 6:
                loadTasks();
                break;

            case 0:
                printf("Goodbye!\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);
}

// Entry point
int main() 
{
    loadTasks();

    menu();

    saveTasks();

    return 0;
}
