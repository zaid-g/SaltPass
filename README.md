# Free secure password manager

compile by running ```gcc saltpass.c -lssl -lcrypto``` or run the executable.

User enters a master password and a "salt" value that represents e.g. the website they are logging into/file they are encrypting/etc. The program then outputs a unique password generated deterministically that can be used for that website. 

Method: The program appends the master password and salt, passes them through a sha512 hash function, and reformats the sha output to an appropriately formatted unique password displayed to the screen. It is preferred that user doesn't copy password to clipboard, as any unprivileged process has access to data stored there. Instead, read output and type manually in the destination box. It is recommended that user runs Wayland protocol (ideally with screenshot disabled) over X for enhanced security. Under X, any unprivileged process can sniff and inject keystrokes from/into other processes (a malicious process can easily read master password and salt as they are inputed using keyboard) as well as read the content of the screen (a malicious process can easily read output password from screen).

## Features:
- Simple
- Fast
- Memoryless: no data is kept/stored. User must re-enter master password every time program is run to retrieve passwords.
- Secure: Process memory is cleared before program exits. Terminal hides password and salt while user inputs on screen. 

## Usage:
To generate a new password, it is recommended to enter the master password twice to decrease the chance that the generated password is invalid. This can be done by executing the program with an argument (any argument):
```
[zaid] ~/dev/SaltPass [master]  % ./saltpass y
Insert password:
Insert password again to verify:
Insert salt (could be website name, file name, etc..):
9zZ!.]p:01?t+lQg
```
The last line is the generated password. Note that all generated passwords will start with 9zZ! (can be modified in saltpass.c file) to make sure they have at least one uppercase, lowercase, numerical, and special character. The length of the passwords is 16 characters (can also be modified in file).

Once that password is used to e.g. sign up for an account on a website, then you can run the file without an argument:
```
[zaid] ~/dev/SaltPass [master]  % ./saltpass 
Insert password:
Insert salt (could be website name, file name, etc..):
9zZ!.]p:01?t+lQg
```
