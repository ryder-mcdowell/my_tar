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
    int size_t;
    char *buffer;

    fd = fopen(argv[2], "w+");

    fprintf(stderr, "filename = %s\n", argv[2]);
    buffer = argv[2];
    printf("%s\n", buffer);

    //Write filename
    size_t = fwrite(buffer, 1, sizeof(buffer), fd);



    stat("file.txt", &sb);

    printf("%c", S_IRUSR & sb.st_mode ? 'r' : '-');
    printf("%ld\n", sb.st_atime);


    fclose(fd);

  }



  return 0;
}
