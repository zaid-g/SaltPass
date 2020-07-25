# Free memoryless password manager
## Features
- **Simple**: code is only 129 lines long, so it's easy to audit and improve to fit your specific needs. Supports password updates/changes using simple method.
- **Fast**
- **Memoryless**: no data is kept/stored, improving security. User must re-enter master password every time program is run to retrieve passwords.
- **Secure**<sup>1</sup>: Process memory is cleared before program exits. Terminal hides password and salt while user inputs on screen. Passing the master password as a command line argument not permited, since it gets saved in command history and would be visible to other processes.. For more notes on security, see Usage section below.

<sup>1</sup>Please note that the code has NOT been independently verified to be secure by a third party.

## Usage
compile by running ```gcc -o saltpass saltpass.c -lssl -lcrypto``` or run the executable.

User enters a master password and a "salt" value that represents e.g. the website they are logging into/file they are encrypting/etc. The program then outputs a unique password generated deterministically consisting of pseudo-random alpha-numeric/special characters. 

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

To generate a new password, it is recommended to enter run the program twice to decrease the chance that the generated password is invalid due to typos. 
```
[zaid] ~/dev/SaltPass [master] M?? % ./saltpass
Insert salt (could be website name, file name, etc..), backspace works:
Insert password, backspace works:
Output password given salt is
?RZ|7n;U!gX2C1J}
Preferably, do not copy the password to clipboard
[zaid] ~/dev/SaltPass [master] M?? % ./saltpass
Insert salt (could be website name, file name, etc..), backspace works:
Insert password, backspace works:
Output password given salt is
?RZ|7n;U!gX2C1J}
Preferably, do not copy the password to clipboard
```
The length of the generated passwords is 16 characters by defualt 

## Method 
The program appends the master password and salt, passes them through a sha512 hash function (from openssl), and reformats the sha output to an appropriately formatted unique password displayed to the screen by rescaling the byte values to ascii range [33,126], which are the typable keyboard characters. 

For example, suppose the password is "p@ssw0rd" and salt is "facebook". The program first appends the strings, producing "p@ssw0rdfacebook". The sha512 hash of "p@ssw0rdfacebook" is

```29cd1bc0bf0e6386680207156837d078f7ab80c55cb2fdc351adb8cb3daa12b659701a3afab2a277244ecaadbff888551c8a08a56b6619b6a5edc3c4251261bb```.

Each byte $b$ lies in the range [0, 255]. Remapping the first `PASS_LENGTH` bytes to range [33, 126] $y = b/255*(126 - 33) + 33$ gives us the output password:

`/k*gf&EQF!#(F5lL`

It is impossible for anyone to know what master password and salt were used to produce the output password. So even if one or more output passwords are compromised, a malicious entity will not have any knowledge of the user's other passwords or master password.

