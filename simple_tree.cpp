#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <assert.h>
#include <errno.h>

static int total_dirs = 0;
static int total_files = 0;

static const char *dir_type_name(unsigned char dir_type) {
    switch (dir_type)
    {
    case DT_BLK:
        return "block device";

    case DT_CHR:
        return "character device";

    case DT_DIR:
        return "directory";

    case DT_FIFO:
        return "named pipe";

    case DT_LNK:
        return "symbolic link";

    case DT_REG:
        return "regular file";

    case DT_SOCK:
        return "UNIX domain socket";

    case DT_UNKNOWN:
        return "unknown";

    default:
        return "no such type";
    }
}

static void traverse_dir(const char *name, int depth = 0) {
    assert(name);
    DIR *dp = opendir(name);
    if (dp == nullptr) {
        printf("opendir error: %d(%s)\n", errno, strerror(errno));
        return;
    }

    struct dirent *dent = readdir(dp);
    while (dent) {
        if (dent->d_name[0] != '.') {
            for (int i = 0; i < depth; i++) {
                printf("|-- ");
            }

            printf("%s, %s\n", dent->d_name, dir_type_name(dent->d_type));

            if (dent->d_type == DT_DIR) {
                char path[1024] = {0};
                sprintf(path, "%s/%s", name, dent->d_name);
                traverse_dir(path, depth + 1);

                total_dirs++;
            } else {
                total_files++;
            }
        }

        dent = readdir(dp);
    }

    closedir(dp);
}

int main(int argc, char *argv[]) {
    traverse_dir("./");

    printf("\ntotal directories: %d, total files: %d\n", total_dirs, total_files);

    return 0;
}
