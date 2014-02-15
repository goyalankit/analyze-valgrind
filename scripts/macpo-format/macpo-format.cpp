#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


#include "mem-info.h"

std::string m_filename;
std::string o_filename;

int fd;

/* Command line parser */
void parse_cli_options(int argc, char* argv[]){
    if (argc < 5) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
        std::cout << "Usage is -in <infile>\n"; // Inform the user of how to use the program
        std::cin.get();
        exit(0);
    } else {
        char* myFile;
        for (int i = 1; i < argc; i+=2) {
            if (i + 1 != argc){
                if (strcmp(argv[i],"-f") == 0) {
                    m_filename = argv[i + 1];
                } else if (strcmp(argv[i],"-o") == 0) {
                    o_filename = argv[i + 1];
                }
            }
        }
    }
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

static inline void fill_struct(int read_write, int line_number, size_t address, int var_idx)
{
    // If this process was never supposed to record stats
    // or if the file-open failed, then return
    if (fd < 0) return;

    node_t node;
    node.type_message = MSG_MEM_INFO;
    node.coreID = 1;

    node.mem_info.read_write = read_write;
    node.mem_info.address = address;
    node.mem_info.var_idx = var_idx;
    node.mem_info.line_number = line_number;

    write(fd, &node, sizeof(node_t));
}


static inline void write_metadata(){
    node_t node;

    time_t t;
    time(&t);
    struct tm *ltm = localtime(&t);

    node.metadata_info.day = ltm->tm_mday;
    node.metadata_info.month = ltm->tm_mon+1;
    node.metadata_info.year = ltm->tm_year+1900;
    node.metadata_info.hour = ltm->tm_hour;
    node.metadata_info.min = ltm->tm_min;
    node.metadata_info.sec = ltm->tm_sec;

    write(fd, &node, sizeof(node_t));
}

int main(int argc, char* argv[]){
    parse_cli_options(argc, argv);
    std::vector<std::string> * tokens = new std::vector<std::string>();

    std::ifstream m_stream(m_filename);
    std::string line;

    const char *macpo_fn = o_filename.c_str();
    fd = open(macpo_fn , O_CREAT | O_APPEND | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);

    //1. Write Metadata to the file
    write_metadata();

    while(std::getline(m_stream, line)){

    }

    delete tokens;
    return 0;
}
