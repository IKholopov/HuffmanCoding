#include "encoder.h"
#include "huffmancoding.h"

Scheme* ReadSchemeFromStream(std::istream& schemeStream)
{
    Scheme* scheme = new Scheme();
    enum ReadStatus
    {
        ReadyToReadSymbol, GotSymbol, ReadingCode
    } status;
    status = ReadStatus::ReadyToReadSymbol;
    char symb, c1, c2;
    std::vector<bool> code;
    schemeStream.get(c1);
    if(c1 != '<')
    {
        std::cerr << "Invalid scheme file format" << std::endl;
        return NULL;
    }
    while(schemeStream.get(c2) && (c1 != '/' || c2 != '>'))
    {
        if(status == ReadStatus::ReadyToReadSymbol)
        {
            status = ReadStatus::GotSymbol;
            symb = c2;
        }
        else if(status == ReadStatus::GotSymbol)
        {
            if(c2 != '|')
            {
                std::cerr << "Invalid scheme file format" << std::endl;
                return NULL;
            }
            code.clear();
            status = ReadStatus::ReadingCode;
        }
        else if(status == ReadStatus::ReadingCode)
        {
            if(c2 != '0' && c2 != '1')
            {
                if(c2 != ';')
                {
                    std::cerr << "Invalid scheme file format" << std::endl;
                    return NULL;
                }
                scheme->insert(std::pair<char, std::vector<bool>>(symb, code));
                status = ReadStatus::ReadyToReadSymbol;
            }
            code.push_back(c2 == '0' ? 0 : 1);
        }

        c1 = c2;
    }
    return scheme;
}
void WriteSchemeToStream(Scheme& scheme, std::ostream& outputScheme)
{
    outputScheme << '<';
    for(auto it = scheme.begin(); it != scheme.end(); ++it)
    {
        outputScheme << it->first;
        outputScheme << '|';
        for(auto jt = it->second.begin(); jt != it->second.end(); ++jt)
            outputScheme << !(*jt == 0);
        outputScheme << ';';
    }
    outputScheme << "/>";
}
void WriteBitsToStream(std::vector<bool> bits, std::ostream& output)
{
    unsigned int lastBitsCount = bits.size() % 8;
    if(lastBitsCount == 0)
        lastBitsCount = 8;
    char mask = 0;
    for(int i = 0; i < lastBitsCount; ++i)
    {
        mask >>= 1;
        mask |= (1 << 7);
    }
    output << mask;
    for(int i = 0; i < bits.size() / 8; ++i)
    {
        char c = 0;
        for(int j = 0; j < 8; ++j)
             c |= (bits[i * 8 + j] << (7 - j));
        output << c;
    }
    if(lastBitsCount != 8)
    {
        char c = 0;
        for(int i = 0; i < lastBitsCount; ++i)
            c |= (bits[i +  (bits.size() / 8) * 8] << (7 - i));
        output << c;
    }
}

void Encode(std::istream &input, std::istream &schemeStream, std::ostream &outputFile, std::ostream &outputScheme)
{
    Scheme* scheme = ReadSchemeFromStream(schemeStream);
    if(scheme == NULL)
        return;
    std::vector<bool> encodingResult = HuffmanCoding::lib().Encode(*scheme, input);
    WriteSchemeToStream(*scheme, outputScheme);
    WriteBitsToStream(encodingResult, outputFile);
    /*for(auto it = encodingResult.begin(); it != encodingResult.end(); ++it)
        outputFile << (*it == 0) ? '0' : '1';*/

    delete scheme;
}
void Encode(std::istream &input, std::ostream &outputFile, std::ostream &outputScheme)
{
    std::pair<Scheme*, std::vector<bool> > encodingResult = HuffmanCoding::lib().Encode(input);
    Scheme* scheme = encodingResult.first;
    std::vector<bool> code = encodingResult.second;
    WriteSchemeToStream(*scheme, outputScheme);
    WriteBitsToStream(code, outputFile);
    delete scheme;
}
void Decode(std::istream &input, std::istream &schemeStream, std::ostream &outputFile)
{
    Scheme* scheme = ReadSchemeFromStream(schemeStream);
    if(scheme == NULL)
        return;
    std::vector<bool> data;
    char c1, c2,  mask;
    if(!input.get(mask))
    {
        std::cerr << "Invalid encoded file" << std::endl;
        delete scheme;
        return;
    }
    if(!input.get(c1))
    {
        delete scheme;
        return;
    }
    while(input.get(c2))
    {
        for(int i = 0; i < 8; ++i)
        {
            bool c = (c1 >> (7 - i)) & 1;
            data.push_back(c);
        }
        c1 = c2;
    }
    c1 &= mask;
    for(int i = 0; i < 8 && ((mask >> (7 - i)) & 1); ++i)
    {
        bool c = (c1 >> (7 - i)) & 1;
        data.push_back(c);
    }
    outputFile << HuffmanCoding::lib().Decode(*scheme, data);
    delete scheme;
}
