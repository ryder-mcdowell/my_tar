#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <utime.h>
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
    char tmp[1];
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
        if (i < argc) {
          //restat file
          check_int = stat(argv[i], &sb);
          if (check_int == -1) {
            perror("ERROR");
            exit(1);
          }
        } else {
          exit(1);
        }
      }

      //write filename to stdout
      filename = argv[i];
      fwrite(filename, strlen(filename), 1, stdout);
      fwrite("\n", 1, 1, stdout);

      //write file stat info to stdout
      fwrite(&sb, sizeof(struct stat), 1, stdout);

      //write file contents of file to stdout
      fd = fopen(argv[i], "rb");
      if (fd == NULL) {
        perror("ERROR");
        exit(1);
      }
      c = fgetc(fd);
      while (c != EOF) {
        tmp[0] = c;
        fwrite(tmp, sizeof(char), 1, stdout);
        c = fgetc(fd);
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
    struct utimbuf tb;
    char filename[255];
    int c, i;
    char *check;
    int check_int;

    while (!feof(stdin)) {
      //get filename
      check = fgets(filename, 255, stdin);
      if (check == '\0') {
        free(check);
        return 0;
      }
      if (check == NULL) {
        perror("ERROR");
        exit(1);
      }
      filename[strlen(filename) - 1] = '\0';

      //create file
      fd = fopen(filename, "wb");

      //stat
      check_int = stat(filename, &sb);
      if (check_int == -1) {
        perror("ERROR");
        exit(1);
      }
      fread(&sb, sizeof(struct stat), 1, stdin);

      //contents
      char file_contents[sb.st_size];
      fread(file_contents, sizeof(file_contents), 1 , stdin);
      fwrite(file_contents, sizeof(file_contents), 1, fd);

      fclose(fd);

      //set file permissions
      if ((S_IRUSR & sb.st_mode) != 0) {
        check_int = chmod(filename, S_IRUSR);
        if (check_int == -1) {
          perror("ERROR");
          exit(1);
        }
      }
      if ((S_IWUSR & sb.st_mode) != 0) {
        check_int = chmod(filename, sb.st_mode | S_IWUSR);
        if (check_int == -1) {
          perror("ERROR");
          exit(1);
        }
      }
      if ((S_IXUSR & sb.st_mode) != 0) {
        check_int = chmod(filename, sb.st_mode | S_IXUSR);
        if (check_int == -1) {
          perror("ERROR");
          exit(1);
        }
      }
      if ((S_IRGRP & sb.st_mode) != 0) {
        check_int = chmod(filename, sb.st_mode | S_IRGRP);
        if (check_int == -1) {
          perror("ERROR");
          exit(1);
        }
      }
      if ((S_IWGRP & sb.st_mode) != 0) {
        check_int = chmod(filename, sb.st_mode | S_IWGRP);
        if (check_int == -1) {
          perror("ERROR");
          exit(1);
        }
      }
      if ((S_IXGRP & sb.st_mode) != 0) {
        check_int = chmod(filename, sb.st_mode | S_IXGRP);
        if (check_int == -1) {
          perror("ERROR");
          exit(1);
        }
      }
      if ((S_IROTH & sb.st_mode) != 0) {
        check_int = chmod(filename, sb.st_mode | S_IROTH);
        if (check_int == -1) {
          perror("ERROR");
          exit(1);
        }
      }
      if ((S_IWOTH & sb.st_mode) != 0) {
        check_int = chmod(filename, sb.st_mode | S_IWOTH);
        if (check_int == -1) {
          perror("ERROR");
          exit(1);
        }
      }
      if ((S_IXOTH & sb.st_mode) != 0) {
        check_int = chmod(filename, sb.st_mode | S_IXOTH);
        if (check_int == -1) {
          perror("ERROR");
          exit(1);
        }
      }

      //set file owner
      check_int = chown(filename, sb.st_uid, sb.st_gid);
      if (check_int == -1) {
        perror("ERROR");
        exit(1);
      }

      //set file times
      tb.modtime = sb.st_mtime;
      tb.actime = sb.st_atime;
      check_int = utime(filename, &tb);
      if (check_int == -1) {
        perror("ERROR");
        exit(1);
      }
    }
  }

  return 0;
}
