#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OWNER_FILE "owner.dat"
#define STUDENT_FILE "students.dat"
#define ATTENDANCE_FILE "attendance.dat"
#define TEMP_FILE "temp.dat"

/* ================= STRUCTURES ================= */

typedef struct
{
    char name[50];
    int roll, id, age;
    char branch[30];
    float marks;
} Student;

typedef struct
{
    char username[50];
    char password[50];
} Owner;

typedef struct
{
    int roll;
    char date[11];
    int present;
} Attendance;

/* ================= OWNER ================= */

int ownerExists()
{
    Owner o;
    FILE *fp = fopen(OWNER_FILE, "rb");

    if (!fp)
        return 0;

    int result = fread(&o, sizeof(o), 1, fp);

    fclose(fp);

    return result == 1;
}

/* ================= CREATE ACCOUNT ================= */

void createOwner()
{
    Owner o;

    FILE *fp = fopen(OWNER_FILE, "wb");

    if (!fp)
    {
        printf("\nError creating account!\n");
        exit(1);
    }

    printf("\n================================\n");
    printf("        CREATE ACCOUNT\n");
    printf("================================\n");

    printf("Create Username: ");
    scanf("%49s", o.username);

    printf("Create Password: ");
    scanf("%49s", o.password);

    fwrite(&o, sizeof(o), 1, fp);

    fclose(fp);

    printf("\nAccount created successfully!\n");
}

/* ================= LOGIN ================= */

int login()
{
    Owner o;

    char username[50];
    char password[50];

    FILE *fp = fopen(OWNER_FILE, "rb");

    if (!fp)
    {
        printf("\nAccount not found!\n");
        return 0;
    }

    for (int attempt = 1; attempt <= 3; attempt++)
    {
        printf("\n================================\n");
        printf("             LOGIN\n");
        printf("================================\n");

        printf("Username: ");
        scanf("%49s", username);

        printf("Password: ");
        scanf("%49s", password);

        rewind(fp);

        if (fread(&o, sizeof(o), 1, fp) == 1 &&
            strcmp(o.username, username) == 0 &&
            strcmp(o.password, password) == 0)
        {
            fclose(fp);

            printf("\n================================\n");
            printf("        LOGIN SUCCESSFUL\n");
            printf("================================\n");

            return 1;
        }

        printf("\nWrong username or password!\n");
        printf("Attempt %d/3\n", attempt);
    }

    fclose(fp);

    printf("\n================================\n");
    printf("       3 ATTEMPTS FAILED\n");
    printf("================================\n");

    return 0;
}

/* ================= FORGOT PASSWORD ================= */

void forgotPassword()
{
    Owner o;

    char username[50];
    char newPassword[50];

    FILE *fp = fopen(OWNER_FILE, "rb+");

    if (!fp)
    {
        printf("\nAccount not found!\n");
        return;
    }

    printf("\n================================\n");
    printf("        FORGOT PASSWORD\n");
    printf("================================\n");

    printf("Username: ");
    scanf("%49s", username);

    if (fread(&o, sizeof(o), 1, fp) == 1 &&
        strcmp(o.username, username) == 0)
    {
        printf("\nUsername verified.\n");

        printf("New Password: ");
        scanf("%49s", newPassword);

        strcpy(o.password, newPassword);

        fseek(fp, -(long)sizeof(o), SEEK_CUR);

        fwrite(&o, sizeof(o), 1, fp);

        printf("\nPassword updated successfully!\n");
    }
    else
    {
        printf("\nUsername not found!\n");
    }

    fclose(fp);
}

/* ================= CHECK ROLL ================= */

int rollExists(int roll)
{
    Student s;

    FILE *fp = fopen(STUDENT_FILE, "rb");

    if (!fp)
        return 0;

    while (fread(&s, sizeof(s), 1, fp))
    {
        if (s.roll == roll)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);

    return 0;
}

/* ================= ADD STUDENT ================= */

