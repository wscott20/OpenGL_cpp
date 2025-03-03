#ifndef read_file_hpp
#define read_file_hpp
#include <fstream>
#include <string>
using std::string;
using ifs = std::ifstream;
string readFile(string filename) {
    ifs file(filename);
    string output,read;
    while (getline(file,read)) {
        output += read + "\n";
    }
    return output;
}
#endif
