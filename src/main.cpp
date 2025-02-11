#include "huffman.h"
#include "main_constants.h"

int main(int argc, char* argv[]){
    if (argc != main_constants::arguments){
        return -1;
    }
    std::string oper, input, output;
    for (int i = 1; i < argc; ++i){
        std::string str = std::string(argv[i]);
        if (str == main_constants::UNARCHIVED_MODE || str == main_constants::ARCHIVED_MODE){
            oper = str;
        }
        else if (str == main_constants::INPUT_F_MODE || str == main_constants::INPUT_FILE_MODE){
            input = std::string(argv[i + 1]);
            i++;
        }
        else if (str == main_constants::INPUT_O_MODE || str == main_constants::INPUT_OUTPUT_MODE){
            output = std::string(argv[i + 1]);
            i++;
        }
        else {
            return -1;
        }
    }

    if (oper == main_constants::UNARCHIVED_MODE){
        std::ifstream file_in(input, std::ios_base::binary);
        if (!file_in.is_open()){
            throw std::runtime_error(main_constants::OPEN_FILE_ERROR);
        }
        std::ofstream file_out(output);
        if (!file_out.is_open()){
            throw std::runtime_error(main_constants::OPEN_FILE_ERROR);
        }
        try{
            huffman::HuffmanArchiver archiver = huffman::HuffmanArchiver();
            archiver.unarchive(file_in, file_out);
        }
        catch(std::invalid_argument& e){}

    }
    if (oper == main_constants::ARCHIVED_MODE){
        std::ifstream file_in(input);

        if (!file_in.is_open()){
            throw std::runtime_error(main_constants::OPEN_FILE_ERROR);
        }
        std::ofstream file_out(output, std::ios_base::binary);
        if (!file_out.is_open()){
            throw std::runtime_error(main_constants::OPEN_FILE_ERROR);
        }
        try{
            huffman::HuffmanArchiver archiver = huffman::HuffmanArchiver();
            archiver.archive(file_in, file_out);
        }
        catch(std::invalid_argument& e){}

    }

}