void addStudent()
{
    Student s;

    printf("\n================================\n");
    printf("          ADD STUDENT\n");
    printf("================================\n");

    printf("Name: ");
    scanf(" %49[^\n]", s.name);

    printf("Roll Number: ");
    scanf("%d", &s.roll);

    if (rollExists(s.roll))
    {
        printf("\nRoll Number already exists!\n");
        return;
    }

    printf("Student ID: ");
    scanf("%d", &s.id);

    printf("Branch: ");
    scanf(" %29[^\n]", s.branch);

    printf("Age: ");
    scanf("%d", &s.age);

    printf("Marks: ");
    scanf("%f", &s.marks);

    FILE *fp = fopen(STUDENT_FILE, "ab");

    if (!fp)
    {
        printf("\nStudent file error!\n");
        return;
    }

    fwrite(&s, sizeof(s), 1, fp);

    fclose(fp);

    printf("\nStudent added successfully!\n");
}

/* ================= VIEW STUDENTS ================= */

void viewStudents()
{
    Student s;

    FILE *fp = fopen(STUDENT_FILE, "rb");

    if (!fp)
    {
        printf("\nNo student records found!\n");
        return;
    }

    printf("\n");
    printf("-------------------------------------------------------------------------\n");

    printf("%-6s %-20s %-8s %-15s %-5s %-7s\n",
           "Roll",
           "Name",
           "ID",
           "Branch",
           "Age",
           "Marks");

    printf("-------------------------------------------------------------------------\n");

    while (fread(&s, sizeof(s), 1, fp))
    {
        printf("%-6d %-20s %-8d %-15s %-5d %-7.2f\n",
               s.roll,
               s.name,
               s.id,
               s.branch,
               s.age,
               s.marks);
               printf("-------------------------------------------------------------------------\n");
    }

    printf("-------------------------------------------------------------------------\n");

    fclose(fp);
}

/* ================= SEARCH STUDENT ================= */

