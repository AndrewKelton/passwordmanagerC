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

// function definitions
void openFile(account_t * user, int * currentsize);
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
    account_t * user = malloc(sizeof(account_t) * LIMIT);
    int currentsize = 0;

    openFile(user, &currentsize); // call openfile
    int originalnum = currentsize;

    int choice;

    welcome(); // call welcome

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

        // check choice value
        switch(choice)
        {
            case 1:
                addAccount(user, &currentsize); // call addAccount
                break;
            case 2:
                printAll(user, currentsize); // call printAll
                break;
            case 3:
                deleteUser(user, &currentsize); // call deleteUser
                break;
            case 4:
                updateFile(user, currentsize, originalnum); // call updateFile
                freeAll(user, currentsize); // call freeAll
                exitMenu(); // call exitMenu
                break;
            default: // // invalid input
                printf("Incorrect Input\n"); 
        }
    } while (choice != 4); // check value of choice

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

    // check access value
    if (access("passwordmanager.txt", 0) == 0)
    {
        managerf = fopen("passwordmanager.txt", "r"); // open file to read

        // loop through to EOF
        while (fscanf(managerf,"%s %s %s %s", garb1, username, garb2, password) != EOF)
        {      
            user[*currentsize].account = malloc(sizeof(manager_t)); // allocate memory
 
            strcpy(user[*currentsize].account->username, username); // copy strings
            strcpy(user[*currentsize].account->password, password); // copy strings
            
            (*currentsize)++; // increment currentsize
        }
    } 
    else
    {
       managerf = fopen("passwordmanager.txt", "w"); // open file to write
       
    }
    free(user[*currentsize].account); // free user account 
    fclose(managerf); // close file 
}

void addAccount( account_t * user, int * currentsize)
{
    int answer;
    char names[LIMIT];

    printf("\n");

    printf("Here you can add an account.\n");
    printf("Enter your name: ");
    
    // collect input
    fgets(names, LIMIT, stdin);
    clearBuffer();
    clean(names);

    strcpy(user[*currentsize].name, names); // copy names to user name

    user[*currentsize].account = malloc(sizeof(manager_t)); // allocate memory to user account

    // check addUsername value
    if (addUsername( user, *currentsize) == 1)
    {
        clearBuffer();

        // check addUsername value
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

    // check scanf return
    if (scanf("%s",user[currentsize].account->password) == 1)
    {
        clean(user[currentsize].account->password); // call clean
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
    
    // check scanf return
    if (scanf("%s",user[currentsize].account->username) == 1)
    {
        clean(user[currentsize].account->username); // call clean
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

    int passlen;
    char hashed = '*';

    if (currentsize != 0) // check value of currentsize
    {
        // loop through user accounts
        for (int i = 0; i < currentsize; i++)
        {
            // print accounts
            printf("Account %d\n", i + 1);
            printf("Username: %s\n", user[i].account->username);
            printf("Password: ");

            passlen = strlen(user[i].account->password);    

            for (int x = 0; x < passlen; x++)
            {
                printf("%c", hashed);
            }
            printf("\n");

            // check currentsize value
            if (i != currentsize - 1)
            {
                printf("-----------------------------\n");
            }
        }
    }
    else
    {
        printf("No accounts to display...\n");
        printf("Sending you back to main menu...\n");
    }

    printf("\n");

    int accountnum;
    int correct = 0;

    // get accountnum
    printf("To print the password of the account, enter the account number.\n");
    scanf("%d", &accountnum);

    // loop through currentsize
    for (int i = 0; i < currentsize; i++)
    {   
        // check accountnum value to i
        if (accountnum - 1 == i)
        {
            printf("Account %d\n", i + 1);
            printf("Username: %s\n", user[i].account->username);
            printf("Password: %s", user[i].account->password);

            correct++; // increment correct
            break;
        }
    }
    if (correct == 0)
    {
        printf("Account not located...\n");
        printf("Sending you back to main menu.\n");
    }

    printf("\n");
}

void freeAll( account_t * user, int currentsize)
{
    // check currentsize value
    if (currentsize > 0)
    {
        // loop through user accounts
        for (int i = 0; i < currentsize; i++)
        {
            //free(user[i].name);
            free(user[i].account);
        }
    }
    else
    {
        printf("No users have been entered.\n");
    }
    free(user); // free user
}

void updateFile(account_t * user, int currentsize, int originalnum)
{
    FILE * managerf = fopen("passwordmanager.txt", "w"); // open file
    
    for (int i = 0; i < currentsize; i++)
    {
        //fprintf(managerf, "%s\n", user[i].name);
        fprintf(managerf, "Username: %s\t", user[i].account->username);
        fprintf(managerf, "Password: %s\n", user[i].account->password);
    }
    fclose(managerf);
}

void deleteUser(account_t * user, int * currentsize)
{
    int userdel; // user to delete
    
    printf("Enter account number to delete: ");
    scanf("%d", &userdel);

    // loop through user.account
    if (*currentsize >= userdel)
    {
        printf("Deleting user #%d\n", userdel);
        free(user[userdel - 1].account); // free user account

        user[userdel - 1].account = user[userdel].account; // set selection to next account

        // loop through user accounts
        for (int i = userdel; i < *currentsize; i++)
        {
            user[i].account = user[i + 1].account; // set next account to previous
        }

        (*currentsize)--; // decrement currentsize
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
    
    // loop to main menu
    for (int i = 0; i < 100; i++)
    {
        printf("\n");
    }
}

// exit function
void exitMenu()
{
    printf("Exiting Menu...\n");
    sleep(1);
    printf("All usernames & passwords will be uploaded to a file 'passwordmanager.txt'.\n");
    exit(1); 
}

// clear \n
void clean(char word[])
{
    if (word[strlen(word) - 1] == '\n')
        word[strlen(word) - 1] = '\0';
}

void clearBuffer()
{
    while(getchar() != '\n');
}
