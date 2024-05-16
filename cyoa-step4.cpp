#include "read_file.cpp"

int main(int argc, char * argv[]) {
  if (argc != 2) {
    std::cerr << "command input takes one filepath\n";
    return 1;
  }
  std::string directoryPath = argv[1];
  std::string filePath = directoryPath + "/story.txt";

  std::vector<Page> pages = read_file(directoryPath, filePath);
  bool check_status = check_file(pages);

  if (check_status != true) {
    std::cerr << "something wrong with check_file function";
    return 1;
  }

  //play game for step4, start at page0
  size_t x;
  size_t num = 0;
  std::cout << pages[num];  //print the page

  //keep prompting input choices if page is normal
  while (pages[num].getType().compare("N") == 0) {
    //enter the new while loop to check if the condition is met if there is any
    int unaviable = 1;  //change to aviable if condition if met or no condition
    while (unaviable == 1) {
      std::cin >> x;

      //check if input number is valid
      while (!std::cin.good() or x <= 0 or x > pages[num].getChoices().size()) {
        std::cin.clear();
        std::cout << "That is not a valid choice, please try again\n";
        std::cin >> x;
      }

      Choice curr_choice = pages[num].getChoices()[x - 1];  //valid choice

      //if the choice doesnt have a condition
      if (curr_choice.had_condition == false) {
        unaviable = 0;  //will exit the outer while loop
      }

      else {  //if the choice has condition
        std::vector<std::pair<std::string, size_t> > var_list = pages[num].getCurrVar();
        int found = 0;
        for (size_t i = 0; i < var_list.size(); i++) {
          //variable found and values match
          if (curr_choice.condition.first.compare(var_list[i].first) == 0 and
              curr_choice.condition.second == var_list[i].second) {
            unaviable = 0;
            found = 1;
            break;  //break this for loop (through variable list)
          }
          //variable found but values dont match: prompt new input, check again
          if (curr_choice.condition.first.compare(var_list[i].first) == 0 and
              curr_choice.condition.second != var_list[i].second) {
            std::cin.clear();
            std::cout << "That choice is not available at this time, "
                         "please try again\n";
            found = 1;
            break;  //break this for loop on the  variable list
          }
        }
        //if variable not found in the list
        if (found == 0) {
          if (curr_choice.condition.second == 0) {
            unaviable = 0;
          }
          else {
            std::cin.clear();
            std::cout << "That choice is not available at this "
                         "time, please try again\n";
          }
        }
      }
    }

    num = pages[num].getChoices()[x - 1].nextPage;
    pages[num].addVar_global();  //update the global variable list if choose this page
    std::cout << pages[num];
  }

  return 0;
}