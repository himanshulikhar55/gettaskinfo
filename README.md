# Assignment 2
In this assignment, we implemented a system call to get information about a task from the user space. 

## Steps followed:
- Clone this repo
```
git clone https://github.com/himanshulikhar55/gettaskinfo
```
- Navigate to the file ```/home/<your-user-name>/kernel/linux-5.1O.223/arch/x86/entry/syscalls/syscall_64.tbl```.
- Add a new entry with the ```last_system_call_number + 1``` as show below. ![image](https://github.com/user-attachments/assets/405e47fb-f07c-42a8-b9e4-4d46a22d20d9)
- Navigate to the ```/home/<your-user-name>/kernel/linux-5.1O.223/include/linux/syscalls.h```.
- Add the following entry to the end of the file before `#endif` statement. ![image](https://github.com/user-attachments/assets/d1ee5928-7824-4874-957b-7b2934528885)
- In the main kernel directory, make a new folder with any name of your choice.
- In the folder, make 2 files: `Makefile` and `gettaskinfo.c`.
- In the `Makefile`, add the following line:
```
obj-y := gettaskinfo.o
```
- In the `gettaskinfo.c` file, add the code present in this repo file with the same name.
- Compile the kernel and install the newly compiled kernel with the following commands:
```
sudo make -j 8
sudo make modules_install -j 8
sudo make install
```
- After the installation is complete, boot into the newly installed kernel.
- Open the folder containing this repo's code in the terminal.
- Run the following command:
```
gcc user.c lib-gettaskinfo.c
```
- After the command, run the `./a.out` command to get the task info as required.
