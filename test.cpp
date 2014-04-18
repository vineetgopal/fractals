#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include "base.cpp"
#include "simplesearch.cpp"
#include "fractionalcascading.cpp"
#include "vanemdeboas.cpp"
#include "vanemdeboassearch.cpp"
#include "cacheoblivious.cpp"

using namespace std;

void print_vector(std::vector<int> vector) {
  for (int i = 0; i < vector.size(); i++) {
    cout << vector[i] << ", ";
  }
  cout << "\n";
}

void print_vectors(std::vector<std::vector<int> > vector) {
  for (int i = 0; i < vector.size(); i++) {
    print_vector(vector[i]);
  }
}

std::vector<int> parse_ints(string line) {
  std::stringstream stream(line);
  int n;
  std::vector<int> vec;
  while (true) {
    stream >> n;
    if (!stream) {
      break;
    }
    vec.push_back(n);
  }
  return vec;
}

void test_file(string file) {
  ifstream myfile (file);
  string line;
  std::vector<std::vector<int> > lists;
  std::vector<int> queries;
  std::vector<std::vector<int> > expected_answers;
  int query;
  while ( getline (myfile,line) )
  {
    if (line[0] == 'a') {
      std::vector<int> vec = parse_ints(line.substr(1));
      lists.push_back(vec);
    } else if (line[0] == 'q') {
      queries.push_back(std::stoi(line.substr(1)));
    } else if (line[0] == 'e') {
      std::vector<int> vec = parse_ints(line.substr(1));
      expected_answers.push_back(vec);
    }
  }

  myfile.close();

  std::vector<Base*> structures;

  //======ADD NEW STRUCTURES HERE======
  structures.push_back(new SimpleSearch(&lists));
  structures.push_back(new FractionalCascading(&lists));
  structures.push_back(new VanEmdeBoasSearch(&lists));
  structures.push_back(new CacheOblivious(&lists));

  cout << "------------------------------\n";
  cout << file << ": " << "\n";
  cout << "------------------------------\n";
  for (int i = 0; i < queries.size(); i++) {
    int q = queries[i];
    cout << "Query: " << q << "\n";
    for (int j = 0; j < structures.size(); j++) {
      Base* structure = structures[j];
      clock_t t = clock();
      std::vector<int> results;
      structure->query(q, &results);
      cout << fixed << structure->name() << "\t: " << (clock() - t) / (double) (CLOCKS_PER_SEC) << '\n';
      if (results != expected_answers[i]) {
        cout << "------------------------" << "\n";
        cout << "Test failed for " << structure->name() << "\n";
        cout << "Query was: " << q << "\n";
        cout << "Expected results: ";
        print_vector(expected_answers[i]);
        cout << "but actually got: ";
        print_vector(results);
        cout << "------------------------" << "\n";
        exit(1);
      }
    }
    cout << "\n";
  }
}

int main()
{
  std::vector<string> TEST_FILES;

  //======ADD NEW TEST FILES HERE======
  TEST_FILES.push_back("tests/consecutive.txt");

  for (int i = 0; i < TEST_FILES.size(); i++) {
    test_file(TEST_FILES[i]);
  }
}