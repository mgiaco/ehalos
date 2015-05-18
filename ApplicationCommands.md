## Commands for the eHalOS System: Shell, SpaceInvaders and ImageView ##

The operating system (eHalOS) starts by default the idle process as the first system application (first process). Afterwards the idle process starts the shell application, which is the basic process for user interactions. With the shell application the user is able to start new applications (like SpaceInvaders or ImageShow). After starting a new application via the shell, the new applications is running in the window and is therefore available to the user immediately (GUI and UART). The commands for the applications have to be entered via the UART terminal (serial port, settings: baudrate 115200, no parity bits, 1 stop bit). For this use a normal PC or Notebook via serial cable. The following commands are available:

### Shell: ###

  * start PROCESSNAME (E.g. "start SpaceInvaders", "start ImageShow" )
  * kill PROCESS-ID (E.g. "kill 3");
  * top -> shows the current processes in eHalOS
  * help -> shows the available commands for the shell
  * history -> shows the last entered shell commands
  * clear -> clears the screen of the shell

**Its also possible to switch between different processes, therefore use: Ctrl + Alt + |**

For killing/closing a process you have to use the kill command from the shell.


### SpaceInvaders: ###

  * "s" = shot
  * "d" = right
  * "a" = left
  * "o" = Shutdown the application (quit)

### ImageShow: ###

  * has no commands, it runs automatic and switches through several images after around 5 seconds