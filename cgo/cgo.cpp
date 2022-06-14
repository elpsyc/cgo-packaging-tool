#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <map>

std::string OriginFile = "D:/main_origin.out";  //  编译输出文件路径
std::string CompressFile = "D:/main.out";       //  压缩输出文件路径

/// <summary>
/// 遍历此opt，按对应key取对应值.
/// </summary>
std::map<std::string, std::string> opt;
/// <summary>
/// 序列化入参，自写..
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
void parse_arg(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        std::string tempStr = argv[i];
        if (tempStr.find("-") != -1) {
            if (i + 1 < argc) {
                std::string tempStrNext = argv[i + 1];
                if (tempStrNext.find("-") == -1)
                {
                    opt[argv[i]] = argv[i + 1];
                    continue;
                }
            }
            opt[argv[i]] = argv[i];
        }
    }
}

/// <summary>
/// 检查文件是否存在
/// </summary>
/// <param name="name 文件完整路径"></param>
/// <returns>true.存在 false.不存在</returns>
bool isFileExists(std::string name) {
    std::ifstream f(name.c_str());
    return f.good();
}

/// <summary>
/// 删除旧文件残留
/// </summary>
/// <param name="type 1.编译文件  2.压缩文件"></param>
void removeOldFile(int type) {
    if (type == 1) {
        if (isFileExists(OriginFile)) {
            if (DeleteFileA(OriginFile.c_str())) {
                printf("[-] 编译文件 删除成功\n");
            }
            else {
                printf("[err] 编译文件 删除失败,可能被占用..或无权限\n");
            }
        }
    }
    else if (type == 2) {

        if (isFileExists(CompressFile)) {
            if (DeleteFileA(CompressFile.c_str())) {
                printf("[-] 编译文件 删除成功\n");
            }
            else {
                printf("[err] 编译文件 删除失败,可能被占用..或无权限\n");
            }
        }
    }
}

int main(int argc, char* argv[]) {
    //  序列化命令行参数
    parse_arg(argc, argv);
    std::string Prj = opt["-i"];
    if (Prj == "" || Prj == "-i") {
        printf("[*] 参数异常,用法示例：\n cgo -i main.go\n");
        return 0;
    }

    //  移除旧的编译&&压缩结果
    printf("[*] 开始移除上次残留文件..\n");
    removeOldFile(1);
    removeOldFile(2);

    //  开始编译项目
    printf("[*] 开始编译文件..\n项目名:%s\n", Prj.c_str());
    std::string cmd = "set goos=linux&&go build -ldflags \"-w -s\" -o " + OriginFile;
    system(cmd.c_str());

    //  检查编译结果
    if (!isFileExists(OriginFile)) {
        printf("[err] 编译遇到错误,请检查代码..\n");
        return 0;
    }

    printf("[*] 开始压缩文件..\n");
    char compressCmd[1024];
    sprintf_s(compressCmd, "upx -9 %s -o %s", OriginFile.c_str(), CompressFile.c_str());
    system(compressCmd);

    //  打开资源管理器
    system("start d:/");
    printf("[*] powered by : \t\n\t\ttian yue\n");

    //  移除编译文件残留
    removeOldFile(1);
}