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
    int file_size, total_fs;
    char *filename;
    int c, i;
    int check_int;
    char check_char;

    //for each input file
    for (i = 2; i < argc; i++) {

      //stat file
      check_int = stat(argv[i], &sb);
      if (check_int == -1) {
        perror("ERROR:");
        exit(1);
      }

      //check if regular file
      if (!S_ISREG(sb.st_mode)) {
        fprintf(stderr, "WARNING: %s is not a regular file.. skipping.\n", argv[i]);
        i += 1;
      }
      fprintf(stderr, "%s created = %ld\n", argv[i], sb.st_mtime);

      //write filename to stdout
      filename = argv[i];
      fwrite(filename, strlen(filename), 1, stdout);
      fwrite("\n", 1, 1, stdout);

      //write file stat info to stdout
      fwrite(&sb, sizeof(struct stat), 1, stdout);

      //write file contents of file to stdout
      fd = fopen(argv[i], "r");
      if (fd == NULL) {
        perror("ERROR:");
        exit(1);
      }
      while ((c = getc(fd)) != EOF) {                      //Alok Singhal https://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings
        check_char = putc(c, stdout);                      //Short and elegant!
        if (check_char == EOF) {
          perror("ERROR:");
          exit(1);
        }
      }
      fclose(fd);
    }

  }


  if ((argc == 2) && (strcmp(argv[1], "-x") == 0)) {
    FILE *fd;
    struct stat sb;
    char filename[255];
    char file_stat[sizeof(struct stat)];
    int c, i;
    char *check;
    int check_int;

    //get filename
    check = fgets(filename, 255, stdin);
    if (check == NULL) {
      perror("ERROR:");
      exit(1);
    }
    filename[strlen(filename) - 1] = '\0';
    fprintf(stderr, "%s\n", filename);

    //create file
    fd = fopen(filename, "w");
    fwrite("THESE ARE THE FILE CONTENTS", 28, 1, fd);

    //get stat


    check_int = stat(filename, &sb);
    if (check_int == -1) {
      perror("ERROR:");
      exit(1);
    }
    fread(&sb, sizeof(struct stat), 1, stdin);
    fprintf(stderr, "%s date created = %ld\n", filename, sb.st_mtime);


    fclose(fd);
  }

  return 0;
}
