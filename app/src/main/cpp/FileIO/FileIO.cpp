#include "FileIO.h"

FileIO::FileIO(std::string *file) {
    int line = 0;
    for (int i = 0; i < (*file).size(); i++) {
        if ((*file)[i] != '\n') {
            FileIO::getline((*file)[i], &line);
        } else {
            std::string buffer;
            contents.push_back(buffer);
            line++;
        }
    }
    FileIO::getValues();
}

void FileIO::getline(const char ch, int* line) {
    contents[(*line)].push_back(ch);
}

void FileIO::getValues() {
    for (auto& line : contents) {
        if (line.find("Kd") == 0) {
            std::istringstream stream(line.substr(3));// std::istringstream Stream a number till white space is encountered && line.substr(3) removes the "Kd " or "Ks " prefix, leaving only the numeric values ("0.021962 0.022371 0.023650").
            float r, g, b;
            stream >> r >> g >> b;
            Kd.push_back(r);
            Kd.push_back(g);
            Kd.push_back(b);
        } else if (line.find("Ks") == 0) {
            std::istringstream stream(line.substr(3));
            float r, g, b;
            stream >> r >> g >> b;
            Ks.push_back(r);
            Ks.push_back(g);
            Ks.push_back(b);
        } else if (line.find("d") == 0) {
            std::istringstream stream(line.substr(2));
            float value;
            stream >> value;
            d.push_back(value);
        }
    }
}