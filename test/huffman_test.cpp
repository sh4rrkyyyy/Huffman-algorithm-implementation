#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "huffman.h"

TEST_CASE("get_frequencies_test_abcde"){
    std::ifstream file("../samples/get_frequencies_test1.txt");
    std::unordered_map<char, int> test_map = huffman::get_frequencies(file);
    std::unordered_map<char, int> correct_map = {{'a', 1}, {'b', 1}, {'c', 1}, {'d', 1}, {'e', 1}};
    for (auto it1 = test_map.begin(); it1 != test_map.end(); ++it1){
        auto it2 = correct_map.find(it1->first);
        CHECK(it1->second == it2->second);
    }  
}

TEST_CASE("get_frequencies_test_100_000_000_a"){
    std::ifstream file("../samples/get_frequencies_test2.txt");
    std::unordered_map<char, int> test_map = huffman::get_frequencies(file);
    std::unordered_map<char, int> correct_map = {{'a', 100000000}};
    for (auto it1 = test_map.begin(); it1 != test_map.end(); ++it1){
        auto it2 = correct_map.find(it1->first);
        CHECK(it1->second == it2->second);
    }
}

TEST_CASE("get_frequencies_test_empty"){
    std::ifstream file("../samples/get_frequencies_test3.txt");
    std::unordered_map<char, int> test_map = huffman::get_frequencies(file);
    CHECK(test_map.empty());
}

TEST_CASE("HuffmanTree_get_codes_test_aaaabbbccd"){
    /*                   d c b a
    d(1) c(2) b(3) a(4)  0 0 0 0
    b(3) cd(3) a(4)      1 1 0 0 
    a(4) bcd(6)          2 2 1 0
    abcd(10)             3 3 2 1
    */
    std::unordered_map<char, int> m = {{'a', 4}, {'b', 3}, {'c', 2}, {'d', 1}};
    huffman::HuffmanTree tree = huffman::HuffmanTree(m);
    std::unordered_map<char, std::string> codes = tree.get_codes();
    CHECK(codes['a'].size() == 1);
    CHECK(codes['b'].size() == 2);
    CHECK(codes['c'].size() == 3);
    CHECK(codes['d'].size() == 3);
}

TEST_CASE("HuffmanTree_get_codes_test_1symbol"){
    std::unordered_map<char, int> m1 = {{'a', 1}};
    huffman::HuffmanTree tree1 = huffman::HuffmanTree(m1);
    std::unordered_map<char, std::string> codes1 = tree1.get_codes();
    CHECK(codes1['a'].size() == 1);

    std::unordered_map<char, int> m2 = {{'a', 99999999}};
    huffman::HuffmanTree tree2 = huffman::HuffmanTree(m2);
    std::unordered_map<char, std::string> codes2 = tree2.get_codes();
    CHECK(codes2['a'].size() == 1);
}

TEST_CASE("HuffmanTree_get_codes_test_2symbols"){
    std::unordered_map<char, int> m1 = {{'a', 1023042}, {'b', 1}};
    huffman::HuffmanTree tree1 = huffman::HuffmanTree(m1);
    std::unordered_map<char, std::string> codes1 = tree1.get_codes();
    CHECK(codes1['a'].size() == 1);
    CHECK(codes1['b'].size() == 1);

    std::unordered_map<char, int> m2 = {{'a', 99999999}, {'b', 1111111}};
    huffman::HuffmanTree tree2 = huffman::HuffmanTree(m2);
    std::unordered_map<char, std::string> codes2 = tree2.get_codes();
    CHECK(codes2['a'].size() == 1);
    CHECK(codes2['b'].size() == 1);
}

TEST_CASE("HuffmanTree_get_codes_test_same_frequencies"){
    std::unordered_map<char, int> m1 = {{'a', 1000000}, {'b', 1000000}, {'c', 1000000}, {'d', 1000000}};
    huffman::HuffmanTree tree1 = huffman::HuffmanTree(m1);
    std::unordered_map<char, std::string> codes1 = tree1.get_codes();
    CHECK(codes1['a'].size() == 2);
    CHECK(codes1['b'].size() == 2);
    CHECK(codes1['c'].size() == 2);
    CHECK(codes1['d'].size() == 2);
}

TEST_CASE("HuffmanTree_get_codes_one_big_frequency"){
    std::unordered_map<char, int> m = {{'a', 999999}, {'b', 100}, {'c', 100}, {'d', 100}};
    huffman::HuffmanTree tree = huffman::HuffmanTree(m);
    std::unordered_map<char, std::string> codes = tree.get_codes();
    std::multiset<int> sizes;
    CHECK(codes['a'].size() == 1);
    sizes.insert(codes['a'].size());
    sizes.insert(codes['b'].size());
    sizes.insert(codes['c'].size());
    sizes.insert(codes['d'].size());
    std::multiset<int> correct_sizes = {1, 2, 3, 3};
    CHECK(sizes == correct_sizes);
}

