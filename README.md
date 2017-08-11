[TOC]

---

# Virtual Disk

- ���뻷����VS 2015

- ���뷽ʽ��ʹ�� VS 2015 �� VirtualDisk.sln���������

# Ŀ¼�ṹ

## Model

### FileModel

- File ��
- GeneralFile ��
- DirectoryFile ��
- SymbolGeneralFile ��
- SymbolDirectoryFile ��

### DataModel

- Msg ��
- Object ��
- Path ��

## View

- ErrorMessage
- VDos ��

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

# ����˵��

- ���˵��

- ��������б�ܳ�ͻ

����֧������б�ܣ�����ʹ�ò���/argument������±�����·����ʹ��\�������ʹ����ʽ�磺
```
C:\mklink /D .\EL E1
```
����ʹ��\����Ὣ/D������·��

��·���п��Ի�������б��

- ֧��ͨ���

# ֧��ָ��

- dir ����Ŀ¼
    - ֧�ֲ���
        - ad ����ʾ�ļ���
        - /s �ݹ����
    - ������ʽ
```
dir [/arg] [obj]
```

- md ����Ŀ¼
    - ������ʽ
```
md obj [obj]
```

- rd ɾ��Ŀ¼
    - ֧�ֲ���
        - /s ǿ��ɾ��
    - ������ʽ
```
rd [/arg] obj [obj]
```

- cd �л�Ŀ¼
    - ������ʽ
```
cd [obj]
```

- del ɾ���ļ�
    - ֧�ֲ���
        - /s �ݹ�ɾ��
    - ������ʽ
```
del [/arg] obj [obj]
```

- copy �����ļ����򿽱�����������ļ������������
    - ������ʽ
```
copy obj1 obj2
copy @obj obj2
```

- ren ������
    - ������ʽ
```
ren obj1 obj2
```

- move �ƶ��ļ�
    - ֧�ֲ���
        - /y ͬ������
    - ������ʽ
```
move [/arg] obj1 obj2
```

- mklink ��������
    - ֧�ֲ���
        - /D Ϊ�ļ��д�������
    - ������ʽ
```
mklink link obj
mklink /D linkd obj
```

- save ���л�������̵���������ļ�
    - ������ʽ
```
save @obj
```

- load ����������ļ��з����л��������
    - ������ʽ
```
load @obj
```

- cls ����
    - ������ʽ
```
cls
```
