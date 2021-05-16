#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

typedef enum
{
    SINGLE,
    ONEBYONE,
    MEAN
} crossover_type;

class Solution
{
    vector<int> accountDemand;
    vector<int> brokerOffer;
    vector<vector<int>> matrix;
    vector<double> accountPrices;
    vector<double> stats;
    vector<int> assignments;
    int numAccounts, numBrokers;
    double fitnessScore;

    void calculateAccountPrices(const vector<double> &brokerPrices)
    {
        accountPrices = vector<double>(numAccounts);
        for (int i = 0; i < numAccounts; i++)
        {
            long qty = 0;
            accountPrices[i] = 0;
            for (int j = 0; j < numBrokers; j++)
            {
                qty += assignments[i * numAccounts + j];
                accountPrices[i] += assignments[i + numAccounts + j] * brokerPrices[j];
            }
            accountPrices[i] /= qty;
        }
    }
    void calculateStats()
    {
        stats = vector<double>{0, 0};
        for (int i = 0; i < accountPrices.size(); i++)
        {
            stats[0] += accountPrices[i];
        }
        stats[0] /= accountPrices.size();
        for (int i = 0; i < accountPrices.size(); i++)
        {
            stats[1] += (accountPrices[i] - stats[0]) * (accountPrices[i] - stats[0]);
        }
        stats[1] /= accountPrices.size();
    }

public:
    Solution(const vector<int> &sol, const vector<int> &_accountDemand, const vector<int> &_brokerOffer, const vector<vector<int>> &_matrix /*, const vector<double> &brokerPrice*/)
    {
        accountDemand = _accountDemand;
        brokerOffer = _brokerOffer;
        numBrokers = _brokerOffer.size();
        numAccounts = accountDemand.size();
        matrix = _matrix;
        assignments = sol;
        fitnessScore = 0;
        //calculateAccountPrices(brokerPrice);
        //calculateStats();
    }
    Solution(const vector<int> &assignemnts1, const vector<int> &assignments2, crossover_type cType, const vector<int> &_accountDemand, const vector<int> &_brokerOffer, const vector<vector<int>> &_matrix, const vector<double> &brokerPrice)
    {
        accountDemand = _accountDemand;
        brokerOffer = _brokerOffer;
        numBrokers = _brokerOffer.size();
        numAccounts = accountDemand.size();
        matrix = _matrix;
        fitnessScore = 0;
        assignments = vector<int>(assignemnts1);
        srand(time(NULL));
        switch (cType)
        {
        case ONEBYONE:
            for (int i = 0; i < assignments2.size(); i++)
            {
                if (rand() % 100 >= 50)
                    assignments[i] = assignments2[i];
            }
            break;
        case MEAN:
            for (int i = 0; i < assignments2.size(); i++)
            {
                assignments[i] = (assignments[i] + assignments2[i]) / 2;
            }
            break;
        case SINGLE:
        default:
            int first = rand() % assignments.size();
            int last = rand() % assignments.size();
            if (first > last)
            {
                int tmp = first;
                first = last;
                last = tmp;
            }
            for (int i = first; i <= last; i++)
                assignments[i] = assignments2[i];
            break;
        }
        srand(time(NULL));
        for (int i = 0; i < assignments2.size(); i++)
        {
            if (rand() % 100 >= 50)
                assignments[i] = assignments2[i];
        }
        calculateAccountPrices(brokerPrice);
        calculateStats();
    }

