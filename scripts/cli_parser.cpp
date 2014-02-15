#include <iostream>
#include <cstdint>

void parse_cli_options(int argc, char* argv[]){
    if (argc < 1) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
        std::cout << "Usage is -in <infile>\n"; // Inform the user of how to use the program
        std::cin.get();
        exit(0);
    } else { 
        char* myFile;
        std::cout << argv[0];
        for (int i = 1; i < argc; i++) {
            if (i + 1 != argc){
                if (argv[i] == "-f") {
                    myFile = argv[i + 1];
                } else {
                    std::cout << "Not enough or invalid arguments, please try again.\n";
                    exit(0);
                }
            }
        }
    }
}
