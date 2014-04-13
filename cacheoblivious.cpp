#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include <list>
#include <math.h>
#include <stdio.h>
#include <algorithm>

struct funnel_t {
  int last_value;
  int index;
  std::vector<int>* list;
  funnel_t(int _last_value, int _index, std::vector<int>* _list);
  const int front() const { return (index < list->size() ? (*list)[index] : INT_MAX); }
  const int previous() const { return last_value; }
  void increment();
};

funnel_t::funnel_t(int _last_value, int _index, std::vector<int>* _list) {
  last_value = _last_value;
  index = _index;
  list = _list;
}

void funnel_t::increment() {
  last_value = front();
  index++;
}

struct Comp {
  bool operator() (const funnel_t& a, const funnel_t& b) { return (a.front() > b.front()); }
};

class CacheOblivious : public Base {
public:
  int num_lists;
  int height;
  std::vector<int>* list;
  std::vector<int>* vEB;  
  std::vector<int>* list_index;
  CacheOblivious(std::vector<std::vector<int> >* l);
  virtual string name() {return "CacheOblivious";}
  virtual std::vector<int>* query(int q);
};

CacheOblivious::CacheOblivious(std::vector<std::vector<int> >* l) {
  num_lists = l->size();
  list = new std::vector<int>; // the single long array where the data is stored
  std::vector<int> splitters; // the values that split merged array
  list_index = new std::vector<int>; // the index into list for each splitter
  priority_queue<funnel_t, vector<funnel_t>, Comp> funnel;
  vector<funnel_t> merge;
  int total_elements = 0;
  for (int i = 0; i < l->size(); i++) {
    funnel_t heap_element(INT_MIN, 0, &(*l)[i]);
    funnel.push(heap_element);
    merge.push_back(heap_element);
    total_elements += (*l)[i].size();
  }
  splitters.reserve(1 + total_elements/l->size());
  while (funnel.top().front() < INT_MAX) {
    for (int i = 0; i < l->size(); i++) { // only take every k'th element
      funnel_t tmp = funnel.top();
      funnel.pop();
      tmp.increment();
      funnel.push(tmp);
    }
    funnel_t splitter = funnel.top();
    funnel.pop();
    splitters.push_back(splitter.front());
    splitter.increment();
    funnel.push(splitter);
  }
  if (splitters.back() < INT_MAX)
    splitters.push_back(INT_MAX);
  
  vEB = new std::vector<int>(splitters.size(),0);
  height = VanEmdeBoas::getHeight(splitters.size());
  VanEmdeBoas::populate(vEB->data(), height, 0, 0, &splitters);
  list->reserve(total_elements + ((l->size() + 1)*splitters.size()));
  list_index->reserve(splitters.size());
  list->push_back(INT_MIN); // base for first bucket - manually add
  list_index->push_back(0);
  int cumulative_bucket_size = 0;
  for (int i = 0; i < splitters.size(); i++) {
    for (int j = 0; j < l->size(); j++) {
      list->push_back(merge[j].previous());
      while (merge[j].front() < splitters[i]) {
        list->push_back(merge[j].front());
        merge[j].increment();
        cumulative_bucket_size++;
      }
    }
    cumulative_bucket_size += (l->size() + 1);
    list_index->push_back(cumulative_bucket_size);
    list->push_back(splitters[i]);
  }
  list->push_back(INT_MAX);
}

std::vector<int>* CacheOblivious::query(int q) {
  std::vector<int>* vec = new std::vector<int>(num_lists);
  int bucket_index = VanEmdeBoas::queryIndex(vEB->data(), height, q); 
  int index = (*list_index)[bucket_index];
  int base = (*list)[index];
  index++;
  int list_id = 0;
  while (index < ((*list_index)[bucket_index + 1])) {
    if ((*list)[index] <= q) {
      (*vec)[list_id] = (*list)[index];
    }
    index++;
    if ((*list)[index] < base) {
      list_id++;
    }
  }
  return vec;
}

