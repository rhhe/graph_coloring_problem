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
