# Free memoryless password manager
## Features
- **Simple**: code is only ~150 lines long, so it's easy to audit/configure to fit your specific needs. Supports password updates/changes using simple method.
- **Fast**
- **Memoryless**: no data is kept/stored, improving security. User must re-enter master password every time program is run to retrieve passwords.
- **Secure**<sup>1</sup>: Process memory is cleared before program exits. Terminal hides password and salt while user inputs on screen (echo disabled by default, but can be optionally enabled by passing -e flag). Passing the master password as a command line argument not permited, since it gets saved in command history and would be visible to other processes.. For more notes on security, see Usage section below.

<sup>1</sup>Please note that the code has NOT been independently verified to be secure by a third party.

## Usage
```
program [-e] [path_to_salt_file]
```

Program tested on Debian linux.

compile by running ```make```. You can also static compile using ```gcc -o saltpass saltpass.c -static -lcrypto``` to allow for running the executable without relying on potentially different/corrupted required libraries on different systems.

To retrieve the password given a salt (salt could be website name user wants to login to, file name user is encrypting, etc..), user executes the program and enters their master password and the salt. The program then outputs the unique password (generated deterministically at run time) consisting of pseudo-random alpha-numeric/special characters given that salt. 

```
e.g.
password: p@ssw0rd
salt: facebook
unique password output for facebook: /k*gf&EQF!#(F5lL
```
```
password: p@ssw0rd
salt: gmail
unique password output for gmail: L9|Q!EL`s|lqZ\'=
```

It is preferred that user doesn't copy password to clipboard, as any unprivileged process has access to data stored there. Instead, read output and type manually in the destination box. It is recommended that user runs Wayland protocol (ideally with screenshot disabled) over X for enhanced security. Under X, any unprivileged process can sniff and inject keystrokes from/into other processes (a malicious process can easily read master password and salt as they are inputed using keyboard) as well as read the content of the screen (a malicious process can easily read output password from screen). User can discard the code and keep the executable, and run ```chmod 544 <executable>``` to prevent unprivileged replacement of executable by a malicious process.

To generate a new password (for example when newly signing up for a website), it is recommended to run the program twice to decrease the chance that the generated password is invalid due to typos. 

The length of the generated passwords is 16 characters by default (can modify in code).

Whenever the program is run, it looks for the most up to date version of the entered salt value (if it exists) in the file path determined by the argument passed to the executable. This is to enable the user to update passwords.

e.g. If `saltfile.txt` contains
```
google#
icloud##
spotify#
```
And the user executes the program with the argument of that file's path e.g. `./saltpass saltfile.txt` and passes the salt "icloud", it will use the salt value in the file "icloud##". This will lead the program to generate the most up to date password for icloud. The user can edit the saltfile by appending a '#' (or any character(s)) to the relevant salt to update the password for that salt. The user can view their previously used passwords for a given salt by running the program without passing an argument, and entering e.g. a salt value = "icloud" to get their first password, "icloud#" to get their second used password, "icloud####" to get their 5th password, etc.. 

## Method 
The program appends the master password and salt, passes them through a sha512 hash function (from openssl), and reformats the sha output to an appropriately formatted unique password displayed to the screen by rescaling the byte values to ascii range [33,126], which are the typable keyboard characters. 

For example, suppose the password is "p@ssw0rd" and salt is "facebook". The program first appends the strings, producing "p@ssw0rdfacebook". The sha512 hash of "p@ssw0rdfacebook" is

```29cd1bc0bf0e6386680207156837d078f7ab80c55cb2fdc351adb8cb3daa12b659701a3afab2a277244ecaadbff888551c8a08a56b6619b6a5edc3c4251261bb```.

Each byte **b** lies in the range [0, 255]. Remapping the first <password_length> bytes to range [33, 126] **y** = **b**/255*(126 - 33) + 33 gives us the output password:

`/k*gf&EQF!#(F5lL`

It is impossible for anyone to know what master password and salt were used to produce the output password. So even if one or more output passwords are compromised, a malicious entity will not have any knowledge of the user's other passwords or master password.

