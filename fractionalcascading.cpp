#include <vector>
#include <algorithm>
#include <iostream>
#include <list>

struct fractional_cascading_t {
  int value;
  int left_index;
  int down_index; // -1 implies real, non-negative value implies cascaded
  fractional_cascading_t(int, int, int);
  fractional_cascading_t();
};

fractional_cascading_t::fractional_cascading_t(int _value, int _left_index, int _down_index) {
  value = _value;
  left_index = _left_index;
  down_index = _down_index;
}

fractional_cascading_t::fractional_cascading_t() {
  value = 0;
  left_index = 0;
  down_index = 0; 
}

class FractionalCascading : public Base {
public:
  std::vector<std::vector<fractional_cascading_t> >* lists;
  std::vector<std::vector<int> >* original_lists;
  FractionalCascading(std::vector<std::vector<int> >* l);
  ~FractionalCascading();
  virtual string name() {return "FractionalCascading";}
  virtual void query(int q, std::vector<int>* results);
};

FractionalCascading::FractionalCascading(std::vector<std::vector<int> >* l) {
  original_lists = l;
  lists = new std::vector<std::vector<fractional_cascading_t> >;
  lists->reserve(l->size());
  int last_size = 0;
  for (int i = 0; i < l->size(); i++) {
    std::vector<fractional_cascading_t> list((*l)[i].size() + ((last_size + 1) >> 1));
    int last_cascaded_index = 0; // index in old list
    int last_real_index = 0; 
    int last_cascaded_pointer = 0; // pointer index in new list
    int last_real_pointer = 0; 
    int j;
    for (j = 0; (last_cascaded_index < last_size) && (last_real_index < (*l)[i].size()); j++) {
      if ((*l)[i][last_real_index] <= (*lists)[i - 1][last_cascaded_index].value) {
        list[j] = fractional_cascading_t((*l)[i][last_real_index++], last_cascaded_pointer, -1);
        last_real_pointer = j;
      } else {
        list[j] = fractional_cascading_t((*lists)[i - 1][last_cascaded_index].value, last_real_pointer, last_cascaded_index);
        last_cascaded_index += 2;
        last_cascaded_pointer = j;
      }
    }
    for (; last_real_index < (*l)[i].size(); j++) {
      list[j] = fractional_cascading_t((*l)[i][last_real_index++], last_cascaded_pointer, -1);
      last_real_pointer = j;
    }
    for (; last_cascaded_index < last_size; j++) {
      list[j] = fractional_cascading_t((*lists)[i - 1][last_cascaded_index].value, last_real_pointer, last_cascaded_index);
      last_cascaded_index += 2;
      last_cascaded_pointer = j;      
    }
    lists->push_back(list);
    last_size = (*lists)[i].size();
  }
}

FractionalCascading::~FractionalCascading() {
  delete lists;
}

void FractionalCascading::query(int q, std::vector<int>* results) {
  results->resize(lists->size(), 0);
  int left = 0;
  int right = (*lists)[lists->size() - 1].size() - 1;
  while (left < (right - 1)) {
    if ((*lists)[lists->size() - 1][(right + left) >> 1].value <= q) {
      left = (right + left) >> 1;
    } else {
      right = (right + left) >> 1;
    }
  }
  int pred = left;
  for (int i = lists->size() - 1; i >= 0; i--) {
    if ((pred < ((*lists)[i].size() - 1)) && ((*lists)[i][pred + 1].value <= q)) {
      pred++;
    }
    if ((*lists)[i][pred].down_index == -1) {
      // the predecessor is valid element
      (*results)[i] = (*lists)[i][pred].value; 
      pred = (*lists)[i][(*lists)[i][pred].left_index].down_index;
    } else {
      // cascade
      (*results)[i] = (*lists)[i][(*lists)[i][pred].left_index].value; 
      pred = (*lists)[i][pred].down_index;
    }
  }
}