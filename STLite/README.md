# STLite

实现与C++标准库类似的数据结构，包括迭代器等。

在相关.hpp文件中只使用了`cstdio`，`cstring`，`iostream`，`cmath`四个C++标准库，代码中的`exceptions.hpp`和`utility.hpp`两个辅助文件用于支持异常处理，以及提供`sjtu::pair`完成STLite的实现。

本项目提供的容器在接口和复杂度要求上与STL基本一致，除了：

#### 与STL的不同之处

- `sjtu::priority_queue`要求以最高$O(\log n)$的时间复杂度实现合并操作。
- `sjtu::map`的实现使用平衡化（可使用AVL树、红黑树、AA树等）完成。

本项目所有容器检查了内存泄漏。