# CLI-shell-interface
Basic shell that accepts a set of simple commands

Commands: 

exit 
>End the execution of shell379. Wait until all processes initiated by the
shell are complete. Print out the total user and system time for all
processes run by the shell.

jobs
>Display the status of all running processes spawned by shell379. See
the print format below in the example.

kill (int)
>Kill process <int>.
 
resume 
><int> Resume the execution of process <int>. This undoes a suspend.
  
sleep (int)
>Sleep for <int> seconds.
  
suspend (int)
>Suspend execution of process <int>. A resume will reawaken it.
  
  
wait (pid)
>Wait until process <int> has completed execution.

If none of the above commands is input, then the resulting input string is to be executed
> cmd arg

Special Arguments:


&
If used it is the last argument and indicates the command should be executed in the background


"<fname"
File name will be program input


">fname"
File name will be program output
