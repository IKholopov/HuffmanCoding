#ifndef __ENCODER_H_
#define __ENCODER_H_

#include<iostream>

void Encode(std::istream& input, std::istream& schemeStream, std::ostream& outputFile, std::ostream& outputScheme);
void Encode(std::istream& input, std::ostream& outputFile, std::ostream& outputScheme);
void Decode(std::istream& input, std::istream& schemeStream, std::ostream& outputFile);

#endif

//File format for scheme <[symbol]|[code sequence];[symbol]|...|[code sequence];/>
//Example: <a|0;b|1;/>
