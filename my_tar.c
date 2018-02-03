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

  if ((argc == 2) && (strcmp(argv[1], "-c") == 0)) {
    fprintf(stderr, "no files given\n");
  }

  if ((argc >= 3) && (strcmp(argv[1], "-c") == 0)) {
    FILE *fd;
    struct stat sb;
    int file_size;
    char *filename;
    int c, i;

    //for each input file
    for (i = 2; i < argc; i++) {

      stat(argv[i], &sb);

      if (!S_ISREG(sb.st_mode)) {
        fprintf(stderr, "WARNING: %s is not a regular file.. skipping.\n", argv[i]);
      }

      filename = argv[i];
      fd = fopen(argv[i], "r");

      //write filename to stdout
      fwrite(filename, strlen(filename), 1, stdout);

      //write file stat info to stdout
      fwrite(&sb, sizeof(struct stat), 1, stdout);

      //write file contents of file to stdout
      char check;
      while ((c = getc(fd)) != EOF) {                 //Alok Singhal https://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings
        check = putc(c, stdout);                      //Short and elegant!
        if (check == EOF) {
          fprintf(stderr, "Error\n");
          perror("PUTC ERROR:");
        }
      }
      fclose(fd);
    }

  }

  return 0;
}
