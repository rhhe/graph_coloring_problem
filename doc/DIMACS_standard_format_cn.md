# 团和图着色问题格式

*最后修订日期：1993年5月8日*

本文概述了建议的图表格式。 如果您对此格式或其他格式有意见，或者您有信息认为应该包括在内，请发送邮件至challenge@dimacs.rutgers.edu

## 1.简介

DIMACS 挑战的目的是通过提供实例和分析工具的通用测试平台来减轻测试和比较算法和启发式方法所需的工作量。 为了促进这一工作，必须为所解决的问题选择标准格式。 本文档概述了一种图表格式，适合那些查看图表着色和查找图表中的派系的人。 这种格式是一种灵活的格式，适用于多种类型的图和网络问题。 该格式也是第一届网络流和匹配计算挑战赛所选择的格式。

本文档描述了三个问题：未加权团、加权团和图形着色。 为了满足要求，使用了单独的格式。

## 2. 图问题的文件格式

本节描述图形输入和输出的标准文件格式。 对参加者没有要求遵循这些规范； 然而，兼容的实现将能够充分利用 DIMACS 支持工具。 （某些工具假设输出附加到单个文件中的输入。）

欢迎参与者开发翻译程序，以更方便或更方便地将实例转换为实例紧凑表述； 建议使用 Unis **awk** 工具，因为它特别适合此任务。

所有文件都包含 ASCII 字符。 输入和输出文件包含多种类型的*行*，如下所述。 一条线是以行尾字符终止。 每行中的字段至少由一个空格分隔。 每行
以单字符指示符开头，用于标识线路类型。

### 2.1 输入文件

输入文件包含定义团问题或着色问题所需的有关图形的所有信息。 可能包含一些与某个问题无关的信息（例如，不需要节点权重着色问题），因此信息可能会被忽略。 在此格式中，节点编号从 1 到 n。 图中有 m 条边。 假定文件格式正确且内部一致：节点标识符值有效，节点已定义唯一地，恰好定义了 m 个边，等等。 将提供输入检查器以确保与这个标准。

- **注释。** 注释行提供有关文件的人类可读信息，并且会被程序忽略。 评论行可以出现在文件中的任何位置 每个注释行都以小写字符 c 开头。
   ````bash
   c This is an example of a comment line
   ````

- **问题行。** 预输入文件中有一个问题行。 问题行必须出现在任何节点或弧描述符行之前。 对于网络实例，问题行具有以下格式。
   ````bash
   p FORMAT NODES EDGES
   ````
  小写字符 p 表示这是有问题的行。 FORMAT 字段是为了与之前的挑战保持一致，并且应包含单词“edge”。 NODES 字段包含一个整数值，指定 n（图中的节点数）。 EDGES 字段包含一个整数值，指定 m（图中的边数）。

- **点的描述。** 对于本次挑战赛，仅加权团问题需要节点描述符。 这些行将给出分配给团中节点的权重。 每个节点都有一个节点描述符行，其格式如下。 没有描述符的节点将采用默认值 1。
   ````bash
   n ID VALUE
   ````
  小写字符 n 表示这是节点描述符行。 ID 字段给出节点标识号，即 1 到 n 之间的整数。 VALUE 给出了在团中拥有此节点的目标值。 该值被假定为整数并且可以是正数或负数（或零）。

- **边描述符。** 图形的每条边都有一个边描述符行，每条边描述符行都具有以下格式。 每条边 (v, m) 在输入文件中只出现一次，并且不会重复 (w, v)。
   ````bash
   e W V
   ````
  小写字符 e 表示这是一条边缘描述符线。 对于边 (w, v)，字段 W 和 V 指定其端点。

- **可选描述符。** 除了必需的信息之外，还可以有有关图表的其他信息。 这通常会定义用于生成图表的参数或以其他方式定义特定于生成器的信息。 当有趣的问题生成器被决定时，可以添加以下列表：
    - 几何描述符。 生成或显示图的一种常见方法是将节点嵌入到某个空间中，并根据节点之间距离的某个函数（根据某种度量）将边包括在内。 节点信息可以由维度描述符和顶点嵌入描述符来定义。
     ````bash
     d DIM METRIC
     ````
  是维度描述符。 DIM 是给出空间维数的整数，而 METRIC 是表示空间度量的字符串。 METRIC 是一个可以采用多种形式的字符串。 Lp（即 L1、L2、L122 等）表示 l_p 范数，其中嵌入在 (x_1, x_2, ..., x_d) 和 (y_1, y_2, ..., y_d) 处的两个节点之间的距离为 ( \sum_{i=1}^{d} |x_i - y_i|^p)^{1/p}。 字符串 LINF 用于表示 l_{infinite} 范数。 L2S 表示平方欧几里得范数（它不太容易受到舍入和准确性问题方面的计算机差异的影响。）
     ````bash
     v X1 X2 X3 ... XD
     ````
  小写字符 v 表示这是一个顶点嵌入描述符行。 字段 X1、X2、...、XD 给出顶点的 d 坐标值。 请注意，这些行必须出现在 d 描述符之后。

    - 参数描述符。 参数描述符用于提供有关如何生成图的其他信息。 这些行是特定于生成器的，因此预计大多数代码不会使用其中的大部分（或任何）行。 包含它们只是为了帮助那些专门设计用于解决特殊结构问题的代码。 参数描述符的一般形式为：
      ````bash
      x PARAM VALUE
      ````
      小写字符 x 表示这是一个 paramet4er 描述符行。 PARAM 字段是给出参数名称的字符串，而 VALUE 字段是给出相应值的数值。 已定义以下 PARAM 值：

      | 参数 | 描述 |
      | ---- | ---- |
      | MINLENGTH | （几何图形）仅当长度大于或等于 VALUE 时才包含边 |
      | MAXLENGTH | （几何图形）仅当长度小于或等于 VALUE 时才包含边 |

      请注意，此信息是除了所需的边缘描述符之外的信息。

### 2.2 输出文件

每个算法或启发式都应该创建一个输出文件。 该输出文件应包含以下一行或多行，具体取决于算法的类型和要解决的问题。

- **解行**
   
   ````bash
   s TYPE SOLUTION
  ````
小写字符 s 表示这是一条解行。 TYPE 字段表示文件中包含的解决方案的类型。 这应该是以下字符串之一：“col”表示图形着色，“clq”表示最大加权团，“cqu”表示最大未加权团（忽略了 n 个描述符行）。 SOLUTION 字段包含与解值相对应的整数。 这是未加权团的团大小、加权团的团值或用于图形着色的颜色数量。
   
- **界行**
   
   ````bash
   b BOUND
  ````
小写字符 b 表示这是解的界限。 BOUND 字段包含一个整数值，该值给出了解值的界限。 该界限是上限和下限，上限是团和加权团的最大团值，下限是对图着色所需的颜色数量。
   
- **团顶点行**
   
   ````bash
   v V
  ````
小写字母 v 表示这是一条团顶点线。 V 字段给出了团中节点的节点号。 派系中的每个节点都会有一条派系线。
   
- **标签行**
   
   ````bash
   l V N
  ````
  小写字母l表示这是一条标签线，一般用于图形着色。 V 字段给出了团中节点的节点号，而 N 字段给出了相应的标签。 图中的每个节点都有一条标签线。