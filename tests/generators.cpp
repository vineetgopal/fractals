#include <vector>
#include <algorithm>
#include <iostream>
#include <list>
#include <random>

using namespace std;

// Takes in an empty list of lists, and list of ints, and fills it in with appropriate test data
void uniform(std::vector<std::vector<int> >* lists, std::vector<int> * queries) {
  int NUM_LISTS = 500;
  int SIZE_LISTS = 600;
  int MAX_NUM = 100000;

  // generate each list by choose SIZE_LISTS random numbers for each list
  for (int i = 0; i < NUM_LISTS; i++) {
    std::vector<int> list;

    for (int j = 0; j < SIZE_LISTS; j++) {
      int num = rand() % MAX_NUM;
      list.push_back(num);
    }

    std::sort(list.begin(), list.end());
    lists->push_back(list);
  }

  // add some random queries
  queries->push_back(MAX_NUM / 3);
  queries->push_back(MAX_NUM / 2);
  queries->push_back(MAX_NUM - 1);
}

void gaussian(std::vector<std::vector<int> >* lists, std::vector<int> * queries) {
  int NUM_LISTS = 50;
  int SIZE_LISTS = 100;
  int MEAN = 50000;
  int STD_DEV = 20000;

  std::default_random_engine generator;
  std::normal_distribution<double> distribution(MEAN,STD_DEV);
  // generate each list by choose SIZE_LISTS random numbers for each list
  for (int i = 0; i < NUM_LISTS; i++) {
    std::vector<int> list;

    for (int j = 0; j < SIZE_LISTS; j++) {
      list.push_back((int) distribution(generator));
    }

    std::sort(list.begin(), list.end());
    lists->push_back(list);
  }

  // add some random queries
  queries->push_back(MEAN);
  queries->push_back(MEAN - STD_DEV);
  queries->push_back(MEAN + STD_DEV);
}