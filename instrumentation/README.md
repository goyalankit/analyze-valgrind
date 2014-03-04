## Steps to run instrumentation on stampede.

1. load the module

`module load llvm`

2. compile the file

`make`

3. executable by the name of `instrumentation` should be generated

`./instrument bar.c` => `bar_out.c`



Debug:

If you are not able to compile on stampede.
1. Check that env variable `TACC_LLVM_LIB` is set : if not ->  `load module llvm`
    
2. Check `/opt/apps/llvm/3.3/bin/llvm-config` exists.: if not -> check the location of llvm, find llvm-config and replace it in Makefile

3. If you are still not able to run it, take a look at the Makefile

