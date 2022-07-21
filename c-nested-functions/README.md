# c-nested-functions
## Rambling about a theoretical attack (ab)using the GCC nested functions extension

There are 3 C source files in this directory: nested.c, trampoline.c and trampoline_output.c. 
You can compile them using 
```
gcc nested.c -g -o nested
gcc trampoline.c -g -o trampoline
gcc trampoline_output.c -g -o trampoline_output
```

The first one is a simple nested function that is simply called in the scope it's declared.
In this case no trampoline is generated and the resulting binary has NX enabled (default, strong security) as we can see by using the checksec tool included in pwntools.
![image](https://user-images.githubusercontent.com/7304787/179398439-847e0a99-cab4-4e96-ab47-d55a2d89e6c1.png)

The security "downgrade" comes when we get the address of the nested function to, for example, pass it as a parameter to another function which will later call it.
To deal with this case GCC generates some instructions in the stack, which will be executed before the real nested function.
In my test using the x86_64 architecture 3 instructions are generated, 2 movabs and 1 jmp. The first one loads the address of the real function, the second one loads the "static chain register" to a register which will be used inside the real function and finally the third one jumps to the function address loaded before. In my test this static chain register was a pointer to the stack, not far from the trampoline instructions.
Here we see that the generated binary no longer has the security mitigation NX enabled.

![image](https://user-images.githubusercontent.com/7304787/179398508-ab7875e0-d6d3-4219-a09d-91c7f3c3f499.png)

The latest binary trampoline_output.c builds a bit upon the last and prints some values to exemplify a nested function usage which requires a trampoline.

From the security point of view a malicious actor could push some benign code to a C project which includes an unnecessary nested C function that forces the compiler to disable the NX mitigation. After a few days/months/years depending on the software release cycle and system update mechanism, a target system could be updated with the (more easily) exploitable version of the binary, which then the attacker could use to compromise the system.

It could be easy for a code reviewer to let a change like this slip in since no extra compiler arguments are needed and no warnings are produced as a consequence of nested function usage. It's also hard to detect the changes in the produced binary without using advanced tools like checksec so it's likely that the binaries could reach a release without anyone noticing NX is disabled.

Personally I would prefer if a compiler flag had to be passed to enable an extension like this. 

### Sources<br>
https://gcc.gnu.org/onlinedocs/gcc/Nested-Functions.html<br>
https://gcc.gnu.org/onlinedocs/gccint/Trampolines.html

Compiler used: gcc version 12.1.0 (GCC)
