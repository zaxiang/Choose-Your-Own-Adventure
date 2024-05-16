#include "read_file.cpp"

int main(int argc, char * argv[]) {
  if (argc != 2) {
    std::cerr << "command input takes one filepath\n";
    return 1;
  }
  std::string directoryPath = argv[1];
  std::string filePath = directoryPath + "/story.txt";

  std::vector<Page> pages = read_file(directoryPath, filePath);

  // Display pages
  for (size_t i = 0; i < pages.size(); i++) {
    std::cout << "Page " << pages[i].getNum() << "\n";
    std::cout << "==========\n";
    std::cout << pages[i];
  }
  return 0;
}
