#pragma once

#include <sstream>
#include <string>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <iostream>

enum FileType {
    generalFile = 0,
    symlink,
    symlinkd,
    directoryFile
};

class File {
public:
    /**
     * custom constructor
     */
    File(std::string name, FileType type);

    /**
     * copy constructor
     */
    File(const File& file);

    /**
     * move constructor
     */
     File(File&& file);

     ~File();

     std::string getPath() const;

     std::string getFileName() const;

     std::string getFileDate() const;

     std::string getFileTime() const;

     FileType getFileType() const;

     size_t getFileSize() const;

     /*
      * operator=
      */
     File& operator= (const File& file);

     /**
      * operator==
      */
      bool operator== (const File& file);

      /**
       * operator<<
       */
      friend std::ostream& operator<< (std::ostream& os, const File& file);

private:
    std::string m_path;
    std::string m_name;
    std::string m_date;
    std::string m_time;
    FileType m_type;
    size_t m_size = 0;
    void* m_dataPtr = nullptr;
};


File::File(std::string name, FileType type = FileType::generalFile) {
    // TODO: add empty name exception
    m_name = name;

    using namespace std::chrono;
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream dateStream;
    dateStream << std::put_time(std::localtime(&t), "%Y/%m/%d");
    // std::string dateString;
    // dateStream >> dateString;
    // std::cout << dateString << std::endl;
    dateStream >> m_date;

    std::stringstream timeStream;
    timeStream << std::put_time(std::localtime(&t), "%H:%M");
    // std::string timeString;
    // timeStream >> timeString;
    // std::cout << timeString << std::endl;
    timeStream >> m_time;

    m_type = type;
}

File::File(const File& file) {
    m_path = file.m_path;
    m_name = file.m_name;
    m_date = file.m_date;
    m_time = file.m_time;
    m_type = file.m_type;
    m_size = file.m_size;
}

File::File(File&& file) {
    m_path = file.m_path;
    m_name = file.m_name;
    m_date = file.m_date;
    m_time = file.m_time;
    m_type = file.m_type;
    m_size = file.m_size;
}

File::~File() {

}

std::string File::getPath() const {
    return m_path;
}

std::string File::getFileName() const {
    return m_name;
}

std::string File::getFileDate() const {
    return m_date;
}

std::string File::getFileTime() const {
    return m_time;
}

FileType File::getFileType() const {
    return m_type;
}

size_t File::getFileSize() const {
    return m_size;
}

File& File::operator= (const File& file) {
    if(this != &file) {
        m_name = file.m_name;
        m_date = file.m_date;
        m_time = file.m_time;
        m_type = file.m_type;
        m_size = file.m_size;
    }
    return *this;
}

bool File::operator== (const File& file) {
    if(m_name.size() == file.m_name.size()) {
        return m_name.compare(file.m_name);
    }
    return false;
}

std::ostream& operator<< (std::ostream& os, const File& file) {
    os << file.m_name;
    return os;
}
