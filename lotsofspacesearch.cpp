#include <vector>
#include <algorithm>
#include <iostream>
#include <list>

using namespace std;

class LotsOfSpaceSearch : public Base {
public:
  std::vector<std::vector<int> >* all_predecessors;
  std::vector<std::vector<int> >* lists;
  std::vector<int> vEB;
  LotsOfSpaceSearch(std::vector<std::vector<int> >* l);
  virtual string name() {return "LotsOfSpaceSearch";}
  virtual void query(int q, std::vector<int>* results);
};

LotsOfSpaceSearch::LotsOfSpaceSearch(std::vector<std::vector<int> >* l) {
  // std::vector<int> vEB;
  lists = new std::vector<std::vector<int> >;
  for (int i = 0; i < l->size(); i++) {
    std::vector<int> list = l->at(i);
    for (int j = 0; j < list.size(); j++) {
      vEB.push_back(list[j]);
    }
    std::vector<int> vec(l->at(i));
    lists->push_back(vec);
  }

  std::sort(vEB.begin(), vEB.end());

  all_predecessors = new std::vector<std::vector<int> >;
  for (int i = 0; i < vEB.size(); i++) {
    std::vector<int> predecessors(l->size());
    int query = vEB[i];
    for (int j = 0; j < l->size(); j++) {
      std::vector<int> list = l->at(j);
      std::vector<int>::iterator it = std::upper_bound(list.begin(), list.end(), query);
      if (it > list.begin()) {
        predecessors[j] = *(it - 1);
      } else {
        predecessors[j] = -1;
      }
    }
    all_predecessors->push_back(predecessors);
  }
  cout << "\n";
}

void LotsOfSpaceSearch::query(int q, std::vector<int>* results) {
  int index = std::upper_bound(vEB.begin(), vEB.end(), q) - 1 - vEB.begin();
  results->insert(results->begin(), all_predecessors->at(index).begin(), all_predecessors->at(index).end());
}