TEST_CASE("HuffmanTree_get_codes_two_big_frequencies"){
    std::unordered_map<char, int> m = {{'a', 999999}, {'b', 999998}, {'c', 100}, {'d', 100}, {'e', 100}, {'f', 100}, {'g', 100}, {'h', 100}};
    huffman::HuffmanTree tree = huffman::HuffmanTree(m);
    std::unordered_map<char, std::string> codes = tree.get_codes();
    std::multiset<int> sizes;
    CHECK(codes['a'].size() == 1);
    sizes.insert(codes['a'].size());
    sizes.insert(codes['b'].size());
    sizes.insert(codes['c'].size());
    sizes.insert(codes['d'].size());
    sizes.insert(codes['e'].size());
    sizes.insert(codes['f'].size());
    sizes.insert(codes['g'].size());
    sizes.insert(codes['h'].size());
    std::multiset<int> correct_sizes = {1, 2, 4, 4, 5, 5, 5, 5};
    CHECK(sizes == correct_sizes);
}

TEST_CASE("archive_test_empty"){
    std::ifstream file_in("samples/archive_test_empty.txt");
    std::ofstream file_out("samples/archive_test_empty_out");
    huffman::HuffmanArchiver archiver = huffman::HuffmanArchiver();
    archiver.archive(file_in, file_out);
    file_in.close();
    file_out.close();
    std::ifstream file_in_new("samples/archive_test_empty_out");
    file_in_new.clear();
    file_in_new.seekg(0);
    file_in_new.seekg(0, std::ios::end);
    CHECK(file_in_new.tellg() == 0);
}

TEST_CASE("archive_test_one_symbol"){
    std::ifstream file_in("samples/archive_test_one_symbol_input.txt");
    std::ofstream file_out("samples/archive_test_one_symbol_output");
    huffman::HuffmanArchiver archiver = huffman::HuffmanArchiver();
    archiver.archive(file_in, file_out);
    file_in.close();
    file_out.close();
    std::ifstream file_in_new("samples/archive_test_one_symbol_output");
    file_in_new.clear();
    file_in_new.seekg(0);

    size_t size;
    file_in_new.read((char*)&size, sizeof(size_t));
    CHECK(size == 1); // number or bytes
    char byte;
    file_in_new.get(byte);
    CHECK(byte == 'a'); // byte
    char code_size;
    file_in_new.get(code_size);
    CHECK(code_size == 1); // byte code size
    char code;
    file_in_new.get(code);
    CHECK(code == '0'); // byte code
    char rmd; // remainder of encoded text
    file_in_new.get(rmd);
    CHECK(rmd == 1);
    char encoded_text; // bytes of encoded text
    file_in_new.get(encoded_text);
    CHECK(encoded_text == 0);
}

TEST_CASE("archive_test_aaaabbbccd"){
    std::ifstream file_in("samples/archive_test_aaaabbbccd_in.txt");
    std::ofstream file_out("samples/archive_test_aaaabbbccd_out");
    huffman::HuffmanArchiver archiver = huffman::HuffmanArchiver();
    archiver.archive(file_in, file_out);
    file_in.close();
    file_out.close();
    std::ifstream file_in_new("samples/archive_test_aaaabbbccd_out");
    file_in_new.clear();
    file_in_new.seekg(0);

    size_t size;
    file_in_new.read((char*)&size, sizeof(size_t));
    CHECK(size == 4); // number or bytes
    char byte;
    std::unordered_map<char, std::string> m;
    for (int i = 0; i < 4; ++i){
        file_in_new.get(byte);
        if (byte == 'a'){
            file_in_new.get(byte);
            CHECK(byte == 1);
            std::string code = huffman::EMPTY_STRING;
            file_in_new.get(byte);
            CHECK(byte >= '0');
            CHECK(byte <= '1');
            code += byte;
            m['a'] = code;
        }
        else if (byte == 'b'){
            file_in_new.get(byte);
            CHECK(byte == 2);
            std::string code = huffman::EMPTY_STRING;
            for (int i = 0; i < 2; ++i){
                file_in_new.get(byte);
                CHECK(byte >= '0');
                CHECK(byte <= '1');
                code += byte;
            }
            m['b'] = code;
        }
        else if (byte == 'c'){
            file_in_new.get(byte);
            CHECK(byte == 3);
            std::string code = huffman::EMPTY_STRING;
            for (int i = 0; i < 3; ++i){
                file_in_new.get(byte);
                CHECK(byte >= '0');
                CHECK(byte <= '1');
                code += byte;
            }
            m['c'] = code;
        }
        else if (byte == 'd'){
            file_in_new.get(byte);
            CHECK(byte == 3);
            std::string code = huffman::EMPTY_STRING;
            for (int i = 0; i < 3; ++i){
                file_in_new.get(byte);
                CHECK(byte >= '0');
                CHECK(byte <= '1');
                code += byte;
            }
            m['d'] = code;
        }
        else{
            CHECK(false);
        }
    }
    char rmd;
    file_in_new.get(rmd);
    char correct_rmd = (1 * 4 + 2 * 3 + 3 * 2 + 3 * 1) % 8;
    CHECK(rmd == correct_rmd);
    std::string str = "";
    while(file_in_new.get(byte)){
        auto bits = std::bitset<8>(byte).to_string();
        if (file_in_new.peek() != EOF){
            str += bits;
        }
        else{
            str += bits.substr((8 - correct_rmd) % 8);
        }
    }
    std::string correct_str = m['a'] + m['a'] + m['a'] + m['a'] + m['b'] + m['b'] + m['b'] + m['c'] + m['c'] + m['d'];
    CHECK(correct_str == str);
}

