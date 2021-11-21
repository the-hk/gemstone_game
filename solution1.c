#include <stdio.h>
#include <unistd.h>
#include <wait.h>
/*******VARIABLES*************/

const int diamond = 3500;
const int ruby = 50;
const int sapphire = 1200;
const int emerald = 800;
int pid_number;

int fd1[2];  
int fd2[2];  
int fd3[2];  
int fd4[2];  
int fd5[2]; 

int main_result = 0;
int sum = 0;
/*******FUNCTIONS*************/

int go_and_read(int number_of_child){
   char filename[20];
   char temp;
   int result = 0;
   sprintf(filename,"buckets/%d.txt",number_of_child);
   FILE *fp1 = fopen(filename,"r");
   while(fscanf(fp1,"%c",&temp)!= EOF){
      
      if(temp == 'd'){
         //printf("diamond \n");
         result += diamond;
      }else if(temp == 'r'){
         //printf("ruby \n");
         result += ruby;
      }else if(temp == 's'){
         //printf("sapphire \n");
         result += sapphire;
      }else if(temp == 'e'){
         //printf("emerald \n");
         result += emerald;
      }
   }
   return result;

}

// create 1 mother 5 child process
int create_child_process(){
   int pid = fork();
   if(pid>0){      
      int pid2 = fork();
      if(pid2>0){
         int pid3 = fork();
         if(pid3>0){
            int pid4 = fork();
            if(pid4>0){
               int pid5 = fork();
               if(pid5>0){
                  //parent process
                  wait(NULL);
                  read(fd1[0],&main_result,sizeof(int));
                  close(fd1[0]);
                  sum += main_result;

                  read(fd2[0],&main_result,sizeof(int));
                  close(fd2[0]);
                  sum += main_result;

                  read(fd3[0],&main_result,sizeof(int));
                  close(fd3[0]);
                  sum += main_result;

                  read(fd4[0],&main_result,sizeof(int));
                  close(fd4[0]);
                  sum += main_result;

                  close(fd5[1]);
                  read(fd5[0],&main_result,sizeof(int));
                  sum += main_result;
                  printf("[PID:%d] total number: %d\n",getpid(),sum);
               }else if(pid5==0){
                  //child 4
                  main_result +=go_and_read(4);
                  write(fd5[1],&main_result,sizeof(int));
                  close(fd5[1]);
               }
            }else if(pid4==0){
               //child 3
               main_result +=go_and_read(3);
               write(fd4[1],&main_result,sizeof(int));
               close(fd4[1]);
            }
         }else if(pid3==0){
         //child 2
         main_result +=go_and_read(2);
         write(fd3[1],&main_result,sizeof(int));
         close(fd3[1]);
         }
      }else if(pid2==0){
         //child 1
         main_result +=go_and_read(1);
         write(fd2[1],&main_result,sizeof(int));
         close(fd2[1]);
      }
   }else if(pid==0){
      //child 0
      main_result +=go_and_read(0);
      close(fd1[0]);
      write(fd1[1],&main_result,sizeof(int));
      close(fd1[1]);
   }
   return 1;
}

int main() {
   pipe(fd1);
   pipe(fd2);
   pipe(fd3);
   pipe(fd4);
   pipe(fd5);

   create_child_process();
   return 0;
}
