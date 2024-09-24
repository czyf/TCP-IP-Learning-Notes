# 第一章 网络编程和套接字

[toc]


## 1.1 网络编程和套接字

### 1.1.1 构建套接字

**调用socket函数**

```c++
#inclde<sys/socket.h>
int socket(int domain, int type, int protocol);
// 成功时返回文件描述符，失败时返回-1

// type: 指定套接字类型，常见值如 SOCK_STREAM（面向连接的TCP）或 SOCK_DGRAM（无连接的UDP）。
```

- `domain`（地址协议族）: 指定所使用的通信协议族（或地址协议族），决定网络层如何传输数据。常见值有：

  - `AF_INET`:`IPv4`协议，表示使用的`IPv4`地址进行网络通信。
  - `AF_INET6`:`IPv6`协议，表示使用的`IPv6`地址进行网络通信。
  - `AF_UNIX或AF_LOCAL`:用于同一台主机的进程间通信（IPC）。
  - 其他协议族如`AF_PACKET`（原始套接字，直接处理链路层数据），`AF_NETLINK`(与内核通信)。

- **type**（套接字类型）：指定套接字的通信类型，定义了数据传输的方式。常见的值有：

  - `SOCK_STREAM`：面向连接的套接字，主要用于TCP协议。数据在传输中保持顺序且无损。
  - `SOCK_DGRAM`：数据报（无连接）套接字，主要用于UDP协议。数据传输无序且不保证到达。
  - `SOCK_RAW`：原始套接字，允许直接操作底层协议，通常用于处理ICMP或IP协议。
  - `SOCK_SEQPACKET`：有序且可靠的面向连接的消息传输。

- **protocol**（协议）：指定使用的具体协议。当`domain`和`type`确定时，通常可以将此参数设为 `0`，表示使用默认协议。

  - `IPPROTO_TCP`：表示使用TCP协议，与 `SOCK_STREAM` 配合使用。
  - `IPPROTO_UDP`：表示使用UDP协议，与 `SOCK_DGRAM` 配合使用。
  - `IPPROTO_ICMP`：用于ICMP协议，适用于网络控制消息。
  - 当使用`SOCK_RAW`时，可以指定自定义的协议，如 `IPPROTO_RAW`。

- 返回值：

  `socket()`函数成功时返回一个文件描述符（即套接字描述符），用于后续操作，如 `bind()`、`listen()` 等。失败时返回 `-1`，并设置 `errno` 以提供具体错误信息，如 `EMFILE`（已达到最大文件描述符数）、`EAFNOSUPPORT`（指定的协议族不支持）等。

**调用bind函数**

利用`bind`函数给创建好的套接字分配地址信息（IP地址和端口号），主要用于服务端程序。

```c++
#include<sys/socket.h>
int bind(int sockfd, struct sockaddr *myaddr, socketlen_t addrlen);
// 成功时返回0，失败时返回-1
```

调用bind函数给套接字分配地址之后，就基本完成了所有的准备工作。

- **sockfd**（套接字描述符）：这是 `socket()` 函数返回的文件描述符，代表要绑定的套接字。此参数用于标识正在操作的套接字。

- **myaddr**（指向地址结构的指针）：指向一个 `sockaddr` 结构体，包含要绑定的地址和端口信息。根据不同的协议族，`sockaddr` 可能会被具体类型（如 `sockaddr_in` 或 `sockaddr_in6`）取代：

  - 对于 IPv4 地址，使用 `struct sockaddr_in`。

  - 对于 IPv6 地址，使用 `struct sockaddr_in6`。

  - 对于本地进程通信（UNIX域套接字），使用 `struct sockaddr_un`。
  - 结构体通常包含以下信息：
    - `sin_family`：地址族（如 `AF_INET` 用于 IPv4）。
    - `sin_addr`：IP地址（例如，通过 `INADDR_ANY` 绑定到所有可用地址）。
    - `sin_port`：端口号（通常使用 `htons()` 函数将端口号转为网络字节序）。

- **addrlen**（地址长度）：指定 `myaddr` 指针指向的地址结构体的大小。对于 IPv4 地址，通常为 `sizeof(struct sockaddr_in)`；对于 IPv6 地址，使用 `sizeof(struct sockaddr_in6)`

- 返回值：

  - 成功时，`bind()` 返回 `0`，表示套接字成功绑定到指定的IP地址和端口号上。

  - 失败时，返回 `-1`，并设置 `errno` 以指示错误原因（如 `EADDRINUSE` 表示地址已被使用，`EINVAL` 表示无效参数）。

**调用listen函数**

是用于将套接字设置为被动模式，以便接受传入的连接请求。常用于服务器端的网络编程。各个参数的详细介绍如下：

1. **sockfd**（套接字描述符）：这是一个已经通过 `socket()` 函数创建并通过 `bind()` 绑定了IP地址和端口号的套接字。`listen()` 函数将此套接字转换为被动监听模式，准备接受连接请求。

2. **backlog**（最大连接等待队列长度）：指定内核为该套接字维护的已完成三次握手的连接队列的最大长度。这个参数决定了内核允许多少个连接处于等待状态。在达到该限制后，新的连接请求会被拒绝（客户端通常会收到 `ECONNREFUSED` 错误）。

   - 例如，如果 `backlog` 设置为 5，内核最多允许 5 个连接在等待被 `accept()` 处理。
   - 在某些操作系统上，`backlog` 的实际大小可能会受到系统默认值的限制或调整。

3. 返回值：

   成功时，`listen()` 返回 `0`，表示套接字已经成功进入监听状态。

   失败时，返回 `-1`，并设置 `errno` 以指示错误原因（如 `EBADF` 表示无效的文件描述符，`EADDRINUSE` 表示地址已经在使用）

