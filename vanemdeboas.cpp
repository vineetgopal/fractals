#include <vector>
#include <algorithm>
#include <iostream>
#include <list>
#include <math.h>
#include <stdio.h>
#include <algorithm>

class VanEmdeBoas : public Base {
public:
  std::vector<std::vector<int> >* lists;
  std::vector<std::vector<int> >* vEB;
  std::vector<int>* height;
  VanEmdeBoas(std::vector<std::vector<int> >* l);
  virtual string name() {return "VanEmdeBoas";}
  virtual std::vector<int>* query(int q);
  static const int baseHeightSize = 3;
private:
  void populate(int *vEB, int height, int levelsBelow, int offset, std::vector<int>* data);
  int queryIndex(int *vEB, int height, int q);
};

VanEmdeBoas::VanEmdeBoas(std::vector<std::vector<int> >* l) {
  height = new std::vector<int>;
  vEB = new std::vector<std::vector<int> >;
  lists = l;
  for (int i = 0; i < l->size(); i++) {
    height->push_back((int) ceil(log2((double) (*l)[i].size())));
    std::vector<int> vec((1 << (*height)[i]) - 1,0);
    populate(vec.data(), (*height)[i], 0, 0, &(*l)[i]);
    vEB->push_back(vec);
  }
}

// either write the contents of the BST at the position pointed to by vEB or recursively subdivide the space into an upper triangle and set of lower triangles
void VanEmdeBoas::populate(int *vEB, int height, int levelsBelow, int offset, std::vector<int>* data) {
  if (height <= baseHeightSize) {
    int indexOffset = offset * (1 << (levelsBelow + height));
    int root = (1 << (height + levelsBelow - 1)) - 1;
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < (1 << i); j++) {
        int index = indexOffset + root + j*((root + 1) << 1);
        if (index < data->size()) {
          *vEB = (*data)[index];
        } else {
          *vEB = INT_MAX;
        }
        vEB++;
      }
      root = root >> 1;
    }
  } else {
    int heightTop = (height >> 1);
    int heightBottom = ((height + 1) >> 1);
    populate(vEB, heightTop, levelsBelow + heightBottom, offset, data);
    vEB += (1 << heightTop) - 1;
    for (int i = 0; i < (1 << heightTop); i++) {
      populate(vEB, heightBottom, levelsBelow, offset*(1 << heightTop) + i, data);
      vEB += (1 << heightBottom) - 1;
    }
  }
}

  // loop through lists and call query(int *, int, int) on vector::data();
std::vector<int>* VanEmdeBoas::query(int q) {
  std::vector<int>* vec = new std::vector<int>(lists->size());
  for (int i = 0; i < lists->size(); i++) {
    (*vec)[i] = (*lists)[i][queryIndex((*vEB)[i].data(), (*height)[i], q) - 1];
  }
  return vec;
}

int VanEmdeBoas::queryIndex(int *vEB, int height, int q) {
  int index = 0;
  if (height <= baseHeightSize) {
    // traverse upper triangle using heap ordering
    for (int i = 0; i < height; i++) {
      if (*(vEB + index) > q) {
        index = ((index + 1) << 1) - 1; // go left
      } else {
        index = ((index + 1) << 1);
      }
    }
    index -= ((1 << height) - 1);
  } else {
    // find out which leaf structure we need to follow
    int heightTop = (height >> 1);
    int heightBottom = ((height + 1) >> 1);
    index = queryIndex(vEB, heightTop, q);
    vEB += index * ((1 << heightBottom) - 1) + ((1 << heightTop) - 1);     
    index *= (1 << heightBottom);
    index += queryIndex(vEB, heightBottom, q);
  }  
  return index;
}
