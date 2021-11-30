#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MAX 10
  
// structure for message queue  4
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;

/*******VARIABLES*************/
const int diamond = 3500;
const int ruby = 50;
const int sapphire = 1200;
const int emerald = 800;

int fd1[2];
int fd2[2];
int fd3[2];
int fd4[2];
int fd5[2];

/*******FUNCTIONS*************/

void *create_shared_memory(size_t size){

  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANONYMOUS;
  return mmap(NULL, size, protection, visibility, -1, 0);
}
//--------------------------------------------------

void print_new_child(int main_id,int child_id){
   printf("[PID:%d] A new child has been created with PID: %d\n",main_id,child_id);
}
//--------------------------------------------------

void print_child_sleep(int main_id,int sleep_time){
   printf("[PID:%d] Tired. Going to sleep for %dmsec.\n",main_id,sleep_time);
}


void go_and_read(int number_of_child,void *shared_mem){
   char filename[20];
   char temp;
   sprintf(filename,"buckets/%d.txt",number_of_child);
   FILE *fp1 = fopen(filename,"r");
   while(fscanf(fp1,"%c",&temp)!= EOF){
      //printf("i cant go out\n");
      if(temp == 'd'){
         //printf("diamond \n");
         char message []="diamond";
         memcpy(shared_mem, message, sizeof(message));
      }else if(temp == 'r'){
         //printf("ruby \n");
         char message []="ruby";
         memcpy(shared_mem, message, sizeof(message));
      }else if(temp == 's'){
         //printf("sapphire \n");
         char message []= "sapphire";
         memcpy(shared_mem, message, sizeof(message));
      }else if(temp == 'e'){
         //printf("emerald \n");
         char message []="emerald";
         memcpy(shared_mem, message, sizeof(message));
      }  
   }
}

int creating_child_process(void *shared_mem){
   int pid=fork();

   if(pid>0){
      printf("Child read: %s\n", shared_mem);
      int pid2=fork();

      if(pid2>0){
         int pid3=fork();

         if(pid3>0){
            int pid4=fork();

            if(pid4>0){
               int pid5=fork();

               if(pid5>0){
               
               }else if(pid5==0){

               }
            }else if(pid4==0){

            }

         }else if(pid3==0){

         }

      }else if(pid2==0){

      }

   }else if(pid==0){
      go_and_read(0,shared_mem);
   }
   return 0;
}

int main() {
   //pipe(fd1);
   //pipe(fd2);
   //pipe(fd3);
   //pipe(fd4);
   //pipe(fd5);
//
   //void *shared_mem = create_shared_memory(200);
   //creating_child_process(shared_mem);
   //return 0;


   void* shmem = create_shared_memory(128);
   creating_child_process(shmem);
   return 0;

}