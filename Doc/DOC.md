[TOC]

---

# 项目名称

虚拟磁盘项目 V2

# 项目简述

在内存中模拟一个磁盘，在内存中完成一些文件操作的功能

# 要求

- 测试环境 VS
- C++
- 三种以上设计模式
- 无异常、无内存泄漏
- 健壮性、容错性
- 所有路径支持中文、正反斜杠

# 相关内容

## 斜杠方向

## 路径

- 绝对路径
- 相对路径

## 符号链接

- 软链接
- mklink 命令

## 磁盘路径

- `@`表示物理路径

## 通配符

- *
- ?

## 命令要求

以下命令除 save、load 均可参考 dos 命令，其输入输出格式参考 dos

- dir
- mkdir
- rd
- cd
- del
- copy
- ren
- move
- mklink
- save
将虚拟磁盘序列化到一个文件中
- load
从一个文件中反序列化为虚拟磁盘
- cls

---

# 项目分析

## 技术需求

- windows API // IDEA: deprecated，实际编码中发现可以不用 windows API 就能达到效果
使用相关的 API 定制 dos 上的显示格式，以达到模仿 dos 命令的效果
- dos 命令
熟知 dos 命令的格式和作用
- 编译原理（词法分析+语法分析）
对输入内容进行词法和语法分析
- 内存管理
整个内存模拟虚拟磁盘的磁盘空间分配、销毁、碎片整理等
- 磁盘数据结构
模拟实际磁盘中存储文件的数据结构（B-树）
- 序列化技术
C++文件序列化反序列化技术（C++反射技术的应用）
- 多线程？
保证虚拟磁盘调度过程中的线程安全、多线程处理时的原子操作？ // IDEA: deprecated

## 详细分析

### Windows API

