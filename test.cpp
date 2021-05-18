#include "priceallocator.h"
#include "gtest/gtest.h"

TEST(PATest, Test1)
{
    PriceAllocator allocator = PriceAllocator(50, 2, MEAN, 10);
    vector<int> a{1000, 2000, 3000};
    vector<int> bq{3000, 3000};
    vector<double> bp{100, 101, 102, 103, 104};
    vector<vector<int>> m{{1, 1},
                          {1, 1},
                          {1, 1}};
    allocator.setInput(a, bq, bp, m);
    allocator.run();
}

TEST(PATest, Test2)
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