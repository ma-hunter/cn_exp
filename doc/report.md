# 计算机网络实验报告

| 班级             | 姓名 | 学号       |
| ---------------- | ---- | ---------- |
| 软件工程 18XX 班 | XXX  | U2018XXXXX |

## 实验一：Wireshark 抓包工具使用

| 系统环境            | 软件版本                                             |
| ------------------- | ---------------------------------------------------- |
| Windows 10 Pro 21H1 | WIRE**SHARK** Version 3.4.6 (v3.4.6-0-g6357ac1405b8) |

### 一、实验目的

1. 学习 Wireshark 抓包工具的使用
2. 了解 Wireshark 抓包工具的功能
3. 通过学习，进一步理解协议及网络体系结构思想

### 二、实验内容

使用 Wireshark，并学习使用它进行网络包分析。

### 三、实验原理

Wireshark 是网络包分析工具；网络包分析工具的主要作用是尝试捕获网络包，并尝试显示包的尽可能详细的情况。Wireshark 常见的应用如下：

- 网络管理员用来解决网络问题
- 网络安全工程师用来检测安全隐患
- 开发人员用来测试协议执行情况

这里，我们通过 Wireshark 来学习常见的网络协议。

### 四、实验过程

包含了从安装 Wireshark 开始到使用 Wireshark 观察一些现象的过程：

#### 4.1 下载 Wireshark

Wireshark 的官方网站是：<https://www.wireshark.org/>，下载地址是<https://2.na.dl.wireshark.org/win64/Wireshark-win64-3.4.6.exe>

#### 4.2 准备工作

再 Powershell 中输入 `ipconfig` 查询本机的 IP 信息：

![查看IP地址](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/screenshots/查看IP地址.png)

在同局域网下打开另一台设备，启动一个服务器：

![局域网内开启服务器](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/screenshots/局域网内开启服务器.png)

查看局域网内的这台设备的 IP 地址：

![检查局域网内服务器IP](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/screenshots/检查局域网内服务器IP.png)

可以得到的信息汇总如下：

| 机器   | IP(v4) 地址  | 子网掩码      | 默认网关    |
| ------ | ------------ | ------------- | ----------- |
| 主机   | 192.168.3.2  | 255.255.255.0 | 192.168.3.1 |
| 服务器 | 192.168.3.71 | 255.255.255.0 | 192.168.3.1 |

因为只需要这些信息，所以直接在 Powershell 中获取；需要更详细的信息也可以通过网络适配器选项中来获得。接下来使用 Wireshark 捕获两者之间的通讯数据包，并且进行分析。

#### 4.3 启动 Wireshark

首先需要先选择监听的网络设备：

![打开网络设备](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/screenshots/打开网络设备.png)

然后，就可以开始捕获以太网的通信数据包：

![正在捕获以太网](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/screenshots/正在捕获以太网.png)

接下来，可以进行捕获通过它的特定报文，并且分析内容。

#### 4.4 TCP握手

浏览器访问上面查询到的服务器的地址 `http://192.168.3.71`，并且在 Wireshark 中设定限制条件（过滤器） `ip.src == 192.168.3.71 or ip.dst == 192.168.3.71` 来只捕获来自这两个 IP 地址之间的通信。

![TCP握手](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/screenshots/TCP握手.png)

高亮的部分就是 TCP 的三次握手。

#### 4.5 TCP报文分析示例

这是上述三次握手过程中的第二次握手的报文的详细信息：

![报文详细分析](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/screenshots/报文详细分析.png)

| 项目             | 信息         | 说明                                           |
| ---------------- | ------------ | ---------------------------------------------- |
| 源端口           | 80           | 服务器的 HTTP 默认端口，服务确实开在 80 端口上 |
| 目的端口         | 61577        | 用户浏览器当前开启的用于和服务器通信的端口     |
| TCP 段长度       | 0            | 该报文不携带数据                               |
| Sequence 数字    | 0            | `Seq=0`                                        |
| Acknowledge 数字 | 1            | 期望收到的下一个报文满足 `Seq=1`               |
| 首部长度         | 32 bytes (8) | 8 * 4B = 32 bytes                              |
| 标志             | 0x012        | SYN 和 ACK 位为 1                              |
| 校验和           | 0x5426       | 校验和为 0x5426，未验证                        |

#### 4.6 HTTP报文分析

在 TCP 握手完成之后，服务器将使用 HTTP 协议传输数据到浏览器；我们在紧接着握手完成后面的位置找到了使用 HTTP 协议传输的数据包；可以打开查看其详细信息：

![HTTP报文](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/screenshots/HTTP报文.png)

