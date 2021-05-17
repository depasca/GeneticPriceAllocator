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
    vector<double> accountPrices;
    vector<double> stats;
    vector<int> assignments;
    int numAccounts, numBrokers;
    double fitnessScore;
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
        fitnessScore = 0;
        //calculateAccountPrices(brokerPrice);
        //calculateStats();
    }
    Solution(const Solution *parent1, const Solution *parent2, crossover_type cType)
    {
        numBrokers = parent1->numBrokers;
        numAccounts = parent1->numAccounts;
        fitnessScore = 0;
        assignments = vector<int>(parent1->assignments);
        srand(time(NULL));
        switch (cType)
        {
        case ONEBYONE:
            for (int i = 0; i < parent2->assignments.size(); i++)
            {
                if (rand() % 100 >= 50)
                    assignments[i] = parent2->assignments[i];
            }
            break;
        case MEAN:
            for (int i = 0; i < parent2->assignments.size(); i++)
            {
                assignments[i] = (assignments[i] + parent2->assignments[i]) / 2;
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
                assignments[i] = parent2->assignments[i];
            break;
        }
        srand(time(NULL));
        for (int i = 0; i < parent2->assignments.size(); i++)
        {
            if (rand() % 100 >= 50)
                assignments[i] = parent2->assignments[i];
        }
        //calculateAccountPrices(brokerPrice);
        calculateStats();
    }
    bool getIsValid() { return isValid; }
    void setIsValid(bool valid) { isValid = valid; }
    double getFitnessScore() { return fitnessScore; }
    void setFitnessScore(double score) { fitnessScore = score; }
    bool operator<(const Solution &s2) const
    {
        return fitnessScore > s2.fitnessScore;
    };
    void applyMutation() {}
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
    double calculateSFitness(Solution *s)
    {
        double fitnessScore = 0;
        for (int i = 0; i < numAccounts; i++)
        {
            int totAcct = 0;
            for (int j = 0; j < numBrokers; j++)
            {
                totAcct += s->getAssignments()[i * numBrokers + j];
                if (matrix[i][j] > 0)
                    fitnessScore += s->getAssignments()[i * numBrokers + j];
                else
                    fitnessScore -= s->getAssignments()[i * numBrokers + j];
            }
            if (totAcct > acctQuantities[i])
                fitnessScore -= totAcct - acctQuantities[i];
        }
        return fitnessScore;
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
            if (population[i]->getIsValid())
            {
                cout << i << ": " << population[i]->getFitnessScore() << " / "
                     << maxScore << " - " << population[i]->getIsValid() << endl;
            }
        }
        cout << "best: " << population[0]->getFitnessScore() << endl;
    }
    void printSolution(Solution *s)
    {
        cout << s->getFitnessScore() << ":" << endl;
        vector<int> sol = s->getAssignments();
        for (int i = 0; i < numAccounts; i++)
        {
            for (int j = 0; j < numBrokers; j++)
                cout << sol[i * numBrokers + j] << " ";
            cout << endl;
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
            s->setFitnessScore(calculateSFitness(s));
            s->setIsValid(calculateIsValid(s));
            population.push_back(s);
        }
        sort(population.begin(), population.end());
        printGeneration();
        // loop until target or max iterations
        for (int n = 0; n < maxIterations; n++)
        {
            if (population[0]->getFitnessScore() >= maxScore)
            {
                printSolution(population[0]);
                return;
            }
            vector<Solution *> lastGen(population);
            // remove the worst, keep the best;
            srand(time(NULL));
            for (int n = 1; n < lastGen.size() * gorwthRate; n++)
            {
                int gen1 = 1 + rand() % lastGen.size() - 1;
                int gen2 = 1 + rand() % lastGen.size() - 1;
                Solution *s = new Solution(lastGen[gen1], lastGen[gen2], crossoverType);
                printSolution(lastGen[gen1]);
                cout << endl;
                printSolution(lastGen[gen2]);
                cout << endl;
                printSolution(s);
                cout << endl;
                s->setFitnessScore(calculateSFitness(s));
                s->setIsValid(calculateIsValid(s));
                population.push_back(s);
            }
            sort(population.begin(), population.end());
            printGeneration();
        }
        //  calc stats for each sol
        //  select elite
        //  new generation
        //  mutations
    }
};

int main(int argc, char **argv)
{

    PriceAllocator allocator = PriceAllocator(10, 1, SINGLE, 2);
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
    // growthRate

    //kick off
    //
}