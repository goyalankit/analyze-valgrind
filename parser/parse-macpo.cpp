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
    std::ifstream input( "macout" );
    tr1::unordered_map<std::string, vector< unsigned int > > variables;
    //std::vector< <std::string, vector<s>> > variables;
    vector<string> f_in_cache;
    for(std::string line; getline( input,line ); )
    {
        //put the file in cache: TODO optimize it.
        f_in_cache.push_back(line); 
    }
        
    std::string line;
    std::cout <<"file size" << f_in_cache.size() << std::endl;
    for(std::vector<string>::iterator it = f_in_cache.begin(); it!=f_in_cache.end(); ++it )
    {   
        line = *it;
    
        if (line.find("+")!=std::string::npos)
        {
            std::vector<std::string> temp;
            std::vector<std::string> strs;
            trim(line);
            
            boost::split(temp, line, boost::is_any_of(":")); //line the string to be split
//            std::cout << temp[0] << std::endl;
            boost::split(strs, temp[1], boost::is_any_of("+")); //line the string to be split

            /*
            std::cout << line << std::endl;
            std::cout << temp.size() << std::endl;
            cout << temp[0] << " " << temp[1] << " str " << strs[1] ;
            */
            int x = boost::lexical_cast<int>(strs[1])/4;
            std::cout << "variable:"<< strs[0] << " index:" << x <<  std::endl;
                
        }
    }
}



unsigned int ltox(std::string myhex){
    unsigned int x = strtoul(myhex.c_str(), NULL, 16);
    return x;
}
