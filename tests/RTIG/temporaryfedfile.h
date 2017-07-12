#ifndef TEMPORARYFEDFILE_H
#define TEMPORARYFEDFILE_H

#include <string>

class TemporaryFedFile {
public:
    TemporaryFedFile(const std::string& path);

    ~TemporaryFedFile();

private:
    void createFed();

    std::string my_path;
};

#endif // TEMPORARYFEDFILE_H
