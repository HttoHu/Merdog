## OUTLINE

* Merdog is an C-language-Like grammar language. You can run the merdog program by the merdog interpreter published on Github and www.merdog.cn
  
* advantages: 
  
    * portable (less than 500 KiB on Windows)
    * easy to learn
    * easy to extend. You can packing the core code of your program to merdog which enables you to hot upgrade and distinct between core code and transection code.
    * MIT LISECE enables you to use merdog freely, it is commercial friendly.
* disadvantage

    * the performance is still at a low level
    * the grammar is not as powerful as other mature languages.
  
## How To Run

1. download merdog interpreter
2. open Terminal and open merdog with following argument
3. -v : show the version of merdog
4. file_name : the merdog program you want to run.
5. Under Windows, you can open the merdog program file(.mer) by merdog.exe 

## Hello World Example
```cpp
program main
{
    std.cout("Hello world");
}
```
### Output Function 
* std.cout is an output function which print the arguments to the screen. In the example above, the argument is a string. string is a basic type which you can regard it as a Text, so when you call the std.cout , and pass a "Hello world" to it, the content of "Hello world" will print into the sceen. Also, you can pass integer as 123, or real number(fraction) 2.34 , and a varitable name to show the value of the variable. 
* the name of std.cout derived from std::cout, meaing standard console output. 

### key word program 
* "program" function as the entry of the program, every program execute from the entry. And every entry have a name which can be define by any legal word. 
* A code block is braced by { }. Blocks consist of one or more declarations and statements. In program block, you can add the sentances you want to execute. 