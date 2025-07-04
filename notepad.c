#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>


    
#define MAX_FILENAME 100
#define MAX_CONTENT 10000

char currentFile[MAX_FILENAME] = "";
char fileContent[MAX_CONTENT] = "";


long getFileSize(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}

void listTxtFiles() {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        printf("\nAvailable .txt files:\n");
        int found = 0;
        while ((dir = readdir(d)) != NULL) {
            if (strstr(dir->d_name, ".txt")) {
                printf("- %s\n", dir->d_name);
                found = 1;
            }
        }
        if (!found) {
            printf("No .txt files found.\n");
        }
        closedir(d);
    } else {
        printf("Unable to open directory.\n");
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void displayMenu() {
    printf("\n=== Command Line Notepad By Ashreya and Abinaya===\n");
    if (strlen(currentFile) > 0)
        printf("Opened file: %s\n", currentFile);
    else
        printf("No file currently open.\n");

    printf("1. Create New File\n");
    printf("2. Open Existing File\n");
    printf("3. Append Content\n");
    printf("4. View Current Content\n");
    printf("5. Save File\n");
    printf("6. Clear Current Content\n");
    printf("7. List All .txt Files\n");
    printf("8. Exit\n");
    printf("Choose an option: ");
}

void createNewFile() {
    char tempName[MAX_FILENAME];
    printf("Enter new file name: ");
    fgets(tempName, sizeof(tempName), stdin);
    tempName[strcspn(tempName, "\n")] = '\0'; 

    
    FILE *check = fopen(tempName, "r");
    if (check) {
        fclose(check);
        printf("File '%s' already exists. Overwrite? (y/n): ", tempName);
        char choice = getchar();
        clearInputBuffer();
        if (choice != 'y' && choice != 'Y') {
            printf("File creation canceled.\n");
            return;
        }
    }

    FILE *fp = fopen(tempName, "w");
    if (!fp) {
        printf("Error: Could not create file.\n");
        return;
    }
    fclose(fp);

    // Set currentFile and clear content buffer
    strncpy(currentFile, tempName, MAX_FILENAME);
    fileContent[0] = '\0';

    printf("New file '%s' created successfully.\n", currentFile);
}

void openFile() {
    char tempName[MAX_FILENAME];
    printf("Enter file name to open: ");
    fgets(tempName, sizeof(tempName), stdin);
    tempName[strcspn(tempName, "\n")] = '\0'; 

    FILE *fp = fopen(tempName, "r");
    if (!fp) {
        printf("Error: File not found.\n");
        return;
    }

    size_t bytesRead = fread(fileContent, sizeof(char), MAX_CONTENT - 1, fp);
    fileContent[bytesRead] = '\0';
    fclose(fp);

    strncpy(currentFile, tempName, MAX_FILENAME);

    long size = getFileSize(currentFile);
    if (size >= 0) {
        printf("File '%s' opened successfully. Size: %ld bytes\n", currentFile, size);
    } else {
        printf("File '%s' opened successfully. (Size unavailable)\n", currentFile);
    }
}

void appendContent() {
    if (strlen(currentFile) == 0) {
        printf("No file is open. Please create or open a file first.\n");
        return;
    }

    char temp[1000];
    printf("Enter content to append (end with a single line 'EOF'):\n");

    while (1) {
        if (!fgets(temp, sizeof(temp), stdin)) break; // handle EOF or error

        if (strcmp(temp, "EOF\n") == 0) break;

        if (strlen(fileContent) + strlen(temp) < MAX_CONTENT - 1) {
            strcat(fileContent, temp);
        } else {
            printf("Warning: Buffer full. Cannot append more content.\n");
            break;
        }
    }

    printf("Content stored in memory. Use 'Save File' to write changes.\n");
}

void viewContent() {
    if (strlen(currentFile) == 0) {
        printf("No file is open.\n");
        return;
    }

    printf("\n--- File Content ---\n%s\n", fileContent);
}

void saveFile() {
    if (strlen(currentFile) == 0) {
        printf("No file is open to save.\n");
        return;
    }

    FILE *fp = fopen(currentFile, "w");
    if (!fp) {
        printf("Error: Could not save file.\n");
        return;
    }

    fwrite(fileContent, sizeof(char), strlen(fileContent), fp);
    fclose(fp);
    printf("File saved successfully.\n");
}

void clearContent() {
    if (strlen(currentFile) == 0) {
        printf("No file is open.\n");
        return;
    }

    fileContent[0] = '\0';
    printf("File content cleared from memory.\n");
}

int main() {
    int choice;

    while (1) {
        displayMenu();

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();  // clear leftover newline after number input

        switch (choice) {
            case 1: createNewFile(); break;
            case 2: openFile(); break;
            case 3: appendContent(); break;
            case 4: viewContent(); break;
            case 5: saveFile(); break;
            case 6: clearContent(); break;
            case 7: listTxtFiles(); break;
            case 8:
                printf("Exiting program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
