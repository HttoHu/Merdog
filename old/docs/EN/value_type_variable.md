## Create a variable 

Look at this table

|typename| value type| literal instance|
|-|-|-|
|int|integer| 1|
|real| decimal| 1.23|
|bool| boolean | true or false|
|char| character|'A'|
|string| a string of characters| "Hello World"|

Every literal value has a type, the type of 1 is int, "Great" is string. All above is const literal value which means we can know its value merely by its representation.

To create a variable we should clarify the type and name , value of what you need to define
for example

```cpp
int x=2;
```

we defined the x as int variable and initialize it with a literal const value 1, whose type is int as well.

here are some more examples
```cpp
real a=2.34; // define a decimal number
int b=234;  // define a integer number
char ch='A'; // define a character
string str="Hi"; // define a string
bool condition=true;// define a boolean variable 
b=345*34; // we can change the variable as we want 
a=2*b+3; // the value of the varaible can convert to other types
```

Note that:

* The value used to initialize should accord with the type of the variable (real a=1); is illegal and the right answer is: (real a=1.0;)
  
* ; is the end token of every sentence, if you forget write it, the interpreter will complain and spit out an error.

* we get the value of variable by its name, if we defined two same-name variable, the variable will be ambiguous even if their type are same and the interpreter will remind you of this fatal error. 