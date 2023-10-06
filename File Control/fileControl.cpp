#include "fileControl.hpp"

#include <iostream>
#include <fstream>

bool FileControl::GetFileData(std::filesystem::path& filePath, std::vector<unsigned char>& data){
    try{
        std::ifstream inputFile(filePath, std::ios::binary);
        if (!inputFile) {
            return false;
        }else{
            std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
            inputFile.close();
            
            data = buffer;
            return true;
        }
    }catch(...){
        return false;
    }
}

bool FileControl::SetFileData(std::filesystem::path& filePath, std::vector<unsigned char> data){
    try{
        std::ofstream outputFile(filePath, std::ios::binary);
        if (!outputFile) {
            return false;
        }else{
            outputFile.write(reinterpret_cast<char*>(data.data()), data.size());
            outputFile.close();
            return true;
        }
    }catch(...){
        return false;
    }
}