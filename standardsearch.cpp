#include <vector>
#include <algorithm>
#include <iostream>
#include <list>

using namespace std;

class StandardSearch : public Base {
public:
  std::vector<std::vector<int> >* lists;
  StandardSearch(std::vector<std::vector<int> >* l);
  virtual string name() {return "StandardSearch   ";}
  virtual void query(int q, std::vector<int>* results);
};

StandardSearch::StandardSearch(std::vector<std::vector<int> >* l) {
  lists = new std::vector<std::vector<int> >;
  for (int i = 0; i < l->size(); i++) {
    std::vector<int> vec(l->at(i));
    lists->push_back(vec);
  }
}

void StandardSearch::query(int q, std::vector<int>* results) {
  for (int i = 0; i < lists->size(); i++) {
    int predecessor = *(std::upper_bound(lists->at(i).begin(), lists->at(i).end(), q) - 1);
    results->push_back(predecessor);
  }
}