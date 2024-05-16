#include "read_file.cpp"

int main(int argc, char * argv[]) {
  if (argc != 2) {
    std::cerr << "command input takes one filepath\n";
    return 1;
  }
  std::string directoryPath = argv[1];
  std::string filePath = directoryPath + "/story.txt";

  std::vector<Page> pages = read_file(directoryPath, filePath);

  //from step2: check if the story is valid
  bool check_status = check_file(pages);
  if (check_status != true) {
    std::cerr << "something wrong with the check_file function\n";
    return 1;
  }

  //step3:
  std::vector<std::vector<int> > paths = getAllWinPaths(pages);
  if (paths.size() >= 1) {
    for (size_t i = 0; i < paths.size(); i++) {
      std::vector<int> p = paths[i];
      for (size_t j = 0; j < p.size() - 1; j++) {
        std::cout << p[j];
        for (size_t x = 0; x < pages[p[j]].getChoices().size(); x++) {
          if ((int)pages[p[j]].getChoices()[x].nextPage == p[j + 1]) {
            std::cout << "(" << x + 1 << "),";
          }
        }
      }
      std::cout << p[p.size() - 1] << "(win)\n";
    }
  }
  return 0;
}
