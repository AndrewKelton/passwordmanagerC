#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<stdlib.h>
#define LIMIT 50

// manager struct
typedef struct{
    
    char username[LIMIT];
    char password[LIMIT];

} manager_t;

// account struct
typedef struct{

    char name[LIMIT];
    manager_t * account;

} account_t;

void openFile(account_t * user, int * currentsize); // open file
void updateFile(account_t * user, int currentsize, int originalnum);
void addAccount(account_t * user, int * currentsize);
int addPassword(account_t * user, int currentsize);
int addUsername(account_t * user, int currentsize);
void printAll(account_t * user, int currentsize);
void deleteUser(account_t * user, int * currentsize);
void freeAll(account_t * user, int currentsize);
void welcome();
void exitMenu();
void clean(char word[]);
void clearBuffer();

// main function
int main()
{
    account_t *user = malloc(sizeof(account_t) * LIMIT);
    int currentsize = 0;

    openFile(user, &currentsize);
    int originalnum = currentsize;

    int choice;

    welcome();

    do 
    {
        printf("\n");
        printf("\t<<<Main Menu>>>\n");
        printf("(1) Add Account\n");
        printf("(2) Print all accounts\n");
        printf("(3) Delete User\n");
        printf("(4) Exit\n");

        printf("Decision: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                addAccount(user, &currentsize);
                break;
            case 2:
                printAll(user, currentsize);
                break;
            case 3:
                deleteUser(user, &currentsize);
                break;
            case 4:
                updateFile(user, currentsize, originalnum);
                freeAll(user, currentsize);
                exitMenu();
                break;
            default:
                printf("Incorrect Input\n");
        }
    } while (choice != 4);

    return 0;
}

// open file function
void openFile( account_t * user, int * currentsize)
{
    // declare variables
    FILE * managerf;
    char username[LIMIT];
    char password[LIMIT];
    char garb1[LIMIT]; // rid fscanf of "username"
    char garb2[LIMIT]; // rid fscanf of "password"

    if (access("passwordmanager.txt", 0) == 0)
    {
        printf("Exists\n");
        managerf = fopen("passwordmanager.txt", "r");

        while (fscanf(managerf,"%s %s %s %s", garb1, username, garb2, password) != EOF)
        {   
            user[*currentsize].account = malloc(sizeof(manager_t));
            strcpy(user[*currentsize].account->username, username);
            strcpy(user[*currentsize].account->password, password);
            (*currentsize)++;
        }
    } 
    else
    {
       printf("Does not\n");
       managerf = fopen("passwordmanager.txt", "w");
       
    }
    free(user[*currentsize].account);
    fclose(managerf);
}

void addAccount( account_t * user, int * currentsize)
{
    int answer;
    char name[LIMIT];

    printf("Here you can add an account.\n");
    printf("Enter your name: ");

    fgets(name, LIMIT, stdin);
    clearBuffer();
    clean(name);

    user[*currentsize].account = malloc(sizeof(manager_t));

    if (addUsername( user, *currentsize) == 1)
    {
        clearBuffer();
        if (addPassword( user, *currentsize) == 1)
        {
            clearBuffer();
            (*currentsize)++;
        }
    }
}

int addPassword( account_t * user, int currentsize)
{
    printf("Enter the password: ");

    if (scanf("%s",user[currentsize].account->password) == 1)
    {
        clean(user[currentsize].account->password);
        return 1;
    }
    else
    {
        return 0;
    }
}

int addUsername( account_t * user, int currentsize)
{
    printf("Enter the username: ");
    
    if (scanf("%s",user[currentsize].account->username) == 1)
    {
        clean(user[currentsize].account->username);
        return 1;
    }
    else
    {
        return 0;
    }
}

void printAll( account_t * user, int currentsize)
{
    printf("\n");
    for (int i = 0; i < currentsize; i++)
    {
        printf("Account %d\n", i + 1);
        printf("Username: %s\n", user[i].account->username);
        printf("Password: %s\n", user[i].account->password);
        printf("-----------------------------\n");
    }
}

void freeAll( account_t * user, int currentsize)
{
    if (currentsize > 0)
    {
        for (int i = 0; i < currentsize; i++)
        {
            free(user[i].account);
        }
    }
    else
    {
        printf("No users have been entered.\n");
    }
    free(user);
}

void updateFile(account_t * user, int currentsize, int originalnum)
{
    FILE * managerf = fopen("passwordmanager.txt", "a");
    
    for (int i = originalnum; i < currentsize; i++)
    {
        fprintf(managerf, "Username: %s\t", user[i].account->username);
        fprintf(managerf, "Password: %s\n", user[i].account->password);
    }
}

void deleteUser(account_t * user, int * currentsize)
{
    int userdel;

    printf("Enter account number to delete: ");
    scanf("%d", &userdel);

    if (*currentsize >= userdel)
    {
        printf("Deleting user #%d\n", userdel);
        free(user[userdel - 1].account);
        (*currentsize)--;
    }
    else
    {
        printf("User does not exit...\n");
        printf("Sending you back to main menu\n");
    }
}

void welcome()
{
    printf("Welcome to my password manager.\n"); sleep(2); 
    printf("To use, enter the number selection\n"); sleep(2); 
    printf("Sending you to the main menu...\n"); sleep(2); //printf("\33[2K\r");

    for (int i = 0; i < 100; i++)
    {
        printf("\n");
    }
}

void exitMenu()
{
    printf("Exiting Menu...\n");
    sleep(1);
    printf("All usernames & passwords will be uploaded to a file 'passwordmanager.txt'.\n");
    exit(1);
}

void clean(char word[])
{
    if (word[strlen(word) - 1] == '\n')
        word[strlen(word) - 1] = '\0';
}

void clearBuffer()
{
    while(getchar() != '\n');
}