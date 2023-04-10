// ДЗ - исследование процесса записи данных в файл в синхронном и многопоточном режимах
// 1. Реализовать функцию записи данных в файл в синхронном режиме
// 2. Реализовать функцию записи данных в файл в многопоточном режиме
// 3. Измерить время выполнения шага 1 и шага 2
// Примечание:
// 1. Использовать все доступные потоки (кол-во доступных получать через API)
// 2. Размер файла 5ГБ
// 3. Данные записываются блочно, размер блока данных = 0x1000 (4096)
// Вопросы:
// 1. Какая из функций выполнилась быстрее? Почему?
// 2. Как проводилось измерение? Какой API был использован и почему?
// 3. Как изменится время выполнения, если увеличить размер файла (например, 10ГБ)?
// 4. Как изменится время выполнения, если использовать в 2 раза меньше потоков? В 2 раза больше?

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

const std::string fileNameSync = "outputSync.bin";
const std::string fileNameMultithread = "outputMultithread.bin";
const size_t blockSize = 0x1000;
size_t fileSize = 5ull * 1024 * 1024 * 1024;
std::mutex mutex;

void syncWrite(const std::vector<char>& data) {
  std::ofstream file(fileNameSync, std::ios::binary);
  if (!file) {
    std::cerr << "Error\n";
    return;
  }

  for (size_t i = 0; i < data.size(); i +=  blockSize) {
    file.write(&data[i], blockSize);
  }
  file.close();
}

void writeThread(const std::vector<char>& data, size_t start, size_t end) {
    std::lock_guard<std::mutex> lock(mutex);
    std::ofstream file(fileNameMultithread, std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "Error\n";
        return;
    }
    for (size_t i = start; i < end; i += blockSize) {
        file.write(&data[i], blockSize);
    }
    file.close();
}

void writeMultithread(const std::vector<char>& data){
    size_t numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(numThreads);

    size_t dataSizePerOneThread = data.size() / numThreads;

    for (size_t i = 0; i < numThreads; ++i) {
        size_t start = i * dataSizePerOneThread;
        size_t end = (i == numThreads - 1) ? data.size() : (i + 1) * dataSizePerOneThread;
        threads.emplace_back(writeThread, std::cref(data), start, end);
    }
    
    for (auto& thread : threads) {
        thread.join();
    }

}

int main(){
    std::vector<char> data(fileSize);

    auto startSync = std::chrono::steady_clock::now();
    syncWrite(data);
    auto endSync = std::chrono::steady_clock::now();
    std::chrono::duration<double> durationSync = endSync - startSync;
    std::cout << "Time sync: " << durationSync.count() << " s\n";


    auto startMulti = std::chrono::steady_clock::now();
    writeMultithread(data);
    auto endMulti = std::chrono::steady_clock::now();
    std::chrono::duration<double> durationMulti = endMulti - startMulti;
    std::cout << "Time multithread: " << durationMulti.count() << " s\n";
    return 0;

}