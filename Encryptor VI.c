#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>

#define MAX_SIZE 10000

typedef struct Node
{
    char data;
    struct Node *next;
}Node;


Node *top = NULL;

FILE *text_file;
FILE *encrypted_file;
FILE *pass_file;

char password[100], pass_from_file[100], password2[100], file_name[1000];

int isEmpty()
{
    return (top == NULL);
}

void push(char c)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = c;
    temp->next = top;
    top = temp;
}

char pop()
{
    char c = top->data;
    Node *temp = top;
    top = top->next;
    free(temp);
    return c;
}

void encrypt(char *c)
{
    *c = ~(*c^2);
}

void decryptor()
{
    char c, ch;
    encrypted_file = fopen("Encrypted", "r");
    if(encrypted_file == NULL)
    {
        printf("\nERROR: Encrypted file not found. Missing 'Encrypted'.\n");
        system("PAUSE");
        return;
    }
    else
    {
        printf("\nEncrypted file open successful.\n\tRunning Decryption.......\n");
    }
    text_file = fopen(file_name, "w");

    for(c = getc(encrypted_file); c!=EOF; c = getc(encrypted_file))
    {
        encrypt(&c);
        push(c);
    }

    while(!isEmpty())
    {
        c = pop();
        fputc(c, text_file);
    }

    fclose(text_file);
    fclose(encrypted_file);
    if(remove("Encrypted") == 0)
    {
        printf("\nDecryption success..\n");
    }
    else
    {
        perror("ERROR:  ");
        printf("\n");
    }
    system("PAUSE");
}

void encryptor()
{
    char c, ch;
    text_file = fopen(file_name, "r");
    if(text_file == NULL)
    {
        printf("\a");
        perror("ERROR: ");
        printf("\n");
        system("PAUSE");
        return;
    }
    else
    {
        printf("\nText file open successful.\n\tRunning Encryption.......\n");
    }
    encrypted_file = fopen("Encrypted", "w");

    for(c = getc(text_file); c!=EOF; c = getc(text_file))
    {
        encrypt(&c);
        push(c);
    }

    while(!isEmpty())
    {
        c = pop();
        fputc(c, encrypted_file);
    }

    fclose(text_file);
    fclose(encrypted_file);
    if(remove(file_name) == 0)
    {
        printf("\nEncryption success..\n");
    }
    else
    {
        printf("\a");
        perror("ERROR:  ");
        printf("\n");
    }
    system("PAUSE");
}

void getPass(char array[])
{
    char c; int i = 0, j = 0;

    while(1)
    {
        c = _getch();
        if(c == 13)
        {
            break;
        }
        else if(c == 8)
        {
            printf("\b \b");
            i-=1;
        }
        else
        {
            printf("!");
            encrypt(&c);
            //password[i] = c;
            array[i] = c;
            i+=1;
        }
    }
}

void writePasswordToFile()
{
    int i;
    pass_file = fopen("##", "w");
    /*for(i = 0; i < strlen(pass_from_file); i++)
    {
        encrypt(&password[i]);
    }*/
    for(i = 0; i < strlen(password); i++)
    {
        putc(password[i], pass_file);
    }
    fclose(pass_file);
}

void getPassFromFile()
{
    char c; int i;
    pass_file = fopen("##", "r");
    if(pass_file == NULL)
    {
        printf("\nERROR: Pass File not found!\n");
        return;
    }
    for(c = getc(pass_file), i = 0; c != EOF; c = getc(pass_file), i++)
    {
        pass_from_file[i] = c;
    }
    fclose(pass_file);
}

int matchPass(char array1[], char array2[])
{
    int i;
    if(strlen(array1) != strlen(array2))
    {
        printf("\a\nERROR: Password does not match!\n");
        return 0; //0 is FALSE
    }
    for(i = 0; i < strlen(array1); i++)
    {
        if(array1[i] != array2[i])
        {
            printf("\a\nERROR: Password does not match!\n");
            return 0; //0 is FALSE
        }
    }
    return 1; // 1 is TRUE
}

void getFileName()
{
    fgets(file_name, 1000, stdin);
    file_name[strlen(file_name)-1] = 0;
}

void Operation()
{
    encrypted_file = fopen("Encrypted", "r");
    if(encrypted_file == NULL)
    {
        system("CLS");
        printf("\nEnter the name of the text file to be encrypted: ");
        getFileName();
        encryptor();
    }
    else
    {
        system("CLS");
        printf("\nEnter the name of the text file to store decrypted data: ");
        getFileName();
        fclose(encrypted_file);
        decryptor();
    }
}

int initialise()
{
    int result = 0, count = 0;
    encrypted_file = fopen("Encrypted", "r");
    pass_file = fopen("##", "r");
    if(encrypted_file == NULL && pass_file == NULL)
    {
        fclose(pass_file);
        fclose(encrypted_file);
        system("CLS");
        printf("\n\n\t\tWelcome");
        do
        {
            printf("\n\n");
            printf("\t\tCreate password: ");
            getPass(password);
            printf("\n\t\tRe-enter password: ");
            getPass(password2);
            count+=1;
            result = matchPass(password, password2);
            if(result != 1)
            {
                password[0] = -1;
                password2[0] = -1;
                system("PAUSE");
                system("CLS");
            }
        }while(result != 1 && count < 5);
        if(result == 1)
        {
            writePasswordToFile();
            Operation();
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else if(encrypted_file != NULL && pass_file == NULL)
    {
        fclose(pass_file);
        fclose(encrypted_file);
        printf("\n\n\aERROR: Pass File not found.\nContact Developer...\n");
        system("PAUSE");
        return -1;
    }
    else
    {
        fclose(encrypted_file);
        fclose(pass_file);
        return 1;
    }
}

void main()
{
    int count, result;
    system("TITLE  Text Encryptor developed by DDS");
    system("CLS");
    system("COLOR F0");
    if(initialise() == 1 || initialise == 0)
    {
        count = 0; result = 0;
        getPassFromFile();
        if(pass_file == NULL)
        {
            system("PAUSE");
            return;
        }
        do
        {
            system("CLS");
            printf("\n\n\t\tEnter Password: ");
            getPass(password);
            count+=1;
            result = matchPass(password, pass_from_file);
            if(result != 1)
            {
                password[0] = -1;
                printf("\n\tRetry remaining: %d\n", 5-count);
                system("PAUSE");
            }
        }
        while(count != 5 && result != 1);

        if(result == 1)
        {
            Operation();
        }

        if(count == 5)
        {
            encrypted_file = fopen("Encrypted","r");
            if(encrypted_file == NULL)
            {
                printf("\nExiting...\n");
                system("PAUSE");
                return;
            }
            else
            {
                fclose(encrypted_file);
                remove("Encrypted");
                printf("\n\a Number of tries exceeded.\n\n\t\t *** Data erased ***\n");
                system("PAUSE");
            }
        }
    }
    //system("PAUSE");
}