```c++
#include<sys/socket.h>
int listen(int sockfd, int backlog);
// 成功时返回0， 失败时返回-1
```

**调用accept函数**

```c++
#include<sys/socket.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
// 成功时返回文件描述符，失败时返回-1
```

是用于从监听队列中提取第一个完成连接的客户端连接，生成一个新的套接字用于与客户端进行通信。这个函数常用于服务器端的网络编程，配合 `listen()` 使用。各个参数的详细介绍如下：

1. **sockfd**（套接字描述符）：
   - 这是一个处于监听状态的套接字描述符，该套接字应该已经通过 `socket()` 创建，并通过 `bind()` 和 `listen()` 函数进行绑定和监听。
   - `accept()` 从此套接字的等待队列中取出第一个已经完成三次握手的连接，并为该连接生成一个新的套接字来进行数据通信。
2. **addr**（客户端地址结构体指针）：
   - 指向 `sockaddr` 类型的结构体，用于存储连接的客户端地址信息。具体类型通常是 `struct sockaddr_in`（IPv4）或 `struct sockaddr_in6`（IPv6）。
   - 如果不需要获取客户端地址信息，`addr` 可以设置为 `NULL`。
3. **addrlen**（地址结构体大小）：
   - 指向一个 `socklen_t` 类型的变量，表示 `addr` 所指向结构体的大小。函数调用完成后，`addrlen` 会被修改为实际存储的地址大小。
   - 如果 `addr` 为 `NULL`，`addrlen` 也可以设置为 `NULL`。

返回值：

- 成功时，`accept()` 返回一个新的套接字描述符，该套接字用于与客户端进行通信。
- 失败时，返回 `-1`，并设置 `errno` 以指示错误原因（如 `EAGAIN` 或 `EWOULDBLOCK` 表示操作会阻塞但套接字是非阻塞模式）。

**网络编程中和接收连接请求的套接字创建过程**：

1. 调用socket函数创建套接字（文件描述符）；
2. 调用bind函数分配IP地址和端口号；
3. 调用listen函数转换为可接受请求状态；
4. 调用accept函数受理套接字请求。

### 1.1.2 编写Hello World套接字程序

**服务端**：

[服务端程序](./hello_server.cpp)

**客户端**：

[客户端程序](./hello_client.cpp)

**编译**：

分别对客户端和服务端程序进行编译：

```
gcc hello_server.c -o hserver
gcc hello_client.c -o hclient
```

**运行**：

```
./hserver 9190
./hclient 127.0.0.1 9190
```



运行的时候，首先再 9190 端口启动服务，然后 heserver 就会一直等待客户端进行响应，当客户端监听位于本地的 IP 为 127.0.0.1 的地址的9190端口时，客户端就会收到服务端的回应，输出`Hello World!`



##  1.2 基于Linux的文件操作

在linux中，socket也是文件的一种，因此在网络数据传输过程中自然可以使用I/O的相关函数。Windows与Linux不同，是需要去区分socket和文件的。

### 1.2.1 低层访问和文件描述符

分配给标准输入输出及错误的文件描述符

| 文件描述符 | 对象                      |
| ---------- | ------------------------- |
| 0          | 标准输入：Standard Input  |
| 1          | 标准输出：Standard Output |
| 2          | 标准错误：Standard error  |

文件和套接字一般经过创建过程才会被分配文件描述符。

文件描述符也被称为【文件句柄】。

### 1.2.2 打开文件

```c++
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int open(const char *path, int flag);
/*
	成功时返回文件描述符，失败时返回-1
	path：文件名的字符串地址
	flag：文件打开模式信息
*/
```

文件打开模式如下表：

| 打开模式 | 含义                       |
| -------- | -------------------------- |
| O_CREAT  | 必要时创建文件             |
| O_TRUNC  | 删除全部现有数据           |
| O_APPEND | 维持现有数据，保存到其后面 |
| O_RDONLY | 只读打开                   |
| O_WRONLY | 只写打开                   |
| O_RDWR   | 读写打开                   |

### 1.2.3 关闭文件

```c++
#include<unistd.h>

int close(int fd)
/*
	成功时返回0， 失败时返回-1
	fd：需要关闭的文件或套接字的文件描述符
*/
```

若调用此函数同时传递文件描述符参数，则关闭（终止）响应文件。另外需要注意的是，此函数不仅可以关闭文件，还可以关闭套接字。再次证明了「Linux 操作系统不区分文件与套接字」的特点。

### 1.2.4 将数据写入文件

```c++
#include<unistd.h>
ssize_t write(int fd, const void *buf, size_t nbytes);
/*
成功时返回写入的字节数 ，失败时返回 -1
fd : 显示数据传输对象的文件描述符
buf : 保存要传输数据的缓冲值地址
nbytes : 要传输数据的字节数
*/
```

在此函数的定义中，size_t 是通过 typedef 声明的 unsigned int 类型。对 ssize_t 来说，ssize_t 前面多加的 s 代表 signed ，即 ssize_t 是通过 typedef 声明的 signed int 类型。

代码：[write](./write.cpp)

### 1.2.5 读取文件中的数据：

与之前的`write()`函数相对应，`read()`用来输入（接收）数据。

```c++
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t nbytes);
/*
成功时返回接收的字节数（但遇到文件结尾则返回 0），失败时返回 -1
fd : 显示数据接收对象的文件描述符
buf : 要保存接收的数据的缓冲地址值。
nbytes : 要接收数据的最大字节数
*/
```

代码：[read](./read.cpp)

### 1.2.6 文件描述符与套接字

