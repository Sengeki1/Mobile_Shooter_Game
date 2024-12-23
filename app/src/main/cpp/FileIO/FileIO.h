#ifndef SHOOTERGAME_FILEIO_H
#define SHOOTERGAME_FILEIO_H

#include <android/log.h>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <map>

class FileIO {
    public:
        FileIO(std::string* file);
        void getline(const char ch, int* line);
        void getValues();

        std::vector<float> Kd;
        std::vector<float> Ks;
        std::vector<float> d;
    private:
        std::vector<std::string>contents = { "" };
};


#endif //SHOOTERGAME_FILEIO_H
