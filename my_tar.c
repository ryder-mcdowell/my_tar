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
    char *filename;
    int c, i;
    int check_int;
    char check_char;

    //for each input file
    for (i = 2; i < argc; i++) {

      //stat file
      check_int = stat(argv[i], &sb);
      if (check_int == -1) {
        perror("ERROR");
        exit(1);
      }

      //check if regular file
      if (!S_ISREG(sb.st_mode)) {
        fprintf(stderr, "WARNING: %s is not a regular file.. skipping.\n", argv[i]);
        i += 1;
      }
      fprintf(stderr, "%s time created = %ld\n", argv[i], sb.st_mtime);
      fprintf(stderr, "%s time accessed = %ld\n", argv[i], sb.st_atime);
      fprintf(stderr, "%s user id = %u\n", argv[i], sb.st_uid);
      fprintf(stderr, "%c", S_IRUSR & sb.st_mode ? 'r' : '-');
      fprintf(stderr, "%c", S_IWUSR & sb.st_mode ? 'w' : '-');
      fprintf(stderr, "%c", S_IXUSR & sb.st_mode ? 'x' : '-');
      fprintf(stderr, "%c", S_IRGRP & sb.st_mode ? 'r' : '-');
      fprintf(stderr, "%c", S_IWGRP & sb.st_mode ? 'w' : '-');
      fprintf(stderr, "%c", S_IXGRP & sb.st_mode ? 'x' : '-');
      fprintf(stderr, "%c", S_IROTH & sb.st_mode ? 'r' : '-');
      fprintf(stderr, "%c", S_IWOTH & sb.st_mode ? 'w' : '-');
      fprintf(stderr, "%c", S_IXOTH & sb.st_mode ? 'x' : '-');
      fprintf(stderr, "\n");

      //write filename to stdout
      filename = argv[i];
      fwrite(filename, strlen(filename), 1, stdout);
      fwrite("\n", 1, 1, stdout);

      //write file stat info to stdout
      fwrite(&sb, sizeof(struct stat), 1, stdout);

      //write file contents of file to stdout
      fd = fopen(argv[i], "r");
      if (fd == NULL) {
        perror("ERROR");
        exit(1);
      }
      while ((c = getc(fd)) != EOF) {                      //Alok Singhal https://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings
        check_char = putc(c, stdout);                      //Short and elegant!
        if (check_char == EOF) {
          perror("ERROR");
          exit(1);
        }
      }

      //remove and close file
      check_int = remove(filename);
      if (check_int == -1) {
        perror("ERROR");
        exit(1);
      }
      fclose(fd);
    }

  }


  if ((argc == 2) && (strcmp(argv[1], "-x") == 0)) {
    FILE *fd;
    struct stat sb;
    char filename[255];
    int c, i;
    char *check;
    int check_int;

    while (!feof(stdin)) {
      //get filename
      check = fgets(filename, 255, stdin);
      if (check == '\0') {
        fprintf(stderr, "Extraction complete\n");
        return 0;
      }
      if (check == NULL) {
        perror("ERROR");
        exit(1);
      }
      filename[strlen(filename) - 1] = '\0';
      fprintf(stderr, "%s\n", filename);

      //create file
      fd = fopen(filename, "w");

      //stat
      check_int = stat(filename, &sb);
      if (check_int == -1) {
        perror("ERROR");
        exit(1);
      }
      fread(&sb, sizeof(struct stat), 1, stdin);
      fprintf(stderr, "%s time created = %ld\n", filename, sb.st_mtime);
      fprintf(stderr, "%s time accessed = %ld\n", filename, sb.st_atime);
      fprintf(stderr, "%s user id = %u\n", filename, sb.st_uid);
      fprintf(stderr, "%c", S_IRUSR & sb.st_mode ? 'r' : '-');
      fprintf(stderr, "%c", S_IWUSR & sb.st_mode ? 'w' : '-');
      fprintf(stderr, "%c", S_IXUSR & sb.st_mode ? 'x' : '-');
      fprintf(stderr, "%c", S_IRGRP & sb.st_mode ? 'r' : '-');
      fprintf(stderr, "%c", S_IWGRP & sb.st_mode ? 'w' : '-');
      fprintf(stderr, "%c", S_IXGRP & sb.st_mode ? 'x' : '-');
      fprintf(stderr, "%c", S_IROTH & sb.st_mode ? 'r' : '-');
      fprintf(stderr, "%c", S_IWOTH & sb.st_mode ? 'w' : '-');
      fprintf(stderr, "%c", S_IXOTH & sb.st_mode ? 'x' : '-');
      fprintf(stderr, "\n");

      //contents
      char file_contents[sb.st_size];
      fread(file_contents, sizeof(file_contents), 1 , stdin);
      fwrite(file_contents, sizeof(file_contents), 1, fd);

      fclose(fd);
    }
  }

  return 0;
}
