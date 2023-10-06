#include "fileSearch.hpp"

#include <iostream>
#include <chrono>
#include <fstream>

void FileSearch::ProcessMatchingFile(const std::filesystem::path& file_path, std::ofstream& outputFile) {
    outputFile << file_path.string() << "\n";
}

void FileSearch::FindMatchingFiles(const std::filesystem::path& directory, std::ofstream& outputFile, bool exMode) {
    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (std::filesystem::is_directory(entry)) {
                FindMatchingFiles(entry, outputFile, exMode);
            } else if (std::filesystem::is_regular_file(entry) && extensions.count(entry.path().extension()) && exMode) {
                ProcessMatchingFile(entry, outputFile);
            } else if (std::filesystem::is_regular_file(entry)) {
                ProcessMatchingFile(entry, outputFile);
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void FileSearch::Start(std::string outputFileName, std::string searchDirectory, std::string outputDirectory, bool exMode){
    std::ofstream outputFile(outputFileName);

    if (!outputFile) {
        std::cerr << "The file was not created or opened!" << std::endl;
    }else{
        std::string search_directory = searchDirectory;
        std::filesystem::path directory_path(search_directory);
        
        if (std::filesystem::is_directory(directory_path)) {
            FindMatchingFiles(directory_path, outputFile, exMode);
        } else {
            std::cerr << "The specified directory is not valid." << std::endl;
        }

        outputFile.close();
    }
}