    double calculateFitness()
    {
        fitnessScore = 0;
        for (int i = 0; i < accountDemand.size(); i++)
        {
            int totAcct = 0;
            for (int j = 0; j < brokerOffer.size(); j++)
            {
                totAcct += assignments[i * numBrokers + j];
                if (matrix[i][j] > 0)
                    fitnessScore += assignments[i * numBrokers + j];
                else
                    fitnessScore -= assignments[i * numBrokers + j];
            }
            if (totAcct > accountDemand[i])
                fitnessScore -= totAcct - accountDemand[i];
        }
        return fitnessScore;
    }
    void applyMutation() {}
    bool isValid()
    {
        vector<int> acctTmp = accountDemand;
        vector<int> brkTmp = brokerOffer;
        for (int i = 0; i < numAccounts; i++)
        {
            for (int j = 0; j < numBrokers; j++)
            {
                if (assignments[i * numBrokers + j] != 0 && matrix[i][j] == 0)
                    return false;
                acctTmp[i] -= assignments[i * numBrokers + j];
                if (acctTmp[i] < 0)
                    return false;
                brkTmp[j] -= assignments[i * numBrokers + j];
                if (brkTmp[j] < 0)
                    return false;
            }
        }
        return true;
    }
    //price mean and MSE
    vector<double> getStats() { return stats; }
};

class PriceAllocator
{
    int firstGenSize;
    int maxIterations;
    int eliteSelectionNum;
    int numAccounts;
    int numBrokers;
    int maxScore;
    crossover_type crossoverType;
    int genCount;
    vector<int> acctQuantities;
    vector<int> brokerQuantities;
    vector<double> brokerPrices;
    vector<vector<int>> matrix;
    vector<double> bestSolution;
    vector<Solution *> population;
    vector<Solution *> getNewGeneration()
    {
        vector<Solution *> ciccio;
        return ciccio;
    }
    static bool betterSolution(Solution *s1, Solution *s2)
    {
        return s1->calculateFitness() > s2->calculateFitness();
    }

public:
    PriceAllocator(int _firstGenSize, crossover_type _crossoverType, int _maxIterations, int _eliteSelectionNum)
    {
        firstGenSize = _firstGenSize;
        crossoverType = _crossoverType;
        maxIterations = _maxIterations;
        eliteSelectionNum = _eliteSelectionNum;
    }
    void setInput(vector<int> &_acctQty, vector<int> &_brokerQty, vector<double> &_brokerPrice, vector<vector<int>> &_matrix)
    {
        acctQuantities = _acctQty;
        brokerQuantities = _brokerQty;
        brokerPrices = _brokerPrice;
        matrix = _matrix;
        numAccounts = _acctQty.size();
        numBrokers = _brokerQty.size();
        maxScore = 0;
        for (int i = 0; i < acctQuantities.size(); i++)
            maxScore += acctQuantities[i];
    }

    void run()
    {
        // firt generation
        srand(time(NULL));
        for (int n = 0; n < numAccounts * numBrokers; n++)
        {
            vector<int> ass(numAccounts * numBrokers);
            for (int i = 0; i < acctQuantities.size(); i++)
            {
                for (int j = 0; j < brokerQuantities.size(); j++)
                {
                    ass[i * numBrokers + j] = matrix[i][j] > 0 ? rand() % acctQuantities[i] : 0;
                }
            }
            population.push_back(new Solution(ass, acctQuantities, brokerQuantities, matrix));
        }
        for (int i = 0; i < population.size(); i++)
        {
            sort(population.begin(), population.end(), betterSolution);
            if (population[i]->isValid())
            {
                cout << i << ": " << population[i]->calculateFitness() << " / "
                     << maxScore << " - " << population[i]->isValid() << endl;
            }
        }
        // loop until target or max iterations
        for (int n = 0; n < maxIterations; n++)
        {
        }
        //  calc stats for each sol
        //  select elite
        //  new generation
        //  mutations
    }
};

int main(int argc, char **argv)
{

    PriceAllocator allocator = PriceAllocator(1, SINGLE, 100, 2);
    vector<int> a{10, 20, 30};
    vector<int> bq{30, 30};
    vector<double> bp{100, 100};
    vector<vector<int>> m{{0, 1}, {0, 1}, {1, 1}};
    allocator.setInput(a, bq, bp, m);
    allocator.run();
    //read config
    // firstGenSize
    // crossoverType (single, onebyone)
    // maxIterations
    // priceMSETarget
    // eliteSelectionNumber
    // worstEliminationNumber
    // mutationProbThreshold

    //read input from file
    // acctQtyList
    // brokerQtyList
    // brokerMatrix

    //kick off
    //
}