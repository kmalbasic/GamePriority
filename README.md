# GamePriority
Program that forces process priority of a process while lowering other process priorities

# How does it work?
This program will ask you for the name of your game/program and force the priority you specify
while lowering all other process priorities to low/idle.

#Usage
- You can create a shortcut with command line arguments (GamePriority.exe a notepad.exe 4)
- Run the program normally and manually type in the values

Note: This program needs administrator rights and might trigger some protection software as it
opens a handle (desiredAccess = PROCESS_SET_INFORMATION) to modify the priority.
