# GeneticPriceAllocator

A C++ implementation of a genetic algorithm for solving fair price allocation problems. This project is an experiment: it uses evolutionary computation techniques to find near-optimal solutions for distributing prices across multiple entities while maintaining fairness constraints. However it does not converge to an optimal solution, because while genetic algorithms work well in continuous space, the solution space of this problem is discrete. Nonetheless it is an interesting experiment that shows how genetic algorithms work.

🎯 Overview

GeneticPriceAllocator tackles the complex problem of price allocation using genetic algorithms - a metaheuristic optimization approach inspired by natural selection. The algorithm evolves a population of candidate solutions over successive generations, using operations like selection, crossover, and mutation to converge towards optimal price distributions.

🚀 Features

Genetic Algorithm Implementation: Custom-built genetic algorithm engine optimized for price allocation problems
Fair Distribution: equitable price allocation across all entities
Configurable Parameters: Adjustable population size, mutation rates, crossover strategies, and fitness functions
Efficient C++ Implementation: High-performance implementation for handling large-scale allocation problems
Testing Suite: test cases to validate algorithm behavior

📋 Requirements

- C++ compiler with C++11 support or later (g++, clang++, etc.)
- Make build system
- Standard C++ libraries
- Google Test (gtest) framework - required for running tests


⚙️ Configuration

The genetic algorithm can be tuned through various parameters:

Population Size: Number of candidate solutions in each generation
Mutation Rate: Probability of random changes in solutions
Crossover Rate: Probability of combining parent solutions
Selection Strategy: Method for choosing parents (tournament, roulette, etc.)
Termination Criteria: Convergence threshold or maximum generations
