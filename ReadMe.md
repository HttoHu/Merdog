
#   Merdog
Merdog is a mini C-Like interpreter

![](https://img.shields.io/badge/C--Like-interpreter-brightgreen)
![](https://img.shields.io/github/license/HttoHu/Merdog)
![](https://img.shields.io/github/repo-size/HttoHu/Merdog)
![](https://img.shields.io/badge/interpreter-%3C1MB-lightgrey)

## Other languages
* [简体中文](https://github.com/HttoHu/Merdog/blob/master/docs/ReadMe_CN.md)
## Grammar
1. basic types **int, real, string, bool，char**
2. allocate heap objects by key word new **based on the std::shared_ptr**
3. basic control statement **if / else / elif / while / do while / for / switch**
4. support the declaration and defination of a function
5. support defining struct, and member function
6. support array and *(The feature will be added after v3.1.9)some STL containers (vector, deque, map,set,multiset,multimap)*
7. some basic and useful functions
  
## exmplaes:
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

## About
* Compilers: passing by Clang 9.0.0 | Visual Studio 2019
* Website: www.merdog.cn
* Email **huyuantao@outlook.com**
* inspired by:
  
  * *Compilers: Principle Techniques and Tools Second Edition*
  * *[How to Build an interpreter](https://ruslanspivak.com/lsbasi-part1/)*

## License
Copyright (c) **HttoHu**. All rights reserved.

Licensed under the [MIT](LICENSE) license.
