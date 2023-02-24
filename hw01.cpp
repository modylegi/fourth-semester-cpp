#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <vector>
#include <iostream>

std::vector<int> hw01_test(std::vector<int> nums) {
  // Код домашнего задания
  // Фильтрация массива
  // На вход подается список целочисленных чисел. Требуется отфильтровать по следующему правилу:
  // 1. Отсеять четные числа - на данном этапе новый вектор будет содержать только нечетные числа
  std::vector<int> firstTask(nums.size());
  auto lastElemIt = std::copy_if(nums.begin(),nums.end(),firstTask.begin(),[](int i){return i % 2 != 0;});
  firstTask.resize(std::distance(firstTask.begin(),lastElemIt));
  // 2. Отсеять числа, которые деляться на 3 или на 11
  auto firstElemIt = std::remove_if(nums.begin(),nums.end(),[](int i){
        return i % 3 == 0 || i % 11 == 0;
    });
  nums.erase(firstElemIt,nums.end());
  // 3. В начало вектора добавить 5
  int taskThree = 5;
  nums.insert(nums.begin(),taskThree);
  // 4. Отсортировать по убыванию
  std::sort(nums.begin(),nums.end(),[](int a, int b){return a > b;});
  // 5. Пробежаться по вектору и если встретилось число в диапазоне [5...15], то удвоить его
  std::for_each(nums.begin(),nums.end(),[](int& i){
    if(i>=5 && i<=15){
            i = i * 2;
        }
  });
  // 6. Написать 2-3 теста
  // Подсказки: std::copy_if, std::for_each
  
  return std::vector<int>(nums);
}



TEST_CASE("taskTwo", "[test]") {
  REQUIRE(hw01_test(std::vector<int>{1, 2, 3, 4, 9, 11}) == std::vector<int>{1, 2, 4});
}

TEST_CASE("taskFour", "[test]") {
  REQUIRE(hw01_test(std::vector<int>{1, 2, 3, 4}) == std::vector<int>{4, 3, 2, 1});
}

TEST_CASE("taskFive", "[test]") {
  REQUIRE(hw01_test(std::vector<int>{1, 2, 3, 4, 5, 6}) == std::vector<int>{1, 2, 3, 4, 10, 12});
}