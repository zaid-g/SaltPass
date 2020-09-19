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
	size_t file_ind = 0; //salt file
    size_t optind;
    for(optind = 1; optind < argc; optind++){
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
				file_ind = optind;
        }   
    } 

    //read salt 
    puts("\nInsert salt (could be website name, file name, etc..), backspace works:");
    char salt[BUFSIZ]; 
    memclear_string(salt, sizeof(salt));
    read_string(salt, terminal_echo);
    salt[strlen(salt) - 1] = '\0';//replace newline with 0

    //get up to date salt value if file path was passed as argument, and salt exists in file 
	if(file_ind != 0){
        FILE *f;
        if ((f = fopen(argv[file_ind],"r")) == NULL){
            fclose(f);
        }
        else{
            fseek(f, 0, SEEK_END);
            long fsize = ftell(f);
            fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
            char slt_f[fsize + 1];
            slt_f[fsize] = '\0';
            // read full file
            fread(&slt_f, 1, fsize, f);
            fclose(f);
            char *ind = strstr(slt_f, salt); //see if newer salt exists
            if(ind != NULL)
            {
                printf("\n**Found and loaded up to date salt from file**\n\n");
                memclear_string(salt, sizeof(salt));
                for(int i = 0; i < BUFSIZ; i++){
                    if(ind[i] != '\n')
                        salt[i] = ind[i];
                    else
                        break;
                }
            }
        }
    }

    //read password 
    char pass[BUFSIZ]; 
    memclear_string(pass, sizeof(pass));
    puts("Insert password, backspace works:");
    read_string(pass, terminal_echo);
    pass[strlen(pass) - 1] = '\0';

    //create string that will be hashed to produce unique password
    char data[BUFSIZ*2]; 
    memclear_string(data, sizeof(data));

    int stop_ind = 0;
    for(int i = 0; i < BUFSIZ; i++){
        if(pass[i] != '\0' && pass[i] != '\000' && pass[i] != '\n')
            data[i] = pass[i];
        else{
            stop_ind = i;
            break;
        }
    }
    //append salt
    for(int i = 0; i <= BUFSIZ; i++){
        if(salt[i] != '\0' && salt[i] != '\000' && salt[i] != '\n')
            data[stop_ind + i] = salt[i];
        else{
            stop_ind += i;
            break;
        }
    } 
    
    //clear memory
    stop_ind = 0; 
    memclear_string(pass, sizeof(pass));
    memclear_string(salt, sizeof(salt));

    char hash[64 + 1];
    hash[64] = '\0';
    // pass to sha512 hash function
    SHA512(data, strlen(data), hash);
    memclear_string(data, sizeof(data));

    const int PASS_LENGTH = 16;
    char out[PASS_LENGTH + 1];
    out[PASS_LENGTH] = '\0';

    //convert to output password by remapping to appropriate ascii chars range[33,126]
    for(int i = 0; i < PASS_LENGTH; i++){
        out[i] = (float)(unsigned char)hash[i]/255*(126-33) + 33;
    }
    printf("\nOutput password given salt is\n");
    puts(out);
    printf("Preferably, do not copy the password to clipboard\n");
    memclear_string(out, sizeof(out));
    memclear_string(hash, sizeof(hash));

    return 0;
}
