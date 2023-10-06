// FileSearch.hpp

#ifndef FILESEARCH_HPP
#define FILESEARCH_HPP

#include <filesystem>
#include <set>

class FileSearch {
    private:
        std::set<std::filesystem::path> extensions = {".jpg", ".mp4", ".txt", ".par"};
        void ProcessMatchingFile(const std::filesystem::path& file_path, std::ofstream& outputFile);
        void FindMatchingFiles(const std::filesystem::path& directory, std::ofstream& outputFile, bool exMode);
    public:
        void Start(std::string outputFile, std::string searchDirectory, std::string outputDirectory, bool exMode);
};

#endif /* FILESEARCH_HPP */