| 内容                     | 信息                                                         |
| ------------------------ | ------------------------------------------------------------ |
| `GET / HTTP/1.1`         | 操作类型：GET；遵循了 HTTP 1.1 版本的协议                    |
| `Host: 192.168.3.71`     | 主机名：192.168.3.71；当绑定域名的场合下会是主机的域名       |
| `Connection: keep-alive` | 连接类型是保持持久连接                                       |
| `User-Agent: ...`        | 用户的客户端信息；这里有个 `Mozilla/5.0 (Windows NT 10.0 ...)` 说明是运行在 Windows 10 上的火狐浏览器 |
| ……                       | ……                                                           |

HTTP 头能塞的东西还挺多的，这里就不全部说明了；当然，这里所包含的项目也未必完整。

### 五、思考题

**网络工程师能通过 Wireshark 做哪些工作？**

- 检查网络协议的执行情况
- 排查网络故障，解决网络问题
- 网络攻防，检测网络安全隐患

## 实验二：WinPcap 编程

| 系统环境            | 开发环境                                              |
| ------------------- | ----------------------------------------------------- |
| Windows 10 Pro 21H1 | CLion 2021.1; CMake 3.19; tdm-gcc 9.3; NpCap SDK 1.07 |

### 一、实验目的

- 了解 NpCap 架构和运行逻辑
- 学习使用 NpCap SDK 进行编程

### 二、实验内容

通过学习 NpCap SDK，编写一个网络抓包程序

### 三、实验原理

本实验主要基于 NpCap 完成，所以这里主要是关于 NpCap 的介绍：

#### 3.1 NpCap概述

WinPcap 是一个基于Win32平台的，用于捕获网络数据包并进行分析的开源库；在 Linux 上也有对应的 LibPcap；目前 WinPcap 已经处于无人维护的状态，对于 Windows 10 有更新的且目前有人维护的开源项目 NpCap。

大多数网络应用程序通过被广泛使用的操作系统元件来访问网络，比如 sockets——这是一种简单的实现方式，因为操作系统已经妥善处理了底层具体实现细节（比如协议处理，封装数据包等等工作），并且提供了一个与读写文件类似的，令人熟悉的接口；但是有些时候，这种“简单的实现方式”并不能满足需求，因为有些应用程序需要直接访问网络中的数据包：也就是说原始数据包——即没有被操作系统利用网络协议处理过的数据包。而 WinPcap/NpCap 则为 Win32 应用程序提供了这样的接口：

- 捕获原始数据包；无论它是发往某台机器的，还是在其他设备（共享媒介）上进行交换的
- 在数据包发送给某应用程序前，根据指定的规则过滤数据包
- 将原始数据包通过网络发送出去
- 收集并统计网络流量信息

SDK 提供的这些功能需要借助运行在 Win32 内核中的网络设备驱动程序来实现；在安装完成驱动之后，SDK 将这些功能作为一个接口表现出来以供使用。

#### 3.2 要使用的接口

以下介绍了实现后文提到的 demo 所需要使用的 NpCap API 的简单介绍：

##### 3.2.1 `pcap_findalldevs`

NpCap 提供了 `pcap_findalldevs_ex` 和 `pcap_findalldevs` 函数来获取计算机上的网络接口设备的列表；此函数会为传入的 `pcap_if_t` 赋值——该类型是一个表示了设备列表的链表头；每一个这样的节点都包含了 `name` 和 `description` 域来描述设备。

除此之外，`pcap_if_t` 结构体还包含了一个 `pcap_addr` 结构体；后者包含了一个地址列表、一个掩码列表、一个广播地址列表和一个目的地址的列表；此外，`pcap_findalldevs_ex` 还能返回远程适配器信息和一个位于所给的本地文件夹的 pcap 文件列表。

##### 3.2.2 `pcap_open`

用来打开一个适配器，实际调用的是 `pcap_open_live`；它接受五个参数：

- `name`：适配器的名称（GUID）
- `snaplen`：制定要捕获数据包中的哪些部分。在一些操作系统中 (比如 xBSD 和 Win32)，驱动可以被配置成只捕获数据包的初始化部分：这样可以减少应用程序间复制数据的量，从而提高捕获效率；本次实验中，将值定为 `65535`，比能遇到的最大的MTU还要大，因此总能收到完整的数据包。
- `flags`：主要的意义是其中包含的混杂模式开关；一般情况下，适配器只接收发给它自己的数据包， 而那些在其他机器之间通讯的数据包，将会被丢弃。但混杂模式将会捕获所有的数据包——因为我们需要捕获其他适配器的数据包，所以需要打开这个开关。
- `to_ms`：指定读取数据的超时时间，以毫秒计；在适配器上使用其他 API 进行读取操作的时候，这些函数会在这里设定的时间内响应——即使没有数据包或者捕获失败了；在统计模式下，`to_ms` 还可以用来定义统计的时间间隔：设置为 `0` 说明没有超时——如果没有数据包到达，则永远不返回；对应的还有 `-1`：读操作立刻返回。
- `errbuf`：用于存储错误信息字符串的缓冲区

