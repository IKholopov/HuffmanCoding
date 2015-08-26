#include <iostream>
#include "huffmancoding.h"
#include "encoder.h"
#include <vector>
#include <utility>
#include <fstream>
#include <string.h>

int main(int argc, char** argv)
{
    /*std::pair<Scheme, std::vector<bool>> codes = HuffmanCoding::lib().Encode(std::cin);
    std::string out = HuffmanCoding::lib().Decode(codes.first, codes.second);
    std::cout << out;*/
    std::istream& defaultInputStream = std::cin;
    std::istream* inputStream = &defaultInputStream;
    std::istream* schemeStream =  NULL;
    std::ostream& defaultOutputFileStream = std::cout;
    std::ostream* outputFileStream = &defaultOutputFileStream;
    std::ostream& defaultOutputSchemeStream = std::cout;
    std::ostream* outputSchemeStream = &defaultOutputSchemeStream;
    bool input = 0;
    bool output = 0;
    bool clear = 0;
    bool outputScheme = 0;
    bool scheme = 0;
    bool decode = 0;
    for(int i = 0; i < argc; ++i)
    {
        if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            std::cout << "Huffman Coding v." << VERSION << std::endl;
            std::cout << "USAGE: huffman [OPTIONS]" << std::endl;
            std::cout << "OPTIONS:" << std::endl;
            std::cout << "\t-h --help Displays this message" << std::endl;
            std::cout << "\t-i --input [PATH] Input file path" << std::endl;
            std::cout << "\t-d --decode Decode instead of encode" << std::endl;
            std::cout << "\t-o --output [PATH] Output file path" << std::endl;
            std::cout << "\t-c --clear Separate scheme and coded file" << std::endl;
            std::cout << "\t\t-os --output-scheme [PATH] Generated scheme file path" << std::endl;
            std::cout << "\t-s --scheme [PATH] Custom scheme file path" << std::endl;
            return 0;
        }
        if(strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0)
        {
            input = 1;
            if(++i == argc)
            {
                std::cout << "Invalid -i argument parameter" << std::endl;
                return 1;
            }
            std::ifstream* in = new std::ifstream(argv[i]);
            if(!in)
            {
                std::cout << "Invalid -i argument parameter - file does not exist" << std::endl;
                return 1;
            }
            inputStream = in;
            if(i + 1 == argc)
                break;
            else continue;
        }
        if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
        {
            output = 1;
            if(++i == argc)
            {
                std::cout << "Invalid -o argument parameter" << std::endl;
                return 1;
            }
            std::ofstream* out = new std::ofstream(argv[i]);
            outputFileStream = out;
            if(i + 1 == argc)
                break;
            else continue;
        }
        if(strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--clear") == 0)
        {
            clear = 1;
            if(i + 1 == argc)
                break;
            else continue;
        }
        if(strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decode") == 0)
        {
            decode = 1;
            if(i + 1 == argc)
                break;
            else continue;
        }
        if(strcmp(argv[i], "-os") == 0 || strcmp(argv[i], "--output-scheme") == 0)
        {
            outputScheme = 1;
            if(++i == argc)
            {
                std::cout << "Invalid -os argument parameter" << std::endl;
                return 1;
            }
            std::ofstream* out = new std::ofstream(argv[i]);
            outputSchemeStream = out;
            if(i + 1 == argc)
                break;
            else continue;
        }
        if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--scheme") == 0)
        {
            scheme = 1;
            if(++i == argc)
            {
                std::cout << "Invalid -s argument parameter" << std::endl;
                return 1;
            }
            std::ifstream* in = new std::ifstream(argv[i]);
            if(!in)
            {
                std::cout << "Invalid -s argument parameter - file does not exist" << std::endl;
                return 1;
            }
            schemeStream = in;
            if(i + 1 == argc)
                break;
            else continue;
        }
    }
    if(decode)
    {
        if(!scheme)
            schemeStream = inputStream;
        Decode(*inputStream, *schemeStream, *outputFileStream);
    }
    else
    {
        if(output && !outputScheme)
            outputSchemeStream = outputFileStream;
        if(scheme)
            Encode(*inputStream, *schemeStream, *outputFileStream, *outputSchemeStream);
        else
            Encode(*inputStream, *outputFileStream, *outputSchemeStream);
    }
    if(inputStream != &defaultInputStream)
        ((std::ifstream*)inputStream)->close();
    if(schemeStream != NULL)
        ((std::ifstream*)schemeStream)->close();
    if(outputFileStream != &defaultOutputFileStream)
        ((std::ofstream*)outputFileStream)->close();
    if(outputSchemeStream != &defaultOutputSchemeStream)
        ((std::ofstream*)outputSchemeStream)->close();
    return 0;
}

