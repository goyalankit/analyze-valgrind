#include<iostream>
#include<vector>
#include<string>
#include<fstream>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "boost/lexical_cast.hpp"


using namespace std;
using namespace boost::algorithm;

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

std::ostream& operator<<(std::ostream& os, const vector<string>& obj)
{
    for(int i=0; i<obj.size(); i++)
        os << " " << obj[i];
    os << "\n";

    return os;
}


int main(int argc, char* argv[]){
    vector<string> s1, s2;

    if(argv[1] == string("-s")){
        std::cout << "Processing the strings." << std::endl;
        split(s1, argv[2], boost::is_any_of("\t "));
        split(s2, argv[3], boost::is_any_of("\t "));
    }
    else
    {
        if(argc < 3){
            std::cout << "Not enough parameters. Give the two sequence files" << std::endl;
        }
        s1 = read_file_to_vector(argv[1]);
        s2 = read_file_to_vector(argv[2]);
    }

    unsigned int distance = levenshtein_distance< vector<string> >(s1, s2);
    std::cout << distance << std::endl;

    return 0;
}
