Analyze valgrind output from Lackey tool
=======================

Instrument the c code using clang and then parse the output of Valgrind.

#### Intructions

1. Get llvm and clang from: http://clang.llvm.org/get_started.html
2. git clone https://github.com/goyalankit/analyze-valgrind.git
3. Inside instrumentation directory change `LLVMCONFIG` variable in `Makefile`. Set it to your llvm build path.
4. Parser requires boost libraries.

#### Instrumentation
```
cd instrumentation
make
./instrument <foo.c>
```

A new file `foo_c.c` will be generated with appropriate print instructions inserted in the source code.

#### Parser
name the trace from valgrind lackey as `res`. //TODO: need to add a cli argument for the filename

```
cd parser
make 
./pv > access_file
```

Currently it will print all the **sequence of indices accessed of various arrays** on `stdout`. Hence it is redirected to `access_file` above. //TODO: write it to a file
