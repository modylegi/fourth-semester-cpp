#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>

#include <vector>

std::vector<char> hw02_test(std::vector<int> ascii) {
  // Код домашнего задания
  // Преобразование ASCII кода в символьное представление (5 баллов)
  // На вход подается список - численное представление символов в ASCII виде
  // Требуется:
  //  a. представить ASCII код в виде символов (2 балла)
  //  b. отсортировать символы по убыванию (lambda) (1 балл)
  // Пример: input = {65, 66, 67}, output = {'C', 'B', 'A'}
  // Написать 5 тестов
  std::vector<char> letters(ascii.size());
  std::transform(ascii.begin(),ascii.end(),letters.begin(),[](int i){
    return static_cast<char>(i);
  });
  std::sort(letters.begin(),letters.end(),[](int a, int b){return a > b;});
  return letters;
}

struct Blob{
  char data[26];
};


void hw02_complex_test() {
  
  Blob blob;

  std::ifstream fs_rd("hw02_data.bin", std::ios::in | std::ios::binary);
  fs_rd.read(reinterpret_cast<char*>(&blob), sizeof(blob));
  fs_rd.close();
  

  for(const auto& item: blob.data){
    std::cout << item << "";
  }

  // Код домашнего задания (*)
  // Интерпретация блоб в виде структуры (10 баллов)
  // На вход подается бинарный файл (блоб)
  // Требуется:
  //  a. понять структуру блоба
  //  b. реализовать структуру на С++
  //  с. реализовать распаковку блоба в структуру
  //  d. вывести данные на консоль
  //  e. написать небольшой отчет по исследованию и реализации
}






TEST_CASE("hw02_1", "[test]") {
  REQUIRE(hw02_test(std::vector<int>{68, 69, 70}) == std::vector<char>{'F', 'E', 'D'});
}

TEST_CASE("hw02_2", "[test]") {
  REQUIRE(hw02_test(std::vector<int>{70, 71, 72}) == std::vector<char>{'H', 'G', 'F'});
}

TEST_CASE("hw02_3", "[test]") {
  REQUIRE(hw02_test(std::vector<int>{73, 74, 75}) == std::vector<char>{'K', 'J', 'I'});
}

TEST_CASE("hw02_4", "[test]") {
  REQUIRE(hw02_test(std::vector<int>{76, 77, 78}) == std::vector<char>{'N', 'M', 'L'});
}

TEST_CASE("hw02_5", "[test]") {
  REQUIRE(hw02_test(std::vector<int>{79, 80, 81}) == std::vector<char>{'Q', 'P', 'O'});
}