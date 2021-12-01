#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


const int diamond = 3500;
const int ruby = 50;
const int sapphire = 1200;
const int emerald = 800;

void print_new_child(int main_id,int child_id){
   printf("[PID:%d] A new child has been created with PID: %d\n",main_id,child_id);
}


int main(){
    int fd;
    int pid;
    pid = fork();
    if(pid>0){
        int fd1;
        int result = 0;
        char * myfifo = "/tmp/myfifo";
        mkfifo(myfifo, 0666);

        char str1[80], str2[80];
        while (1)
        {
            fd = open(myfifo,O_RDONLY);
            read(fd, str1, 80);
            if(strcmp(str1,"diamond")==0){
                //printf("diamond \n");
                result += diamond;
            }else if(strcmp(str1,"ruby")==0){
               //printf("ruby \n");
               result += ruby;
            }else if(strcmp(str1,"sapphire")==0){
               //printf("sapphire \n");
               result += sapphire;
            }else if(strcmp(str1,"emerald")==0){
               //printf("emerald \n");
               result += emerald;
            }
            fd = open(myfifo,O_WRONLY);
            printf("char is %s and total number is %d\n",str1,result);
            if(result>=200){
                strcpy(str2,"exit");
                write(fd, str2, strlen(str2)+1);
                close(fd);
                exit(EXIT_FAILURE);
            }
            strcpy(str2,"continue");
            write(fd, str2, strlen(str2)+1);
            close(fd);

        }

    }else if(pid == 0){
        print_new_child(getppid(),getpid());
        char filename [20]; 
        char * myfifo = "/tmp/myfifo";
        char temp;
        sprintf(filename,"buckets/%d.txt",0);
        FILE *fp1 = fopen(filename,"r");
        mkfifo(myfifo, 0666);

        char arr1[80], arr2[80];
        while (fscanf(fp1,"%c",&temp)!=EOF)
        {   
            fd = open(myfifo, O_WRONLY);
            if(temp == 'd'){
            //printf("diamond \n");
                strcpy(arr2,"diamond");
            }else if(temp == 'r'){
               //printf("ruby \n");
               strcpy(arr2,"ruby");
            }else if(temp == 's'){
               //printf("sapphire \n");
               strcpy(arr2,"sapphire");
            }else if(temp == 'e'){
               //printf("emerald \n");
               strcpy(arr2,"emerald");
            }

            write(fd, arr2, strlen(arr2)+1);
            close(fd);
            fd = open(myfifo, O_RDONLY);
            read(fd, arr1, sizeof(arr1));

            if(strcmp(arr1,"exit")==0){
                printf("child %d was exited\n",getpid());
                exit(EXIT_FAILURE);
            }
            close(fd);
        }
        fclose(fp1);

    }
    
    return 0;

}