void searchStudent()
{
    Student s;

    int roll;
    int found = 0;

    FILE *fp = fopen(STUDENT_FILE, "rb");

    if (!fp)
    {
        printf("\nNo student records found!\n");
        return;
    }

    printf("\nEnter Roll Number: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(s), 1, fp))
    {
        if (s.roll == roll)
        {
            printf("\n================================\n");
            printf("       STUDENT DETAILS\n");
            printf("================================\n");

            printf("Name   : %s\n", s.name);
            printf("Roll   : %d\n", s.roll);
            printf("ID     : %d\n", s.id);
            printf("Branch : %s\n", s.branch);
            printf("Age    : %d\n", s.age);
            printf("Marks  : %.2f\n", s.marks);

            found = 1;

            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("\nStudent not found!\n");
}

/* ================= UPDATE STUDENT ================= */

void updateStudent()
{
    Student s;

    int roll;
    int found = 0;

    FILE *fp = fopen(STUDENT_FILE, "rb");
    FILE *temp = fopen(TEMP_FILE, "wb");

    if (!fp || !temp)
    {
        printf("\nFile error!\n");

        if (fp)
            fclose(fp);

        if (temp)
            fclose(temp);

        return;
    }

    printf("\nEnter Roll Number to update: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(s), 1, fp))
    {
        if (s.roll == roll)
        {
            found = 1;

            printf("\nNew Name: ");
            scanf(" %49[^\n]", s.name);

            printf("New Roll Number: ");
            scanf("%d", &s.roll);

            printf("New Student ID: ");
            scanf("%d", &s.id);

            printf("New Branch: ");
            scanf(" %29[^\n]", s.branch);

            printf("New Age: ");
            scanf("%d", &s.age);

            printf("New Marks: ");
            scanf("%f", &s.marks);
        }

        fwrite(&s, sizeof(s), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename(TEMP_FILE, STUDENT_FILE);

    if (found)
        printf("\nStudent updated successfully!\n");
    else
        printf("\nStudent not found!\n");
}

/* ================= DELETE STUDENT ================= */

void deleteStudent()
{
    Student s;

    int roll;
    int found = 0;

    FILE *fp = fopen(STUDENT_FILE, "rb");
    FILE *temp = fopen(TEMP_FILE, "wb");

    if (!fp || !temp)
    {
        printf("\nFile error!\n");

        if (fp)
            fclose(fp);

        if (temp)
            fclose(temp);

        return;
    }

    printf("\nEnter Roll Number to delete: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(s), 1, fp))
    {
        if (s.roll == roll)
        {
            found = 1;
            continue;
        }

        fwrite(&s, sizeof(s), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename(TEMP_FILE, STUDENT_FILE);

    if (found)
        printf("\nStudent deleted successfully!\n");
    else
        printf("\nStudent not found!\n");
}

/* ================= MARK ATTENDANCE ================= */

void markAttendance()
{
    Attendance a;

    printf("\n================================\n");
    printf("        MARK ATTENDANCE\n");
    printf("================================\n");

    printf("Date (DD-MM-YYYY): ");
    scanf("%10s", a.date);

    printf("Student Roll: ");
    scanf("%d", &a.roll);

    if (!rollExists(a.roll))
    {
        printf("\nStudent does not exist!\n");
        return;
    }

    printf("\n1. Present\n");
    printf("2. Absent\n");

    printf("Choice: ");
    scanf("%d", &a.present);

    if (a.present == 1)
        a.present = 1;
    else if (a.present == 2)
        a.present = 0;
    else
    {
        printf("\nInvalid choice!\n");
        return;
    }

    FILE *fp = fopen(ATTENDANCE_FILE, "ab");

    if (!fp)
    {
        printf("\nAttendance file error!\n");
        return;
    }

    fwrite(&a, sizeof(a), 1, fp);

    fclose(fp);

    printf("\nAttendance saved successfully!\n");
}

/* ================= VIEW ATTENDANCE ================= */

void viewAttendance()
{
    Attendance a;

    FILE *fp = fopen(ATTENDANCE_FILE, "rb");

    if (!fp)
    {
        printf("\nNo attendance records found!\n");
        return;
    }

    printf("\n--------------------------------------\n");

    printf("%-15s %-10s %-10s\n",
           "Date",
           "Roll",
           "Status");

    printf("--------------------------------------\n");

    while (fread(&a, sizeof(a), 1, fp))
    {
        printf("%-15s %-10d %-10s\n",
               a.date,
               a.roll,
               a.present ? "Present" : "Absent");
    }

    printf("--------------------------------------\n");

    fclose(fp);
}

/* ================= STUDENT MENU ================= */

void studentMenu()
{
    int choice;

    while (1)
    {
        printf("\n========================================\n");
        printf("       STUDENT MANAGEMENT SYSTEM\n");
        printf("========================================\n");

        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Mark Attendance\n");
        printf("7. View Attendance\n");
        printf("8. Exit\n");

        printf("----------------------------------------\n");

        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addStudent();
            break;

        case 2:
            viewStudents();
            break;

        case 3:
            searchStudent();
            break;

        case 4:
            updateStudent();
            break;

        case 5:
            deleteStudent();
            break;

        case 6:
            markAttendance();
            break;

        case 7:
            viewAttendance();
            break;

        case 8:
            printf("\nThank you!\n");
            exit(0);

        default:
            printf("\nInvalid choice! Enter 1-8.\n");
        }
    }
}

/* ================= LOGIN MENU ================= */
/*
   Ye menu sirf tab aayega
   jab 3 login attempts fail hon.
*/

void loginMenu()
{
    int choice;

    while (1)
    {
        printf("\n================================\n");
        printf("             LOGIN\n");
        printf("================================\n");

        printf("1. Login\n");
        printf("2. Forgot Password\n");
        printf("3. Exit\n");

        printf("--------------------------------\n");

        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:

            if (login())
            {
                studentMenu();
            }

            break;

        case 2:

            forgotPassword();

            break;

        case 3:

            printf("\nThank you!\n");
            exit(0);

        default:

            printf("\nInvalid choice! Enter 1-3.\n");
        }
    }
}

/* ================= MAIN ================= */

int main()
{
    /*
       First run:
       Account create hoga.
    */

    if (!ownerExists())
    {
        createOwner();
    }

    /*
       Program start:
       Direct Login
       Username + Password
    */

    if (login())
    {
        studentMenu();
    }

    /*
       3 attempts fail hone ke baad
       Login Menu show hoga.
    */

    loginMenu();

    return 0;
}