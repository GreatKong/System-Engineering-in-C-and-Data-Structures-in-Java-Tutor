/******************************************************************************
 * 
 * This code I practiced processing command line argument such as, getopt.
 * And I also used the file I/O.
 * I used the struct, pointers, and string all in this task.
 * (To reduce the memory, I use the char* instead of string. It's more better than string)
 * Here, I used dynamic memory method such as malloc.
 * Finally, I didn't use sort function in this code.
 * 
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <getopt.h>

// If you type invalid command, then this tells you correct form of command
void end()
{
    printf("Usage: doublesort [-d] [-o output_file_name] input_file_name\n");
    exit(1);
}

// The node is used in doubly linked list.
struct node
{
    char *word;
    struct node *prev, *next;
}
*head, *tail;

// This is the part that creates a new node in a doubly linked list.
struct node* create(char *s)
{
    // I used malloc function for dynamic memory.
    struct node *u = (struct node*)malloc(sizeof(struct node));
    
    // This part is u->word creating part.(malloc dynamic memory)
    if (s)
    {
        u->word = (char*)malloc(sizeof(s));
        strcpy(u->word, s);
    }
    else
    {
        u->word = NULL;
    }
    
    u->prev = u->next = NULL;
    return u;
}

// Insert new NULL node in doubly linked list.
void init()
{
    head = create(NULL), tail = create(NULL);
    head->next = tail;
    tail->prev = head;
    tail->word = (char*)malloc(2);
    tail->word[0] = '}';
    tail->word[1] = '\0';
}

// Insert new node in doubly linked list.
// This node indicate string which inserted in doubly linked list.
void insert(char *word)
{
    // Through the nodes in doubly linked list, I found the suitable position of inserted node.
    for (struct node *current = head->next, *u = create(word); ; current = current->next)
    {
        // Insert new node between current->prev, current.
        if (strcmp(u->word, current->word) < 0)
        {
            struct node *ls = current->prev, *rs = current;
            u->prev = ls;
            ls->next = u;
            rs->prev = u;
            u->next = rs;
            return;
        }
        
        // This means that the duplication occurs, so I ignore this node.
        if (strcmp(u->word, current->word) == 0)
        {
            free(u);
            return;
        }
        
        if (current == tail)
        {
            break;
        }
    }
}

// This part is the final printing answer part.
// Here, if reverse is true, it means that, I have to print the array ascending order.
// And if reverse is false, it means that, I have to print the array descending order.
void print(bool reverse, FILE *out_file)
{
    if (reverse)
    {
        for (struct node *u = tail->prev; u != head; u = u->prev)
        {
            fprintf(out_file, "%s\n", u->word);
        }
    }
    else
    {
        for (struct node *u = head->next; u != tail; u = u->next)
        {
            fprintf(out_file, "%s\n", u->word);
        }
    }
}

// The s used for only inputing the words.
char s[1024];

// This is the main part of my code.
// Here, the argc is an integer that contains the number of arguments passed to the program from the command line.
// And the argv is an array of strings that contains each argument passed to the program.
int main(int argc, char *argv[])
{
    // reverse indicate the sorting order of final array.
    bool reverse = 0;
    char *out_file_name = NULL;
    
    // Here, I used ":" character because, option requires an argument.
    for (char opt; ~(opt = getopt(argc, argv, "do:")); )
    {
        if (opt == 'd')
        {
            reverse = 1;
        }
        else if (opt == 'o')
        {
            out_file_name = optarg;
        }
        else
        {
            end();
        }
    }
    
    // The optind is a variable to keep track of the current index into the command-line arguments array (argv[]).
    if (argc - optind != 1)
    {
        end();
    }
    
    char *in_file_name = argv[optind];
    FILE *in_file = fopen(in_file_name, "r");
    FILE *out_file = stdout;
    
    if (in_file == NULL)
    {
        printf("Could not open input file '%s'\n", in_file_name);
        return 1;
    }
    
    // Here, I insert two NULL nodes in doubly linked list.
    init();
    
    // Read words from input file and insert into linked list
    while (fgets(s, sizeof(s), in_file))
    {
        // The strtok function make me to input the words correctly.
        char *word = strtok(s, " \t\n\r");
        while (word != NULL)
        {
            // remove non-alphanumeric characters and convert to lowercase
            char *src = word, *dest = word;
            while (*src)
            {
                if (isalnum(*src))
                {
                    *dest++ = tolower(*src);
                }
                
                src++;
            }
            
            *dest = '\0';
            
            // Here, I inserted the new node which indicating the word.
            insert(word);
            word = strtok(NULL, " \t\n\r");
        }
    }
    
    // Output sorted words to file or stdout.
    if (out_file_name)
    {
        out_file = fopen(out_file_name, "w");
        if (out_file == NULL)
        {
            printf("could not open output file '%s'\n", out_file_name);
            return 1;
        }
    }
    
    print(reverse, out_file);
    fclose(in_file);
    fclose(out_file);
    
    // Here, I make memory used by doubly linked list to free.
    for (struct node *u = head->next, *nxt; u != tail; u = nxt)
    {
        nxt = u->next;
        free(u);
    }
    
    free(tail->word);
    free(head);
    free(tail);
    
    return 0;
    // If some error occurs, then I returned 1 in this code.
}
