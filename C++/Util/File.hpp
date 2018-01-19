//
// Created by 董宸 on 20/01/2018.
//

#ifndef RESETCORE_FILE_HPP
#define RESETCORE_FILE_HPP

#include <string>

namespace ResetCore{
    class File {
    public:

        enum Permission { kRead, kWrite, kReadWrite, kAppend };
        /**
         * 打开文件
         * @param path
         * @return
         */
        static auto Open(const std::string& path, Permission)->File*;
        /**
         * 读取文件
         * @param buffer
         * @param size
         */
        void Read(char* buffer, int size);
        void Read(int position, char* buffer, int size);
        /**
         * 写入文件
         * @param buffer
         * @param size
         */
        void Write(char* buffer, int size);
        void Write(int position, char* buffer, int size);

    private:
        std::string file_path_;
        int position_;

    };
}



#endif //RESETCORE_FILE_HPP
