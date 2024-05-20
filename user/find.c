#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//Hard: how to traverse the directory and how file system is arranged

//Hint: JUST to find the full path of the correct path

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), 0, DIRSIZ-strlen(p)); // change ' ' -> 0
  return buf;
}
//Note: the size of to string affects whether 2 Cstring equals


void find(char* path, char* name) {

    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    char *tmp;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:
           
            tmp = fmtname(path);
            if (strcmp(tmp, name) == 0) {
                printf("%s\n", path);
            }
            break;

        case T_DIR:
            strcpy(buf, path);  
            p = buf+strlen(buf);    
            *p++ = '/'; //add slash to . -> ./

            while(read(fd, &de, sizeof(de)) == sizeof(de)){//try to read a dir
                
                //when "." and ".." inum == 1
                //every dir has "." and ".."
                //and those dots share the same de.inum with dir
                if(de.inum <= 1) {
                    continue; 
                }   

                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0){
                    //don't recurse into '.' '..'
                    continue;

                }

                // copy at most 14 names to p, where each name is a dirent
                memmove(p, de.name, DIRSIZ); 
                // set the last bit to be 0, maybe a ending bit for Cstring
                p[DIRSIZ] = 0; 

                //increment a file or dir, the the inum increases
                if(stat(buf, &st) < 0){
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }

                if (st.type == T_DIR) {
                    if (strcmp(name, de.name) == 0) {
                        printf("%s\n", buf);
                    }

                }
                find(buf , name); 

            }
            break;

    }
    close(fd);
    return;

}


int main(int argc, char* argv[]) {

    int i;
    if (argc < 2) {
        exit(0);
    }

    for (i = 1; i < argc; i++) {
        find(".", argv[i]);
    }
    exit(0);
}