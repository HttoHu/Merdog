   Merdog
## 简介
Merdog 是一个简单的开源解释器，（重写自Simcc** 尚未完成**， 可以尝试一下simcc目录下的一个解释器，需要在Microsoft CR 上编译，在clang上编译后出现了一些问题.Simcc 因为内存泄露，性能低下，代码不规范已停止更新，你可以在 www.github.com\HttoHu\Simcc 去下载完整代码)
## 支持的语法
1. 基本类型 ** int, double, string, bool
2. 支持引用ref
3. 基本语句 if / else / elif / while / for
```c++
function int sum (int x, int y)
{
  return x+y;
}
pragram main
{
   cout(sum(1,2));
}.
```

## 有关编译器
我使用的是Microsoft Visual Studio 2017 + Clang 开发的，还没有在Linux/Mac OSX下进行编译。

## 其他
* **部分算法来(表达式翻译,和一些语法分析)自Ruslan 的博客 https://ruslanspivak.com/lsbasi-part1/ 其他全靠想象力....**
* **作者正忙于复习会考，更新可能不那么频繁，见谅**
* 邮箱 [Email]huyuantao@outlook.com
