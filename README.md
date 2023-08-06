[TOC]

# graph_coloring_problem 图着色问题

## 1. dataset 数据集

（1）数据集下载地址:
[https://mat.tepper.cmu.edu/COLOR/instances.html](https://mat.tepper.cmu.edu/COLOR/instances.html)

（2）如果要使用该数据集，需要下载数据预处理文件。 在上述链接中找到并下载 translator 链接中的文件`binformat.shar`。

（3）运行`/bin/sh binformat.shar`生成`.c`文件。

（4）将这些`.c`文件放入`data_process`目录。并在其中运行`make`，会发现其中缺乏`#include<stdlib.h>` 和`#include <string.h>`。补充上述头文件。

（5）在cmakelist增加
```cmake
add_executable(asc2bin data_process/asc2bin.c)
add_executable(bin2asc data_process/bin2asc.c)
add_executable(showpreamble data_process/showpreamble.c)
```

（6）build即可生成如下可执行文件
```bash
cmake-build-release/asc2bin
cmake-build-release/bin2asc
cmake-build-release/showpreamble
```

（7）例子，如果已经下载了数据集
`DSJC500.5.col.b`，则运行
```bash
% cmake-build-release/showpreamble DSJC500.5.col.b
```
输出如下：
```bash
c FILE: DSJC500.5
c
c SOURCE: David Johnson (dsj@research.att.com)
c
c DESCRIPTION: Random graph used in the paper
c              "Optimization by Simulated Annealing: An
c               Experimental Evaluation; Part II, Graph
c               Coloring and Number Partitioning" by
c              David S. Johnson, Cecilia R. Aragon, 
c              Lyle A. McGeoch and Catherine Schevon
c              Operations Research, 39, 378-406 (1991)
c
p edge 500 125248
```

把二进制文件转化为asc格式文件，如下命令
```bash
cmake-build-release/bin2asc DSJC500.5.col.b
```
得到文件`DSJC500.5.col`，文件内容如下
```bash
c FILE: DSJC500.5
c
c SOURCE: David Johnson (dsj@research.att.com)
c
c DESCRIPTION: Random graph used in the paper
c              "Optimization by Simulated Annealing: An
c               Experimental Evaluation; Part II, Graph
c               Coloring and Number Partitioning" by
c              David S. Johnson, Cecilia R. Aragon, 
c              Lyle A. McGeoch and Catherine Schevon
c              Operations Research, 39, 378-406 (1991)
c
p edge 500 125248
e 2 1
e 3 1
e 4 2
...
e 500 493
e 500 497

```

## 2. 数据格式说明

在数据集页面的`DIMACS standard forma`链接，下载数据格式说明文件，这个文件也放在了doc目录下。引用部分格式说明原文如下：

### 2.1 输入格式

输入文件包含定义团问题或着色问题所需的有关图形的所有信息。 可能包含一些与某个问题无关的信息（例如，不需要节点权重着色问题），因此信息可能会被忽略。 在此格式中，节点编号从 1 到 n。 图中有 m 条边。 假定文件格式正确且内部一致：节点标识符值有效，节点已定义唯一地，恰好定义了 m 个边，等等。 将提供输入检查器以确保与这个标准。

- **注释。** 注释行提供有关文件的人类可读信息，并且会被程序忽略。 评论行可以出现在文件中的任何位置 每个注释行都以小写字符 c 开头。
   ````bash
   c This is an example of a comment line
   ````

- **问题行。** 预输入文件中有一个问题行。 问题行必须出现在任何节点或弧描述符行之前。 对于网络实例，问题行具有以下格式。
   ````bash
   p FORMAT NODES EDGES
   ````
  小写字符 p 表示这是有问题的行。 FORMAT 字段是为了与之前的挑战保持一致，并且应包含单词“edge”。 NODES 字段包含一个整数值，指定 n（图中的节点数）。 EDGES 字段包含一个整数值，指定 m（图中的边数）。(实际的情况是，对于一个无向的边，在图中被统计了两次，至少在数据文件500.5中是这样的。)

- **边描述符。** 图形的每条边都有一个边描述符行，每条边描述符行都具有以下格式。 每条边 (v, w) 在输入文件中只出现一次，并且不会重复 (w, v)。
   ````bash
   e W V
   ````
  小写字符 e 表示这是一条边缘描述符线。 对于边 (w, v)，字段 W 和 V 指定其端点。

### 2.2 输出格式

每个算法或启发式都应该创建一个输出文件。 该输出文件应包含以下一行或多行，具体取决于算法的类型和要解决的问题。

- **解行**

  ```bash
   s TYPE SOLUTION
  ```
  小写字符 s 表示这是一条解行。 TYPE 字段表示文件中包含的解决方案的类型。 这应该是以下字符串之一：“col”表示图形着色，“clq”表示最大加权团，“cqu”表示最大未加权团（忽略了 n 个描述符行）。 SOLUTION 字段包含与解值相对应的整数。 这是未加权团的团大小、加权团的团值或用于图形着色的颜色数量。

- **界行**

  ```bash
   b BOUND
  ```
  小写字符 b 表示这是解的界限。 BOUND 字段包含一个整数值，该值给出了解值的界限。 该界限是上限和下限，上限是团和加权团的最大团值，下限是对图着色所需的颜色数量。

- **标签行**

  ```bash
   l V N
  ```
  小写字母l表示这是一条标签线，一般用于图形着色。 V 字段给出了团中节点的节点号，而 N 字段给出了相应的标签。 图中的每个节点都有一条标签线。
