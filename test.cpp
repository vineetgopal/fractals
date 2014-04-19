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
#include "lotsofspacesearch.cpp"
#include "fractionalcascading.cpp"
#include "vanemdeboas.cpp"
#include "vanemdeboassearch.cpp"
#include "cacheoblivious.cpp"
#include "tests/generators.cpp"

using namespace std;

#define STRUCTURE1 SimpleSearch
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
  structures.push_back(new LotsOfSpaceSearch(&lists));

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

      // TEST CORRECTNESS OF QUERIES
      
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

void write_test_to_file(string name, std::vector<std::vector<int> > lists, std::vector<int> queries) {
  ofstream myfile;
  myfile.open(name);
  for (int i = 0; i < lists.size(); i++) {
    myfile << "a ";
    for (int j = 0; j < lists[i].size(); j++) {
      myfile << lists[i][j] << " ";
    }
    myfile << "\n";
  }
  myfile << "\n";
  for (int i = 0; i < queries.size(); i++) {
    myfile << "q " << queries[i] << "\n";
  }
  myfile.close();
}

void test_generator(void (*f)(std::vector<std::vector<int> >*, std::vector<int>*)) {
  std::vector<std::vector<int> > lists;
  std::vector<int> queries;
  (*f)(&lists, &queries);

  // In case we want to save the testing data for some reason.
  write_test_to_file("testing.txt", lists, queries);

  std::vector<Base*> structures;

  cout << "---------------------------------------------------\n";
  cout << "Initialization\n";
  cout << "---------------------------------------------------\n";

  clock_t t = clock();

  t = clock(); structures.push_back(new SimpleSearch(&lists)); printf("%-25s%f\n", (*(structures.end()-1))->name().c_str(), (clock() - t) / (double) (CLOCKS_PER_SEC));
  t = clock(); structures.push_back(new VanEmdeBoasSearch(&lists)); printf("%-25s%f\n", (*(structures.end()-1))->name().c_str(), (clock() - t) / (double) (CLOCKS_PER_SEC));
  t = clock(); structures.push_back(new FractionalCascading(&lists)); printf("%-25s%f\n", (*(structures.end()-1))->name().c_str(), (clock() - t) / (double) (CLOCKS_PER_SEC));
  t = clock(); structures.push_back(new CacheOblivious(&lists)); printf("%-25s%f\n", (*(structures.end()-1))->name().c_str(), (clock() - t) / (double) (CLOCKS_PER_SEC));
  t = clock(); structures.push_back(new LotsOfSpaceSearch(&lists)); printf("%-25s%f\n", (*(structures.end()-1))->name().c_str(), (clock() - t) / (double) (CLOCKS_PER_SEC));

  cout << "------------------------------\n";
  for (int i = 0; i < queries.size(); i++) {
    int q = queries.at(i);
    cout << "Query: " << q << "\n";
    for (int j = 0; j < structures.size(); j++) {
      Base* structure = structures[j];
      clock_t t = clock();
      std::vector<int> results;
      structure->query(q, &results);
      printf("%-25s%f\n", structure->name().c_str(), (clock() - t) / (double) (CLOCKS_PER_SEC));
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

  test_generator(uniform);
  // test_generator(gaussian);
}