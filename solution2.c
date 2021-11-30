#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define MAX 1024
  
/*******STRUCTURES*************/
struct mesg_buffer {
    long mesg_type;
    char mesg_text[1024];
} message;

/*******VARIABLES*************/
const int diamond = 3500;
const int ruby = 50;
const int sapphire = 1200;
const int emerald = 800;

int fd1[2];
int local_counter = 0;
int msgid;

/*******FUNCTIONS*************/
void write_message(key_t key,char text[]){
    int msgid;
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.mesg_type = 1;
    strcpy(message.mesg_text,text);
    msgsnd(msgid, &message, sizeof(message), 0);
    printf("my message :%s\n",message.mesg_text);
}
//-----------------------------------
int read_message(key_t key){
   int msgid;
   msgid = msgget(key, 0666 | IPC_CREAT);
   msgrcv(msgid, &message, sizeof(message), 1, 0);
   printf("Data Received is : %s \n", message.mesg_text);
   return msgid;
}
//--------------------------------------------------

void print_new_child(int main_id,int child_id){
   printf("[PID:%d] A new child has been created with PID: %d\n",main_id,child_id);
}
//--------------------------------------------------

void print_child_sleep(int main_id,int sleep_time){
   printf("[PID:%d] Tired. Going to sleep for %dmsec.\n",main_id,sleep_time);
}

//-----------------------------------------------------------
void go_and_read(int number_of_child,key_t key){
   char filename[20];
   char temp;
   int local_counter = 0;
   sprintf(filename,"buckets/%d.txt",number_of_child);
   FILE *fp1 = fopen(filename,"r");
   while(fscanf(fp1,"%c",&temp)!= EOF){
      //printf("i cant go out\n");
      if(temp == 'd'){
         //printf("diamond \n");
         char message []="diamond";
         write_message(key,message);
         local_counter ++;
      }else if(temp == 'r'){
         //printf("ruby \n");
         char message []="ruby";
         write_message(key,message);
         local_counter ++;
         printf("girdim\n");
      }else if(temp == 's'){
         //printf("sapphire \n");
         char message []= "sapphire";
         write_message(key,message);
         local_counter ++;
      }else if(temp == 'e'){
         //printf("emerald \n");
         char message []="emerald";
         write_message(key,message);
         local_counter ++;
      }  
   }
   close(fd1[0]);
   write(fd1[1],&local_counter,sizeof(int));
   close(fd1[1]);
   printf("girdim\n");
}

int creating_child_process(key_t key){

   int pid=fork();
   wait(NULL);
   close(fd1[1]);
   read(fd1[0],&local_counter,sizeof(int));
   close(fd1[0]);
   printf("%d\n",local_counter);
   for(int i=0;i<local_counter;i++){
      msgid = read_message(key);          
   }
   msgctl(msgid, IPC_RMID, NULL);

   if(pid>0){
      int pid2=fork();

      if(pid2>0){
         int pid3=fork();

         if(pid3>0){
            int pid4=fork();
      
            if(pid4>0){
               int pid5=fork();
               if(pid5>0){
               
               }else if(pid5==0){
                  go_and_read(4,key);
               }
            }else if(pid4==0){
               go_and_read(3,key);
            }

         }else if(pid3==0){
            go_and_read(2,key);
         }

      }else if(pid2==0){
         go_and_read(1,key);
      }

   }else if(pid==0){
      go_and_read(0,key);
   }
   return 0;
}

int main() {
   printf("i cant enter");
   key_t key;
   pipe(fd1);
   key = ftok("progfile", 65);
   printf("i cant enter");
   creating_child_process(key);
   return 0;

}

//ftok(): is use to generate a unique key.
//msgget(): either returns the message queue identifier for a newly created message queue or returns the identifiers for a queue which exists with the same key value.
//msgsnd(): Data is placed on to a message queue by calling msgsnd().
//msgrcv(): messages are retrieved from a queue.
//msgctl(): It performs various operations on a queue. Generally it is use to destroy message queue.