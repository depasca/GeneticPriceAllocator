#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include "solution.h"
using namespace std;

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
    vector<Solution *> getNewGeneration();
    double calculateCost(Solution *s);
    bool calculateIsValid(Solution *s);

public:
    PriceAllocator(int _firstGenSize, double _growthRate, crossover_type _crossoverType, int _maxIterations);
    void setInput(vector<int> &_acctQty, vector<int> &_brokerQty, vector<double> &_brokerPrice, vector<vector<int>> &_matrix);
    void printGeneration();
    void printSolution(Solution *s);
    void run();
    static bool compareSolutions(Solution *s1, Solution *s2)
    {
        return s1->getCost() < s2->getCost();
    }
};