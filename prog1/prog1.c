#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "students.dat"

typedef struct {
    int  deleted;   
    char name[50];
    float cgpa;
    char SRN[20];
} Student;

void writeBinaryFile(int n) {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    Student s;
    for (int i = 0; i < n; i++) {
        printf("Enter name, CGPA, SRN for student %d: ", i + 1);
        scanf("%s %f %s", s.name, &s.cgpa, s.SRN);
        s.deleted = 0; 
        fwrite(&s, sizeof(Student), 1, fp); 
    }

    fclose(fp);
    printf("Records saved.\n");
}

void getRecord(int m) {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Student s;

    fseek(fp, (long)m * sizeof(Student), SEEK_SET);

    if (fread(&s, sizeof(Student), 1, fp) != 1) {
        printf("Record %d not found.\n", m);
        fclose(fp);
        return;
    }

    if (s.deleted) {
        printf("Record %d has been deleted.\n", m);
    } else {
        printf("\nRecord %d:\n", m);
        printf("  Name : %s\n",   s.name);
        printf("  CGPA : %.2f\n", s.cgpa);
        printf("  SRN  : %s\n",   s.SRN);
    }

    fclose(fp);
}

void outputRecords() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Student s;
    int index = 0;
    printf("\n%-5s %-15s %-8s %-15s\n", "Slot", "Name", "CGPA", "SRN");
    printf("-----------------------------------------------\n");

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.deleted)
            printf("%-5d [DELETED]\n", index);
        else
            printf("%-5d %-15s %-8.2f %-15s\n", index, s.name, s.cgpa, s.SRN);
        index++;
    }

    fclose(fp);
}

void deleteRecord(int m) {
    FILE *fp = fopen(FILENAME, "r+b");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Student s;

    fseek(fp, (long)m * sizeof(Student), SEEK_SET);

    if (fread(&s, sizeof(Student), 1, fp) != 1) {
        printf("Record %d not found.\n", m);
        fclose(fp);
        return;
    }

    if (s.deleted) {
        printf("Record %d is already deleted.\n", m);
        fclose(fp);
        return;
    }

    s.deleted = 1;
    strcpy(s.name, "DELETED");
    s.cgpa = 0;
    strcpy(s.SRN, "DELETED");

    fseek(fp, (long)m * sizeof(Student), SEEK_SET);
    fwrite(&s, sizeof(Student), 1, fp);

    printf("Record %d deleted.\n", m);
    fclose(fp);
}

int main() {
    int n, choice, m;

    printf("Enter number of students: ");
    scanf("%d", &n);
    if (n <= 0) { printf("Invalid input.\n"); 
        return 1; 
    }

    writeBinaryFile(n);

    while (1) {
        printf("\n1. Get mth record\n");
        printf("2. Display all\n");
        printf("3. Delete mth record\n");
        printf("4. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter m (0-indexed): "); 
                scanf("%d", &m);
                getRecord(m);
                break;
            case 2:
                outputRecords();
                break;
            case 3:
                printf("Enter m to delete (0-indexed): "); 
                scanf("%d", &m);
                deleteRecord(m);
                break;
            case 4:
                return 0;
        }
    }
}