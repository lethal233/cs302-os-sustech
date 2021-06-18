# Report3

- What is a process? What is a program? And what is the difference?

  - program: a group of instructions to carry out a specified task

  - process is an execution of certain program with its own address space, and consists of program, data section, and PCB. Process is the unit of scheduling and allocating resources in OS.

  - differences:

    - program is a passive entity, but process is an active entity

    - program is static, but process is dynamic

    - program uses storage resources, but a process uses CPU and memory resources.

- What is job? And what is the difference between process and job?
  - job is a series of programs submitted to os for some goals. 
  - A job is a unit of work or unit of execution. A component of a job is called a task or a step. As a unit execution, a job may be concretely identified with a single process, which may in turn have subprocesses (child processes, the process corresponding to the job being the parent process) which perform the tasks or steps that comprise the work of the job; or with a process group; or with an abstract reference to a process or process group, as in Unix job control. （from Wiki)
  - difference:
    - job is a set of work, and it can consist of one or more processes.

- What are the differences between job scheduling and process scheduling?
  - job scheduling is to load the READY job into the memory and create the corresponding processes in the job and allocate resources to the process respectively.
  - process scheduling is to schedule the process in the READY queue and assign a processor to execute it according to some strategy or method.

- How many statuses are in a job (in the code for this report—scheduler)? And what are they?
  - READY: the job is ready to be executed
  - RUNNING: the job is running 
  - DONE: the job is finished

-  What programs for job control are used in this experiment? And their function?
  - enq: 给scheduler调度程序发送入队请求，将作业提交给系统运行
  - deq: 给scheduler调度程序发送出队请求
  - stat: 给scheduler调度程序发送打印请求，使得scheduler在标准输出流上输出当前运行作业和waiting_queue中所有作业的信息

- What is used for inter-process communication in this experiment? And its function?

  - FIFO

    1. scheduler调度程序创建一个FIFO文件

    2. 三种命令程序把命令按 struct jobcmd的格式写进FIFO中

    3. 调度程序从FIFO中读取提交的命令

    4. FIFO可以用于非亲缘关系进程的通信

- Run the job scheduler program, And analyze the execution of the submitted job:
  - 已提交的作业有三种状态: READY, RUNNING, DONE
  - 在每个时间片内，scheduler有三种策略来调度作业：
    1. 如果没有作业，那么不调度
    2. 一个作业，只调度这一个作业
    3. 如果有多个作业，那么最高优先级的作业最先开始运行，如果优先级一样，那么调度等待最长的作业开始运行。每个作业100ms后优先级+1（优先级最高为3）且作业运行完成后会重置成初始优先级
  - 当有多个作业调度时，在每个时间片结束时都会停止当前作业，将其状态由RUNNING变成READY，之后会从READY队列中选出一个作业继续运行。
    - READY->RUNNING:向该进程发出SIGNCONT信号，使其继续运行，且设置运行的作业的状态为RUNNING
    - RUNNING->READY:向该进程发出SIGTOP信号，使其停止运行，根据策略改变其优先级，且将其状态设置为READY
    - RUNNING->DONE:如果运行中的进程退出了，该作业的状态将设置为DONE，并且在内存中清除其信息。

