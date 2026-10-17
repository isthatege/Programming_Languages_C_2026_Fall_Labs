// week5_task3_student_management_system.c
// Name: Ege   Surname: Engin  Student ID: 260ADM039
//
// Task 3: Mini-project – Student management system with file persistence
// Week 5 – Files & Modular Programming
// TODO: Implement the functions to load, save, add, and list students.
//
// Build: make lab5   Run: ./bin/week5_task3_student_management_system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
    char name[NAME_LEN];
    int id;
    float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls them directly)
int  load_students(Student arr[], int max);          // returns number of records loaded
int  save_students(const Student arr[], int count);  // 0 on success, -1 on error
void add_student(Student arr[], int *count);
void list_students(const Student arr[], int count);
int  read_line(const char *prompt, char *buf, size_t size);

int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;
    char input[LINE_LEN];
    int running = 1;

    // Load existing student data.
    count = load_students(students, MAX_STUDENTS);
    printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

    while (running) {
        printf("\n=== Student Management System ===\n");
        printf("1. List students\n");
        printf("2. Add student\n");
        printf("3. Save and Exit\n");

        int choice;

        if (read_line("Select an option: ", input, sizeof input) != 0) {
            choice = 3;
        } else {
            char *end;
            long value = strtol(input, &end, 10);
            choice = (end != input && *end == '\0') ? (int)value : 0;
        }

        switch (choice) {
            case 1:
                list_students(students, count);
                break;

            case 2:
                add_student(students, &count);
                break;

            case 3:
                if (save_students(students, count) != 0) {
                    printf("Error saving data.\n");
                } else {
                    printf("Data saved. Goodbye!\n");
                }

                running = 0;
                break;

            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return EXIT_SUCCESS;
}

// Provided helper – prints a prompt and reads one line from stdin into buf.
// The trailing newline is removed. Returns 0 on success, -1 on end of input (EOF).
int read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    fflush(stdout);

    if (fgets(buf, (int)size, stdin) == NULL) {
        return -1;
    }

    size_t len = strcspn(buf, "\r\n");

    if (buf[len] == '\0' && len == size - 1) {
        // Line was longer than the buffer: discard the rest of it.
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }

    buf[len] = '\0';
    return 0;
}

// Load student records from DATA_FILE.
// If the file cannot be opened, return 0.
// Read lines with fgets() and parse "name,id,gpa" using sscanf().
// Skip blank or malformed lines and never load more than max records.
int load_students(Student arr[], int max) {
    FILE *fp = fopen(DATA_FILE, "r");

    if (fp == NULL) {
        return 0;
    }

    char line[LINE_LEN];
    int loaded = 0;

    while (loaded < max && fgets(line, sizeof line, fp) != NULL) {
        size_t len = strcspn(line, "\r\n");
        line[len] = '\0';

        if (line[0] == '\0') {
            continue;
        }

        Student s;
        int result = sscanf(line, "%49[^,],%d,%f",
                            s.name, &s.id, &s.gpa);

        if (result == 3) {
            arr[loaded] = s;
            loaded++;
        }
    }

    fclose(fp);
    return loaded;
}

// Save student records to DATA_FILE in the format name,id,gpa.
int save_students(const Student arr[], int count) {
    FILE *fp = fopen(DATA_FILE, "w");

    if (fp == NULL) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        if (fprintf(fp, "%s,%d,%.2f\n",
                    arr[i].name, arr[i].id, arr[i].gpa) < 0) {
            fclose(fp);
            return -1;
        }
    }

    if (fclose(fp) != 0) {
        return -1;
    }

    return 0;
}

// Add a student after validating the name, ID, and GPA.
void add_student(Student arr[], int *count) {
    if (*count >= MAX_STUDENTS) {
        printf("Cannot add more students (limit reached).\n");
        return;
    }

    char name[NAME_LEN];
    char id_str[LINE_LEN];
    char gpa_str[LINE_LEN];

    if (read_line("Enter name: ", name, sizeof name) != 0) {
        return;
    }

    if (name[0] == '\0' || strchr(name, ',') != NULL) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    if (read_line("Enter ID: ", id_str, sizeof id_str) != 0) {
        return;
    }

    char *endptr;
    long id_value = strtol(id_str, &endptr, 10);

    if (id_str[0] == '\0' || *endptr != '\0') {
        printf("Invalid input. Student not added.\n");
        return;
    }

    /*
     * Verify that the ID fits in an int without requiring limits.h.
     */
    int converted_id = (int)id_value;

    if ((long)converted_id != id_value) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    if (read_line("Enter GPA: ", gpa_str, sizeof gpa_str) != 0) {
        return;
    }

    float gpa = strtof(gpa_str, &endptr);

    if (gpa_str[0] == '\0' || *endptr != '\0' ||
        !(gpa >= 0.0f && gpa <= 4.0f)) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    arr[*count].id = converted_id;

    strncpy(arr[*count].name, name, NAME_LEN - 1);
    arr[*count].name[NAME_LEN - 1] = '\0';

    arr[*count].gpa = gpa;

    (*count)++;

    printf("Student added successfully!\n");
}

// List all students, or display a message if there are none.
void list_students(const Student arr[], int count) {
    if (count == 0) {
        printf("No students found.\n");
        return;
    }

    printf("%-5s %-20s %s\n", "ID", "Name", "GPA");

    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %.2f\n",
               arr[i].id, arr[i].name, arr[i].gpa);
    }
}
