#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *file_name)
{
  printf("path: %s\n", path);
  char new_path[512];
  int fd;
  struct dirent de; //目录项，包含inum和name
  struct stat st;   //文件的FCB

  //0是只读 #define O_RDONLY  0x000
  //先打开文件，打开失败return
  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  //fstat系统调用从文件描述符所引用的inode中检索信息
  // fstat从文件描述符fd中获取FCB
    if ((fstat(fd, &st) < 0) || st.type != T_DIR)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // 判断当前路径拼接下一级路径后是否会过长
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(new_path))
    {
        printf("find:path too long\n");
        return;
    }

    // 遍历该目录下的所有文件
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
            continue;

        // 跳过当前目录与上一级目录，防止死循环
        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;

        // 拼接新的路径
        strcpy(new_path, path);
        char *ptr = new_path + strlen(path);
        *ptr++ = '/';
        memmove(ptr, de.name, DIRSIZ);
        *(ptr + DIRSIZ) = 0; // 字符串以整型0结尾表示结束

        // stat从文件名中FCB，用以判断文件格式
        if (stat(new_path, &st) < 0)
            continue;

        switch (st.type)
        {
        // 如果是目录则继续递归遍历
        case T_DIR:
            find(new_path, file_name);
            break;

        // 如果是文件则判断是否是目标，注意找到目标后不要返回，以为目标可能不止一个
        case T_FILE:
            if (!strcmp(de.name, file_name))
            {
                printf("%s\n", new_path);
                break;
            }
        }
    }
    close(fd);
}

int main(int argc, char *argv[])
{
  if (argc != 3) {
    printf("error\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}