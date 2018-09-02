# stak (not an acronym , just a different way of saying stack)
Simple but complete stack based programming language, 100% written in c++

Interpreter is just a [main.cpp](/stak/main.cpp) file with ~ 300 lines of VERY commented code.

Has variables, strings, numbers, comments, math, logic operations, jumps, conditional jumps, console input and output, code injection.

The jumps can be used to form loops and functions.

Its interpreted (interpreter in c++), but can be compiled (will probs make a compiler in future).

Language is optimized for simplicity of writing an interpreter for it,
so the interpreter is quite short & simple and very easy to change / add features to.

You make all .stak files open with stak/debug/stak.exe so you can run them.

No error handling, so expect wierd errors if your stak code is wrong

## Examples

If you want a script that covers most below examples, look here: [script.stak](/script.stak)

![script](https://user-images.githubusercontent.com/41348897/44952387-4f0a7d80-ae7e-11e8-9843-1ed5da09d780.png)




The program closes after being executed, so add a "stop" at end to make it not close.

push string literal "Hello world" to the stack and then print.
~~~
"Hello world" print
~~~

push number literal 3.14 to stack then print.
~~~
3.14 print
~~~

print and printl print the last thing on the stack. printl adds a newline at end.

push name "a" to stack, then push string "variable!", then push function '=' which sets value to the name, declaring the variable a as string "variable!"
~~~
a "variable!" =
~~~

push variable a to stack then print its value
~~~
a print
~~~

comments
~~~
; this is a comment, comments start with ";"
a print ; a will print fine
~~~

math and logic operations. These push the resulting value to the stack
~~~
a 3 =
b 4 =

a b +
; you can use + - / * % > < == ! instead of +
; == , ! also work on strings, others dont.
~~~

"stacking" operations
~~~
3 2 + print ; pushes 3 then 2 to stack, then does + , then prints

; output is 5

a 1 =
b 3 =

c a b + = ; push c, then "a b +" (4) then =, resulting in c 4 =

c c 1 - = ; c | c 1 - | = , resulting in c 3 = (c = c-1; in normal programming)

~~~

input,inputraw,inject
~~~
a input = ; sets whatever string user inputs into variable a. input wraps the input with quotes
b inputraw = ; inputraw doesnt wrap with quotes, so you can also enter numbers or a name of a different variable
c inject = ; inject pastes code. So you can do like "3 4 *" as input and it will result in c 12 =
~~~

jumps
~~~
;start
"." print
start jmp
; will print dots forever
~~~

if
~~~
"input: " print
a inputraw =

a 1 ==
"result1 jmp"
if

;if a != 0 then jump to result, else continue (resulting in 0)

"result 0" print
stop

;result
"result 1" print
stop
~~~

ifjmp If there is a 1 or 0 (string or number) at top of stack, then jump
~~~
"input: " print

a inputraw =

a 1 ==
result1
if1jmp ; if a == 1 then jump to result1, else continue (resulting in 2)

"result 0" print
stop

;result1
"result 1" print
stop

~~~

example of a for loop:
~~~
"How many times to repeat? " print i inputraw =

;loop
"hey" printl

i i 1 - = ; i = i-1

i 0 ==
"done jmp"
if
loop jmp

;done
"done" printl
stop

; will print "hey" however many times you tell it to
~~~

other commands:

put - puts string value of element on stack

dup - duplicates last stack element

###- prints stack info (size, elements)

empty - cleans stack

pop - deletes top stack element

ifput - like ifjmp but does put command instead of jmp

wait - takes number (or variable) and waits for that many seconds: 3 wait
