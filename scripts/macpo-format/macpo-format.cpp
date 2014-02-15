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
#include <unordered_map>
#include <set>

#include "mem-info.h"

#define READ "1"
#define WRITE "2"

typedef std::unordered_map<std::string, std::string> SS_MAP; //string to string map
typedef std::unordered_map<std::string, int> SI_MAP; //string to int map

SI_MAP var_map;
std::string m_filename;
std::string o_filename;

int fd;

/* Command line parser */
void parse_cli_options(int argc, char* argv[]){
    if (argc < 5) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
        std::cout << "Usage is -f <valgrind filename> -o <output filename>\n"; // Inform the user of how to use the program
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

void indigo__write_idx_c(const char* var_name, const int length)
{
    node_t node;
    node.type_message = MSG_STREAM_INFO;
#define my__MIN(a,b)    (a) < (b) ? (a) : (b)
    int dst_len = my__MIN(STREAM_LENGTH-1, length);
#undef my__MIN

    strncpy(node.stream_info.stream_name, var_name, dst_len);
    node.stream_info.stream_name[dst_len] = '\0';

    write(fd, &node, sizeof(node_t));
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

SS_MAP parse_line(const std::string &s){
    SS_MAP * mymap = new SS_MAP(); //Will be freed when by the destructor
    std::vector<std::string> temp = split(s, ':');

    if(temp[0] == "R ")  
        (*mymap)["rw"] = READ;
    else if(temp[0] == "W ") 
        (*mymap)["rw"] = WRITE;

    temp = split(temp[1], '+');

    (*mymap)["vname"] = temp[0];
    (*mymap)["address"] = temp[1];

    return *mymap;
}


size_t strToSizt_t(std::string str){
    size_t size_t_address;
    std::stringstream(str) >> size_t_address;
    return size_t_address;
}

int main(int argc, char* argv[]){
    parse_cli_options(argc, argv);
    std::vector<std::string> tokens;

    std::ifstream m_stream(m_filename);

    //to store variables for map
    std::set<std::string> variables;

    //vector of map containing access information. 
    //"rw" = [0|1], "vname" = variable name, "index" = index of the variable accessed
    std::vector < SS_MAP > cached_accesses; 

    /* Get the file descriptor for the output file */
    const char *macpo_fn = o_filename.c_str();
    fd = open(macpo_fn , O_CREAT | O_APPEND | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);


    SS_MAP access_map;
    std::string line;

    int var_ind=0;
    /* read, cache the file and build map */
    while(std::getline(m_stream, line)){

        access_map = parse_line(line);
        cached_accesses.push_back(access_map); //cache the access

        int map_size = var_map.size();
        var_map[access_map["vname"]] = var_ind; //add the variables to their map

        if(var_map.size() > map_size) var_ind++;

    }

    /* 1. Write Metadata to the file */
    write_metadata();

    /* 2. write variable map */
    for(SI_MAP::const_iterator it = var_map.begin(); it != var_map.end(); ++it){
        indigo__write_idx_c((it->first).c_str(), (it->first).length());
    }

    /* 3. read_write line_number address var_idx */
    for (std::vector<SS_MAP>::const_iterator it = cached_accesses.begin(); it != cached_accesses.end(); ++it) {
        SS_MAP local_a_map = *it;

        int variable_index = var_map[local_a_map["vname"]];
        size_t address = strToSizt_t(local_a_map["address"]);
        int read_write = atoi(local_a_map["rw"].c_str());

        fill_struct(read_write, -1, address, variable_index);

        //DEBUG:
        std::cout << local_a_map["vname"] << address << " " << read_write << std::endl;
    }

    /* 4. Terminator */

    return 0;
}
