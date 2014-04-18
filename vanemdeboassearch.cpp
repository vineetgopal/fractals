#include <vector>
#include <algorithm>
#include <iostream>
#include <list>

using namespace std;

class VanEmdeBoasSearch : public Base {
public:
  std::vector<VanEmdeBoas> vEB;
  VanEmdeBoasSearch(std::vector<std::vector<int> >* l);
  virtual string name() {return "VanEmdeBoasSearch";}
  virtual void query(int q, std::vector<int>* results);
};

VanEmdeBoasSearch::VanEmdeBoasSearch(std::vector<std::vector<int> >* l) {
  for (int i = 0; i < l->size(); i++) {
    VanEmdeBoas _vEB(&(*l)[i]);
    vEB.push_back(_vEB);
  }
}

void VanEmdeBoasSearch::query(int q, std::vector<int>* results) {
  for (int i = 0; i < vEB.size(); i++) {
    results->push_back(vEB[i].query(q));
  }
}