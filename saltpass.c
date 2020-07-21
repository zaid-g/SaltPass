#include <string.h>
#include <termios.h>
#include <stdio.h>
#include <openssl/sha.h>

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

void zero_out_str(char str[]){
    for(int i = 0; i < BUFSIZ; i++)
        str[i] = '0'; //clear out memory
}

int main(int argc, char** argv)
{

    //read password without displaying in terminal
    char pass[BUFSIZ];
    puts("Insert password:");
    read_string(pass);

    //if user wants to make sure he inputs correct password (useful when first creating password for a website), he can pass any argument  
    if(argc > 1){
        char password_1[BUFSIZ];
        puts("Insert password again to verify:");
        read_string(password_1);
        if(strcmp(pass, password_1) != 0)
        {
            puts("Passwords don't match, exiting");
            zero_out_str(pass);
            zero_out_str(password_1);
            return 0;
        }
        else
            zero_out_str(password_1);
    }

    //read salt without displaying in terminal
    puts("Insert salt (could be website name, file name, etc..):");
    char salt[BUFSIZ];
    read_string(salt);

    //create string that will be hashed to produce unique password
    char data[BUFSIZ*2];
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

    // truncate string
    char string_to_hash[stop_ind + 1];
    string_to_hash[stop_ind] = '\0';
    for(int i = 0; i < stop_ind; i++)
        string_to_hash[i] = data[i];
    
    //clear memory
    zero_out_str(data);
    stop_ind = 0; 
    zero_out_str(pass);
    zero_out_str(salt);

    char hash[SHA512_DIGEST_LENGTH];
    SHA512(string_to_hash, sizeof(string_to_hash) - 1, hash);

    //convert to output password by filtering for appropriate ascii chars
    const int PASS_LENGTH = 20;
    char out[PASS_LENGTH + 1];
    for(int i = 0; i < PASS_LENGTH + 1; i++)
        out[i] = '\0';
    //adding lowercase, uppercase, numeric, and special character to meet password requirements in case hash output doesn't contain any
    out[0] = '9';
    out[1] = 'z';
    out[2] = 'Z';
    out[3] = '!';
    int outind = 4;
    for(int i = 0; i < SHA512_DIGEST_LENGTH; i++){
        if((int)hash[i] >= 33 && (int)hash[i] <= 126){
            out[outind] = hash[i];
            outind++;
        }
        if(outind >= PASS_LENGTH){
            break;
        }
    }
    puts(out);

    //clear memory
    zero_out_str(out);
    zero_out_str(hash);
    zero_out_str(string_to_hash);

    return 0;
}
