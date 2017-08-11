[TOC]

---

# Virtual Disk

- 编译环境：VS 2015

- 编译方式：使用 VS 2015 打开 VirtualDisk.sln，编译调试

# 目录结构

## Model

### FileModel

- File 类
- GeneralFile 类
- DirectoryFile 类
- SymbolGeneralFile 类
- SymbolDirectoryFile 类

### DataModel

- Msg 类
- Object 类
- Path 类

## View

- ErrorMessage
- VDos 类

## Controller

- CommandFactory
- DiskSystem
- ObjectGenerator
- StringParser
- StringParserCmd
- StringParserCmd0
- StringParserCmd1
- StringParserCmd2
- StringParserCmdN

## Others

- tinyxml2

---

# 程序说明

- 相关说明

- 关于正反斜杠冲突

由于支持正反斜杠，所以使用参数/argument的情况下必须在路径中使用\，最基本使用形式如：
```
C:\mklink /D .\EL E1
```
若不使用\，则会将/D解析成路径

纯路径中可以混用正反斜杠

- 支持通配符

# 支持指令

- dir 遍历目录
    - 支持参数
        - ad 仅显示文件夹
        - /s 递归遍历
    - 操作格式
```
dir [/arg] [obj]
```

- md 创建目录
    - 操作格式
```
md obj [obj]
```

- rd 删除目录
    - 支持参数
        - /s 强制删除
    - 操作格式
```
rd [/arg] obj [obj]
```

- cd 切换目录
    - 操作格式
```
cd [obj]
```

- del 删除文件
    - 支持参数
        - /s 递归删除
    - 操作格式
```
del [/arg] obj [obj]
```

- copy 拷贝文件，或拷贝以物理磁盘文件到虚拟磁盘中
    - 操作格式
```
copy obj1 obj2
copy @obj obj2
```

- ren 重命名
    - 操作格式
```
ren obj1 obj2
```

- move 移动文件
    - 支持参数
        - /y 同名覆盖
    - 操作格式
```
move [/arg] obj1 obj2
```

- mklink 创建链接
    - 支持参数
        - /D 为文件夹创建链接
    - 操作格式
```
mklink link obj
mklink /D linkd obj
```

- save 序列化虚拟磁盘到物理磁盘文件
    - 操作格式
```
save @obj
```

- load 从物理磁盘文件中反序列化虚拟磁盘
    - 操作格式
```
load @obj
```

- cls 清屏
    - 操作格式
```
cls
```
