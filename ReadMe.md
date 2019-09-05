
#   Merdog
## 简介
Merdog 是一个简单的开源解释器C-Like 语言语法

![](https://img.shields.io/badge/C--Like-interpreter-brightgreen)
![](https://img.shields.io/badge/merdog-easy%20to%20use-orange)
[![](https://img.shields.io/github/languages/top/HttoHu/Merdog)](http://www.cplusplus.com/)  

## Merdog 2.1 支持的语法
1. 基本类型 **int, real, string, bool**
2. 基本语句 if / else / elif / while / for
3. 支持函数的声明和定义
4. 支持复合变量 struct
5. 支持数组
6. 内置基本的IO库和数学函数库（std函数的封装）
```c++
function int sum (int x, int y)
{
   return x+y;
}
program main
{
  std.cout(sum(1,2));
}.
```

## 有关编译器
使用Mircosoft Visual Studio 2017/2019 和 Clang 编译通过并成功运行.

## 其他
* 以 编译原理 为参考书
* **部分算法来(表达式翻译,和一些语法分析)自Ruslan 的博客 https://ruslanspivak.com/lsbasi-part1/**
* 邮箱 [Email]huyuantao@outlook.com
