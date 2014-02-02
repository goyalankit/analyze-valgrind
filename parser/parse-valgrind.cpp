#include <fstream>
#include<iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include<tr1/unordered_map>
#include <cstdio>



#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "boost/lexical_cast.hpp"

using namespace std;
using namespace boost::algorithm;
unsigned int ltox(std::string hex);


int main()
{
    std::ifstream input( "res" );
    tr1::unordered_map<std::string, vector< unsigned int > > variables;
    //std::vector< <std::string, vector<s>> > variables;
    vector<string> f_in_cache;
    for(std::string line; getline( input,line ); )
    {
        if (line.find("==")!=std::string::npos)
            continue;
        else if(line.find("varinfo")!=std::string::npos){
            std::vector<std::string> strs;
            boost::split(strs, line, boost::is_any_of(":")); //line the string to be split

            variables[strs[1]].resize(2);
            variables[strs[1]][0] = ltox(strs[3]); //storing the base address
            variables[strs[1]][1] = ltox(strs[3]) + (4 * strtoul(strs[2].c_str(),NULL,10)); //storing the end address
            std::cout << ltox(strs[3]) << std::endl;
            std::cout << variables[strs[1]][0] << std::endl;
            std::cout << variables[strs[1]][1] << std::endl;
        }else{
            
        }
        f_in_cache.push_back(line);
    }
        
    std::string line;
    std::cout <<"file size" << f_in_cache.size() << std::endl;
    bool start_recording = false;
    for(std::vector<string>::iterator it = f_in_cache.begin(); it!=f_in_cache.end(); ++it )
    {   
        line = *it;
    
        if (line.find("==")!=std::string::npos) continue;
        else if(line.find("varinfo")!=std::string::npos) continue;
        else{
            
            if(!start_recording && (line.find("beforemethod")!=std::string::npos)){
   //                 std::cout << "before method...starting recording" << std::endl;
                    start_recording = true; 
                    continue;
            } else if(!start_recording){ continue; }
            else if(start_recording && (line.find("aftermethod")!=std::string::npos)){
     //           std::cout << "after method...stop recording" << std::endl;
                start_recording = false;
                continue;
            }


            std::vector<std::string> temp;
            std::vector<std::string> strs;
            trim(line);
            if(line=="") continue;
            boost::split(temp, line, boost::is_any_of("  ")); //line the string to be split
            if(temp.size() == 2)
                boost::split(strs, temp[1], boost::is_any_of(",")); //line the string to be split
            else
                boost::split(strs, temp[2], boost::is_any_of(",")); //line the string to be split
            
/*            std::cout << line << std::endl;
            std::cout << temp.size() << std::endl;
            cout << temp[0] << " " << temp[1] << " str " << strs[1] ;
  */          
            tr1::unordered_map<std::string, vector< unsigned int > >::iterator iter;

            for (iter = variables.begin() ; iter != variables.end(); ++iter){
                unsigned int add_acc = ltox(strs[0]);
                if((add_acc >= iter->second[0]) && (add_acc <=  iter->second[1])){
                    unsigned int index_ac = (add_acc - iter->second[0]) / 4;
                    std::cout << "variable:"<< iter->first << " index:" << index_ac <<  std::endl;
                }
                
            }
        }
    }
}

unsigned int ltox(std::string myhex){
    unsigned int x = strtoul(myhex.c_str(), NULL, 16);
    return x;
}
