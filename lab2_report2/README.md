# Report2

- What is a system call
  - System call is the programmatic way in which a computer program requests a service from the kernel of the operating system it is executed on.
  - System call provides the services of the OS to the user programs via API, and all programs needing resources must use system calls.
  - System calls also provide kernel protection.

- What is fork
  - Fork() system call is used for creating a new process(child process), which runs concurrently with the process that makes the fork() call.
  - After a new child process is created, both processes will execute the next instruction following the fork() syscall. A child process uses the same pc, same CPU registers, same open files used in the parent process. In fact, the child process is the copy of the parent process. 

- How to realize inter-process communication

  1. shared memory

     a region of memory that is shared by the cooperating processes is established. Processes can then exchange information by reading and writing data to the shared region.

  2. message passing

     communication takes place by means of messages exchanged between the cooperating processes.

  3. pipes

     Pipes were one of the first IPC mechanisms in early UNIX system.

- How to realize inter-process connection
  - We can realize interprocess connection by pipe.
  - We can create pipe by int pipe(int pipefd[2]), use fork() to share the pipe with the children, and use read and write data to connect with parent and children processes.

- The prototype of these functions

  | Function  | Prototype                                              |
  | --------- | ------------------------------------------------------ |
  | fork      | pid_t fork(void);                                      |
  | signal    | sighandler_t signal(int signum, sighandler_t handler); |
  | pipe      | int pipe(int pipefd[2]);                               |
  | tcsetpgrp | int tcsetpgrp(int fd, pid_t pgrp);                     |

- Execute "fork.c" and observe, please describe the result (not execution result)

  - 执行结果为 命令 `ls -l / ` 的执行结果

  - 原因：

    fork.c 文件中定义了参数列表数组 `{"/bin/ls","-l","/",NULL}`

    子进程调用了`execvp("/bin/ls",{"/bin/ls","-l","/",NULL})`函数，执行了当前命令, 将当前进程替换为一个新的进程，且新进程与原进程有相同的PID。

    `printf("argc = %d, argv[0] = %s",argc,argv[0]);` 一行为何未打印：

    因为没有`\n`刷新缓冲区, stdout 默认是以行缓冲的

    两种解决方法：

    1. 将这一行改成`printf("argc = %d, argv[0] = %s\n",argc,argv[0]);`

    2. 在下一行加上`fflush(stdout)`

- Execute "fork.c" and observe, please describe how to distinguish between parent and child processes in a program
  - 可以通过fork()函数的返回值来确定父子进程
  - 如果fork() 返回值为0，则该进程为子进程，如果fork()返回值为正数，那么该进程为父进程。

- Execute "pipe.c" and observe, please describe the result (not execution result)

  - 首先输出---|send--一行，再输出--start1|send---一行，再输出 ----start1start2|rec------一行

  - 然后输出`ls -l /etc | more`的结果

  - 原因

    - pipe.c main方法中定义了两个参数数组 `{"/bin/ls","-l","/etc/",NULL} `和 `{"/bin/more",NULL}`

    - 主进程写入管道，并关闭管道写，ls子进程会阻塞读取管道

    - `ls`子进程将命令`ls -l /etc`结果`tmp`输出写入管道，`more`子进程从管道中读取输入，然后将命令`more tmp`的结果输出到标准输出流。

- Execute "pipe.c" and observe. Is `execvp(prog2_argv[0],prog2_argv)`(Line 56) executed? And why?
  - 不一定会执行，如果为more创建子进程失败就不会执行 56 行的代码
- Execute "signal.c" and observe, please describe the result (not execution result)
  - 会一直输出父进程和子进程的PID，子进程被kill了之后父进程输出父进程PID
  - 原因: 程序中创建了父子进程，并且注册了信号处理函数，且允许向处理函数传递信息，通过sigaction() 来改变接受信号之后的行为

- Execute "signal.c" and observe. Please answer, how to execute function `ChildHandler`?

  - 打开另一终端，输入命令 `kill -9 PID(child)`即可让`ChildHandler`执行

- Execute "process.c" and observe, please describe the result (not execution result)

  - 首先会输出父进程和子进程的PID，子进程进入vi编辑器，能够通过 :q 正常退出，子进程正常退出之后，父进程等待输入之后回显

  - `execvp("/bin/vi","vi",NULL)` 为何无法生效？

    - 原因：`int execvp(const char *file, char *const argv[])` 仅接受两个参数，不支持三个参数。

    - 有两种改法

      1. 将execvp 换成execlp

      2. 将这一行改成两行：

         `char* argv[] = {"vi",NULL};`

         `execvp("/bin/vi",argv);`

  - 为何父进程无法成为前台进程组？

    - 因为父进程在后台，向控制台终端尝试写时，会触发`SIGNTTOU`信号量，进程将会被暂停，read/write将会返回错误。

    - 解决方法：
      - 在`waitpid(cpid,0,NULL)`下面一行添加`signal(SIGTTOU,SIG_IGN);`即可正常运行

- execute "process.c" and observe. Please answer, how many ./process in the process list? And what’s the difference between them?
  - 2个 ./process ，一个是父进程，一个是子进程, 2个./process 的进程号不一样

- Execute "process.c" and observe. Please answer, what happens after killing the main process
  - 杀死主进程之后，程序终止，然后将刚才输入的字符显示在终端