该函数返回一个 `pcap_t` 类型的 handle。

##### 3.2.3 `pcap_loop`

API 函数 `pcap_loop` 和 `pcap_dispatch` 都用来在打开的适配器中捕获数据包；但是前者会已知捕获直到捕获到的数据包数量达到要求数量，而后者在到达了前面 API 设定的超时时间之后就会返回（尽管这得不到保证）；前者会在一小段时间内阻塞网络的应用，故一般项目都会使用后者作为读取数据包的函数；虽然在本次实验中，使用前者就够了。

这两个函数都有一个回调函数；这个回调函数会在这两个函数捕获到数据包的时候被调用，用来处理捕获到的数据包；这个回调函数需要遵顼特定的格式。但是需要注意的是我们无法发现 CRC 冗余校验码——因为帧到达适配器之后，会经过校验确认的过程；这个过程成功，则适配器会删除 CRC；否则，大多数适配器会删除整个包，因此无法被 NpCap 确认到。

##### 3.2.4 `pcap_datalink`

用于对 MAC 层进行了检测，以确保在处理一个以太网络，确保 MAC 首部是14位的。IP 数据包的首部就位于 MAC 首部的后面，将从 IP 数据包的首部解析到源 IP 地址和目的 IP 地址。

##### 3.2.5 `pcap_compile` & `pcap_setfilter`

用来设置过滤器，以避免处理一些无用的包，提高包处理的效率。在本次实验中我们需要将过滤器字符串设置成 `ip and udp`，使得我们传入的回调只处理基于 IPv4 的 UDP 数据包；大大简化了解析过程和回调函数的调用次数。

#### 3.3 其他的补充

处理 UDP 数据包的首部时存在一些困难：因为 IP 数据包的首部的长度并不是固定的，但是可以通过 IP 数据包的 `length` 域来得到它的长度；一旦知道了 UDP 首部的位置，就能解析到源端口和目的端口。

### 四、实验过程

从安装 NpCap 到运行 NpCap 示例程序的全部过程；

#### 4.1 安装 NpCap

