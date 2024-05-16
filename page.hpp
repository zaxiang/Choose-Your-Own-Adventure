#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Choice {
 public:
  size_t currPage;
  std::string choiceText;
  size_t nextPage;
  bool had_condition;
  std::pair<std::string, size_t> condition;
  Choice(size_t curr, const std::string & text, size_t next);
  void addCondition(std::string name, size_t vaue);
};

class Page {
 private:
  size_t pageNumber;
  std::string pageType;  // 'N' for Normal, 'W' for Win, 'L' for Lose
  std::string contentFile;
  std::vector<Choice> choices;
  int choice_num;
  int refered;  //num of times this page is refered
 public:
  bool has_var;
  std::vector<std::pair<std::string, size_t> > var;              //var set at this page
  static std::vector<std::pair<std::string, size_t> > var_list;  //keep track of all var

 public:
  Page(size_t num, const std::string type, const std::string file);
  void addChoice(const Choice & choice);
  void addVar_global();  //update global in step4
  void addVar(std::string name, size_t value);
  void addRefer() { refered += 1; }
  size_t getNum() const { return pageNumber; }
  std::string getType() const { return pageType; }
  int getRefered() const { return refered; }
  std::vector<Choice> getChoices() const { return choices; }
  std::vector<std::pair<std::string, size_t> > getCurrVar() const { return var_list; }
  friend std::ostream & operator<<(std::ostream & os, const Page & page);
};
std::vector<std::pair<std::string, size_t> > Page::var_list;  //declare static field
