// FileControl.hpp

#ifndef FILECONTROL_HPP
#define FILECONTROL_HPP

#include <vector>
#include <filesystem>

class FileControl {
    private:

    public:
        bool GetFileData(std::filesystem::path& filePath, std::vector<unsigned char>& data);
        bool SetFileData(std::filesystem::path& filePath, std::vector<unsigned char> data);
};

#endif /* FILECONTROL_HPP */