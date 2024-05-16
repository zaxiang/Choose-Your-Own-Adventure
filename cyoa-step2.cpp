
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

  //play the game, start at page0

  size_t x;
  size_t num = 0;
  std::cout << pages[num];
  while (pages[num].getType().compare("N") == 0) {
    std::cin >> x;
    while (!std::cin.good() or x <= 0 or x > pages[num].getChoices().size()) {
      std::cin.clear();
      std::cout << "That is not a valid choice, please try again\n";
      std::cin >> x;
    }
    num = pages[num].getChoices()[x - 1].nextPage;
    std::cout << pages[num];
  }

  return 0;
}
