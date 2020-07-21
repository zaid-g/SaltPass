# Free secure memoryless password generator (beta version)

compile by running ```gcc -o saltpass saltpass.c -lssl -lcrypto``` or run the executable.

User enters a master password and a "salt" value that represents e.g. the website they are logging into/file they are encrypting/etc. The program then outputs a unique password generated deterministically consisting of pseudo-random alpha-numeric/special characters<b>*</b>. 

<b>*</b>4 characters in the password (length 20 by default) are not pseudo-random, the rest are. 

```
e.g.
password: p@ssw0rd
salt: facebook
unique password output: 9zZ!)chh7x\Q=Yp:w$NU
```

## Features:
- Simple
- Fast
- Memoryless: no data is kept/stored. User must re-enter master password every time program is run to retrieve passwords.
- Secure: Process memory is cleared before program exits. Terminal hides password and salt while user inputs on screen. 


# Method 
The program appends the master password and salt, passes them through a sha512 hash function (from openssl), and reformats the sha output to an appropriately formatted unique password displayed to the screen by capturing the bytes that are in ascii range [33,126]. 

For example, suppose the password is "p@ssw0rd" and salt is "facebook". The program first appends the strings, producing "p@ssw0rdfacebook". The sha512 hash of "p@ssw0rdfacebook" is

```29cd1bc0bf0e6386680207156837d078f7ab80c55cb2fdc351adb8cb3daa12b659701a3afab2a277244ecaadbff888551c8a08a56b6619b6a5edc3c4251261bb```.

The ascii output of this  is:

```)ÍÀ¿chh7Ðx÷«Å\²ýÃQ­¸Ë=ª¶Yp:ú²¢w$NÊ­¿øU¥kf¶¥íÃÄ%a»```

filtering for the first 16 ascii characters in range [33,126]:

```)chh7x\Q=Yp:w$NU```

appending 4 characters to beginning of string, the result is the unique password:

```9zZ!)chh7x\Q=Yp:w$NU```

## Usage:
It is preferred that user doesn't copy password to clipboard, as any unprivileged process has access to data stored there. Instead, read output and type manually in the destination box. It is recommended that user runs Wayland protocol (ideally with screenshot disabled) over X for enhanced security. Under X, any unprivileged process can sniff and inject keystrokes from/into other processes (a malicious process can easily read master password and salt as they are inputed using keyboard) as well as read the content of the screen (a malicious process can easily read output password from screen).

To generate a new password, it is recommended to enter the master password twice to decrease the chance that the generated password is invalid due to typos. This can be done by executing the program with an argument (any argument):
```
[zaid] ~/dev/SaltPass [master]  % ./saltpass y
Insert password:
Insert password again to verify:
Insert salt (could be website name, file name, etc..):
9zZ!.]p:01?t+lQg
```
The last line is the generated password. Note that all generated passwords will start with 9zZ! (can be modified in saltpass.c file) to make sure they have at least one uppercase, lowercase, numerical, and special character. The length of the generated passwords is 20 characters (can also be modified in file). There is a tiny probability that the generated passwords will be shorter.

Once that password is used to e.g. sign up for an account on a website, then you can run the executable without an argument to retrieve the password:
```
[zaid] ~/dev/SaltPass [master]  % ./saltpass 
Insert password:
Insert salt (could be website name, file name, etc..):
9zZ!.]p:01?t+lQg
```

