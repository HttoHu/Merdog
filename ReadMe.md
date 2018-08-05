
#   Merdog
## 作者忙于2019年高考中, 现在暂停开发了, 目前功能缺失仍然十分严重.
## 简介
Merdog 是一个简单的开源解释器 前身Simcc
## Merdog 2.1 支持的语法
1. 基本类型 ** int, real, string, bool
2. 基本语句 if / else / elif / while / for
3. 支持命名空间
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
使用Mircosoft 2017 和 Clang 编译通过并成功运行.

## 其他
* 以 编译原理 为参考书 
* **部分算法来(表达式翻译,和一些语法分析)自Ruslan 的博客 https://ruslanspivak.com/lsbasi-part1/ 其他全靠想象力....**
* 邮箱 [Email]huyuantao@outlook.com
