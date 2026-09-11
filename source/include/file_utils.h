#include <fstream>
#include <filesystem>
#include <stdexcept>

//Read file into buffer
inline const char* readFile(std::filesystem::path path){
  if(!std::filesystem::exists(path)) throw std::runtime_error(std::string("Path does not exist: ") + path.string());

  char* buffer;
  try{
    auto size = std::filesystem::file_size(path);

    buffer = new char[size + 1];
    buffer[size] = '\0';

    std::ifstream in(path, std::ios::binary);
    in.read(buffer, size);
  }
  catch(...){
    throw std::runtime_error(std::string("Failed to read file: ") + path.string());
  }

  return buffer;
}

