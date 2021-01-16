#include <string.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <math.h>

void read_string(char* password, int terminal_echo)
{
	if(terminal_echo == 0){
		static struct termios old_terminal;
		static struct termios new_terminal;

		// get settings of the actual terminal
		tcgetattr(0, &old_terminal);

		// do not echo the characters
		new_terminal = old_terminal;
		new_terminal.c_lflag &= ~(ECHO);

		// set this as the new terminal options
		tcsetattr(0, TCSANOW, &new_terminal);

		// get the password
		// the user can add chars and delete if he puts it wrong
		// the input process is done when he hits the enter
		fgets(password, BUFSIZ, stdin);

		// go back to the old settings
		tcsetattr(0, TCSANOW, &old_terminal);
	}
	else{
		fgets(password, BUFSIZ, stdin);
	}
}

void memclear_string(char *ch, int l){
    for(int i = 0; i < l; i++)
        ch[i] = '\0'; 
    l = 0;
}

int main(int argc, char* argv[])
{
	//parse command line args
	int terminal_echo = 0; //echo disabled by default (will not display typed characters in terminal)
    for(size_t optind = 1; optind < argc; optind++){
		switch(argv[optind][0])
		{
			case '-':
				switch(argv[optind][1])
				{
					case 'e': 
						terminal_echo = 1; //enable echo with 'e' flag
						break; 
					default:
						fprintf(stderr, "Usage: %s [-e] [file...]\n", argv[0]);
						exit(EXIT_FAILURE);
				}
				break;
			default:
                fprintf(stderr, "Usage: %s [-e] [file...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }   
    } 

    //read string that includes salt (e.g. website), user, master password, state (e.g. iteration)
    char full_string[BUFSIZ]; 
    memclear_string(full_string, sizeof(full_string));
    puts("Insert full string e.g. '<salt><user-initial><password>[U[state][P]][state]':");
    read_string(full_string, terminal_echo);

    // pass to sha256 hash function
    unsigned char hash[32 + 1];
    hash[32] = '\0';
    SHA256(full_string, strlen(full_string), hash);
    memclear_string(full_string, sizeof(full_string));

    //convert to hex representation and print
    const int PASS_LENGTH = 13;
    for(int i=0; i < PASS_LENGTH; i++)
        printf("%02x", hash[i]);
    memclear_string(hash, sizeof(hash));
    printf("\n");
    
    return 0;
}
