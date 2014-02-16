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
#include "macpo_record.h"

#define READ "1"
#define WRITE "2"

typedef std::unordered_map<std::string, std::string> SS_MAP; //string to string map
typedef std::unordered_map<std::string, int> SI_MAP; //string to int map

SI_MAP var_map;
SS_MAP var_base_map;

std::string m_filename;
std::string o_filename;

int fd;
bool insertInCache;

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


/* write the address access to the file */
static inline void fill_trace_struct(int read_write, int line_number, size_t base, size_t p, int var_idx)
{
    // If this process was never supposed to record stats
    // or if the file-open failed, then return
    if (fd < 0)	return;

    size_t address_base = (size_t) base;
    size_t address = (size_t) p;

    node_t node;
    node.type_message = MSG_TRACE_INFO;

    node.trace_info.coreID = 1;
    node.trace_info.read_write = read_write;
    node.trace_info.base = address_base;
    node.trace_info.address = address;
    node.trace_info.var_idx = var_idx;
    node.trace_info.line_number = line_number;

    write(fd, &node, sizeof(node_t));
}

//Not needed now.
static inline void fill_mem_struct(int read_write, int line_number, size_t p, int var_idx, int type_size)
{
    // If this process was never supposed to record stats
    // or if the file-open failed, then return
    if (fd < 0)	return;

    node_t node;
    node.type_message = MSG_MEM_INFO;

    node.mem_info.coreID = 1;
    node.mem_info.read_write = read_write;
    node.mem_info.address = p;
    node.mem_info.var_idx = var_idx;
    node.mem_info.line_number = line_number;
    node.mem_info.type_size = type_size;

    write(fd, &node, sizeof(node_t));
}



/* write the metadata */
static inline void write_metadata(){

    if (fd < 0) return;

    node_t node;

    time_t t;
    time(&t);
    struct tm *ltm = localtime(&t);

    node.type_message = MSG_METADATA;
    size_t exe_path_len = readlink ("/proc/self/exe", node.metadata_info.binary_name, STRING_LENGTH-1);

    if (exe_path_len == -1)
        perror ("MACPO :: Failed to read name of the binary from /proc/self/exe");
    else
    {
        node.metadata_info.binary_name[o_filename.length()]='\0';
        time(&node.metadata_info.execution_timestamp);

        write(fd, &node, sizeof(node_t));
    }
}

/* write the map to the binary */
void indigo__write_idx_c(const char* var_name, const int length) {

    if (fd < 0) return;

    node_t node;
    node.type_message = MSG_STREAM_INFO;
#define my__MIN(a,b)    (a) < (b) ? (a) : (b)
    int dst_len = my__MIN(STREAM_LENGTH-1, length);
#undef my__MIN

    strncpy(node.stream_info.stream_name, var_name, dst_len);
    node.stream_info.stream_name[dst_len] = '\0';

    write(fd, &node, sizeof(node_t));
}

/* Method to split the string based on a delim. without the boost lib */
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
}//END of the string spit methods

/* parse the line from the input file. create a string to string map with the values.  */
SS_MAP parse_line(const std::string &s){

    SS_MAP * mymap = new SS_MAP(); //will be freed by destructor
    std::vector<std::string> temp = split(s, ':');

    bool isVarInfo = (temp[0] == "varinfo");

    if(__builtin_expect(isVarInfo, 0))
    {
        var_base_map[temp[1]] = temp[2];
        insertInCache = false;
    }
    else
    {
        if(temp[0] == "R ")
            (*mymap)["rw"] = READ;
        else if(temp[0] == "W ") 
            (*mymap)["rw"] = WRITE;

        temp = split(temp[1], '+');

        (*mymap)["vname"] = temp[0];
        (*mymap)["address"] = temp[1];
        insertInCache = true;
    }
    return *mymap;
}

//conversion from string address to size_t address.
size_t strToSize_t(std::string str){
    std::stringstream ss(str);
    void * result;
    ss >> result;
    return size_t(result);
}

int main(int argc, char* argv[]){
    parse_cli_options(argc, argv);
    std::vector<std::string> tokens;

    std::ifstream m_stream(m_filename);

    //to store variables for map
    std::set<std::string> variables;

    //vector of map containing access information. 
    //"rw" = [0|1], "vname" = variable name, "address" = address of the variable accessed
    std::vector < SS_MAP > cached_accesses; 

    /* Get the file descriptor for the output file */
    const char *macpo_fn = o_filename.c_str();
    fd = open(macpo_fn , O_CREAT | O_APPEND | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);


    SS_MAP access_map;
    std::string line;

    int var_ind=0;
    /* read, cache the file and build map */
    while(std::getline(m_stream, line)){
        insertInCache = true;
        access_map = parse_line(line);

        if(!insertInCache) continue;

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
        size_t address = strToSize_t(local_a_map["address"]);
        size_t base_address = strToSize_t(var_base_map[local_a_map["vname"]]);
        int read_write = atoi(local_a_map["rw"].c_str());

        fill_trace_struct(read_write, -1, base_address, address, variable_index);

        //DEBUG:
        std::cout << local_a_map["vname"] << address << " " << read_write << std::endl;
    }

    /* 4. Terminator */

    return 0;
}
