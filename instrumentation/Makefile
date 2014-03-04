CXX := clang++
LLVMCOMPONENTS := cppbackend
RTTIFLAG := -fno-rtti

LLVMLIB := $(shell echo $(TACC_LLVM_LIB))

ifeq ($(LLVMLIB), /opt/apps/llvm/3.3/lib)
	LLVMCONFIG := /opt/apps/llvm/3.3/bin/llvm-config
else
	LLVMCONFIG := ~/DevelopmentKits/build/Debug+Asserts/bin/llvm-config
endif


CXXFLAGS := -I$(shell $(LLVMCONFIG) --src-root)/tools/clang/include -I$(shell $(LLVMCONFIG) --obj-root)/tools/clang/include $(shell $(LLVMCONFIG) --cxxflags) $(RTTIFLAG)
LLVMLDFLAGS := $(shell $(LLVMCONFIG) --ldflags --libs $(LLVMCOMPONENTS))

SOURCES = instrument.cpp \

OBJECTS = $(SOURCES:.cpp=.o)
EXES = $(OBJECTS:.o=)
CLANGLIBS = \
                                -lclangTooling\
                                -lclangFrontendTool\
                                -lclangFrontend\
                                -lclangDriver\
                                -lclangSerialization\
                                -lclangCodeGen\
                                -lclangParse\
                                -lclangSema\
                                -lclangStaticAnalyzerFrontend\
                                -lclangStaticAnalyzerCheckers\
                                -lclangStaticAnalyzerCore\
                                -lclangAnalysis\
                                -lclangARCMigrate\
                                -lclangRewriteFrontend\
                                -lclangRewriteCore\
                                -lclangEdit\
                                -lclangAST\
                                -lclangLex\
                                -lclangBasic\
                                $(shell $(LLVMCONFIG) --libs)\
                -lcurses

all: $(OBJECTS) $(EXES)

%: %.o
	$(CXX) -o $@ $< $(CLANGLIBS) $(LLVMLDFLAGS)

clean:
	-rm -f $(EXES) $(OBJECTS) *~
