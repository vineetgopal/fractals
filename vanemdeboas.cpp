#include <vector>
#include <algorithm>
#include <iostream>
#include <list>
#include <math.h>
#include <stdio.h>
#include <algorithm>

class VanEmdeBoas {
  int height;
  std::vector<int> vEB;
  std::vector<int>* list;
  static const int base_height_size = 3;
  int queryIndex(int *vEB, int height, int q);
  int getHeight(int length) { return (int) ceil(log2((double) length)); }
  void populate(int *vEB, int height, int levels_below, int offset, std::vector<int>* data);
public:
  VanEmdeBoas(std::vector<int>* l);
  int query(int q);
  int queryIndex(int q) { return queryIndex(vEB.data(), height, q); }
};

VanEmdeBoas::VanEmdeBoas(std::vector<int>* l) {
  height = getHeight(l->size());
  vEB.resize((1 << height), 0);
  populate(vEB.data(), height, 0, 0, l);
  list = l;
}

// either write the contents of the BST at the position pointed to by vEB or recursively subdivide the space into an upper triangle and set of lower triangles
void VanEmdeBoas::populate(int *vEB, int _height, int levels_below, int offset, std::vector<int>* data) {
  if (_height <= base_height_size) {
    int index_offset = offset * (1 << (levels_below + _height));
    int root = (1 << (_height + levels_below - 1)) - 1;
    for (int i = 0; i < _height; i++) {
      for (int j = 0; j < (1 << i); j++) {
        int index = index_offset + root + j*((root + 1) << 1);
        *vEB =  (index < data->size()) ? (*data)[index] : INT_MAX;
        vEB++;
      }
      root = root >> 1;
    }
  } else {
    int height_top = (_height >> 1);
    int height_bottom = ((_height + 1) >> 1);
    populate(vEB, height_top, levels_below + height_bottom, offset, data);
    vEB += (1 << height_top) - 1;
    for (int i = 0; i < (1 << height_top); i++) {
      populate(vEB, height_bottom, levels_below, offset*(1 << height_top) + i, data);
      vEB += (1 << height_bottom) - 1;
    }
  }
}

int VanEmdeBoas::queryIndex(int *vEB, int _height, int q) {
  int index = 0;
  if (_height <= base_height_size) {
    // traverse upper triangle using heap ordering
    for (int i = 0; i < _height; i++) {
      if (*(vEB + index) > q) {
        index = ((index + 1) << 1) - 1; // go left
      } else {
        index = ((index + 1) << 1);
      }
    }
    index -= ((1 << _height) - 1);
  } else {
    // find out which leaf structure we need to follow
    int height_top = (_height >> 1);
    int height_bottom = ((_height + 1) >> 1);
    index = queryIndex(vEB, height_top, q);
    vEB += index * ((1 << height_bottom) - 1) + ((1 << height_top) - 1);     
    index *= (1 << height_bottom);
    index += queryIndex(vEB, height_bottom, q);
  }  
  return index;
}

int VanEmdeBoas::query(int q) {
  return (list->front() <= q) ? (*list)[queryIndex(vEB.data(), height, q) - 1] : INT_MIN;
}


class VanEmdeBoasSearch : public Base {
public:
  std::vector<std::vector<int> >* lists;
  std::vector<VanEmdeBoas> vEB;
  VanEmdeBoasSearch(std::vector<std::vector<int> >* l);
  virtual string name() {return "VanEmdeBoasSearch";}
  virtual void query(int q, std::vector<int>* results);
};

VanEmdeBoasSearch::VanEmdeBoasSearch(std::vector<std::vector<int> >* l) {
  lists = l;
  for (int i = 0; i < l->size(); i++) {
    VanEmdeBoas _vEB(&(*l)[i]);
    vEB.push_back(_vEB);
  }
}

void VanEmdeBoasSearch::query(int q, std::vector<int>* results) {
  for (int i = 0; i < lists->size(); i++) {
    results->push_back(vEB[i].query(q));
  }
}

