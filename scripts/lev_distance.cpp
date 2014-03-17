#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include <unordered_map>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "boost/lexical_cast.hpp"


using namespace std;
using namespace boost::algorithm;


//to map variable -> memory accesses. Example: a -> [Ra12, Ra14, Wa12]
typedef std::unordered_map< std::string,std::vector<std::string> > StringVectorMap;

/*-------------------------  Levenshtein distance method -------------------------*/

template<class T>
unsigned int levenshtein_distance(const T &s1, const T & s2) {
    const size_t len1 = s1.size(), len2 = s2.size();
    vector<unsigned int> col(len2+1), prevCol(len2+1);
 
    for (unsigned int i = 0; i < prevCol.size(); i++)
        prevCol[i] = i;
    for (unsigned int i = 0; i < len1; i++) {
        col[0] = i+1;
        for (unsigned int j = 0; j < len2; j++)
            col[j+1] = std::min( std::min(prevCol[1 + j] + 1, col[j] + 1),
                                prevCol[j] + (s1[i]==s2[j] ? 0 : 1) );
        col.swap(prevCol);
    }
    return prevCol[len2];
}

/*--------------------------------  PRINT METHODS ---------------------------------*/

// print method for vector
std::ostream& operator<<(std::ostream& os, const vector<string>& obj)
{
    for(int i=0; i<obj.size(); i++)
        os << " " << obj[i];
    
    os << "\n";
    
    return os;
}

// print method for map
std::ostream& operator<<(std::ostream& os,const StringVectorMap& obj)
{
    for ( StringVectorMap::const_iterator it = obj.begin(); it != obj.end(); ++it )
        os << " " << it->first << " " << it->second;
    
    os << "\n";
    
    return os;
}

/*------------------------------  PARSING FILE METHODS ---------------------------*/

// read file to a single vector. [Ra12, Wb12, Ra45, Wa32]
vector<string> read_file_to_vector(string filename){
    string line;
    vector<string> seqVector;
    ifstream seq_stream(filename.c_str());
    while(getline(seq_stream, line)){
        trim(line);
        seqVector.push_back(line);
    }
    return seqVector;
}

// read file to map with variables as key. a = [Ra12, Ra45, Wa32]; b = [Rb12, Wb32]
StringVectorMap read_file_to_vector_map(string filename){
    
    vector<string> seqVector, temp;
    StringVectorMap variableAccessMap;

    
    ifstream seq_stream(filename.c_str());
    string line;
    while(getline(seq_stream, line)){
        // line contains R:a:12
        trim(line);
        
        // get the a from Ra12
        split(temp, line, boost::is_any_of(":"));
        
        // add line to vector corresponding to variable.
        variableAccessMap[temp[1]].push_back(line);
    }
    return variableAccessMap;
}

/*-----------------  Call the levenshtein distance method -----------------------*/

void unsorted_basic_distance(vector<string>& s1, vector<string>& s2){
    unsigned int distance = levenshtein_distance< vector<string> >(s1, s2);
    std::cout << distance << std::endl;
}

void sorted_distance(const StringVectorMap& mapS1, const StringVectorMap& mapS2){

    unsigned int distance = 0;
    for (StringVectorMap::const_iterator it = mapS1.begin(); it != mapS1.end(); ++it) {
        distance += levenshtein_distance< vector<string> >(it->second, mapS2.at(it->first));
    }
    
    std::cout << distance << std::endl;
}


/* -------------------------------- MAIN -------------------------------------- */

int main(int argc, char* argv[]){
    
    vector<string> s1, s2;
    StringVectorMap mapS1, mapS2;

    if(argc < 3){
        std::cout << "Not enough parameters. Give the two sequence files" << std::endl;
        std::cout << "Usage: ./a.out <fileNum1> <fileNum2>" << std::endl;
        std::cout << "Usage: ./a.out <fileNum1> <fileNum2> --sort" << std::endl;
        std::cout << "Usage: ./a.out -s 'string1' 'string2'" << std::endl;

        return 1;
    }
    
    if(argv[1] == string("-s")){
        std::cout << "Processing the strings...." << std::endl;
        
        split(s1, argv[2], boost::is_any_of("\t "));
        split(s2, argv[3], boost::is_any_of("\t "));
        
        unsorted_basic_distance(s1, s2);
    }
    else if(argc > 3 && argv[3] == string("--sort")){
        std::cout << "Processing the traces by individual variables..." << std::endl;
        
        mapS1 = read_file_to_vector_map(argv[1]);
        mapS2 = read_file_to_vector_map(argv[2]);
        
        sorted_distance(mapS1, mapS2);
    }
    else
    {
        std::cout << "Processing the complete traces at once...." << std::endl;
        
        s1 = read_file_to_vector(argv[1]);
        s2 = read_file_to_vector(argv[2]);
        
        unsorted_basic_distance(s1, s2);
    }

    return 0;
}
