#pragma once

#include <vector>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <bitset>

namespace empty_file_constants{
    const size_t ORIGINAL_FILE_SIZE = 0;
    const size_t ARCHIVED_FILE_SIZE = 0;
    const size_t HELP_DATA = 0;
}

namespace huffman{

    const size_t MAX_CODE = 256;
    const size_t BYTE_SIZE = 8;
    const char CHAR_ZERO = '0';
    const char CHAR_ONE = '1';
    const std::string EMPTY_STRING = "";
    const size_t BIN_MODE = 2;

    std::unordered_map<char, int> get_frequencies(std::ifstream& in);  

class HuffmanArchiver{
public:
    void archive(std::ifstream& in, std::ofstream& out);
    void unarchive(std::ifstream& in, std::ofstream& out);
private:
    void unarchive_codes_input(std::ifstream& in);
    void unarchive_text_input(std::ifstream& in);
    void unarchive_output(std::ofstream& out);

    std::unordered_map<std::string, char> _codes;
    size_t _file_size = 0;
    size_t _codes_size = 0;
    char _last_byte_remainder;
    std::string _encoded_text = EMPTY_STRING;


};


struct HuffmanTreeNode{
    HuffmanTreeNode* left;
    HuffmanTreeNode* right;
    int frequency;
    int smb;
    
    HuffmanTreeNode();
    ~HuffmanTreeNode();
    HuffmanTreeNode(size_t frequency);
    
};
struct comparator{
    bool operator()(HuffmanTreeNode const* HTnode1, HuffmanTreeNode const* HTnode2) const{
        return HTnode1->frequency > HTnode2->frequency;
    }
};

class HuffmanTree{
public:
    HuffmanTree(std::unordered_map<char, int>& v);
    ~HuffmanTree();
    std::unordered_map<char, std::string>& get_codes();
private:
    HuffmanTreeNode* _root;
    std::unordered_map<char, std::string> _umap;

    void make_codes(HuffmanTreeNode* HTnode, std::string cur_code);


};


}
