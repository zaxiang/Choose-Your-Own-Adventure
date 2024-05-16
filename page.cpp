#include "page.hpp"

Choice::Choice(size_t curr, const std::string & text, size_t next) :
    currPage(curr), choiceText(text), nextPage(next), had_condition(false) {
}

void Choice::addCondition(std::string name, size_t value) {
  had_condition = true;
  condition.first = name;
  condition.second = value;
}

Page::Page(size_t num, const std::string type, const std::string file) :
    pageNumber(num),
    pageType(type),
    contentFile(file),
    choice_num(0),
    refered(0),
    has_var(false) {
}

void Page::addChoice(const Choice & choice) {
  choices.push_back(choice);
  choice_num += 1;
}

void Page::addVar_global() {
  //put all the variables in the current page to universal list
  if (has_var == true) {
    //update the universal list if choose this page
    for (size_t i = 0; i < var.size(); i++) {
      int found = 0;
      for (size_t j = 0; j < var_list.size(); j++) {
        if (var_list[j].first.compare(var[i].first) == 0) {
          var_list[j].second = var[i].second;
          found = 1;
        }
      }
      if (found == 0) {
        var_list.push_back(var[i]);
      }
    }
  }
}

void Page::addVar(std::string name, size_t value) {
  //define the var information for this page
  has_var = true;
  //append to the list
  std::pair<std::string, size_t> temp;
  temp.first = name;
  temp.second = value;
  var.push_back(temp);
}

std::ostream & operator<<(std::ostream & os, const Page & page) {
  std::ifstream contentStream(page.contentFile.c_str());
  if (contentStream.is_open()) {
    std::string line;
    while (std::getline(contentStream, line)) {
      os << line << "\n";
    }
    contentStream.close();
  }
  os << std::endl;  //blank line

  std::string win("W");
  std::string lose("L");
  std::string normal("N");
  if (page.pageType.compare(win) == 0) {
    os << "Congratulations! You have won. Hooray!\n";
  }
  else if (page.pageType.compare(lose) == 0) {
    os << "Sorry, you have lost. Better luck next time!\n";
  }
  else if (page.pageType.compare(normal) == 0) {
    os << "What would you like to do?\n\n";  //blank line

    //print each choice
    for (int i = 0; i < page.choice_num; i++) {
      Choice choice = page.choices[i];
      os << " " << i + 1 << ". ";  //the number of the choice

      //step4: check if condition has been met
      if (choice.had_condition == false) {
        os << choice.choiceText << "\n";  //print choice if no condition
      }
      else {
        int found = 0;  //indicator on variable found in the list
        for (size_t i = 0; i < page.var_list.size(); i++) {
          if (page.var_list[i].first.compare(choice.condition.first) == 0) {
            found = 1;
            if (page.var_list[i].second == choice.condition.second) {
              os << choice.choiceText << "\n";  //print the choice if condition met
              break;
            }
            else {
              os << "<UNAVAILABLE>\n";  //unavailable if found var but num doesnt match
              break;
            }
          }
        }
        if (found == 0) {
          if (choice.condition.second == 0) {
            os << choice.choiceText << "\n";
          }
          else {
            os << "<UNAVAILABLE>\n";  //unavialble if var not set & condition is not 0
          }
        }
      }
    }
  }

  return os;
}
