# C TODO List Manager

A simple command-line TODO list manager written in C. It allows you to keep track of tasks, mark them as done, and manage them via a menu or direct command-line arguments. Tasks are stored persistently in a binary file.

---

## Features

- **Add Tasks** – Add one or more tasks to your TODO list.  
- **View Tasks** – Display all tasks with their status (TODO/DONE).  
- **Remove Tasks** – Delete a task by its number.  
- **Mark Tasks Done** – Mark a task as completed.  
- **Interactive & CLI Modes** – Use either a menu or direct commands.  
- **Color-coded Output** – Green (info), Yellow (warnings), Red (errors), Blue (exit).

---

## Usage

### Interactive Mode
Run the program without arguments:
```bash
./todo
You will see a menu:

markdown
Copy code
_____________
|1. Show
|2. Add
|3. Remove
|4. Mark
|5. Exit
|Option:
Enter the number corresponding to your choice.

Command-line Mode
You can also use the program directly with commands:

bash
Copy code
./todo get       # Show all tasks
./todo add       # Add tasks
./todo remove    # Remove a task
./todo mark      # Mark a task as done
Adding Tasks
Enter tasks one per line.

Type a single . on a line to finish adding.

File Storage
Tasks are stored in todo.dat in the current directory.

Each task has:

Description – up to 64 characters.

Status – 0 (TODO) or 1 (DONE).

Compilation
Compile the program using GCC:

bash
Copy code
gcc todo.c -o todo
Example
bash
Copy code
$ ./todo
_____________
|1. Show
|2. Add
|3. Remove
|4. Mark
|5. Exit
|Option: 2
Enter tasks (end with a single '.' on a line):
Buy groceries
Clean desk
.
Task added successfully!

$ ./todo
_____________
|1. Show
|2. Add
|3. Remove
|4. Mark
|5. Exit
|Option: 1
______________________
TODO LIST: todo.dat
1. Buy groceries [TODO]
2. Clean desk [TODO]
______________________
