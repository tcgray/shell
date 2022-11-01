# shell
A fun project where I emulated a Linux shell that can take in built-in Linux commands and custom functions but instead, the arguments are in reverse.

For example, "ls -l" would be typed in as "-l ls". Supports output redirection via "<*" instead of ">" and has some commands such as listchildprocs() which lists PIDs generated from the previous 20 arguments, and backgrounding and foregrounding a process.

To background a process, use '&' before your command (ex: & 10 sleep)
To foreground a process, simply type "fg." This will wait for the child process to terminate. Note that this waits for any child process to terminate, so it is best used with one background process at a time.

To support output redirection, use "<*" instead (ex: abc.txt <* -l ls) - this will redirect the output of "-l ls" to "abc.txt."

To list the most recent child processes, type in "listchildprocs." The most recent 20 child processes (commands) will be displayed.

I used execlp() instead of execvp() to better understand how the exec() family of functions works in C - the syntax would be much simpler using execvp()

Written in C
