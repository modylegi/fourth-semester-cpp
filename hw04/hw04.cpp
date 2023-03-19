#include <boost/program_options.hpp>
#include <boost/filesystem/operations.hpp>

#include <string>
#include <iostream>
#include <fstream>

namespace po = boost::program_options;
namespace fs = boost::filesystem;



std::string createRulesFile(){
  fs::path rulesFile("rules.txt");
  std::ofstream file(rulesFile.string());
  if (file.is_open()){
     file << "jpg:Images\n";
     file << "png:Images\n";
     file << "mp3:Music\n";
     file << "txt:Docs\n";
     file << "docx:Docs\n";
    }
  file.close();
  return fs::absolute(rulesFile).string();
}

std::string createFilesDir(){
  fs::create_directory("input_folder");
  fs::path input_folder("input_folder");
  fs::create_directory("input_folder/images");
  fs::create_directory("input_folder/text");
  fs::create_directory("input_folder/music");

  for(int i = 1; i <= 5; i++){
    std::string filenameJpg = "input_folder/images/picture" + std::to_string(i) + ".jpg";
    std::ofstream fileJpg(filenameJpg);
    std::string filenamePng = "input_folder/images/picture" + std::to_string(i) + ".png";
    std::ofstream filePng(filenamePng);
    std::string filenameOther = "input_folder/images/picture" + std::to_string(i) + ".newklrgnklsdbsfg";
    std::ofstream fileOther(filenameOther);
  }

  for(int i = 1; i <= 5; i++){
    std::string filename = "input_folder/music/song" + std::to_string(i) + ".mp3";
    std::ofstream file(filename);
    std::string filenameOther = "input_folder/music/song" + std::to_string(i) + ".ioqwrthfd";
    std::ofstream fileOther(filenameOther);

  }

  for(int i = 1; i <= 5; i++){
    std::string filenameDocx = "input_folder/text/document" + std::to_string(i) + ".docx";
    std::ofstream fileDocx(filenameDocx);
    std::string filenameTxt = "input_folder/text/document" + std::to_string(i) + ".txt";
    std::ofstream fileTxt(filenameTxt);
    std::string filenameOther = "input_folder/text/document" + std::to_string(i) + ".oiweitj";
    std::ofstream fileOther(filenameOther);

  }

  return fs::absolute(input_folder).string();
  
}



void hw04(std::string rulesFilePath, std::string filesDirPath){
  try{
    std::vector<std::string> dirs;
    std::vector<std::string> exts;
    std::ifstream rulesFile(rulesFilePath);
    std::string line;
    while (std::getline(rulesFile, line)) {
      std::istringstream ss(line);
      std::string dirName;
      std::string extName;
      std::getline(ss, extName,':');
      std::getline(ss, dirName);
      if (std::find(dirs.begin(), dirs.end(), dirName) == dirs.end()) {
          dirs.push_back(dirName);
      }
      if (std::find(exts.begin(), exts.end(), extName) == exts.end()) {
          exts.push_back(extName);
      }
    }

    dirs.push_back("Other");
    for(auto i:dirs){
      fs::create_directory(filesDirPath + "/" + i);
    }
    
    std::vector<fs::path> files;
    for(auto i: fs::recursive_directory_iterator(filesDirPath)){
      if(fs::is_regular_file(i)){
        files.push_back(i);
      }
    }
    

    for(auto& i: files){
      std::string extension = i.extension().string();
      std::string clearExtension = extension.erase(0,1);
      if(std::find(exts.begin(), exts.end(), clearExtension) != exts.end()){
      }else{
        fs::path oldPath("input_folder/"+i.parent_path().filename().string()+ "/" + i.filename().string());
        fs::path newPath("input_folder/Other/" + i.filename().string());
        fs::copy_file(oldPath, newPath);
        fs::remove(oldPath);
      }
      if(clearExtension == "txt" || clearExtension == "docx"){
        fs::path oldPath("input_folder/"+i.parent_path().filename().string()+ "/" + i.filename().string());
        fs::path newPath("input_folder/Docs/" + i.filename().string());
        fs::copy_file(oldPath, newPath);
        fs::remove(oldPath);
      } 
      if(clearExtension ==  "mp3"){
        fs::path oldPath("input_folder/"+i.parent_path().filename().string()+ "/" + i.filename().string());
        fs::path newPath("input_folder/Music/" + i.filename().string());
        fs::copy_file(oldPath, newPath);
        fs::remove(oldPath);
      }
      if(clearExtension ==  "png" || clearExtension ==  "jpg"){
        fs::path oldPath("input_folder/"+i.parent_path().filename().string()+ "/" + i.filename().string());
        fs::path newPath("input_folder/Images/" + i.filename().string());
        fs::copy_file(oldPath, newPath);
        fs::remove(oldPath);
      }
    }

    for(auto i: fs::directory_iterator(filesDirPath)){
      if(fs::is_directory(i)){
        if(std::find(dirs.begin(), dirs.end(), i.path().filename().string()) != dirs.end()){
        } else {
          fs::remove(i);
        } 
      }
    }
  } catch (const fs::filesystem_error& e) {
    std::cerr << "Error: " << "Data is already structured!\n" << e.what() << std::endl;
  }
}


// Код домашнего задания
  // Сортировщик файлов
  // На вход программе подается:
    // 1. путь к папке с файлами
    // 2. путь к файлу с правилами
  // Требуется: Выполнить сортировку файлов на основе правил (Фильтр-файл)
  // 
  // Фильтр-файл:
    // Тип (расширение):Папка
    // Пример = jpg:Images
  //
  // Пример:

    // rules
    // jpg:Images
    // png:Images
    // mp3:Music
    // txt:Docs
    // docx:Docs

    // input_folder
    // - music.mp3
    // - img.png
    // - pic.jpg
    // - inner_folder
    //  - exp1.jpg
    //  - exp2.txt
    //  - exp3.ext
    //  - exp4.docx

  // Результат работы программы
    // input_folder
    // Images
    //  - img.png
    //  - pic.jpg
    //  - exp1.jpg
    // Music
    //  - music.mp3
    // Docs
    //  - exp2.txt
    //  - exp4.docx
    // Other
    //  - exp3.ext

    // Требования
    // - Расширения: png, jpg, docx, txt, mp3
    // - Минимум 5 файлов на каждое расширение
    // - Содержимое файлов не важно
    // - Вложенные директории с файлами
int main(int argc, char** argv){
  
  // if(!fs::exists("rules.txt")){
  //   std::string pathRules = createRulesFile();
  //   std::cout << "path to rules: " << pathRules << "\n";

  // }
  // if(!fs::exists("input_folder")){
  //   std::string pathDir = createFilesDir();
  //   std::cout << "path to input folder: " << pathDir << "\n";
  // }
  // Если нету файлов.
  
  
  
  po::options_description desc("Allowed options");
    desc.add_options()
        ("input-folder,i", po::value<fs::path>(), "path to input folder")
        ("rules-file,r", po::value<fs::path>(), "path to rules file")
    ;

    po::positional_options_description p;
    p.add("input-folder", 1);
    p.add("rules-file", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("input-folder") && vm.count("rules-file")) {
        fs::path inputFolder = vm["input-folder"].as<fs::path>();
        fs::path rulesFile = vm["rules-file"].as<fs::path>();
        hw04(rulesFile.string(), inputFolder.string());
    } else {
        std::cout << "Usage: hw04 -r <path to rules> -i <path to input folder>" << std::endl;
    }

    
  return 0;
}