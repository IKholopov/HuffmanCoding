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
            outputScheme << (*jt == 0) ? '0' : '1';
        outputScheme << ';';
    }
    outputScheme << "/>";
}
void Encode(std::istream &input, std::istream &schemeStream, std::ostream &outputFile, std::ostream &outputScheme)
{
    Scheme* scheme = ReadSchemeFromStream(schemeStream);
    if(scheme == NULL)
        return;
    std::vector<bool> encodingResult = HuffmanCoding::lib().Encode(*scheme, input);
    WriteSchemeToStream(*scheme, outputScheme);
    for(auto it = encodingResult.begin(); it != encodingResult.end(); ++it)
        outputFile << (*it == 0) ? '0' : '1';
    delete scheme;
}
void Encode(std::istream &input, std::ostream &outputFile, std::ostream &outputScheme)
{
    std::pair<Scheme, std::vector<bool> > encodingResult = HuffmanCoding::lib().Encode(input);
    Scheme& scheme = encodingResult.first;
    std::vector<bool> code = encodingResult.second;
    WriteSchemeToStream(scheme, outputScheme);
    for(auto it = code.begin(); it != code.end(); ++it)
        outputFile << (*it == 0) ? '0' : '1';
}
void Decode(std::istream &input, std::istream &schemeStream, std::ostream &outputFile)
{
    Scheme* scheme = ReadSchemeFromStream(schemeStream);
    if(scheme == NULL)
        return;
    std::vector<bool> data;
    char c;
    while(input.get(c))
    {
        if(c != '0' && c != '1')
        {
            std::cerr << "Invalid encoded file" << std::endl;
            return;
        }
        data.push_back((c == '0') ? 0 : 1);
    }
    outputFile << HuffmanCoding::lib().Decode(*scheme, data);
}

