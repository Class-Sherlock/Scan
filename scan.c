#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FNAME_SIZE 512
#define MAX_FILES 1024

struct fsandfn {
    off_t fsize;
    char* fname;
    
  } fsandfn[MAX_FILES];
 
int endsWith(const char *str, const char *strfix)
{
    if (!str || !strfix)
        return 0;
	
    size_t Lstr = strlen(str);
    size_t Lstrfix = strlen(strfix);
	
    if (Lstrfix >  Lstr)
        return 0;
	
    return strncmp(str + Lstr - Lstrfix, strfix, Lstrfix) == 0;
}

int Fcomp(const void * x, const void * y)
{
  const struct fsandfn *fa = x, *fb = y;
  return fb->fsize - fa->fsize;
}



int main( int argc, char ** argv) {
  
  // handle command line arguments
  if( argc != 3) {
    fprintf(stderr,"I don't take command line arguments.\n");
    exit(-1);
  }
  // open 'find...'
  char cmdStr[255];
  FILE * fp = popen("find . -type f", "r");
  if( fp == NULL) {
    perror("popen failed:");
    exit(-1);
  }
  // read in all fnames
  char buff[MAX_FNAME_SIZE];
  long  nFiles = 0;
  //char * files[MAX_FILES];
  while(fgets(buff,MAX_FNAME_SIZE,fp)) {
    int len = strlen(buff) - 1;
    if (endsWith(strndup(buff, len), argv[1])){
      fsandfn[nFiles].fname = strndup(buff,len);
      nFiles ++;
    }
  }
  fclose(fp);
  int n = strtol(argv[2], NULL, 10);
  printf("Found %d files:\n", n);
  // get file sizes for each file and sum them up
  long long totalSize = 0;
  struct stat st[n+1];
  for(int i = 0 ; i < nFiles ; i ++ ) {
    if( 0 != stat(fsandfn[i].fname,&st[i])) {
      perror("stat failed:");
      exit(-1);
    }
    fsandfn[i].fsize = st[i].st_size;
    
  }
  qsort(fsandfn, nFiles, sizeof(struct fsandfn), Fcomp);
  for(int i = 0; i < n; i++){
     totalSize += fsandfn[i].fsize;
     if(fsandfn[i].fname != NULL)
       printf("\t%s : %ld\n", fsandfn[i].fname, fsandfn[i].fsize);
  }
  printf("Total size: %lld bytes.\n", totalSize);
  
  return 0;
}

