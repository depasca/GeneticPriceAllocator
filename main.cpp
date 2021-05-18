#include "priceallocator.h"

int main(int argc, char **argv)
{
    PriceAllocator allocator = PriceAllocator(50, 2, MEAN, 10);
    vector<int> a{1000, 2000, 3000, 1000, 2000, 3000};
    vector<int> bq{2000, 2000, 5000, 5000};
    vector<double> bp{100, 101, 102, 103, 104};
    vector<vector<int>> m{{0, 1, 1, 1},
                          {1, 0, 1, 1},
                          {1, 1, 0, 1},
                          {1, 1, 1, 0},
                          {1, 1, 1, 1},
                          {1, 1, 1, 1}};
    allocator.setInput(a, bq, bp, m);
    allocator.run();
}