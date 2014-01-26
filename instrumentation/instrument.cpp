/***   Instrumentation.cpp   ******************************************************
 * 
 * Add print statements after the declaration of variables in c++ code
 * 
 * Usage:
 * CIrewriter <options> <file>.c
 * where <options> allow for parameters to be passed to the preprocessor
 * such as -DFOO to define FOO.
 *
 * Generated as output <file>_out.c
 * 
 * Original code: https://github.com/loarabia/Clang-tutorial/
 * Modified by: Ankit Goyal
 *
 *****************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include <sstream>

#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/FileSystem.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Lexer.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;

// RecursiveASTVisitor is is the big-kahuna visitor that traverses
// everything in the AST.
class MyRecursiveASTVisitor
: public RecursiveASTVisitor<MyRecursiveASTVisitor>
{

    public:
        MyRecursiveASTVisitor(Rewriter &R) : Rewrite(R) { }
        void InstrumentDeclStmt(DeclStmt *s);
        bool VisitStmt(Stmt *s);

        Rewriter &Rewrite;
};

void MyRecursiveASTVisitor::InstrumentDeclStmt(DeclStmt *s)
{

    for (clang::DeclStmt::const_decl_iterator DI = s->decl_begin(), DE = s->decl_end(); DI != DE; ++DI) {
        const VarDecl *V = cast<VarDecl>(*DI);
        std::string variable_name = V->getNameAsString();

        SourceLocation END = V->getLocEnd();

        //    llvm::errs() << s->getLocStart();
        // MeasureTokenLength gets us past the last token, and adding 1 gets
        // us past the ';'.
        int offset = Lexer::MeasureTokenLength(END,
                Rewrite.getSourceMgr(),
                Rewrite.getLangOpts()) + 1;

        SourceLocation END1 = END.getLocWithOffset(offset);
        std::stringstream method_name;
        method_name << "\nprintme(&" << variable_name << ");\n";

        Rewrite.InsertText(END1, method_name.str(), true, true);
    }
}


// Override Statements which includes expressions and more
bool MyRecursiveASTVisitor::VisitStmt(Stmt *s)
{
    if (isa<DeclStmt>(s))
    {
        // Cast s to IfStmt to access the then and else clauses
        DeclStmt *TH = cast<DeclStmt>(s);
        // Add braces if needed to then clause
        InstrumentDeclStmt(TH);

    }
    return true; // returning false aborts the traversal
}

class MyASTConsumer : public ASTConsumer
{
    public:

        MyASTConsumer(Rewriter &Rewrite) : rv(Rewrite) { }
        virtual bool HandleTopLevelDecl(DeclGroupRef d);

        MyRecursiveASTVisitor rv;
};


bool MyASTConsumer::HandleTopLevelDecl(DeclGroupRef d)
{
    typedef DeclGroupRef::iterator iter;

    for (iter b = d.begin(), e = d.end(); b != e; ++b)
    {
        rv.TraverseDecl(*b);
    }

    return true; // keep going
}


int main(int argc, char **argv)
{
    struct stat sb;

    if (argc < 2)
    {
        llvm::errs() << "Usage: CIrewriter <options> <filename>\n";
        return 1;
    }

    // Get filename
    std::string fileName(argv[argc - 1]);

    // Make sure it exists
    if (stat(fileName.c_str(), &sb) == -1)
    {
        perror(fileName.c_str());
        exit(EXIT_FAILURE);
    }

    CompilerInstance compiler;
    DiagnosticOptions diagnosticOptions;
    compiler.createDiagnostics();
    //compiler.createDiagnostics(argc, argv);

    // Create an invocation that passes any flags to preprocessor
    CompilerInvocation *Invocation = new CompilerInvocation;
    CompilerInvocation::CreateFromArgs(*Invocation, argv + 1, argv + argc,
            compiler.getDiagnostics());
    compiler.setInvocation(Invocation);

    // Set default target triple
    llvm::IntrusiveRefCntPtr<TargetOptions> pto( new TargetOptions());
    pto->Triple = llvm::sys::getDefaultTargetTriple();
    llvm::IntrusiveRefCntPtr<TargetInfo>
        pti(TargetInfo::CreateTargetInfo(compiler.getDiagnostics(),
                    pto.getPtr()));
    compiler.setTarget(pti.getPtr());

    compiler.createFileManager();
    compiler.createSourceManager(compiler.getFileManager());

    HeaderSearchOptions &headerSearchOptions = compiler.getHeaderSearchOpts();

    // <Warning!!> -- Platform Specific Code lives here
    // This depends on A) that you're running linux and
    // B) that you have the same GCC LIBs installed that
    // I do.
    // Search through Clang itself for something like this,
    // go on, you won't find it. The reason why is Clang
    // has its own versions of std* which are installed under
    // /usr/local/lib/clang/<version>/include/
    // See somewhere around Driver.cpp:77 to see Clang adding
    // its version of the headers to its include path.
    // To see what include paths need to be here, try
    // clang -v -c test.c
    // or clang++ for C++ paths as used below:
    headerSearchOptions.AddPath("/usr/include/c++/4.6",
            clang::frontend::Angled,
            false,
            false);
    headerSearchOptions.AddPath("/usr/include/c++/4.6/i686-linux-gnu",
            clang::frontend::Angled,
            false,
            false);
    headerSearchOptions.AddPath("/usr/include/c++/4.6/backward",
            clang::frontend::Angled,
            false,
            false);
    headerSearchOptions.AddPath("/usr/local/include",
            clang::frontend::Angled,
            false,
            false);
    headerSearchOptions.AddPath("/usr/local/lib/clang/3.3/include",
            clang::frontend::Angled,
            false,
            false);
    headerSearchOptions.AddPath("/usr/include/i386-linux-gnu",
            clang::frontend::Angled,
            false,
            false);
    headerSearchOptions.AddPath("/usr/include",
            clang::frontend::Angled,
            false,
            false);
    // </Warning!!> -- End of Platform Specific Code


    // Allow C++ code to get rewritten
    LangOptions langOpts;
    langOpts.GNUMode = 1; 
    langOpts.CXXExceptions = 1; 
    langOpts.RTTI = 1; 
    langOpts.Bool = 1; 
    langOpts.CPlusPlus = 1; 
    Invocation->setLangDefaults(langOpts,
            clang::IK_CXX,
            clang::LangStandard::lang_cxx0x);

    compiler.createPreprocessor();
    compiler.getPreprocessorOpts().UsePredefines = false;

    compiler.createASTContext();

    // Initialize rewriter
    Rewriter Rewrite;
    Rewrite.setSourceMgr(compiler.getSourceManager(), compiler.getLangOpts());

    const FileEntry *pFile = compiler.getFileManager().getFile(fileName);
    compiler.getSourceManager().createMainFileID(pFile);
    compiler.getDiagnosticClient().BeginSourceFile(compiler.getLangOpts(),
            &compiler.getPreprocessor());

    MyASTConsumer astConsumer(Rewrite);

    // Convert <file>.c to <file_out>.c
    std::string outName (fileName);
    size_t ext = outName.rfind(".");
    if (ext == std::string::npos)
        ext = outName.length();
    outName.insert(ext, "_out");

    llvm::errs() << "Output to: " << outName << "\n";
    std::string OutErrorInfo;
    llvm::raw_fd_ostream outFile(outName.c_str(), OutErrorInfo, llvm::sys::fs::F_None);

    if (OutErrorInfo.empty())
    {
        // Parse the AST
        ParseAST(compiler.getPreprocessor(), &astConsumer, compiler.getASTContext());
        compiler.getDiagnosticClient().EndSourceFile();

        // Output some #ifdefs
        outFile << "#define L_AND(a, b) a && b\n";
        outFile << "#define L_OR(a, b) a || b\n\n";

        // Now output rewritten source code
        const RewriteBuffer *RewriteBuf =
            Rewrite.getRewriteBufferFor(compiler.getSourceManager().getMainFileID());
        outFile << std::string(RewriteBuf->begin(), RewriteBuf->end());
    }
    else
    {
        llvm::errs() << "Cannot open " << outName << " for writing\n";
    }

    outFile.close();

    return 0;
}

