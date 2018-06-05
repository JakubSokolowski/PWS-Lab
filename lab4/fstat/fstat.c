#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char * argv[]){

  struct stat file_stat;
  int file = open(argv[1],O_RDONLY);
  int res = fstat(file, &file_stat);
  if(res < -1) {
    perror("fstat");
    exit(1);
  }
  printf("Plik: %s\n",argv[1] );
  off_t filesize = file_stat.st_size;
  printf("Rozmiar pliku: %d\n",(int)filesize);
  printf("Liczba linkow: %d\n", (int)file_stat.st_nlink);

  printf("Pozwolenia: ");
  printf( (S_ISDIR(file_stat.st_mode)) ? "d" : "-");
  printf( (file_stat.st_mode & S_IRUSR) ? "r" : "-");
  printf( (file_stat.st_mode & S_IWUSR) ? "w" : "-");
  printf( (file_stat.st_mode & S_IXUSR) ? "x" : "-");
  printf( (file_stat.st_mode & S_IRGRP) ? "r" : "-");
  printf( (file_stat.st_mode & S_IWGRP) ? "w" : "-");
  printf( (file_stat.st_mode & S_IXGRP) ? "x" : "-");
  printf( (file_stat.st_mode & S_IROTH) ? "r" : "-");
  printf( (file_stat.st_mode & S_IWOTH) ? "w" : "-");
  printf( (file_stat.st_mode & S_IXOTH) ? "x" : "-");

}
