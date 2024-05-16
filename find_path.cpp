#include <vector>

#include "page.hpp"

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

std::vector<std::vector<int> > getAllWinPaths(const std::vector<Page> & pages) {
  std::vector<std::vector<int> > paths;
  std::vector<int> currentPath;
  std::vector<bool> visited(pages.size(), false);  // To track visited pages

  // Start traversal from page 0
  const Page & startPage = pages[0];
  findAllWinPaths(pages, startPage, paths, currentPath, visited);

  return paths;
}