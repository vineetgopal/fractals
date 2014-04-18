using namespace std;
class Base {
public:
  // Should have constructor which takes std::vector<std::vector<int> >*
  virtual void query(int q, std::vector<int>* results) = 0;
  virtual string name() = 0;
};