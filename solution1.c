#include <stdio.h>

/////////////*******VARIABLES*************\\\\\\\\\\\\
const int diamond = 3500;
const int ruby = 50;
const int sapphire = 1200;
const int emarald = 800;

/////////////*******FUNCTIONS*************\\\\\\\\\\\\

int go_and_read(int pid_of_child){
   FILE *fp1 = fopen()

}

// create 1 mother 5 child process
int create_process(){
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
               }else if(pid4==0){
                  //child 5
               }
            }else if(pid4==0){
               //child 4
            }
         }else if(pid3==0){
         //child 3
         }
      }else if(pid2==0){
         //child 2
      }
   }else if(pid==0){
      //child 2
   }
   return 0;
}

int main() {
   // printf() displays the string inside quotation
   int a=0;
   a++;
   printf("Result 1600\n");
   return 0;
}