[参考文档 - Console Functions](https://docs.microsoft.com/en-us/windows/console/console-functions)

提前定义好各种不同命令的输出格式（提前画好界面），根据实际情况添加输出信息

### dos 命令模型

提供命令 API，实现其功能（仿照 dos 命令实现其功能）
大部分文件是对目录的操作，可以转化为 B- 树相关问题

假设有如下文件夹：
ExampleDoc 中含有 E.txt，文件夹 E1，文件夹 E2，文件夹 E3
E1中有文件 E1.txt，文件夹 E11
E2中有文件 E2.txt
E3中有文件 E3.txt

#### dir 命令

无参数
- 功能
输出当前目录下文件及文件夹
- 格式

```
D:\>dir ExampleDoc

D:\ExampleDoc 的目录

2017/07/20 17:12 <DIR> .
2017/07/20 17:12 <DIR> ..
2017/07/20 17:12 21 E.txt
2017/07/20 17:13 <DIR> E1
2017/07/20 17:13 <DIR> E2
2017/07/20 17:13 <DIR> E3
1 个文件 21 字节
5 个目录 321,979,527,168 可用字节
```

- 设计
访问当前 B- 树结点

参数
- /ad
- 功能
只输出其子目录
- 格式

```
D:\>dir ExampleDoc

D:\ExampleDoc 的目录

2017/07/20 17:12 <DIR> .
2017/07/20 17:12 <DIR> ..
2017/07/20 17:12 21 E.txt
2017/07/20 17:13 <DIR> E1
2017/07/20 17:13 <DIR> E2
2017/07/20 17:13 <DIR> E3
1 个文件 21 字节
5 个目录 321,979,527,168 可用字节
```

- /s
- 功能
输出目录及其所有子目录下所有文件
- 格式

```
D:\>dir ExampleDoc

D:\ExampleDoc 的目录

2017/07/20 17:12 <DIR> .
2017/07/20 17:12 <DIR> ..
2017/07/20 17:12 21 E.txt
2017/07/20 17:13 <DIR> E1
2017/07/20 17:13 <DIR> E2
2017/07/20 17:13 <DIR> E3
1 个文件 21 字节
5 个目录 321,979,527,168 可用字节
```

- 设计
遍历 B- 树子树

#### mkdir 命令

无参数
- 功能
创建一个新的文件夹
- 设计
创建 B- 树子结点

#### rd 命令

无参数
- 功能
删除一个空目录
- 设计
删除 B- 树叶子结点

参数
- /s
- 功能
递归删除目录及其所有子目录下指定文件
- 设计
删除 B- 树子树

#### cd 命令

无参数
- 功能
显示当前目录名或切换当前目录
- 格式

```
D:\>cd
D:\

D:\>cd ExampleDoc
```

- 设计
显示 B- 树当前结点到根结点的值

#### del 命令

无参数
- 功能
删除一个或多个文件
- 设计
释放文件内存到内存池

#### copy 命令

无参数
- 功能
深拷贝文件到指定目录中
- 设计
B- 树结点访问，文件内存分配、回收

#### ren 命令

无参数
- 功能
给一个文件或目录改名
- 设计
B- 树结点访问，修改值

#### move 命令

无参数
- 功能
移动一个或多个文件、文件夹
B- 树结点移动，文件内存分配，回收

参数
- /y
- 功能
遇到同名的文件或者文件夹覆盖
- 设计
B- 树结点移动，文件内存分配，回收

#### mklink 命令

无参数
- 功能
对虚拟磁盘中的文件建立 SYMLINK 类型符号链接
- 设计
创建 B- 树子节点，文件内存分配，回收（符号链接视为一种文件）

参数
- /D
- 功能
为文件夹创建 SYMLINKD 类型符号链接
- 设计
创建 B- 树子节点，文件内存分配，回收（符号链接视为一种文件）

> 注
> - 不能为不存在的路径创建链接
> - 链接的文件或者目录要能够被复制和删除

#### save 命令

无参数
- 功能
将整个虚拟磁盘序列化到一个物理文件
- 设计 IDEA: 不够详细
采用序列化技术，将虚拟磁盘信息序列化

#### load 命令

无参数
- 功能
从磁盘载入文件，重建虚拟磁盘
- 设计 IDEA: 不够详细
采用反序列化技术，将文件反序列化为虚拟磁盘

#### cls 命令

无参数
- 功能
清空屏幕，移动光标至第一行第一个位置
- 设计
使用 Windows 内建函数 cls

### 编译原理

输入指令的解析过程

命令原语 {[参数]} [{操作对象}]

- 建立语法

Vn = {命令原语、参数、操作路径、操作对象}

Vt = {操作对象}

P = {

S->ABCD

A->命令原语

B->B参数|e

C->C操作路径|e

D->D操作对象|e

}

S = 输入流

### 内存管理

[参考文档 - 内存池-百度百科](https://baike.baidu.com/item/%E5%86%85%E5%AD%98%E6%B1%A0)

- 内存分配
    - 连续区间分配
    - 非连续区间分配

- 内存回收
    - 回收并非真正的释放，而是将控制权交到虚拟磁盘的手中

- 碎片整理
    - 碎片整理触发条件
    - 碎片整理算法

- 类型擦除技术（待定）
在C++中将对象的创建和销毁分为以下四步：
1、内存分配
2、内存初始化
3、对象删除
4、内存回收
内存池技术中配置的空间得是**未经初始化的空间**，但是 std::allocator 中需要指定空间分配类型，所以这里可能需要考虑使用类型擦除技术。

### 磁盘存储模型

[参考文档 - 硬盘数据结构-百度百科](https://baike.baidu.com/item/%E7%A1%AC%E7%9B%98%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84)

对一个虚拟磁盘（大块连续内存），划分为 n 个分区

IDEA: 按大小等分，最后一块不补齐

将磁盘中划分为**磁盘数据**，**目录数据**和**文件数据**

> 磁盘数据：记录磁盘上文件的位置，将位置与文件建立一一对应关系
> 目录数据：目录并不是文件数据，而是一棵 B- 树，每一个结点中含有一个指向文件数据的某一个位置的指针
> 文件数据：存储在虚拟磁盘上的实际数据实际数据

建立磁盘数据表和文件数据表

磁盘数据表如下

|名称|类型|含义|说明|
|---|
|分区标识|char|磁盘分区标识|d:|
|分区大小|unsigned int|磁盘分区大小标识|区分两个不同的分区|
|目录数据|n 叉树|记录磁盘中的文件夹的信息|d:\github\LOG.md|

文件数据表如下

|名称|类型|含义|说明|
|---|
|文件类型|FileType|文件类型标识|区分该文件是一般文件还是文件夹|
|最后修改日期|string|文件访问时间标识|记录该文件最后一次访问的时间，精确到分钟，用以 dir 命令的输出|
|文件名称|string|文件名称标识|example.txt|
|文件长度|unsigned int|文件长度标识|example.txt 内有一个字符c，其文件大小是1|
|文件数据|void*|文件数据|文件数据首地址||

#### 序列化技术

[参考文档 - 序列化-百度百科](https://baike.baidu.com/item/%E5%BA%8F%E5%88%97%E5%8C%96)

- 序列化

- 反序列化

---

# 项目设计

## 数据结构设计

磁盘数据 ADT

|名称|类型|说明|
|---|
|name|char|分区标识|
|length|unsigned int|分区大小|
|free|unsigned int|可用分区大小|
|dtree|NTree*|当前分区的 n 叉树|
|address|void*|分区首地址|
|address|multimap < size_t, void* > | 以长度为关键字的小区间指针集合|

IDEA: 小区间是指当前已分配的文件结尾后第一个指针到下一个文件的头指针开始中间的区间小于某个特定值（这里涉及到对 B- 树遍历查找离当前指针最近的文件指针，因为虚拟磁盘的物理空间是一段连续的内存池）

文件数据 ADT

|名称|类型|说明|
|---|
|size|enum|文件类型|
|length|unsigned int|文件大小|
|name|char*|文件名称|
|address|void*|文件首地址|
|date|char*|最后修改日期|

文件类型 enum

|名称|值|说明|
|---|
|generalFile|0|一般文件|
|symlink|1|链接文件类型1 SYMLINK|
|symlinkd|2|链接文件类型2 SYMLINKD|
|directoryFile|3|目录文件|

## 领域类提出

|类名称|类职责|
|---|
|VDos|模拟 dos 环境类，与外界的借口|
|StringParser|字符串解析类，根据输入分析指令|
|Printer|打印系统类，根据不同的命令打印打印不同格式的内容|
|CommandDIRPrinter|dir 命令类，实现 dir 命令的输出格式|
|CommandCDPrinter|cd 命令类，实现 cd 命令的输出格式|
|Command|命令类，根据 StringParser 的结果返回命令的方法|
|DiskPartition|分区对象类，分区空间管理（提供自由空间指针，回收空间，空间碎片整理），维护分区的 n 叉树，参考磁盘数据 ADT|
|DiskSystem|提供磁盘全局访问点，划分磁盘空间，维护分区对象容器|
|File|文件类，用以表示虚拟磁盘上的某个具体文件，参考文件数据 ADT|

## 模块化设计

|模块名称|模块说明|涉及类|
|---|
|Dos Module|模拟 dos 的 I/O 环境|StringParser,Printer|
|Execution Module|dos 命令的执行模块|Command*|
|Disk Module|模拟磁盘的存储结构|Disk*,File|

## 备选设计模式

- 策略模式
用于 Printer 类在接收不同的命令的时候打印不同格式的内容

- 外观模式
降低圈复杂度，用统一的外观（VDos 类）与外界输入交互

- 单例模式
磁盘控制的全局访问点，通过该访问点访问磁盘的不同分区

- 工厂模式（对每个命令建立一个类会带来庞大复杂的类体系，增加虚指针转换的代价） // IDEA: deprecated
为每个不同的命令创建对应的命令工厂，可以考虑返回命令的执行句柄将命令的执行延迟到真正需要执行的时候

- 观察者模式 （只有一个观察者对象，直接关联比转发调用代价小） // IDEA: deprecated
观察虚拟磁盘中小区间的指针数，如小区间过多，则通知对应磁盘分区对象进行空间碎片整理
观察命令系统发出的对磁盘空间操作的请求，如果有空间操作，则通知磁盘分区对象提供分配空间指针

- 装饰模式（带来庞大复杂的类体系） // IDEA: deprecated
为带有参数的命令类，通过添加参数动态地添加一些额外的职责

- 模板方法模式（依旧需要对命令建立类，带来庞大复杂的类体系） // IDEA: deprecated
代替装饰模式，根据不同的参数提供命令关于参数的子类型，将不同参数的实现放到子类型中

## UML 类图

[策略模式 - 类图](http://172.17.100.141/liyunpeng/HelloWorld/blob/master/StrategyPattern.jpg)
[外观模式 - 类图](http://172.17.100.141/liyunpeng/HelloWorld/blob/master/FacadePattern.jpg)
[交互流程 - 时序图](http://172.17.100.141/liyunpeng/HelloWorld/blob/master/SequenceDiagram.jpg)
[程序流程 - 流程图]()

程序流程：流程图待补充
Start -> Init Program -> Wait Operation -> End

Step: Init Program
- Disk Module
- allocate memory
- if program doesn't have argument
- build memory partitions
- for each memory partition
- build btree
- initialize memory partition information
- if program has argument
- deserialize Virtual Disk file
- Execution Module
- create command object
- create command hash table

Step: Wait Operation
- input string
- analysis string
- match command
- execute command
- output result

Step: End
- Execution Module
- destory command object
- destory command hash table
- Disk Module
- serialize Virtual Disk // IDEA: add two additional commands to do serialization/deserialization in Step Wait Operation
- deallocate memory

## 补充说明

设计可能会根据实际的编码情况来进行修改，如在编码某个模块的时候添加一些辅助的类、结构体

# 待解决问题

## 命令的不带参数形式和带参数形式之间的组合方式？

- 根据解析的结果通过虚指针转换成特定类型的子类型（三层 1，n，n 方式）

缺点：每个命令都需要用一个类来描述，又会根据带参数的不同生成更多的子类，若有 n 个命令，每个命令有0-2个参数，连同命令基类和带参子类一共平均（假设 n 个命令各有1个参数）需要 2n 个类，算上所有命令的基类是一个三层的集成，其虚指针转换代价集中在命令基类->具体的命令类->具体的带参数命令类。

优点：更容易组织代码结构，更容易理解的结构，使不同的命令和命令的不同形式分隔开。

- 根据解析的结果通过命令中的参数进行筛选执行（双层 1，n 方式）

缺点：每个不同的命令类需要保存一个参数表，根据参数表提供不同的附加操作，虚指针转换在命令基类->具体的命令类，然后对参数表有一个查找的过程，此过程或许可以通过建立哈希表压缩为O(1)的代价。

优点：减少虚指针转换的代价，命令的不带参数和带参数可以通过代码语义进行分割，单独维护带参数和不带参数的部分，通过返回函数对象来添加带参数部分的操作。

- 建立不同命令的哈希表，在命令的哈希表中再根据命令的参数建立哈希表（双层 1，1 方式）

缺点：可能会破坏设计模式，哈希表较难设计。

优点：命令基类完全当做一个调用哈希函数的控制点，跳过了虚指针转换的过程。如果不考虑空间代价，可以直接在初始化时生成命令对象，在解析后直接使用命令对象即可，将命令对象的生成时间代价移动到编译期。

## 多线程的应用？

因为是模仿 dos 进行虚拟磁盘的操作，那么多线程不可能应用在响应用户命令上（dos 本身就是单线程处理），只可能应用在后台整理磁盘空间碎片上。即：
- 主线程
用于处理用户的指令操作
- 后台线程
用于在适当的时候整理磁盘空间碎片

另，命令执行模块若采用双层 1，n 的设计方式，那么可以采用多线程在初始化过程中，一个线程对虚拟磁盘的空间进行分配和分区产生磁盘信息，另一个线程产生命令对象。

## 序列化与反序列化

复杂对象的序列化，实际序列化的部分是 Disk Module，而 Execution Module 和 Dos Module 无需序列化。

Disk Module 序列化与反序列化的必要数据是什么？

关于复杂对象序列化与反序列化的问题。

使用第三方库进行序列化？
- Google Proteocol Buffers
- Boost.Serialization

自己实现简单的序列化过程？
- xml 存储，使用 tinyxml2
- 序列化/反序列化接口

## B- 树还是 B+ 树还是其他？

因为是模拟磁盘，所有没有磁盘与内存交互的IO效率等等的问题，所以不一定用得到B树。那么该使用何种数据结构来描述磁盘以提高查询效率？

备选数据结构

- 哈希表
首推哈希表，但是哈希表的哈希函数设计很复杂，没法直接建立键址一一对应的关系。

- rb-tree
直接使用 std::map，std::map 底层使用了红黑树，文件必须以关键字排序，而且不能重复，那么关键字必须是绝对路径，由此会增加关键字匹配的时间代价，以及关键字排序的时间代价，不过可以考虑将关键字的排序（调整树形）放到后台线程中进行，那么这里就无法使用 std::map，需要自己实现一个双线程版本的 rb-tree。

- AVL-tree
同上。

- n 叉树
极端情况下会直接变成线性查找效率（变成深度为 m 的1叉树或深度为1的 n 叉树），而且无关键字排序？但是相对于平衡二叉排序树，没有了排序结点调整树形的过程。适用于非查找操作，如 dir 的递归遍历，rd 的递归删除。但是对于带路径的操作，则会在路径中的每一个目录内有线性时间的查找代价。

# 关键算法

特定算法依赖于数据结构，因为这里底层的实现本质上是一个内存池，那么对于线性排布的地址首选哈希表数据结构。

## 空间管理（虚拟磁盘的空间）

- 内存分配算法（提供需求大小，返回分配指针，或者用其他数据结构（哈希表，红黑树）保存所有空闲空间指针）
- 内存回收算法
- 碎片整理算法

## 泛型哈希表

- 模板特化 Command 类哈希表数据结构
- 模板特化 Command 类哈希函数

> 注：《STL 源码剖析》中提到 STL 中有一个不对外开放的非 STL 标准的 hash_table 容器，底层实现是哈希表，可以考虑使用。

> 补充：直接使用 C++11 提供的 unordered_map，其底层实现是 hash table，其前身就是 boost::hash_map。

## n 叉树

- 数据结构
- 相关算法

## 命令解析

- 字符串解析算法

## 自定义序列化

- 序列化接口
- 序列化实现
- 反序列化实现

# 关键算法实现

- n 叉树
    - 泛型 n 叉树数据结构
    - 泛型 n 叉树相关算法
- 命令解析
    - 基本情况实现解析
    - 容错性处理
- 内存池 // IDEA: consider to deprecate
    - 内存池单例类
    - 一般内存池分配大小
    - C++ 类型擦除技术
        - boost::any 类实现
    - 内存分配算法
- 内存分配
    - 使用 malloc 和 free

# 关键类实现

- Command 类
- Printer 类
    - CommandDIRPrinter 类
    - CommandCDPrinter 类
- DiskSystem 类
- DiskPartition 类
- VDos 类

# 命令实现

- mkdir
各种使用形式：
```
mkdir folder // 在当前目录（通过 DiskSystem 获得）下创建 folder 文件夹
mkdir .\folder // 同上
mkdir ..\folder // 在上一级目录（通过 DiskSystem 获得当前目录然后计算上一级目录）下创建 folder 文件夹
mkdir \folder // 在当前盘符（通过 DiskSystem 获得）下创建 folder 文件夹
mkdir folder\folder // 在当前目录（通过 DiskSystem 获得）下的 folder 文件夹（若没有，则创建）中创建 folder 文件夹
```

# 程序栈分析

|Step|Function|Note|
|---|
|0|main()|-|
|0-1|VDos::VDos()|-|
|0-2|Command::Command()|-|
|1|VDos::run()|std::string|-|
|1-1|StringParser::parseString()|-|
|1-2|StringParser::parseResult()|for test|
|1-3|Command::getExecutor()|-|
|1-4|@command()|-|
|1-5|StringParser::clear()||  

# 第一次重构

## 重构内容

- Command
- StringParser 类
- NTree 类

## 重构原因

### StringParser 与 Command

StringParser 类只是简单的解析出了以下三部分：
- 命令
- 参数表
- 操作对象表

而在实现 command 具体命令的时候发现仅仅这三部分不够，所以在原语法中添加新的非终结符，并且将 StringParser 类重构，使之解析结果更加细致化

StringParser 类的**职责不够集中**，有部分职责分散给了具体 command 的实现

### NTree

NTree 类原本将本身的值信息、文件信息，结点信息无抽象层次地混合为同一层次的内容。

NTree 原数据如下：
```
std::shared_ptr<T> m_treeInfo = nullptr;
NTree<T>* m_parent = nullptr;
std::vector<std::shared_ptr<T>> m_treeValue;
std::vector<std::shared_ptr<NTree>> m_children;
```

- treeInfo 表示当前结点信息，比如该结点是文件夹或某一文件
- m_parent 裸指针保存父节点，不用 shared_ptr 是因为子节点用了 shared_ptr 会造成**循环引用**
- m_treeValue 保存当前目录的文件，若当前结点是一文件，则为空
- m_children 保存所有子目录

可以看到保存的内容**抽象层次不一致**，《代码大全2》中提出一点：保持一致的抽象层次。所以进行重构：
- 暂时去掉 smart_ptr 的使用，在需要使用树的地方使用，对于结点操作全部使用裸指针
- 保持一致的抽象层次，即所有文件、文件夹都以 File 类的结点表示，结点信息则是对应 File 对象的 fileName
```
T m_value;
NTree<T>* m_parent;
std::map<std::string, NTree<T>*> m_children;
```

- m_value 保存当前结点的值，在此项目中是 File 类对象
- m_parent 用裸指针保存父节点
- m_children 考虑到当前结点是文件夹的情况，则会有文件夹内的排序（dir 命令输出的内容是按名称排序的）和查找（使用关键字向上或向下查找结点），使用 RB-tree 或 AVL-tree 的效率会好一点
