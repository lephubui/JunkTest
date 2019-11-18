#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<termios.h>
#include<stdbool.h>
#include <ctype.h>

#include"hashmap.h"

#define MAXSIZE_INPUT 60
#define MAXIMUM_ALIAS 20


// Function to get user input using getchar() 
void get_user_input(char* input, int my_char, int count) {
    char* scan_input; 

    scan_input = malloc(sizeof(char)*1024);
    // Get the entire sequence of character from user input
    while ((my_char = getchar()) != '\n') {
       	scan_input[count] = my_char;
        count++;
   }
   
   // Copy user input if it is not empty
   if (strlen(scan_input) != 0) {
   	strcpy(input, scan_input);
   } else {
	strcpy(input, scan_input);
        printf("Please enter a command!\n");
   }
   printf("At getChar: %s\n", input);
   free(scan_input);
}

// Function to remove " " in the character pointer
char* remove_quote(char* input_str) {
	int i, j;
	for(i=0,j=0;i<strlen(input_str);i++)
		{
			if(*(input_str+i)!='"')
			{
				*(input_str+j)=*(input_str+i);
				j++;	
			}
		}
	input_str[j]='\0';

	return input_str;
	
}

// Function for parsing the user input to args in the system 
void parse_commands(char* input, char** args_parsed) {
    int i; // Increase the counter
    
    for (i = 0; i < MAXSIZE_INPUT; i++) { // Parsing the string sequence of character
        args_parsed[i] = strsep(&input, " "); // from the getchar to args 
		if(args_parsed[i] == NULL) { 
			break; // If no more character to store, quit the loop
		}
    }
}

// Function to execute each command in the mutiple commands
void execute_commandlines(char** parsing_arguments) {
	// Create a system fork
    pid_t pid_fork = fork();

    if (pid_fork == -1 ) {
        printf("Failed forking child process!\n");
		return;
    } else if (pid_fork == 0) {
	if (execvp(parsing_arguments[0], parsing_arguments) < 0){		    
            printf("Invalid command %s , this shell is NOT supporting it!\n", parsing_arguments[0]);
	}
		exit(0);
    } else {
        wait(NULL);
        return; // Wait for child to terminate
    } 
}

int main() {
    char* input;
	int my_char, counter = 0;
    char* argv[MAXSIZE_INPUT];
    const char *alias_command = "alias";
    const char *unalias_command = "unalias";

	input = malloc(sizeof(char)*MAXSIZE_INPUT + 1); // Delocate memory
    

    hashtable_t *hash_table;

    hash_table = malloc(sizeof(hashtable_t)*1);
    
    hash_table = ht_create(MAXIMUM_ALIAS);

    char *key, *value;
    key =  malloc(sizeof(char)*MAXIMUM_ALIAS + 1);
    value = malloc(sizeof(char)*MAXIMUM_ALIAS + 1);
    char *separate_temp = malloc(sizeof(char)*MAXIMUM_ALIAS + 1);
    char *command_alias, *token_alias , *end_alias;

     while (1) {
        printf(">:");
        // Get the user input
        get_user_input(input, my_char, counter); 

        if (strstr(input, alias_command) != NULL) { // If the input contains alias in the string
            // Duplicate the command to process the divide
            input = strdup(input);
            token_alias = input;
            end_alias = input;
            int count = 0;
            while (token_alias != NULL) {
                separate_temp = strsep(&end_alias, "="); // Separating by command
                if(count == 0) {
                    strcpy(key, separate_temp);
                    printf("Command 1 value %s\n", key);
                } else if (count == 1){
                    printf("ANY %s\n", separate_temp);
                    strcpy(value, separate_temp);
                    printf("Command 2 value %s\n", value);
                } else {
                    printf("It is not valid alias command syntax\n");
                }
                token_alias = end_alias;
                count++;
            }
            
            printf( "After added %s\n", ht_get( hash_table, input) );

            // If the key and valus is not NULL, store it to the hash table, ortherwise, execute it
            if (strlen(value) != 0) {
                char *rcv_value = remove_quote(ht_get(hash_table, key));
                parse_commands(rcv_value, argv); 
                execute_commandlines(argv);   
            } else {
                ht_set(hash_table, key, value);
            }
        } else {
            printf("Not valid alias command %s\n", input);
        }
    
    }

    free(input);
    free(key);
	free(value);
    free(token_alias);
    free(end_alias);
    free(separate_temp);
    free(command_alias);
    return 0;
}
