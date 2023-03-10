#include <cstring> //std::memcmp
#include <fstream> //std::ifstream
#include <iostream>
#include <string>
#include <vector>


// Код домашнего задания
  // Распарсить блоб
  // На вход подается блоб - бинарный файл
  // Требуется:
  //  a. распаковать данные в структуру Blob
  // СТРУКТУРА БЛОБА:
  // version - 2 байта
  // size - 4 байта
  // data - 12 байт
  //  b. написать 3 exception класса
  //    a. обрабатывает значение version = валидные значения [1-7] // version_exception
  //    b. обрабатывает значение size = валидное значение 12 (0xC)
  //    c. обрабатывает значение data = валидное значение "Hello, World" (12 байт без нуль-терминатора)
  //  с. написать обработчик 3 exception классов
  //  d (*). модифицировать распаковку данных в структуру Blob с учетом того, что version и size могут быть big-endian

#pragma pack(1)
struct Blob{
  uint16_t version;
  uint32_t size;
  char data[12];
};
#pragma pack(0)


class VersionException {
  public:
    VersionException(const std::string& msg) : msg_(msg) {};
    ~VersionException() {};

    std::string getMessage() const {return(msg_);}
  private:
    std::string msg_;

};


class SizeException {
  public:
    SizeException(const std::string& msg) : msg_(msg) {};
    ~SizeException() {};
    std::string getMessage() const {return(msg_);}
  private:
    std::string msg_;
};

class DataException {
  public:
    DataException(const std::string& msg) : msg_(msg) {};
    ~DataException() {};
    std::string getMessage() const {return(msg_);}
  private:
    std::string msg_;

};


void do_fs(const std::string &filename) {
  std::ifstream reader(filename, std::ios::in | std::ios::binary);
  if (reader.is_open()) {
    // read bytes from file to buffer
    std::vector<char> bytes;
    int data_sz = 18;
    bytes.resize(data_sz);
    reader.read(reinterpret_cast<char *>(bytes.data()), data_sz);

    Blob blob;
    std::memcpy(&blob.version, bytes.data(), 2);
    std::memcpy(&blob.size, bytes.data() + 2 , 4);
    std::memcpy(&blob.data, bytes.data() + 6 , 12);
    
    if (blob.version > 0 && blob.version < 8) {
      std::cout << "Version is correct"  << std::endl;
    } else {
      throw VersionException("валидные значения [1-7]");
    }
    
    std::uint32_t compareSize = 12;
    if(std::memcmp(bytes.data() + 2, reinterpret_cast<char *>(&compareSize), sizeof(blob.size)) == 0){
      std::cout << "Size is correct" << std::endl;
    } else {
      throw SizeException("валидное значение 12 (0xC)");
    }

    const char* compareData = "Hello, World";
    if (std::memcmp(bytes.data() + 6, compareData, sizeof(blob.data)) == 0) {
      std::cout << "Data is correct" << std::endl;
    } else {
      std::cout << "валидное значение 'Hello, World' (12 байт без нуль-терминатора)" << std::endl;
    }
  }
}

int main()
{

    try
    {
      do_fs("../bin_data_hw03_le.bin");
    }
    catch(const DataException& e)
    {
      std::cout << e.getMessage() << std::endl;
    
    }
    catch(const VersionException& e)
    {
      std::cout << e.getMessage() << std::endl;
    }
    catch(const SizeException& e)
    {
      std::cout << e.getMessage() << std::endl;
    }
}