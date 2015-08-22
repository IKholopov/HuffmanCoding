#include <iostream>
#include "huffmancoding.h"
#include <vector>
#include <utility>

int main()
{
    std::pair<Scheme, std::string> codes = HuffmanCoding::lib().Encode("Issues");
    std::string out = HuffmanCoding::lib().Decode(codes.first, codes.second);
    return 0;
}