NpCap 是 WinPcap for Windows 10；它的官方下载页面是 [Npcap: Windows Packet Capture Library & Driver (nmap.org)](https://nmap.org/npcap/#download)；在这里我们需要下载：

- NpCap 内核驱动：[Npcap 1.31 installer](https://nmap.org/npcap/dist/npcap-1.31.exe) 
- NpCap SDK 文件：[Npcap SDK 1.07](https://nmap.org/npcap/dist/npcap-sdk-1.07.zip)

安装完成驱动后，再在 IDE 中为项目配置导入 NpCap SDK 文件。NpCap SDK 文件包中包括了使用 NpCap 实现的基本功能的 demo。

#### 4.2 使用 NpCap 的功能

项目的文件结构如下：

```
CMake target: if_list udp_dump basic_dump_ex
(root)
├ module
│ └ (NpCap Library files)				NpCap SDK 库文件
├ src
│ ├ basic_dump_ex.c						目标 basic_dump_ex 的源文件
│ ├ if_list.c							目标 if_list 的源文件
│ └ udp_dump.c							目标 udp_dump 的源文件
└ CMakeList.txt							CMake 项目定义文件
```

这些使用 NpCap 功能的 demo 的实现代码都位于：<https://github.com/ma-hunter/cn_exp>

### 五、实验结果

上述代码包括的，使用 NpCap 实现的几项基本功能的运行结果（非截图）。

#### 5.1 读取网络设备列表

使用 CMake，构建并运行上述项目中的目标 `if_list`：

```
"D:\Program Files\JetBrains\Toolbox\apps\CLion\ch-0\211.6693.114\bin\cmake\win\bin\cmake.exe" --build D:\Workspaces\CLion\cn_exp\cmake-build-debug --target if_list -- -j 4
Scanning dependencies of target if_list
[ 50%] Building C object CMakeFiles/if_list.dir/src/if_list.c.obj
[100%] Linking C executable if_list.exe
[100%] Built target if_list

构建已完成
```

运行结果如下：

```
D:\Workspaces\CLion\cn_exp\cmake-build-debug\if_list.exe \Device\NPF_{5C8B26D4-9439-4304-B8FB-48A81CB33CF9}
        Description: WAN Miniport (Network Monitor)
        Loopback: no

\Device\NPF_{C21F9683-BA8A-4AFE-827A-166030BB1466}
        Description: WAN Miniport (IPv6)
        Loopback: no

\Device\NPF_{1C99E263-4855-4786-97ED-ECAC30A11761}
        Description: WAN Miniport (IP)
        Loopback: no

\Device\NPF_{1A386F2B-3A3E-49A7-B899-2629C46C4FB3}
        Description: Hyper-V Virtual Ethernet Adapter #2
        Loopback: no
        Address Family: #23
        Address Family Name: AF_INET6
        Address Family: #2
        Address Family Name: AF_INET
        Address: 172.26.112.1
        Netmask: 0.240.255.255
        Broadcast Address: 255.31.112.1

\Device\NPF_{A30B9DA2-E755-4DF6-8BD4-010B6590B072}
        Description: Hyper-V Virtual Ethernet Adapter
        Loopback: no
        Address Family: #23
        Address Family Name: AF_INET6
        Address Family: #2
        Address Family Name: AF_INET
        Address: 172.26.0.1
        Netmask: 0.240.255.255
        Broadcast Address: 255.31.0.1

\Device\NPF_{21EAB23F-F3E5-48E0-B370-35B69F40B833}
        Description: Bluetooth Device (Personal Area Network) #3
        Loopback: no
        Address Family: #23
        Address Family Name: AF_INET6
        Address Family: #2
        Address Family Name: AF_INET
        Address: 169.254.62.143
        Netmask: 0.0.255.255
        Broadcast Address: 255.255.62.143

\Device\NPF_{5CCBE082-6FB3-4754-B27A-3659F98979BE}
        Description: Realtek RTL8188EU Wireless LAN 802.11n USB 2.0 Network Adapter
        Loopback: no
        Address Family: #23
        Address Family Name: AF_INET6
        Address Family: #2
        Address Family Name: AF_INET
        Address: 169.254.125.212
        Netmask: 0.0.255.255
        Broadcast Address: 255.255.125.212

\Device\NPF_{E919BE86-5601-4998-B187-2C629DE1F9B9}
        Description: Microsoft Wi-Fi Direct Virtual Adapter #2
        Loopback: no
        Address Family: #23
        Address Family Name: AF_INET6
        Address Family: #2
        Address Family Name: AF_INET
        Address: 169.254.194.126
        Netmask: 0.0.255.255
        Broadcast Address: 255.255.194.126

\Device\NPF_{FF7337C5-5DA3-4AF4-8C55-5A066C578D65}
        Description: Microsoft Wi-Fi Direct Virtual Adapter
        Loopback: no
        Address Family: #23
        Address Family Name: AF_INET6
        Address Family: #2
        Address Family Name: AF_INET
        Address: 169.254.180.74
        Netmask: 0.0.255.255
        Broadcast Address: 255.255.180.74

\Device\NPF_{43524049-AA60-4F6E-A89E-8AE5BA1EAA93}
        Description: Realtek PCIe GbE Family Controller
        Loopback: no
        Address Family: #23
        Address Family Name: AF_INET6
        Address Family: #2
        Address Family Name: AF_INET
        Address: 192.168.3.2
        Netmask: 0.255.255.255
        Broadcast Address: 255.168.3.2

\Device\NPF_Loopback
        Description: Adapter for loopback traffic capture
        Loopback: yes


进程已结束，退出代码为 0
```

于此同时可以看到控制面板下的网络适配器页面；可以看到基本的适配器信息是一致的：

![网络适配器.png](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/screenshots/网络适配器.png)

我们的 demo 输出的更多是因为包含了一些被操作系统用户级别隐藏的接口。

#### 5.2 打开设别并抓包监听

使用 CMake，构建并运行上述项目中的目标 `basic_dump_ex`：

```
"D:\Program Files\JetBrains\Toolbox\apps\CLion\ch-0\211.6693.114\bin\cmake\win\bin\cmake.exe" --build D:\Workspaces\CLion\cn_exp\cmake-build-debug --target basic_dump_ex -- -j 4
Scanning dependencies of target basic_dump_ex
[ 50%] Building C object CMakeFiles/basic_dump_ex.dir/src/basic_dump_ex.c.obj
[100%] Linking C executable basic_dump_ex.exe
[100%] Built target basic_dump_ex

构建已完成
```

运行结果如下：

```
D:\Workspaces\CLion\cn_exp\cmake-build-debug\basic_dump_ex.exe
1. \Device\NPF_{5C8B26D4-9439-4304-B8FB-48A81CB33CF9} (WAN Miniport (Network Monitor))
2. \Device\NPF_{C21F9683-BA8A-4AFE-827A-166030BB1466} (WAN Miniport (IPv6))
3. \Device\NPF_{1C99E263-4855-4786-97ED-ECAC30A11761} (WAN Miniport (IP))
4. \Device\NPF_{1A386F2B-3A3E-49A7-B899-2629C46C4FB3} (Hyper-V Virtual Ethernet Adapter #2)
5. \Device\NPF_{A30B9DA2-E755-4DF6-8BD4-010B6590B072} (Hyper-V Virtual Ethernet Adapter)
6. \Device\NPF_{21EAB23F-F3E5-48E0-B370-35B69F40B833} (Bluetooth Device (Personal Area Network) #3)
7. \Device\NPF_{5CCBE082-6FB3-4754-B27A-3659F98979BE} (Realtek RTL8188EU Wireless LAN 802.11n USB 2.0 Network Adapter)
8. \Device\NPF_{E919BE86-5601-4998-B187-2C629DE1F9B9} (Microsoft Wi-Fi Direct Virtual Adapter #2)
9. \Device\NPF_{FF7337C5-5DA3-4AF4-8C55-5A066C578D65} (Microsoft Wi-Fi Direct Virtual Adapter)
10. \Device\NPF_{43524049-AA60-4F6E-A89E-8AE5BA1EAA93} (Realtek PCIe GbE Family Controller)
11. \Device\NPF_Loopback (Adapter for loopback traffic capture)
Enter the interface number (1-11):10

listening on Realtek PCIe GbE Family Controller...
13:36:31,027242 len:208
13:36:31,068904 len:1071
13:36:31,122063 len:54
13:36:31,423026 len:60
13:36:31,423094 len:54
13:36:31,750427 len:336
13:36:31,750466 len:320
13:36:31,782177 len:66
```

程序将会运行到被外部中断阻止后才会停止运行。

#### 5.3 捕获UDP包并分析

使用 CMake，构建并运行上述项目中的目标 `basic_dump_ex`：

```
"D:\Program Files\JetBrains\Toolbox\apps\CLion\ch-0\211.6693.114\bin\cmake\win\bin\cmake.exe" --build D:\Workspaces\CLion\cn_exp\cmake-build-debug --target udp_dump -- -j 4
[100%] Built target udp_dump

构建已完成
```

运行结果如下：

```
D:\Workspaces\CLion\cn_exp\cmake-build-debug\udp_dump.exe
1. \Device\NPF_{5C8B26D4-9439-4304-B8FB-48A81CB33CF9} (WAN Miniport (Network Monitor))
2. \Device\NPF_{C21F9683-BA8A-4AFE-827A-166030BB1466} (WAN Miniport (IPv6))
3. \Device\NPF_{1C99E263-4855-4786-97ED-ECAC30A11761} (WAN Miniport (IP))
4. \Device\NPF_{1A386F2B-3A3E-49A7-B899-2629C46C4FB3} (Hyper-V Virtual Ethernet Adapter #2)
5. \Device\NPF_{A30B9DA2-E755-4DF6-8BD4-010B6590B072} (Hyper-V Virtual Ethernet Adapter)
6. \Device\NPF_{21EAB23F-F3E5-48E0-B370-35B69F40B833} (Bluetooth Device (Personal Area Network) #3)
7. \Device\NPF_{5CCBE082-6FB3-4754-B27A-3659F98979BE} (Realtek RTL8188EU Wireless LAN 802.11n USB 2.0 Network Adapter)
8. \Device\NPF_{E919BE86-5601-4998-B187-2C629DE1F9B9} (Microsoft Wi-Fi Direct Virtual Adapter #2)
9. \Device\NPF_{FF7337C5-5DA3-4AF4-8C55-5A066C578D65} (Microsoft Wi-Fi Direct Virtual Adapter)
10. \Device\NPF_{43524049-AA60-4F6E-A89E-8AE5BA1EAA93} (Realtek PCIe GbE Family Controller)
11. \Device\NPF_Loopback (Adapter for loopback traffic capture)
Enter the interface number (1-11):10

listening on Realtek PCIe GbE Family Controller...
13:38:26.314222 len:181 192.168.3.2.7726 -> 180.97.33.12.8829
13:38:26.339699 len:163 180.97.33.12.8829 -> 192.168.3.2.7726
13:38:28.517235 len:361 223.166.151.86.8000 -> 192.168.3.2.4017
13:38:28.517526 len:97 192.168.3.2.4017 -> 223.166.151.86.8000
13:38:29.100916 len:82 192.168.3.2.60173 -> 192.168.3.1.53
13:38:29.151404 len:149 192.168.3.1.53 -> 192.168.3.2.60173
13:38:30.577642 len:129 223.166.151.86.8000 -> 192.168.3.2.4017
```

程序将会运行到被外部中断阻止后才会停止运行。

### 六、思考题

**WINPCAP是否能实现服务质量的控制？**

不能。WinPcap 可以独立地通过主机协议发送和接受数据，如同TCP/IP；这就意味着 WinPcap 不能阻止、过滤或操纵同一机器上的其他应用程序的通讯：它仅仅能简单地“监视”在网络上传输的数据包。所以，它不能提供类似网络流量控制、服务质量调度和个人防火墙之类的支持，因而不能实现服务质量的控制。

## 实验三：协议分析&流量统计程序的编写

| 系统环境            | 开发环境                                              |
| ------------------- | ----------------------------------------------------- |
| Windows 10 Pro 21H1 | CLion 2021.1; CMake 3.19; tdm-gcc 9.3; NpCap SDK 1.07 |

### 一、实验目的

- 理解协议在通信中的作用，掌握常见 IP 协议；
- 掌握基于 NpCap 的抓包软件的开发；
- 掌握协议解析和流量统计的编程方法；

### 二、实验内容

利用 NpCap 编写协议分析工具；输出抓取的包和协议分析结构，并统计 IP 的流量（即包的数量）。

功能要求：

1. 利用 NpCap 捕获数据包，并可根据要求进行数据包过滤。
2. 根据IP协议，解析每个数据包的PCI，展示其在不同网络层次所使用的协议结构和具体信息。
3. 根据IP地址，统计源自该IP地址的流量，即捕获到的数据包的数量。

运行程序后将捕获的信息输出到标准输出流中。

### 三、实验原理

实验的设计依据的原理，包括协议的概念和 NpCap 的接口使用逻辑。

#### 3.1 协议说明

下面的内容包含了本次实验所涉及到的协议类型的说明。

##### 3.1.1 协议栈分析

因为 TCP/IP 协议采用分层的结构，所以网络通信时，要传输的数据在发送端是一个逐层封装的过程；而相应地在接收端则是一个逐层分解的过程；如下图所示：

![协议栈分析](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/协议栈分析.png)

在接收端的逐层分解，就是上述封装的逆过程；

##### 3.1.2 以太网协议

以太网 II 格式时一种帧格式，应用最为广泛，几乎成为了当前以太网的现行标准；它由 RFC894 定义，如下图所示：

![以太网协议.png](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/以太网协议.png)

##### 3.1.3 IP 协议

IP 协议是 Internet 的核心协议，它工作在网络层，提供了不可靠无连接的数据传送服务；协议格式如图所示：

![IP协议](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/IP协议.png)



##### 3.1.4 ICMP协议

ICMP 的全称是 Internet 控制信息协议 (Internet Control Message Protocol)。它提供了很多 Internet 的信息描述服务：例如能够检测网络的运行状况，通知协议有用的网络状态信息；ICMP 是基于 IP 协议的，ICMP 协议格式如图所示：

![ICMP协议.png](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/ICMP协议.png)

##### 3.1.5 TCP协议

TCP协议是基于连接的可靠的协议：它负责发收端的协定，然后保持正确可靠的数据传输服务；它在 IP 协议上运行，而 IP 无连接的协议，所以TCP丰富了IP协议的功能，使它具有可靠的传输服务；TCP 协议格式如图所示：

![TCP协议.png](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/TCP协议.png)

可以看到 TCP 报文段结构由以下的部分组成：

- 源端口：指定了发送端的端口 
- 目的端口：指定了接受端的端口号
- 序号：指明了段在即将传输的段序列中的位置
- 确认号：规定成功收到段的序列号，确认序号包含发送确认的一端所期望收到的下一个序号
- TCP 偏移量：指定了段头的长度。段头的长度取决与段头选项字段中设置的选项
- 保留：指定了一个保留字段，以备将来使用
- 标志：`SYN`、`ACK`、`PSH`、`RST`、`URG`、`FIN`
  - `SYN`： 表示同步
  - `ACK`： 表示确认
  - `PSH`： 表示尽快的将数据送往接收进程
  - `RST`： 表示复位连接
  - `URG`： 表示紧急指针
  - `FIN`： 表示发送方完成数据发送
- 窗口：指定关于发送端能传输的下一段的大小的指令
- 校验和：校验和包含 TCP 段头和数据部分，用来校验段头和数据部分的可靠性 
- 紧急指针：指明段中包含紧急信息，只有当 U R G 标志置1时紧急指针才有效
- 选项：指定了公认的段大小，时间戳，选项字段的末端，以及指定了选项字段的边界选项

TCP 连接的建立和释放采用了三步握手法，如下图所示：

![TCP连接建立.jpg](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/TCP连接建立.jpg)

其过程可以描述如下步骤：

1. 主机一发出连接序号为 x（`seq=x`）
2. 主机二应答接受主机一的连接请求，并声明自己的序号为 y（`seq=y`，`ACK=x+1`）
3. 主机一收到确认后，发送第一个数据 TPDU 并确认主机二的序号（`seq=x`，`ACK=y+1`）

至此，整个连接建立过程正常结束，数据传输已经正式开始。

##### 3.1.6 UDP 协议

用户数据报协议 UDP 是在 IP 协议上的传输层协议，它提供了无连接的协议服务；它在IP协议基础上提供了端口的功能，因此既可让应用程序之间进行通信了。UDP 协议格式如图3.7所示：

![UDP协议.png](https://cdn.jsdelivr.net/gh/ma-hunter/cn_exp@main/img/UDP协议.png)

#### 3.2 协议处理

NpCap 按照一定的规则提供了 API，和本次实验相关的逻辑如下：

##### 3.2.1 NpCap 处理流程

NpCap 的工作周期可以被描述为如下序列：

- `pcap_findalldevs` & `pcap_findalldevs_ex`：获得网络接口设备的列表
- `pcap_open` & `pcap_dump_open` & `pcap_open_live`：打开设备/打开设备数据包
- `pcap_freealldevs`：释放获得的设备列表结构所占用的内存
- `pcap_compile` & `pcap_setfilter`：编译并设置过滤器
- `pcap_loop`：根据设定的数量来循环捕获数据包，并调用指定的回调处理数据包
- `packet_handler`：传入 loop 的回调函数，用来处理被捕获的数据包
- `pcap_close`：关闭 NpCap 句柄（即打开的网络设备）

简单地说，使用 NpCap 获得网络接口列表后，我们打开一个设备，并设定循环次数，传入指定类型的回调函数来处理被捕获的数据包，并在程序退出之前关闭设备。

##### 3.2.2 回调函数设计

因为上述协议栈分析中提到了，接收端分析包的内容就是一个逆封装的过程，所以我们可以采用逐层递归的方法来设计我们的回调函数；基本设计如下：

- `(pkt)`：用来传入 NpCap 循环的，帧处理函数
- `(ethernet)`：用来处理以太网协议的部分
- `(ipv4)` & `(ipv6)` & `(arp)`：处理 IPv4、IPv6、ARP 协议的报头
- `(icmp)` & `(tcp)` & `(udp)`：处理 ICMP、TCP、UDP 的报文

根据设计需求，在判断上一层的协议类型后，将待处理的报文递归给下一层的处理函数即可。

### 四、实验设计

关于代码实现方面的设计：

#### 4.1 项目组成

项目可以分为 `handlers`、`helpers` 和 `utils` 三个部分组成；

- `utils` 包含了可复用的无后效逻辑的实现，尽量避免重复代码片段
- `handlers` 包含了传给 NpCap 的回调函数以及递归下降法实现的逆封装函数
- `helpers` 包含了要实现的额外功能，比如本次实验中的流量统计

这三个部分分别使用独立的头文件和源文件组成，被 `main` 模块引用。

#### 4.2 预定义

项目所需要的类型定义都包含在 `definitions.h` 中；包含了根据协议的组成而设计的报文和报头的类型定义；因为最后捕获的数据包是指向一片连续内存块的指针，这里的定义必须保证指定的内存块可以完全转化为对应的包类型才行；因此，需要保证顺序和使用的基本数据类型完全和定义符合。

#### 4.3 项目结构

最后，整个项目的文件结构如下：

```
CMake target: cn_exp
(root)
├ module
│ └ (NpCap Library files)				NpCap SDK 库文件
├ include
│ ├ definitions.h 						预定义的类型声明
│ ├ handlers.h							递归下降法的包处理回调函数的声明
│ ├ helpers.h							流量统计相关方法的声明
│ └ utils.h								可复用模块的函数声明
├ src
│ ├ utils.cpp							可复用模块的函数实现
│ ├ helpers.cpp							流量统计相关方法的实现
│ └ handlers.cpp						递归下降法的包处理回调函数的实现
├ CMakeList.txt							CMake 项目定义文件
└ main.cpp								主进程文件
```

使用 CMake 加载项目，并构建运行 `cn_exp` 即可生成目标。

### 五、实现代码

Github 链接：<https://github.com/ma-hunter/cn_exp>

### 六、实验结果

完成实现上文所述的程序后，运行程序，可以得到下面的结果：

#### 6.1 运行结果

因为上传截图非常的麻烦，这里仅粘贴标准输出的文本。

##### 6.1.1 选择网络设备

```
Hello, World! Hello NpCap!
1.\Device\NPF_{5C8B26D4-9439-4304-B8FB-48A81CB33CF9}WAN Miniport (Network Monitor)
2.\Device\NPF_{C21F9683-BA8A-4AFE-827A-166030BB1466}WAN Miniport (IPv6)
3.\Device\NPF_{1C99E263-4855-4786-97ED-ECAC30A11761}WAN Miniport (IP)
4.\Device\NPF_{1A386F2B-3A3E-49A7-B899-2629C46C4FB3}Hyper-V Virtual Ethernet Adapter #2
5.\Device\NPF_{A30B9DA2-E755-4DF6-8BD4-010B6590B072}Hyper-V Virtual Ethernet Adapter
6.\Device\NPF_{21EAB23F-F3E5-48E0-B370-35B69F40B833}Bluetooth Device (Personal Area Network) #3
7.\Device\NPF_{5CCBE082-6FB3-4754-B27A-3659F98979BE}Realtek RTL8188EU Wireless LAN 802.11n USB 2.0 Network Adapter
8.\Device\NPF_{E919BE86-5601-4998-B187-2C629DE1F9B9}Microsoft Wi-Fi Direct Virtual Adapter #2
9.\Device\NPF_{FF7337C5-5DA3-4AF4-8C55-5A066C578D65}Microsoft Wi-Fi Direct Virtual Adapter
10.\Device\NPF_{43524049-AA60-4F6E-A89E-8AE5BA1EAA93}Realtek PCIe GbE Family Controller
11.\Device\NPF_LoopbackAdapter for loopback traffic capture
Enter the interface number (1-11):
```

##### 6.1.2 设置监听循环的次数

```
Enter the interface number (1-11):10
 listening on Realtek PCIe GbE Family Controller....
Please input the num of packets you want to catch(0 for keeping catching):
```

##### 6.1.3 TCP协议解析示例

```
Time: 12:54:00, 129446		Length: 60
Type: 0x800 (IPv4)
Destination address: c:c:b:6:c:e
Source address: 8:a:0:c:6:c
Version: 5
Header length: 5 (20B)
Type of service: 96
Total length: 40
Identification: 22603
Flags: 0
Fragment offset: 64 (512 B)
Protocol: TCP
Checksum: 32469
Source IP address: 39.96.132.69
Destination IP address: 192.168.3.2
Source port: 443
Destination port: 55308
Sequence: 744910189
Acknowledgement: 846425137
Data offset: 0 (0 B)
Flags: ACK
Window: 28927
Checksum: 54318
Urgent pointer: 0
```

##### 6.1.4 UDP协议解析示例

```
Time: 12:54:00, 148551		Length: 106
Type: 0x800 (IPv4)
Destination address: 8:a:0:c:6:c
Source address: c:c:b:6:c:e
Version: 5
Header length: 5 (20B)
Type of service: 0
Total length: 92
Identification: 21792
Flags: 0
Fragment offset: 0 (0 B)
Protocol: UDP
Checksum: 0
Source IP address: 192.168.3.2
Destination IP address: 112.32.50.208
Source port: 7726
Destination port: 28078
Length: 72
Checksum: 26356
```

##### 6.1.5 流量统计示例

```
Flow counts: 
IP                                         Flow
111.206.210.75                              3
112.32.50.208                               2
13.107.136.9                               53
192.168.3.2                                34
39.96.132.69                                8
```

#### 6.2 构建记录

```
"D:\Program Files\JetBrains\Toolbox\apps\CLion\ch-0\211.6693.114\bin\cmake\win\bin\cmake.exe" -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - MinGW Makefiles" D:\Workspaces\CLion\cn_exp
-- Configuring done
-- Generating done
-- Build files have been written to: D:/Workspaces/CLion/cn_exp/cmake-build-debug

"D:\Program Files\JetBrains\Toolbox\apps\CLion\ch-0\211.6693.114\bin\cmake\win\bin\cmake.exe" --build D:\Workspaces\CLion\cn_exp\cmake-build-debug --target cn_exp -- -j 4
Scanning dependencies of target cn_exp
[ 20%] Building CXX object CMakeFiles/cn_exp.dir/src/handlers.cpp.obj
[ 40%] Linking CXX executable cn_exp.exe
[100%] Built target cn_exp

构建已完成
```

### 七、思考题

**应用WINPCAP能实现哪些网络应用?**

- 捕获原始数据包。不管这个包是发往本地机，还是其他机器之间的交换包
- 在数据包被发送到应用程序之前，通过用户定义的规则过滤
- 向网络发送原始数据包
- 对网络通信量做出统计

## 参考资料

- [计算机网络 | 实验一 wireshark抓包工具使用_哆啦一泓的博客-CSDN博客](https://blog.csdn.net/lee1hong/article/details/106893706)
- [计算机网络 | 实验二 WINPCWP编程_哆啦一泓的博客-CSDN博客](https://blog.csdn.net/lee1hong/article/details/106893772)
- [计算机网络 | 实验三 协议分析程序的编写_哆啦一泓的博客-CSDN博客](https://blog.csdn.net/lee1hong/article/details/106893837)
- [Winpcap进行抓包，分析数据包结构并统计IP流量_include的博客-CSDN博客](https://blog.csdn.net/include_IT_dog/article/details/106813645)

