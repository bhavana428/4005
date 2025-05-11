#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_NAME_LEN 100
#define FILENAME "tasks.txt"

typedef struct 
{
    char name[MAX_NAME_LEN];
    int priority;     // 1 (High) to 5 (Low)
    int hour;         // 0-23
    int minute;       // 0-59
    int isCompleted;  // 0 = no, 1 = yes
} Task;

Task tasks[MAX_TASKS];
int taskCount = 0;

void clearBuffer() 
{
    while (getchar() != '\n');
}

void printTask(Task t, int index) 
{
    printf("%d. [%s] Priority: %d | Time: %02d:%02d | Status: %s\n",
           index + 1, t.name, t.priority, t.hour, t.minute,
           t.isCompleted ? "Done" : "Pending");
}

void addTask() 

{
    if (taskCount >= MAX_TASKS) 

    {
        printf("Task limit reached!\n");
        return;
    }

    Task newTask;
    printf("Enter task name: ");
    fgets(newTask.name, MAX_NAME_LEN, stdin);
    newTask.name[strcspn(newTask.name, "\n")] = 0;  // Remove newline

    printf("Enter priority (1 = High, 5 = Low): ");
    scanf("%d", &newTask.priority);
    printf("Enter due time (HH MM): ");
    scanf("%d %d", &newTask.hour, &newTask.minute);
    clearBuffer();

    if (newTask.hour < 0 || newTask.hour > 23 || newTask.minute < 0 || newTask.minute > 59) 
    {
        printf("Invalid time!\n");
        return;
    }

    newTask.isCompleted = 0;
    tasks[taskCount++] = newTask;
    printf("Task added!\n");
}

void listTasks() 

{
    if (taskCount == 0) 

    {
        printf("No tasks available.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) 
    {
        printTask(tasks[i], i);
    }
}

int comparePriority(const void *a, const void *b) 

{
    return ((Task *)a)->priority - ((Task *)b)->priority;
}

int compareTime(const void *a, const void *b) 
{
    Task *t1 = (Task *)a;
    Task *t2 = (Task *)b;
    if (t1->hour == t2->hour)
        return t1->minute - t2->minute;
    return t1->hour - t2->hour;
}

void sortTasks() 

{
    int choice;
    printf("Sort by:\n1. Priority\n2. Time\nChoice: ");
    scanf("%d", &choice);
    clearBuffer();

    if (choice == 1) 
    {
        qsort(tasks, taskCount, sizeof(Task), comparePriority);
        printf("Tasks sorted by priority.\n");
    }
     else if (choice == 2) 
    {
        qsort(tasks, taskCount, sizeof(Task), compareTime);
        printf("Tasks sorted by time.\n");
    } 
    else 
    {
        printf("Invalid choice.\n");
    }
}

void markTaskDone() 
{
    int num;
    listTasks();
    printf("Enter task number to mark as completed: ");
    scanf("%d", &num);
    clearBuffer();

    if (num < 1 || num > taskCount) 
    {
        printf("Invalid task number.\n");
        return;
    }

    tasks[num - 1].isCompleted = 1;
    printf("Task marked as done.\n");
}

void deleteTask() 

{
    int num;
    listTasks();
    printf("Enter task number to delete: ");
    scanf("%d", &num);
    clearBuffer();

    if (num < 1 || num > taskCount) 
    {
        printf("Invalid task number.\n");
        return;
    }

    for (int i = num - 1; i < taskCount - 1; i++) 
    {
        tasks[i] = tasks[i + 1];
    }
    taskCount--;
    printf("Task deleted.\n");
}

void saveTasks() 
{
    FILE *fp = fopen(FILENAME, "w");
    if (!fp) 

    {
        printf("Error saving tasks.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) 
    {
        Task t = tasks[i];
        fprintf(fp, "%s|%d|%d|%d|%d\n", t.name, t.priority, t.hour, t.minute, t.isCompleted);
    }

    fclose(fp);
    printf("Tasks saved to %s\n", FILENAME);
}

void loadTasks() 

{
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) 
    {
        printf("No saved tasks found.\n");
        return;
    }

    taskCount = 0;
    Task t;
    while (fscanf(fp, " %[^|]|%d|%d|%d|%d\n", t.name, &t.priority, &t.hour, &t.minute, &t.isCompleted) == 5) 

    {
        tasks[taskCount++] = t;
        if (taskCount >= MAX_TASKS)
            break;
    }

    fclose(fp);
    printf("Tasks loaded from %s\n", FILENAME);

}

void menu() 
{

    int choice;
    do 
    {
        printf("\n--- Task Scheduler ---\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Sort Tasks\n");
        printf("4. Mark Task as Done\n");
        printf("5. Delete Task\n");
        printf("6. Save Tasks\n");
        printf("7. Load Tasks\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearBuffer();

        switch (choice) 

        {
            case 1: addTask(); break;
            case 2: listTasks(); break;
            case 3: sortTasks(); break;
            case 4: markTaskDone(); break;
            case 5: deleteTask(); break;
            case 6: saveTasks(); break;
            case 7: loadTasks(); break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid choice.\n");
        }
    }

    while (choice != 0);
}

int main() 

{
    loadTasks();
    menu();
    saveTasks();
    return 0;
}
