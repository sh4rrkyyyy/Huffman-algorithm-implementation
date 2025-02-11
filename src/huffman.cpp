#include "huffman.h"

namespace huffman{

std::unordered_map<char, int> get_frequencies(std::ifstream& in){
    std::unordered_map<char, int> freqs;
    char byte;
    while(in.get(byte)){
        freqs[byte]++;
    }
    return freqs;
}

HuffmanTreeNode::HuffmanTreeNode() : left(nullptr), right(nullptr) {}

HuffmanTreeNode::HuffmanTreeNode(size_t frequency) : left(nullptr), right(nullptr), frequency(frequency) {}

HuffmanTree::~HuffmanTree(){
    delete _root;
}

HuffmanTreeNode::~HuffmanTreeNode(){
    delete left;
    delete right;
}

HuffmanTree::HuffmanTree(std::unordered_map<char, int>& v){
    std::priority_queue<HuffmanTreeNode*, std::vector<HuffmanTreeNode*>, comparator> pq;
    for (const auto& p : v){
        HuffmanTreeNode* HTnode = new HuffmanTreeNode(p.second);
            HTnode->smb = p.first;
            pq.push(HTnode);
        
    }
    while(pq.size() > 1){
        HuffmanTreeNode* HTnode1 = pq.top();
        pq.pop();
        HuffmanTreeNode* HTnode2 = pq.top();
        pq.pop();
        HuffmanTreeNode* new_HTnode = new HuffmanTreeNode(HTnode1->frequency + HTnode2->frequency);
        new_HTnode->left = HTnode1;
        new_HTnode->right = HTnode2;
        pq.push(new_HTnode);

    }
    if (!pq.empty()){
        _root = pq.top();
        pq.pop();
    }
}

void HuffmanTree::make_codes(HuffmanTreeNode* HTnode, std::string str){
    if (HTnode->left == nullptr && HTnode->right == nullptr){
        _umap[HTnode->smb] = str;
        return;
    } 
    if(HTnode->left){
        make_codes(HTnode->left, str + CHAR_ZERO);
    }
    if (HTnode->right){
        make_codes(HTnode->right, str + CHAR_ONE);
    }
}

std::unordered_map<char, std::string>& HuffmanTree::get_codes(){
    if (_root == nullptr){
        return _umap;
    }
    if (_root->left == nullptr && _root->right == nullptr && _root != nullptr){
        _umap[_root->smb] = CHAR_ZERO;
        return _umap;
    }
    make_codes(_root, EMPTY_STRING);
    return _umap;
}

void HuffmanArchiver::archive(std::ifstream& in, std::ofstream& out){
    char byte;
    std::unordered_map<char, int> umap = get_frequencies(in);
    if (umap.empty()){
        std::cout << empty_file_constants::ORIGINAL_FILE_SIZE << std::endl;
        std::cout << empty_file_constants::ARCHIVED_FILE_SIZE << std::endl;
        std::cout << empty_file_constants::HELP_DATA << std::endl;
        return;
    }
    huffman::HuffmanTree tree = HuffmanTree(umap);
    std::unordered_map<char, std::string> codes = tree.get_codes();
    size_t size_of_codes = codes.size();
    out.write((char*)&size_of_codes, sizeof(size_t));
    for (const auto& p : codes){
        out << p.first << static_cast<uint8_t>(p.second.size()) << p.second;
    }
    in.clear();
    in.seekg(0);
    std::string res;
    size_t file_size = 0;
    while (in.get(byte)){
        res += codes[byte];
        file_size++;
    }
    out << static_cast<uint8_t>(res.size() % BYTE_SIZE);
    std::streampos help_data = out.tellp();
    for (size_t i = 0; i < res.length(); i += BYTE_SIZE){
        std::string str = res.substr(i, BYTE_SIZE);
        uint8_t byte = std::stoi(str, nullptr, BIN_MODE);
        out << byte;
    }
    std::cout << file_size << std::endl;
    std::cout << out.tellp() - help_data << std::endl;
    std::cout << help_data << std::endl;
}

void HuffmanArchiver::unarchive_codes_input(std::ifstream& in){
    size_t encoded_symbols = 0;
    in.read((char*)&encoded_symbols, sizeof(size_t));
    _file_size += sizeof(size_t);
    for (size_t i = 0; i < encoded_symbols; ++i){
        char symbol;
        in.get(symbol);
        _file_size++;
        symbol = static_cast<uint8_t>(symbol);
        char code_size;
        in.get(code_size);
        _file_size++;
        code_size = static_cast<int>(code_size);
        std::string code = EMPTY_STRING;
        char byte;
        for (int i = 0; i < code_size; ++i){
            in.get(byte);
            _file_size++;
            code += byte;
        }
        _codes[code] = symbol;
    }
    in.get(_last_byte_remainder);
    _file_size++;
    _last_byte_remainder = static_cast<int>(_last_byte_remainder);
}

void HuffmanArchiver::unarchive_text_input(std::ifstream& in){
    char byte;
     _codes_size = _file_size;
    while (in.get(byte)){
        _file_size++;
        std::string str_bits = std::bitset<BYTE_SIZE>(byte).to_string();
        if (in.peek() != EOF){
            _encoded_text += str_bits;
        }
        else{
            _encoded_text += str_bits.substr((BYTE_SIZE - _last_byte_remainder) % BYTE_SIZE);
        }
    }
}

void HuffmanArchiver::unarchive_output(std::ofstream& out){
    size_t out_size = 0;
    std::string str_res = EMPTY_STRING;
    for (size_t i = 0; i < _encoded_text.size(); ++i){
        str_res += _encoded_text[i];
        if (_codes.find(str_res) != _codes.end()){
            out << _codes[str_res];
            out_size++;
            str_res = EMPTY_STRING;
        }
    }
    std::cout << _file_size - _codes_size << std::endl;
    std::cout << out_size << std::endl;
    std::cout << _codes_size << std::endl;
}
void HuffmanArchiver::unarchive(std::ifstream& in, std::ofstream& out){
    if (in.peek() == EOF){
        std::cout << empty_file_constants::ORIGINAL_FILE_SIZE << std::endl;
        std::cout << empty_file_constants::ARCHIVED_FILE_SIZE << std::endl;
        std::cout << empty_file_constants::HELP_DATA << std::endl;
        return;
    }
    unarchive_codes_input(in);
    unarchive_text_input(in);
    unarchive_output(out);
}
}
