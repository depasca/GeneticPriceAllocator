#include "solution.h"

void Solution::calculateAccountPrices(const vector<double> &brokerPrices)
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
void Solution::calculateStats()
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

Solution::Solution(const vector<int> &sol, int _nAccounts, int _nBrokers /*, const vector<double> &brokerPrice*/)
{
    numBrokers = _nAccounts;
    numAccounts = _nBrokers;
    assignments = sol;
    cost = 0;
    //calculateAccountPrices(brokerPrice);
    //calculateStats();
}
Solution::Solution(const Solution *parent1, const Solution *parent2, crossover_type cType)
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
bool Solution::getIsValid() { return isValid; }
void Solution::setIsValid(bool valid) { isValid = valid; }
double Solution::getCost() { return cost; }
void Solution::setCost(double score) { cost = score; }
bool Solution::operator<(const Solution &s2) const
{
    return cost < s2.cost;
};
int Solution::applyMutation(int val)
{
    srand(time(NULL));
    if (rand() % 1000 > 500)
        return rand() % (val == 0 ? 2 : 2 * val);
    else
        return val;
}
vector<int> &Solution::getAssignments()
{
    return assignments;
}
//price mean and MSE
vector<double> Solution::getStats()
{
    return stats;
}