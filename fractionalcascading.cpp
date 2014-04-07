#include <vector>
#include <algorithm>
#include <iostream>
#include <list>

using namespace std;

class StandardSearch : public Base {
public:
  std::vector<std::vector<int> >* lists;
  StandardSearch(std::vector<std::vector<int> >* l);
  virtual string name() {return "StandardSearch";}
  virtual std::vector<int>* query(int q);
};

StandardSearch::StandardSearch(std::vector<std::vector<int> >* l) {
  lists = new std::vector<std::vector<int> >;
  for (int i = 0; i < l->size(); i++) {
    std::vector<int> vec(l->at(i));
    lists->push_back(vec);
  }
}

std::vector<int>* StandardSearch::query(int q) {
  std::vector<int>* vec = new std::vector<int>(lists->size());
  for (int i = 0; i < lists->size(); i++) {
    int predecessor = *(std::upper_bound(lists->at(i).begin(), lists->at(i).end(), q) - 1);
    (*vec)[i] = predecessor;
  }
  return vec;
}