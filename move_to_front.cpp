#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>

struct MoveToFront {
    static void encode(std::istream &input, std::ostream &output) {
        char c;
        std::vector<char> what(256);
        for (int i = 0; i < 256; i++) {
            what[i] = (char) i;
        }
        int pos;
        while (input >> c) {
            pos = 0;
            while (what[pos] != c) {
                pos++;
            }
            output << what[pos];
            what.erase(what.begin() + pos);
            what.insert(what.begin(), c);
        }
    }

    static void decode(std::istream &input, std::ostream &output) {
        char c;
        std::vector<char> what(256);
        for (int i = 0; i < 256; i++) {
            what[i] = (char) i;
        }
        int pos;
        while (input >> c) {
            pos = (int) c;
            output << what[pos];
            int prev = what[pos];
            what.erase(what.begin() + pos);
            what.insert(what.begin(), prev);
        }
    }
};


int main(int argc, char **argv) {
    if (argc == 3) {
        if (std::strncmp(argv[0], "encode", 6) == 0) {
            std::ifstream input(argv[1]);
            std::ofstream output(argv[2]);
            MoveToFront::encode(input, output);
        }
        if (std::strncmp(argv[0], "decode", 6) == 0) {
            std::ifstream input(argv[1]);
            std::ofstream output(argv[2]);
            MoveToFront::decode(input, output);
        }
    }
}

