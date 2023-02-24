#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <vector>
#include <iostream>


std::vector<int> hw1_test(std::vector<int> nums) {
  std::vector<int> copiedNums(nums.size());
  auto lastElemIt = std::copy_if(nums.begin(),nums.end(),copiedNums.begin(),[](int i){return i % 2 != 0;});
  copiedNums.resize(std::distance(copiedNums.begin(),lastElemIt));

  auto firstElemIt = std::remove_if(copiedNums.begin(),copiedNums.end(),[](int i){
        return i % 3 == 0 || i % 11 == 0;
    });
  copiedNums.erase(firstElemIt,copiedNums.end());

  int taskThree = 5;
  copiedNums.insert(copiedNums.begin(),taskThree);

  std::sort(copiedNums.begin(),copiedNums.end(),[](int a, int b){return a > b;});

  std::for_each(copiedNums.begin(),copiedNums.end(),[](int& i){
    if(i>=5 && i<=15){
            i = i * 2;
        }
  });

  return copiedNums;
}



TEST_CASE("hw1_test_one", "[test]") {
  REQUIRE(hw1_test(std::vector<int>{1, 2, 3, 4}) == std::vector<int>{10, 1});
}

TEST_CASE("hw1_test_two", "[test]") {
  REQUIRE(hw1_test(std::vector<int>{1, 2, 3, 4, 5}) == std::vector<int>{10, 10, 1});
}

TEST_CASE("hw1_test_three", "[test]") {
  REQUIRE(hw1_test(std::vector<int>{1, 2, 3, 4, 5, 7}) == std::vector<int>{14, 10, 10, 1});
}