std::string get_bytes(std::ifstream& in){
    char byte;
    std::string res;
    while(in.get(byte)){
        res += byte;
    }
    return res;
}
TEST_CASE("unarchive_test_empty"){
    std::ifstream file_in("samples/archive_test_empty_out");
    std::ofstream file_out("samples/unarchive_test_empty.txt");
    huffman::HuffmanArchiver archiver = huffman::HuffmanArchiver();
    archiver.unarchive(file_in, file_out);
    file_in.close();
    file_out.close();
    std::ifstream in1("samples/unarchive_test_empty.txt");
    in1.clear();
    in1.seekg(0);
    std::string res1 = get_bytes(in1);
    in1.close();
    std::ifstream in2("samples/archive_test_empty.txt");
    in2.clear();
    in2.seekg(0);
    std::string res2 = get_bytes(in2);
    CHECK(res1 == res2);
}
TEST_CASE("unarchive_test_one_symbol"){
    std::ifstream file_in("samples/archive_test_one_symbol_output");
    std::ofstream file_out("samples/unarchive_test_one_symbol.txt");
    huffman::HuffmanArchiver archiver = huffman::HuffmanArchiver();
    archiver.unarchive(file_in, file_out);
    file_in.close();
    file_out.close();
    std::ifstream in1("samples/unarchive_test_one_symbol.txt");
    in1.clear();
    in1.seekg(0);
    std::string res1 = get_bytes(in1);
    in1.close();
    std::ifstream in2("samples/archive_test_one_symbol_input.txt");
    in2.clear();
    in2.seekg(0);
    std::string res2 = get_bytes(in2);
    CHECK(res1 == res2);
}

TEST_CASE("unarchive_test_aaaabbbccd"){
    std::ifstream file_in("samples/archive_test_aaaabbbccd_out");
    std::ofstream file_out("samples/unarchive_test_aaaabbbccd_out.txt");
    huffman::HuffmanArchiver archiver = huffman::HuffmanArchiver();
    archiver.unarchive(file_in, file_out);
    file_in.close();
    file_out.close();
    std::ifstream in1("samples/unarchive_test_aaaabbbccd_out.txt");
    in1.clear();
    in1.seekg(0);
    std::string res1 = get_bytes(in1);
    in1.close();
    std::ifstream in2("samples/archive_test_aaaabbbccd_in.txt");
    in2.clear();
    in2.seekg(0);
    std::string res2 = get_bytes(in2);
    CHECK(res1 == res2);
}

TEST_CASE("five_mb_file_test"){
    std::ifstream file_in("samples/big_test_archive_book.txt");
    std::ofstream file_out("samples/big_test_archive_book_out");
    huffman::HuffmanArchiver archiver = huffman::HuffmanArchiver();
    archiver.archive(file_in, file_out);
    file_in.close();
    file_out.close();
    std::ifstream in("samples/big_test_archive_book_out");
    std::ofstream out("samples/big_test_archive_book_1.txt");
    archiver.unarchive(in, out);
    in.close();
    out.close();
    std::ifstream in1("samples/big_test_archive_book.txt");
    in1.clear();
    in1.seekg(0);
    std::string res1 = get_bytes(in1);
    in1.close();
    std::ifstream in2("samples/big_test_archive_book_1.txt");
    in2.clear();
    in2.seekg(0);
    std::string res2 = get_bytes(in2);
    CHECK(res1 == res2);
}


