#ifndef __HUFFMANCODING_H_
#define __HUFFMANCODING_H_

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <iostream>

typedef std::unordered_map<char, std::vector<bool>> Scheme ;

class HuffmanCoding
{
    public:
        static HuffmanCoding& lib();
        Scheme* GetCode(
                std::vector<std::pair<char, double>>& probabilities);
        std::pair<Scheme*, std::vector<bool>> Encode(const std::string input);
        std::pair<Scheme*, std::vector<bool>> Encode(std::istream& input);
        std::vector<bool> Encode(const Scheme& scheme, const std::string input);
        std::vector<bool> Encode(const Scheme& scheme, std::istream& input);
        std::string Decode(const Scheme& scheme, const std::vector<bool> input);
    private:
        HuffmanCoding();
        HuffmanCoding(HuffmanCoding const&) = delete;
        void operator=(HuffmanCoding const&) = delete;

        void GetCode(std::vector<std::pair<char, double>>& probabilities,
                     std::vector<std::vector<short> > &codes, std::vector<long> &lengths, long length);
        long Up(long length, double probToPaste, std::vector<std::pair<char, double>>& probabilities);
        void Down(long length, long pivot, std::vector<std::vector<short>>& codes, std::vector<long> &lengths);
};

#endif
