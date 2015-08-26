#include "huffmancoding.h"
#include <algorithm>
#include <errno.h>

HuffmanCoding::HuffmanCoding() {}

HuffmanCoding& HuffmanCoding::lib()
{
    static HuffmanCoding instance;

    return instance;
}

Scheme* HuffmanCoding::GetCode(std::vector<std::pair<char, double> > &probabilities)
{
    struct compare
    {
        bool operator()(const std::pair<char, double>& p1, const std::pair<char, double>& p2)
        {
            return p1.second < p2.second;
        }
    };

    std::sort(probabilities.rbegin(), probabilities.rend(), compare());
    std::vector<std::pair<char, double> > probs = probabilities;
    std::vector<std::vector<short>> codes;
    if(probabilities.size() != 0)
        codes.resize(probabilities.size(), std::vector<short>(probabilities.size() - 1));

    auto result = new Scheme();
    long length = probabilities.size();
    std::vector<long> lengths(length);
    GetCode(probs, codes, lengths, length);
    for(int i = 0; i < length; ++i)
    {
        result->insert(std::pair<char, std::vector<bool>>(probabilities.at(i).first, std::vector<bool>()));
        auto it = result->find(probabilities.at(i).first);
        for(int j = 0; j < lengths[i]; ++j)
            it->second.push_back(codes[i][j]);

    }
    return result;
}
std::pair<Scheme *, std::vector<bool> > HuffmanCoding::Encode(const std::string input)
{
    std::unordered_map<char, unsigned long> counter;
    for(int i = 0; i < input.length(); ++i)
    {
        if(counter.count(input[i]) == 0)
            counter.insert(std::pair<char, unsigned long>(input[i], 1));
        else
            ++counter[input[i]];
    }
    std::vector<std::pair<char, double>> probabilities;
    for(std::unordered_map<char, unsigned long>::iterator item = counter.begin();
            item != counter.end(); ++item)
        probabilities.push_back(std::pair<char, double>(item->first, (double)item->second/(double)input.length()));
    Scheme* scheme = GetCode(probabilities);
    return std::pair<Scheme*, std::vector<bool>>(scheme, Encode(*scheme, input));
}
std::pair<Scheme *, std::vector<bool> > HuffmanCoding::Encode(std::istream &input)
{
    if(input)
    {
        std::string content;
        char c;
        while(input.get(c))
            content += c;
        return Encode(content);
    }
    throw(errno);
}
std::vector<bool> HuffmanCoding::Encode(const Scheme& scheme, const std::string input)
{
    std::vector<bool> output;
    for(int i = 0; i < input.length(); ++i)
    {
        auto it = scheme.find(input[i]);
        if(it != scheme.end())
            output.insert(output.end(), it->second.begin(), it->second.end());
    }
    return output;
}
std::vector<bool> HuffmanCoding::Encode(const Scheme& scheme, std::istream &input)
{
    if(input)
    {
        std::string content;
        char c;
        while(input.get(c))
            content += c;
        return Encode(scheme, content);
    }
    throw(errno);
}
std::string HuffmanCoding::Decode(const Scheme& scheme, const std::vector<bool> input)
{
    std::unordered_map<std::string, char> reversed;
    for(auto it = scheme.begin(); it != scheme.end(); ++it)
    {
        std::string s = "";
        for(auto jt = it->second.begin(); jt != it->second.end(); ++jt)
            s += *jt;
        reversed.insert(std::pair<std::string, char>(s, it->first));
    }
    std::string output = "";
    std::string key  = "";
    for(auto i = input.begin(); i != input.end(); ++i)
    {
        key += *i;
        auto it = reversed.find(key);
        if(it == reversed.end())
            continue;
        key = "";
        output += it->second;
    }
    return output;
}
void HuffmanCoding::GetCode(std::vector<std::pair<char, double> > &probabilities,
                            std::vector<std::vector<short> > &codes, std::vector<long>& lengths, long length)
{
    if(length <= 2)
    {
        if(length == 0)
            return;
        if(length == 1)
        {
            codes[0][0] = 0;
            lengths[0] = 1;
            return;
        }
        codes[0][0] = 0;
        codes[1][0] = 1;
        lengths[0] = 1;
        lengths[1] = 1;
    }
    else
    {
        double probToPaste = probabilities[length - 2].second + probabilities[length - 1].second;
        long pivot = Up(length, probToPaste, probabilities);
        GetCode(probabilities, codes, lengths, length - 1);
        Down(length, pivot, codes, lengths);
    }
}

long HuffmanCoding::Up(long length, double probToPaste, std::vector<std::pair<char, double> > &probabilities)
{
    long pivot = 0;
    for(int i = length - 2; i > 0; --i)
    {
        if(probabilities[i - 1].second < probToPaste)
            probabilities[i] = probabilities[i - 1];
        else
        {
            pivot = i;
            break;
        }
    }
    probabilities[pivot].second = probToPaste;
    return pivot;
}

void HuffmanCoding::Down(long length, long pivot, std::vector<std::vector<short> > &codes, std::vector<long>& lengths)
{
    static std::vector<short> pivotVals;
    pivotVals = codes[pivot];
    long currLength = lengths[pivot];
    for(int i = pivot; i < length - 2; ++i)
    {
        codes[i] = codes[i + 1];
        lengths[i] = lengths[i + 1];
    }
    codes[length - 2] = pivotVals;
    codes[length - 1] = pivotVals;
    codes[length - 2] [currLength] = 0;
    codes[length - 1][currLength] = 1;
    lengths[length - 2] = currLength + 1;
    lengths[length - 1] = currLength + 1;
}
