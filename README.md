# Free memoryless password manager
## Features
- **Simple**: code is only 91 lines long, so it's easy to audit/configure to fit your specific needs. Relies on OpenSSL crypto library for sha512 hash.
- **Fast**
- **Memoryless**: no data is kept/stored, improving security. User must re-enter master password every time program is run to retrieve passwords. User should store state file to keep track of password updates/rotations, however.
- **Secure**: Process memory is cleared before program exits. Terminal hides password and salt while you input on screen (echo disabled by default, but can be optionally enabled by passing -e flag). Passing the master password as a command line argument not permited, since it gets saved in command history and would be visible to other processes.. For more notes on security, see Usage section below.

## Usage
clone the repo, and run ```make```. It will static compile ```saltpass.c``` and create the ```saltpass``` executable, which is the program.

```
./saltpass [-e]
```


To retrieve the password given a salt (salt could be website name you wants to login to, file name you encrypting, etc..), execute the program and enters their master password and the salt. The program then outputs the unique password (generated deterministically at run time) consisting of pseudo-random hexadecimal characters given that salt. Effectively, what the program does is identical to running ```sha256sum <<< "<string>" and truncating the result. Or writing "<string>" to a file and running ```sha256sum``` on it.


It is preferred that you don't copy password to clipboard, as any unprivileged process has access to data stored there. Instead, read output and type manually in the destination box. It is recommended that you run Wayland protocol (ideally with screenshot disabled) over X for enhanced security. Under X, any unprivileged process can sniff and inject keystrokes from/into other processes (a malicious process can easily read master password and salt as they are inputed using keyboard) as well as read the content of the screen (a malicious process can easily read output password from screen). User can discard the code and keep the executable, and run ```chmod 544 <executable>``` to prevent unprivileged replacement of executable by a malicious process.

To generate a new password (for example when newly signing up for a website), it is recommended to run the program twice to decrease the chance that the generated password is invalid due to typos. 

The length of the generated passwords is 20 characters by default (can modify in code).
