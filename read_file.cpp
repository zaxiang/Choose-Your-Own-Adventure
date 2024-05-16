#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "page.cpp"

std::vector<Page> read_file(std::string directoryPath, std::string filePath) {
  std::ifstream inputFile(filePath.c_str());
  if (!inputFile.is_open()) {
    std::cerr << "Unable to open file story.txt\n";
    std::exit(EXIT_FAILURE);
  }

  int maxPageNum = -1;  // To keep track of the maximum page number encountered
  std::vector<Page> pages;

  std::string line;
  while (std::getline(inputFile, line)) {
    if (!line.empty()) {  //ignore empty lines

      //========= For step 4 only: Page Variable ============
      if (line.find("$") != std::string::npos and
          line.find("$") >= 1) {  //if found $ and it has something before it
        //page number
        std::string page_num = line.substr(0, line.find("$"));
        char * p;
        size_t num = strtol(page_num.c_str(), &p, 10);
        if (*p != 0) {
          std::cerr << "Page Variable Line: string" << page_num
                    << "before $ cannot be converted to number\n";
          std::exit(EXIT_FAILURE);
        }
        //variable name
        std::string name =
            line.substr(line.find("$") + 1, line.find("=") - line.find("$") - 1);
        //variable value
        std::string val = line.substr(line.find("=") + 1);
        char * p_val;
        size_t value = strtol(val.c_str(), &p_val, 10);
        if (*p_val != 0) {
          std::cerr << "Page Variable Line: variable's value " << val
                    << "cannot be converted to number\n";
          std::exit(EXIT_FAILURE);
        }

        //add variable to the page
        if (num < pages.size()) {
          pages[num].addVar(name, value);
        }
        else {
          std::cerr << "Page Variable Line: the page doesn't exist yet\n";
          std::exit(EXIT_FAILURE);
        }
      }
      //======================================================

      else if (line.find("@") != std::string::npos and
               line.find("@") >= 1) {  //if found @ and it has something before it

        std::string page_num = line.substr(0, line.find("@"));
        char * p;
        size_t num = strtol(page_num.c_str(), &p, 10);

        if (*p != 0) {
          std::cerr << "Page Line: string" << page_num
                    << "before @ cannot be converted to number\n";
          std::exit(EXIT_FAILURE);
        }
        else if (*p == 0) {  //if the end points at NULL: the string is a valid number

          //check if page num is in order:
          if ((int)num != maxPageNum + 1) {
            std::cerr << "Page num is not in order: " << num << "should be "
                      << maxPageNum + 1 << " instead.\n";
            std::exit(EXIT_FAILURE);
          }
          maxPageNum = (int)num;  //update maxpagenum

          //get page type
          std::string page_type = line.substr(line.find("@") + 1, 1);
          if (page_type.compare("N") != 0 and page_type.compare("W") != 0 and
              page_type.compare("L") != 0) {
            std::cerr << "Page line format is incorrect: the string after @" << page_type
                      << " is not N/W/L\n";
            std::exit(EXIT_FAILURE);
          }

          //checking if have : after type
          std::string colon = line.substr(line.find("@") + 2, 1);
          if (colon.compare(":") != 0) {
            std::cerr << "Page line format is incorrect: the second char" << colon
                      << "after @ is not ':'\n";
            std::exit(EXIT_FAILURE);
          }

          //get page file name and check if we can open it
          std::string file_name = line.substr(line.find("@") + 3);  //get filename after :
          std::string full_file = directoryPath + '/' + file_name;
          std::ifstream file_content(
              full_file.c_str());  //check if we can open the page file
          if (!file_content.is_open()) {
            std::cerr << "Page Line: Unable to open page file " << file_name << "\n";
            std::exit(EXIT_FAILURE);
          }

          //build a page object
          Page page(num, page_type, directoryPath + "/" + file_name);
          pages.push_back(page);
        }
      }

      //Format Error checking
      else if (line.find("@") == 0 or line.find("$") == 0 or line.find("[") == 0) {
        std::cerr << "Find @, $, or [ as the first character in the line\n";
        std::exit(EXIT_FAILURE);
      }

      else if (line.find("$") == std::string::npos or
               line.find("@") == std::string::npos) {
        //if can't find @ or $ at all, then must be a choice line

        int is_condition = 0;
        std::string condition_name = "";
        size_t condition_value = 0;

        size_t curr_page_num;
        //========= For step 4 only: Choice Condition  =========
        if (line.find("[") != std::string::npos and line.find("[") >= 1) {  //if found [
          is_condition = 1;
          //get page number
          std::string curr_page_new = line.substr(0, line.find("["));
          char * p_end;
          curr_page_num = strtol(curr_page_new.c_str(), &p_end, 10);
          if (*p_end != 0) {
            std::cerr << "Condition Choice Line: string " << curr_page_new
                      << " before the [ is not a number\n";
            std::exit(EXIT_FAILURE);
          }
          //get condition
          std::string condition =
              line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1);
          condition_name = condition.substr(0, condition.find("="));
          std::string val = condition.substr(condition.find("=") + 1);
          char * p_end_val;
          condition_value = strtol(val.c_str(), &p_end_val, 10);
          if (*p_end_val != 0) {
            std::cerr << "Condition Choice Line: variable's value " << val
                      << " cannot be converted to number\n";
            std::exit(EXIT_FAILURE);
          }
        }
        //=======================================================
        //get the current page number if not a condition choice
        //didn't find [
        else {
          std::string curr_page = line.substr(0, line.find(":"));
          char * p_end;
          curr_page_num = strtol(curr_page.c_str(), &p_end, 10);
          if (*p_end != 0) {
            std::cerr << "Choice Line: string " << curr_page
                      << " before the first : is not a number\n";
            std::exit(EXIT_FAILURE);
          }
        }

        //next page num and choice content are same for both regular and condition choice
        //get next page number
        size_t first_colon = line.find(":");
        size_t second_colon = line.find(":", first_colon + 1);
        std::string next_page =
            line.substr(first_colon + 1, second_colon - first_colon - 1);
        char * p_end_2;
        size_t next_page_num = strtol(next_page.c_str(), &p_end_2, 10);
        if (*p_end_2 != 0) {
          std::cerr << "Choice Line: string " << next_page
                    << " after the first : and before the second : is not a number\n";
          std::exit(EXIT_FAILURE);
        }
        //get the choice content
        std::string content = line.substr(second_colon + 1);

        //build the choice object and update the page
        Choice choice(curr_page_num, content, next_page_num);
        if (is_condition == 1) {
          choice.addCondition(condition_name, condition_value);
        }

        if (curr_page_num >= pages.size()) {
          std::cerr << "Choice Line: No existing page found with page number "
                    << curr_page_num << "\n";
          std::exit(EXIT_FAILURE);
        }

        //win/lose must not have choice
        if (pages[curr_page_num].getType().compare("N") != 0) {
          std::cerr << "Choice Line: the page object at line " << curr_page_num
                    << " is not a normal type\n";
          std::exit(EXIT_FAILURE);
        }
        //add choice to the page object
        pages[curr_page_num].addChoice(choice);
      }
    }
  }
  inputFile.close();
  return pages;
}

