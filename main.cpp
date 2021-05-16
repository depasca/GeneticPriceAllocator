#include <vector>
#include <random>

class Solution
{
public:
    Solution(vector<vector<int>> sol)
    {
    }
    void applyMutation() {}
    bool isCorrect() {}
    //price mean and std
    vector<double> getStats() {}
};

class PriceAllocator
{
    int firstGenSize;
    vector<Solution> population;
    double getFitnessScore(Solution s) {}
    vector<Solution> getNewGeneration() {}

public:
    PriceAllocator(vector<int> &acctQty, vector<int> &brokerQty, vector<vector<int>> &matrix)
    {
    }
    evolve(int maxIterations) {}
};

int main(int argc, char **argv)
{
    //read input from file
    //kick off
    //
}