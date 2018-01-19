//
// Created by 董宸 on 20/01/2018.
//

#include "File.hpp"


namespace ResetCore{
    auto File::Open(const std::string &path, File::Permission) -> File * {
        return nullptr;
    }

    void File::Read(char *buffer, int size) {

    }

    void File::Read(int position, char *buffer, int size) {

    }

    void File::Write(char *buffer, int size) {

    }

    void File::Write(int position, char *buffer, int size) {

    }
}