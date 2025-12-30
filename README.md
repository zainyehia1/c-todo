# c-todo

A simple command-line todo list application written in C. The tasks are saved in files.

---

The tasks are saved to a file called `todo.txt` by default if the user doesn't include the file name as a command-line argument.

If the user includes a file name as a command-line argument, then tasks will be saved to it and other functions/commands can be used on it.

---

# The Commands

---

## "add":

This adds a task to the file.

### Usage:

`./todo add "Go to the grocery store"` (todo.txt)
#### Or with a custom file:
`./todo add "Go to the grocery store" tasks.txt`
## "list":
This lists all the tasks in the file. <br>
### Usage:
`./todo list` (todo.txt)
#### Or with a custom file:
`./todo list tasks.txt`