
#   Merdog
Merdog 是一个简单的开源解释器C-Like 语言语法

![](https://img.shields.io/badge/C--Like-interpreter-brightgreen)
![](https://img.shields.io/github/license/HttoHu/Merdog)
![](https://img.shields.io/github/repo-size/HttoHu/Merdog)
![](https://img.shields.io/badge/interpreter-%3C1MB-lightgrey)
## 支持的语法
1. 基本类型 **int, real, string, bool，char**
2. 使用new分配动态对象，**有基于std::shared_ptr的RC**
3. 基本语句 **if / else / elif / while / do while / for / switch**
4. 支持函数的声明和定义
5. 支持 struct，且支持在struct里定义成员函数
6. 支持数组，vector, set,map, deque
7. 内置基本的IO库和数学函数库（std函数的封装）和一些实用函数
```c++
function int sum (int x, int y)
{
   return x+y;
}
program main
{
  std.cout(sum(1,2));
}
```

## 关于
* 使用Mircosoft Visual Studio 2017/2019 和 Clang 编译通过并成功运行.
* 网站: www.merdog.cn
* 联系 **huyuantao@outlook.com**
* 参考
  * 龙书: 编译原理（第二版）
  * [How to Build an interpreter](https://ruslanspivak.com/lsbasi-part1/)**

## License
Copyright (c) **HttoHu(胡远韬)**. All rights reserved.

Licensed under the [MIT](LICENSE) license.
