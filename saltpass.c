#include <string.h>
#include <termios.h>
#include <stdio.h>
#include <openssl/sha.h>
#include <math.h>

void read_string(char* password)
{
    static struct termios old_terminal;
    static struct termios new_terminal;

    //get settings of the actual terminal
    tcgetattr(0, &old_terminal);

    // do not echo the characters
    new_terminal = old_terminal;
    new_terminal.c_lflag &= ~(ECHO);

    // set this as the new terminal options
    tcsetattr(0, TCSANOW, &new_terminal);

    // get the password
    // the user can add chars and delete if he puts it wrong
    // the input process is done when he hits the enter
    // the \n is stored, we replace it with \0
    if (fgets(password, BUFSIZ, stdin) == NULL)
        password[0] = '\0';
    else
        password[20-1] = '\0';

    // go back to the old settings
    tcsetattr(0, TCSANOW, &old_terminal);
}

void zero_out_str(char *c){
    for(int i = 0; i < BUFSIZ; i++)
        c[i] = '0'; 
}

int main()
{

    //read password without displaying in terminal
    char pass[BUFSIZ];
    puts("Insert password:");
    read_string(pass);

    //read salt without displaying in terminal
    puts("Insert salt (could be website name, file name, etc..):");
    char salt[BUFSIZ];
    read_string(salt);

    //create string that will be hashed to produce unique password
    char data[BUFSIZ*2];
    int stop_ind = 0;
    for(int i = 0; i < 2*BUFSIZ; i++){
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

    // truncate string
    char string_to_hash[BUFSIZ];
    string_to_hash[stop_ind] = '\0';
    for(int i = 0; i < stop_ind; i++)
        string_to_hash[i] = data[i];
    
    //clear memory
    zero_out_str(data);
    stop_ind = 0; 
    zero_out_str(pass);
    zero_out_str(salt);

    // pass to sha512 hash function
    char hash[BUFSIZ];
    for(int i = 0; i < BUFSIZ + 1; i++)
        hash[i] = '\0';

    SHA512(string_to_hash, strlen(string_to_hash), hash);

    //convert to output password by filtering for appropriate ascii chars
    const int PASS_LENGTH = 20;
    char out[BUFSIZ];
    for(int i = 0; i < BUFSIZ; i++)
        out[i] = '\0';

    for(int i = 0; i < PASS_LENGTH; i++){
        //remap to ascii typable character range
        out[i] = (float)(unsigned char)hash[i]/255*(126-33) + 33;
    }
    puts(out);

    //clear memory
    zero_out_str(out);
    zero_out_str(hash);
    zero_out_str(string_to_hash);

    return 0;
}
