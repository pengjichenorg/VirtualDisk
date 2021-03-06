[TOC]

---

# 2017-7-20
- 分析项目内容
    - 项目概述
    - 技术需求
    - 分析
        - Windows API
        - dos 命令模型
        - 编译原理
        - 内存管理
        - 磁盘存储模型
        - 序列化技术
- 设计
    - 数据结构设计
    - UML 设计
    - 备选设计模式

# 2017-7-21
- 分析
    - 更正 mklink 命令中 /d 参数的含义
    - 更正文件跨越分区的认知错误
    - 更正命令语法，终结符中添加操作对象
    - 去掉多线程问题
- 设计
    - 更正文件跨盘设计
    - 领域类提出
    - 备选设计模式
    - UML 图
        - 策略模式类图
        - 外观模式类图
        - 交互流程时序图
        - 程序流程流程图
    - 补充说明
    - 模块化设计
- 待解决问题
    - 命令的不带参数形式和带参数形式之间的组合方式？
    - 多线程的应用？
    - 序列化与反序列化
    - B- 树还是 B+ 树还是其他？
- 关键算法
    - 内存池
    - 泛型哈希表
    - n 叉树
    - 命令解析
    - 自定义序列化

# 2017-7-22
- 关键算法实现
    - n 叉树
    - 命令解析
- 分析
    - 更正命令语法，终结符中删去操作路径

# 2017-7-23
- 设计
    - 删去 windows API 的使用方案
- 关键算法实现
    - 内存池
    - n 叉树添加反向遍历测试
    - 放弃泛型哈希表，直接使用 C++11 unordered_map
- 分析
    - 补充内存池中类型擦除技术
    - 模仿 boost::any 类的实现
- 关键类实现
    - Printer 类
    - CommandDIRPrinter 类
    - CommandCDPrinter 类

# 2017-7-24
- 设计
    - 修改文件 ADT
    - 添加 VDos 类
    - 修改 Facade Pattern UML 图
    - 修改 Analyzer 类为 StringParser 类
    - 修改 cls 设计
    - 语法中添加非终结符：操作路径
    - 重建语法规则
- 关键算法实现
    - 放弃内存池技术
    - 使用 malloc 和 free
- 关键类实现
    - File 类
- 命令实现
    - cls
- 程序栈分析
- 第一次重构
    - StringParser 类
    - Command
    - NTree 类

# 2017-7-25
- 第一次重构
    - DiskSystem
    - DiskPartition
- 命令实现
    - cd
    - mkdir
    - 自定义路径加法

# 2017-7-26
- 命令实现
    - 完善 mkdir 命令
    - 完善 cd 命令
    - 添加并实现 touch 命令创建文件
    - 实现 rd 命令
    - 实现 del 命令无通配符版本
- 第二次重构
    - NTree 类
    - File 类
    - 泛型自定义路径加法
    - Command
    - StringParser 类
- 重构命令实现
    - mkdir（测试通过）
    - dir 无参数、无通配符
