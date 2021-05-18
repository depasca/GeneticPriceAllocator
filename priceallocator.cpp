
#include "priceallocator.h"
using namespace std;

PriceAllocator::PriceAllocator(int _firstGenSize, double _growthRate, crossover_type _crossoverType, int _maxIterations)
{
    firstGenSize = _firstGenSize;
    gorwthRate = _growthRate;
    crossoverType = _crossoverType;
    maxIterations = _maxIterations;
}
void PriceAllocator::setInput(vector<int> &_acctQty, vector<int> &_brokerQty, vector<double> &_brokerPrice, vector<vector<int>> &_matrix)
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
vector<Solution *> PriceAllocator::getNewGeneration()
{
    vector<Solution *> ciccio;
    return ciccio;
}
double PriceAllocator::calculateCost(Solution *s)
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
bool PriceAllocator::calculateIsValid(Solution *s)
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
void PriceAllocator::printGeneration()
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
void PriceAllocator::printSolution(Solution *s)
{
    vector<int> sol = s->getAssignments();
    for (int i = 0; i < numAccounts; i++)
    {
        for (int j = 0; j < numBrokers; j++)
            cout << sol[i * numBrokers + j] << " ";
        cout << ": " << s->getCost() << endl;
    }
}
void PriceAllocator::run()
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
        //for (int i = 0; i < population.size(); i++)
        //    printSolution(population[i]);
        //cout << endl;
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