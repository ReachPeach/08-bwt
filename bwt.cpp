#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <cstring>
#include "move_to_front.cpp"
#include <set>
#include <fstream>

struct CircularSuffixArray {
private:
    size_t req_length;
    std::vector<int> p;
public:
    explicit CircularSuffixArray(const std::string &input) {
        std::vector<int> cnt, c;
        req_length = input.size();
        cnt.assign(256, 0);
        for (int i = 0; i < req_length; ++i) {
            ++cnt[input[i]];
        }
        for (int i = 1; i < 256; ++i) {
            cnt[i] += cnt[i - 1];
        }
        for (int i = 0; i < req_length; ++i) {
            p[--cnt[input[i]]] = i;
        }
        c[p[0]] = 0;
        int classes = 1;

        for (int i = 1; i < req_length; ++i) {
            if (input[p[i]] != input[p[i - 1]]) { ++classes; }
            else {
                c[p[i]] = classes - 1;
            }
        }
        std::vector<int> pn(req_length), cn(req_length);

        for (int h = 0; (1 << h) < req_length; ++h) {
            for (int i = 0; i < req_length; ++i) {
                pn[i] = p[i] - (1 << h);
                if (pn[i] < 0) {
                    pn[i] += req_length;
                }
            }
            cnt.clear();
            cnt.assign(classes, 0);

            for (int i = 0; i < req_length; ++i) {
                ++cnt[c[pn[i]]];
            }
            for (int i = 1; i < classes; ++i) {
                cnt[i] += cnt[i - 1];
            }
            for (int i = req_length - 1; i >= 0; --i) {
                p[--cnt[c[pn[i]]]] = pn[i];
            }
            cn[p[0]] = 0;
            classes = 1;

            for (int i = 1; i < req_length; ++i) {
                int mid1 = (p[i] + (1 << h)) % req_length;
                int mid2 = (p[i - 1] + (1 << h)) % req_length;
                if (c[p[i]] != c[p[i - 1]] || c[mid1] != c[mid2]) {
                    ++classes;
                } else {
                    cn[p[i]] = classes - 1;
                }
            }
            c.clear();
            c.insert(c.begin(), cn.begin(), cn.end());
        }
    }

    size_t size() const {
        return req_length;
    }

    size_t operator[](size_t n) const {
        return p[n];
    }
};

struct BWT {
    static void transform(std::istream &input, std::ostream &output) {
        std::string orig;
        getline(input, orig);
        CircularSuffixArray suffixArray(orig);
        std::string ans(orig.size(), ' ');

        int index_of_orig = -1;
        for (int i = 0; i < orig.size(); i++) {
            if (suffixArray[i] == 0) {
                index_of_orig = i;
            }
            int pos_under_transform = (orig.size() - 1 + suffixArray[i]) % (orig.size());
            ans[i] = orig[pos_under_transform];
        }

        output << ans << ' ' << index_of_orig;
    }

    static void inverseTransform(std::istream &input, std::ostream &output) {
        int index_of_orig;
        std::string transformed;
        input >> index_of_orig;
        std::getline(input, transformed);
        int pos = transformed.find(' ');
        transformed = transformed.substr(pos + 1);
        std::vector<std::pair<char, int>> sorted_suffix;
        int i = 0;
        for (char symbol: transformed) {
            sorted_suffix.emplace_back(symbol, i++);
        }
        std::stable_sort(sorted_suffix.begin(), sorted_suffix.end());
        std::string original(transformed.size(), ' ');

        for (i = 0; i < transformed.size(); i++) {
            index_of_orig = sorted_suffix[index_of_orig].second;
            original[i] = transformed[index_of_orig];
        }
        output << original;
    }

};

int main(int argc, char **argv) {
    if (argc == 3) {
        if (std::strncmp(argv[0], "transform", 10) == 0) {
            std::ifstream input(argv[1]);
            std::ofstream output(argv[2]);
            BWT::transform(input, output);
        }
        if (std::strncmp(argv[0], "inverseTransform", 17) == 0) {
            std::ifstream input(argv[1]);
            std::ofstream output(argv[2]);
            BWT::inverseTransform(input, output);
        }
    }
}

