using namespace std;
class Base {
public:
  // Should have constructor which takes std::vector<std::vector<int> >*
  virtual std::vector<int>* query(int q) = 0;
  virtual string name() = 0;
};