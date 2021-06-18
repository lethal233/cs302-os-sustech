# Report6



- What is Uniprogramming: What is the shortcoming of Uniprogramming:

  - 指在同一时间，内存中除了OS系统进程外，只存在一个进程。缺点：
    1. 效率低，进程切换代价很大，需要反复读写内存磁盘
    2. 功能弱，没办法同时运行多个进程，内存使用率不高。
    3. 没有翻译和保护

  

- What is Multiprogramming: What is the shortcoming of Multiprogramming:

  - 除了OS系统进程外， 内存里可以同时存在多个进程。内存被分成很多块，多个进程可以在同一时间访问不同内存块。缺点：
    1. 会产生内碎片和外碎片
    2. 进程之间的内存隔离问题
    3. 访问内存时的额外开销回答反馈



- What is the segmentation mechanism and its advantages & disadvantages:

  - 分段机制将内存分成可变长度的内存块。 优点：隔离，进程之间不会相互影响。地址转换比较方便。缺点：复杂会产生碎片，无法利用碎片，造成性能损失。

  

- What is the paging mechanism and its advantages & disadvantages:

  - 内存分页将内存分为很多大小相同的帧(frame)优点：不会产生外碎片，内碎皮只会在进程最后一页出现更高效利用内存缺点：基于分段机制要额外的内存存储页表

  

- How to reduce the occurrence of internal fragmentation when allocating memory? Briefly explain why this method works.

  1. 使用分页机制，使得内碎片只会出现在一个进程的最后一页分页机制可以使内碎片只出现在一个进程的最后一页，相对来说限制了内碎片的大小和出现的频率
  2. 根据进程所需动态分配内存，使得分配空间正好与进程所需相同动态分配内存可以减少内碎片存在

  

- How to reduce the occurrence of external fragmentation when allocating memory? Briefly explain why this method works.

  1. 压缩使用动态重新分配机制，将分配的块重新紧密排列，将外碎片清除
  2. 分页机制分配固定大小的块，减少外碎片存在