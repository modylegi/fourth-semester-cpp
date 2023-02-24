#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <vector>
#include <iostream>

// 1. Отсеять четные числа - на данном этапе новый вектор будет содержать только нечетные числа
std::vector<int> taskOne(std::vector<int> nums) {
  std::vector<int> copiedNums(nums.size());
  auto lastElemIt = std::copy_if(nums.begin(),nums.end(),copiedNums.begin(),[](int i){return i % 2 != 0;});
  copiedNums.resize(std::distance(copiedNums.begin(),lastElemIt));
  return copiedNums;
}

// 2. Отсеять числа, которые деляться на 3 или на 11
std::vector<int> taskTwo(std::vector<int> nums) {
  auto firstElemIt = std::remove_if(nums.begin(),nums.end(),[](int i){
        return i % 3 == 0 || i % 11 == 0;
    });
  nums.erase(firstElemIt,nums.end());
  return nums;
}

// 3. В начало вектора добавить 5
std::vector<int> taskThree(std::vector<int> nums) {
  int taskThree = 5;
  nums.insert(nums.begin(),taskThree);
  return nums;
}

// 4. Отсортировать по убыванию
std::vector<int> taskFour(std::vector<int> nums) {
  std::sort(nums.begin(),nums.end(),[](int a, int b){return a > b;});
  return nums;
}

// 5. Пробежаться по вектору и если встретилось число в диапазоне [5...15], то удвоить его
std::vector<int> taskFive(std::vector<int> nums) {
  std::for_each(nums.begin(),nums.end(),[](int& i){
    if(i>=5 && i<=15){
            i = i * 2;
        }
  });
  return nums;
}


// 6. Написать 2-3 теста

TEST_CASE("taskTwo", "[test]") {
  REQUIRE(taskTwo(std::vector<int>{1, 2, 3, 4, 9, 11}) == std::vector<int>{1, 2, 4});
}

TEST_CASE("taskFour", "[test]") {
  REQUIRE(taskFour(std::vector<int>{1, 2, 3, 4}) == std::vector<int>{4, 3, 2, 1});
}

TEST_CASE("taskFive", "[test]") {
  REQUIRE(taskFive(std::vector<int>{1, 2, 3, 4, 5, 6}) == std::vector<int>{1, 2, 3, 4, 10, 12});
}