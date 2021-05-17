#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
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
    vector<double> accountPrices;
    vector<double> stats;
    vector<int> assignments;
    int numAccounts, numBrokers;
    double cost;
    bool isValid;

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
    Solution(const vector<int> &sol, int _nAccounts, int _nBrokers /*, const vector<double> &brokerPrice*/)
    {
        numBrokers = _nAccounts;
        numAccounts = _nBrokers;
        assignments = sol;
        cost = 0;
        //calculateAccountPrices(brokerPrice);
        //calculateStats();
    }
    Solution(const Solution *parent1, const Solution *parent2, crossover_type cType)
    {
        numBrokers = parent1->numBrokers;
        numAccounts = parent1->numAccounts;
        cost = 0;
        assignments = parent1->assignments;
        srand(time(NULL));
        switch (cType)
        {
        case ONEBYONE:
            for (int i = 0; i < parent2->assignments.size(); i++)
            {
                if (rand() % 100 >= 50)
                    assignments[i] = parent2->assignments[i];
                applyMutation(assignments[i]);
            }
            break;
        case MEAN:
            for (int i = 0; i < parent2->assignments.size(); i++)
            {
                assignments[i] = round((assignments[i] + parent2->assignments[i]) / 2.0);
                applyMutation(assignments[i]);
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
            {
                assignments[i] = parent2->assignments[i];
                applyMutation(assignments[i]);
            }
            break;
        }
        //calculateAccountPrices(brokerPrice);
        calculateStats();
    }
    bool getIsValid() { return isValid; }
    void setIsValid(bool valid) { isValid = valid; }
    double getCost() { return cost; }
    void setCost(double score) { cost = score; }
    bool operator<(const Solution &s2) const
    {
        return cost < s2.cost;
    };
    int applyMutation(int val)
    {
        srand(time(NULL));
        if (rand() % 1000 > 500)
            return rand() % (2 * val);
        else
            return val;
    }
    vector<int> &getAssignments()
    {
        return assignments;
    }
    //price mean and MSE
    vector<double> getStats()
    {
        return stats;
    }
};

class PriceAllocator
{
    int firstGenSize;
    double gorwthRate;
    int maxIterations;
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
    double calculateCost(Solution *s)
    {
        double cost = 0;
        vector<int> acctTmp;
        for (int i = 0; i < numAccounts; i++)
        {
            int totAcct = 0;
            for (int j = 0; j < numBrokers; j++)
                totAcct += s->getAssignments()[i * numBrokers + j];
            cost += (totAcct - acctQuantities[i]) * (totAcct - acctQuantities[i]);
        }
        return cost / numAccounts; // MSE
    }
    bool calculateIsValid(Solution *s)
    {
        vector<int> acctTmp = acctQuantities;
        vector<int> brkTmp = brokerQuantities;
        for (int i = 0; i < numAccounts; i++)
        {
            for (int j = 0; j < numBrokers; j++)
            {
                if (s->getAssignments()[i * numBrokers + j] != 0 && matrix[i][j] == 0)
                    return false;
                acctTmp[i] -= s->getAssignments()[i * numBrokers + j];
                if (acctTmp[i] < 0)
                    return false;
                brkTmp[j] -= s->getAssignments()[i * numBrokers + j];
                if (brkTmp[j] < 0)
                    return false;
            }
        }
        return true;
    }
    static bool compareSolutions(Solution *s1, Solution *s2)
    {
        return s1->getCost() < s2->getCost();
    }

public:
    PriceAllocator(int _firstGenSize, double _growthRate, crossover_type _crossoverType, int _maxIterations)
    {
        firstGenSize = _firstGenSize;
        gorwthRate = _growthRate;
        crossoverType = _crossoverType;
        maxIterations = _maxIterations;
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
    void printGeneration()
    {
        for (int i = 0; i < population.size(); i++)
        {
            //if (population[i]->getIsValid())
            {
                cout << i << ": " << population[i]->getCost() << " - " << population[i]->getIsValid() << endl;
            }
        }
        cout << "best: " << population[0]->getCost() << endl;
        printSolution(population[0]);
        cout << endl;
    }
    void printSolution(Solution *s)
    {
        vector<int> sol = s->getAssignments();
        for (int i = 0; i < numAccounts; i++)
        {
            for (int j = 0; j < numBrokers; j++)
                cout << sol[i * numBrokers + j] << " ";
            cout << ": " << s->getCost() << endl;
        }
    }
    void run()
    {
        // firt generation
        srand(time(NULL));
        for (int n = 0; n < firstGenSize; n++)
        {
            vector<int> ass(numAccounts * numBrokers);
            for (int i = 0; i < acctQuantities.size(); i++)
            {
                for (int j = 0; j < brokerQuantities.size(); j++)
                {
                    ass[i * numBrokers + j] = matrix[i][j] > 0 ? rand() % acctQuantities[i] : 0;
                }
            }
            Solution *s = new Solution(ass, numAccounts, numBrokers);
            s->setCost(calculateCost(s));
            s->setIsValid(calculateIsValid(s));
            population.push_back(s);
        }
        //printGeneration();
        // loop until target or max iterations
        for (int k = 0; k < maxIterations; k++)
        {
            sort(population.begin(), population.end(), compareSolutions);
            //printGeneration();
            for (int i = 0; i < population.size(); i++)
                printSolution(population[i]);
            cout << endl;
            //cout << population.size() << " " << population[0]->getCost() << endl;
            if (population[0]->getCost() == 0)
                break;

            // remove the worst, keep the best;
            vector<Solution *> newGen;
            newGen.push_back(population[0]);
            srand(time(NULL));
            for (int n = 0; n < population.size() * gorwthRate - 1; n++)
            {
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::default_random_engine generator(seed);
                uniform_int_distribution<int> distribution(1, population.size() - 1);
                int gen1 = distribution(generator);
                int gen2 = distribution(generator);
                if (gen2 == gen1)
                    gen2 = (gen1 + 1) % (population.size() - 1);
                //cout << population.size() << " - combining " << gen1 << " and " << gen2 << endl;
                Solution *s = new Solution(population[gen1], population[gen2], crossoverType);
                //Solution *s = new Solution(lastGen[0], lastGen[n % lastGen.size()], crossoverType);
                s->setCost(calculateCost(s));
                s->setIsValid(calculateIsValid(s));
                newGen.push_back(s);
            }
            population = newGen;
            //printGeneration();
        }

        cout << "best solution: " << endl;
        printSolution(population[0]);
        //  calc stats for each sol
        //  select elite
        //  new generation
        //  mutations
    }
};

int main(int argc, char **argv)
{
    PriceAllocator allocator = PriceAllocator(50, 2, MEAN, 10);
    vector<int> a{6000};
    vector<int> bq{6000};
    vector<double> bp{100, 100};
    vector<vector<int>> m{{1}};
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
    // growthRate

    //kick off
    //
}