//step2: check if the story is valid and print error message
bool check_file(std::vector<Page> pages) {
  int num_win = 0;
  int num_lose = 0;

  //check refered page is valid
  for (size_t i = 0; i < pages.size(); i++) {
    if (pages[i].getType().compare("W") == 0) {
      num_win += 1;
    }
    else if (pages[i].getType().compare("L") == 0) {
      num_lose += 1;
    }
    else {
      for (size_t j = 0; j < pages[i].getChoices().size(); j++) {
        if (pages[i].getChoices()[j].nextPage >= pages.size()) {
          std::cerr << "Story is not valid: next page refered "
                    << pages[i].getChoices()[j].nextPage
                    << " is greater than existing pages\n";
          std::exit(EXIT_FAILURE);
        }
        pages[pages[i].getChoices()[j].nextPage].addRefer();
      }
    }
  }

  //check if all have been refered
  for (size_t i = 1; i < pages.size(); i++) {
    if (pages[i].getRefered() == 0) {
      std::cerr << "Story is not valid: page number " << pages[i].getNum()
                << " is never refered\n";
      std::exit(EXIT_FAILURE);
    }
  }

  //check at least one win and one lose page
  if (num_win == 0 or num_lose == 0) {
    std::cerr << "Story is not valid: either no win page or no loss page\n";
    std::exit(EXIT_FAILURE);
  }

  return true;
}

//step3: find all wining paths

void findAllWinPaths(const std::vector<Page> & pages,
                     const Page & currentPage,
                     std::vector<std::vector<int> > & paths,
                     std::vector<int> & currentPath,
                     std::vector<bool> & visited) {
  int currentPageNumber = currentPage.getNum();

  // Check if this page has been visited in the current path
  if (visited[currentPageNumber]) {
    return;
  }

  // Mark the current page as visited
  visited[currentPageNumber] = true;
  currentPath.push_back(currentPageNumber);

  if (currentPage.getType().compare("W") == 0) {
    paths.push_back(currentPath);
  }

  for (size_t i = 0; i < currentPage.getChoices().size(); ++i) {
    size_t nextPageNumber = currentPage.getChoices()[i].nextPage;
    const Page & nextPage = pages[nextPageNumber];

    findAllWinPaths(pages, nextPage, paths, currentPath, visited);
  }

  // Backtrack: Reset visit status and remove the current page from the current path
  visited[currentPageNumber] = false;
  currentPath.pop_back();
}

//function to call recursive helper-func and return all wining paths in 2D vector
std::vector<std::vector<int> > getAllWinPaths(const std::vector<Page> & pages) {
  std::vector<std::vector<int> > paths;
  std::vector<int> currentPath;
  std::vector<bool> visited(pages.size(), false);  // To track visited pages

  // Start traversal from page 0
  const Page & startPage = pages[0];
  findAllWinPaths(pages, startPage, paths, currentPath, visited);

  return paths;
}