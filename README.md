# Assignment 2
In this assignment, we implemented a system call `sys_gettaskinfo` to get information about a task from the user space. 

## Steps to follow to implement the system call:
### NOTE: All paths are relative to the kernel source directory.
- In the file ```./arch/x86/entry/syscalls/syscall_64.tbl``` add a new entry with the ```last_system_call_number + 1``` (=441 in case of kernel 5.10.223) as show below. <br> ![image](https://github.com/user-attachments/assets/405e47fb-f07c-42a8-b9e4-4d46a22d20d9)
- In the `syscalls.h` file present in the `./include/linux/` directory, add the following entry at the end of the file (before the `#endif` statement).
<br><br>![image](https://github.com/user-attachments/assets/d1ee5928-7824-4874-957b-7b2934528885)
- Make a new folder called `gettaskinfo` in the current directory (i.e., `./`).
- Add the `Makefile` and `gettaskinfo.c` files in the `./gettaskinfo` folder.
- Compile and install the kernel using the following commands:
```
sudo make -j <num_of_threads>
sudo make modules_install -j <num_of_threads>
sudo make install
```
## Steps to follow to test the system call:
- After booting into the newly installed kernel, in a folder, place the `user.c, lib_gettaskinfo.h and lib_gettaskinfo.c`files.
- The `lib_gettaskinfo.c` contains the `lib_gettaskinfo` function.
- Run the following command to test the system call:
```
gcc -o <outfile> user.c lib-gettaskinfo.c
```
- After compiling the file, execute the `<outfile>` file to get the output.

## Steps to follow to test the system call:
- Run the `remove_kernel.sh` script file from the terminal as follows:
```bash
bash remove_kernel.sh 5.10.223
```
This command will delete the kernel if you are not booted into that kernel version while running the script.

## Design Decisions:
- Details of the `gettaskinfo` system call:
    - It gets the <b>process name, state, start time, and normal priority</b> (in that order) for the calling userspace process.<br>
      <br> <b>NOTE:</b> The task_struct.comm (i.e., the process name field in `task_struct` is just 16 bytes long. Hence make sure that the `<outfile>` that you name is of length `<=16`. The program will not fail but will print an incomplete name. This limit is defined by the `TASK_COMM_LEN` in the `./include/linux/sched.h` file.<br><br>
    - It uses the `pid` sent by the userspace function (in this case, the `lib_gettaskinfo` wrapper) to find the corresponding `struct pid` and gets the `struct task_struct` using the `pid_task` function.
    - Getting the required fields was pretty straightforward, but printing it into the buffer was a bit tricky.
    - We tried printing it using the `copy_to_user` function but failed since the data was not in string format.
    - We used a local buffer (i.e., one declared in the kernel space) and stored the required info in it using the `vsnprintf` function.
    - We used this function since standard C-libraries are unavailable in the kernel-space.
    - After printing the required info in the buffer, we copied it to the userspace buffer using the `copy_to_user` function.
    - The advantage of using the `vsnprintf` function was getting the number of bytes printed into the buffer.
    - Using this offset, we could print the information in the userspace buffer appropriately.
    - Finally, we decided to convert the `start_time` to a human-readable format in the system call itself.
- Details of the `lib_gettaskinfo` function:
    - It makes the system call for the user.
    - It sends the current process `pid` and a pointer to a locally initialized buffer.
    - It does not take any arguments from the user and returns a pointer to `struct task_info ` if the system call is successful.
    - If the system call is not successful, it prints an error number returned by the system call and a message and returns a NULL pointer which the test user program (`user.c`) handles.
    - If the system call is successful, it tokenizes the string and fills up the fields in the `task` variable (of type `struct task_info`) and returns it.
    - We know in what order the entries will be returned; hence, we can fill them appropriately.
