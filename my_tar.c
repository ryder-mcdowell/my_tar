#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char **argv) {
  if (argc == 1) {
    fprintf(stderr, "usage: ./my_tar option [files...]\n      option = -c | -x\n");
  }

  if ((argc == 3) && (strcmp(argv[1], "-c") == 0)) {
    FILE *fd;
    struct stat sb;
    int i;
    char filewrite[157];

    fd = fopen(argv[2], "a");

    //Get filewrite
    fprintf(stderr, "filewrite = %s\n", argv[2]);
    for (i = 0; i < 100; i++) {
      if (i > strlen(argv[2])) {
        filewrite[i] = ' ';
      } else {
        filewrite[i] = argv[2][i];
      }
    }

    stat(argv[2], &sb);

    printf("%c\n", S_IRUSR & sb.st_mode ? 'r' : '-');
    filewrite[100] = S_IRUSR & sb.st_mode ? 'r' : '-';
    filewrite[101] = S_IRUSR & sb.st_mode ? 'w' : '-';
    filewrite[102] = S_IRUSR & sb.st_mode ? 'x' : '-';
    filewrite[103] = S_IRUSR & sb.st_mode ? 'r' : '-';
    filewrite[104] = S_IRUSR & sb.st_mode ? 'w' : '-';
    filewrite[105] = S_IRUSR & sb.st_mode ? 'x' : '-';
    filewrite[106] = S_IRUSR & sb.st_mode ? 'r' : '-';
    filewrite[107] = S_IRUSR & sb.st_mode ? 'w' : '-';
    filewrite[108] = S_IRUSR & sb.st_mode ? 'x' : '-';

    printf("%u\n", sb.st_uid);
    printf("%u\n", sb.st_gid);

    //Write filewrite
    fwrite(filewrite, 1, 109, fd);


    fclose(fd);

  }



  return 0;
}
