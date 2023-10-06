#include <iostream>
#include <vector>
#include <filesystem>

#include <chrono>

#include <thread>

#include "AES/aes128.hpp"
#include "File Control/fileControl.hpp"
#include "File Search/fileSearch.hpp"

#include <mutex>
#include <condition_variable>

#include <fstream>
#include <string>

#include <cstring>

const int MAX_THREADS = 100;

std::mutex mtx;
std::condition_variable cv;
int activeThreads = 0;

// _mode = -1 null
// _mode =  0 enc
// _mode =  1 dec
int _mode = -1;
bool _keyInputed = false;

void Encryption(std::filesystem::path _pathSource, std::filesystem::path _pathReturn, unsigned char _key[16]) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        ++activeThreads;
    }

    auto start = std::chrono::high_resolution_clock::now();

    FileControl fileControl;
    std::vector <unsigned char> _data;
    
    if(fileControl.GetFileData(_pathSource, _data)){
        Aes128 aes128;
        aes128.Encryption(&_data, _key);

        if(!fileControl.SetFileData(_pathReturn, _data)){
            std::cout << "Encryption Error" << _pathSource << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time Encryption: " << duration.count() << " qs" << std::endl;
    
    {
        std::unique_lock<std::mutex> lock(mtx);
        --activeThreads;
    }
    
    cv.notify_one();
}

void Decryption(std::filesystem::path _pathSource, std::filesystem::path _pathReturn, unsigned char _key[16]) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        ++activeThreads;
    }

    auto start = std::chrono::high_resolution_clock::now();

    FileControl fileControl;
    std::vector <unsigned char> _data;

    start = std::chrono::high_resolution_clock::now();

    if(fileControl.GetFileData(_pathSource, _data)){
        Aes128 aes128;
        aes128.Decryption(&_data, _key);

        if(!fileControl.SetFileData(_pathReturn, _data)){
            std::cout << "Rollback Error" << _pathSource << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time Decryption: " << duration.count() << " qs" << std::endl;
    
    {
        std::unique_lock<std::mutex> lock(mtx);
        --activeThreads;
    }
    
    cv.notify_one();
}

int main(int argc, char *argv[]) {
    unsigned char _key[16];
    bool exMode = false;

    for(int i = 0; i < argc; i++){
        if(argv[i][0] == '-'){
            if(argv[i][1] == 'e'){
                _mode = 0;
            }else if(argv[i][1] == 'd'){
                _mode = 1;
            }else if(argv[i][1] == 'k'){
                const char *_arg = argv[i+1];
                if(strlen(_arg) == 16){
                    for(int j = 0; j < 16; j++){
                        _key[j] = _arg[j];
                    }
                    _keyInputed = true;
                }else{
                    std::cout << "Parameter Error!!";
                    return 0;
                }
            }else if(argv[i][1] == 'x'){
                exMode = true;
            }else{
                std::cout << "Parameter Error!!";
                return 0;
            }
        }
    }

    FileSearch fs;
    fs.Start("fet.fet", "FET", "", exMode);

    if(_mode == 0 && _keyInputed){
        std::vector<std::thread> threads;

        std::ifstream file("fet.fet");
        std::string path;

        if (!file.is_open()) {
            std::cerr << "File not opened!" << std::endl;
        }else{
            while (std::getline(file, path)) {
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [] { return activeThreads < MAX_THREADS; });
                }

                threads.emplace_back(Encryption, path, path, _key);
            }

            for (std::thread& thread : threads) {
                thread.join();
            }
        }

        file.close();
    }else if(_mode == 1 && _keyInputed){
        std::vector<std::thread> threads;

        std::ifstream file("fet.fet");
        std::string path;
        if (!file.is_open()) {
            std::cerr << "File not opened!" << std::endl;
        }else{
            while (std::getline(file, path)) {
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [] { return activeThreads < MAX_THREADS; });
                }

                threads.emplace_back(Decryption, path, path, _key);
            }

            for (std::thread& thread : threads) {
                thread.join();
            }
        }

        file.close();
    }
    return 0;
}
