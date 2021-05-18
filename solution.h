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

    void calculateAccountPrices(const vector<double> &brokerPrices);
    void calculateStats();

public:
    Solution(const vector<int> &sol, int _nAccounts, int _nBrokers /*, const vector<double> &brokerPrice*/);
    Solution(const Solution *parent1, const Solution *parent2, crossover_type cType);
    bool getIsValid();
    void setIsValid(bool valid);
    double getCost();
    void setCost(double score);
    bool operator<(const Solution &s2) const;
    int applyMutation(int val);
    vector<int> &getAssignments();
    //price mean and MSE
    vector<double> getStats();
};
