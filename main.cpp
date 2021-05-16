#include <iostream>
#include <vector>
#include <random>

class Solution
{
    vector<double> accountPrice;
    vector<double> stats;

    void calculateAccountPrices(const vector<double> &brokerPrice)
    {
        for (int i = 0; i < sol.size(); i++)
        {
            long qty = 0;
            accountPrice[i] = 0;
            for (int j = 0; j < sol[i].size(); j++)
            {
                qty += sol[i][j];
                accountPrice[i] += sol[i][j] * brokerPrice[j];
            }
            accountPrice[i] /= qty;
        }
    }
    void calculateStats()
    {
        stats[0] = stats[1] = 0;
        for (int i = 0; i < accountPrice.size(); i++)
        {
            stats[0] += accountPrice[i];
        }
        stats[0] /= accountPrice.size();
        for (int i = 0; i < accountPrice.size(); i++)
        {
            stats[1] += (accountPrice[i] - stats[0]) * (accountPrice[i] - stats[0]);
        }
        stats[1] /= accountPrice.size();
    }

public:
    Solution(vector<vector<int>> sol, const vector<double> &brokerPrice)
    {
        accountPrice = new vector<double>(sol.size());
        stats = new vector<double>(2);
        calculateAccountPrices(brokerPrice);
        calculateStats();
    }
    void applyMutation() {}
    bool isCorrect() {}
    //price mean and MSE
    vector<double> getStats() { return stats; }
};

class PriceAllocator
{
    int firstGenSize;
    int genCount;
    vector<double> bestSolution;
    vector<Solution> population;
    double getFitnessScore(Solution s) {}
    vector<Solution> getNewGeneration() {}

public:
    PriceAllocator(int firstGenSize, int crossoverType, int maxIterations, int eliteSelectionNumber)
    {
    }
    void setInput(vector<int> &acctQty, vector<int> &brokerQty, vector<double> &brokerPrice, vector<vector<int>> &matrix)
    {
    }

    run()
    {
        // firt generation
        // loop until target or max iterations
        //  calc stats for each sol
        //  select elite
        //  new generation
        //  mutations
    }
};

int main(int argc, char **argv)
{
    //read config
    // firstGenSize
    // crossoverType (single, onebyone)
    // maxIterations
    // priceMSETarget
    // eliteSelectionNumber
    // mutationProbThreshold

    //read input from file
    // acctQtyList
    // brokerQtyList
    // brokerMatrix

    //kick off
    //
}