#include <stdint.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

template <uint64_t D>
class tree_cell {
    static_assert(D > 0, "dimension should be greater than zero");
    uint64_t code;
    uint64_t level;
public:
    tree_cell(uint64_t c, uint64_t l): code(c), level(l) {}
    inline bool operator<(const tree_cell& x) {
        return code < x.code;
    }
    bool overlap(const tree_cell& y) const {
        const uint64_t l = level > y.level ? level : y.level;
        return (code >> (l * D)) == (y.code >> (l * D));
    }
    static bool overlap(const tree_cell& x, const tree_cell& y) {
        const uint64_t l = x.level > y.level ? x.level : y.level;
        return (x.code >> (l * D)) == (y.code >> (l * D));
    }
    bool operator==(const tree_cell& y) {
        return overlap(*this, y);
    }
};
template <uint64_t D>
class tree {
    std::vector<tree_cell<D> > list;
public:
    void insert(tree_cell<D> t) {
        auto x = std::find(list.begin(), list.end(), t);
        if (x == list.end()) {
            list.push_back(t);
            std::sort(list.begin(), list.end());
        }
    }
};

int main() {
    tree<2> t;
    tree_cell<2> c(0, 0);
    t.insert(tree_cell<2>(0, 0));
    t.insert(tree_cell<2>(0, 0));
    std::cout << c.overlap(tree_cell<2>(1, 1)) << std::endl;
    return 0;
}
