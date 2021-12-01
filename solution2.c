#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>

#define MAX 1024
  
/*******STRUCTURES*************/
typedef struct mesg_buffer {
   long mesg_type;
   char mesg_text[100];
} mesg_buffer;
mesg_buffer message;
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

int local_counter = 0;
int msgid;

/*******FUNCTIONS*************/
void write_message(key_t key,char text[]){
   int msgid,result;
   msgid = msgget(key, 0666 | IPC_CREAT);
   message.mesg_type = 1;
   strcpy(message.mesg_text,text);
   result = msgsnd(msgid, &message, sizeof(message), 0);
}
//-----------------------------------/
int read_message(key_t key){
   int msgid,result;
   char message_d []="diamond";
   char message_r []="ruby";
   char message_s []="sapphire";
   char message_e []="emerald";
   char main_message[10];
   msgid = msgget(key, 0666 | IPC_CREAT);
   msgrcv(msgid, &message, sizeof(message), 1, 0);
   strcpy(main_message, message.mesg_text);
   if(strcmp(main_message,message_d)==0){
      result = diamond;
   }else if(strcmp(main_message,message_r)==0){
      result = ruby;
   }else if(strcmp(main_message,message_s)==0){
      result = sapphire;
   }else if(strcmp(main_message,message_e)==0){
      result = emerald;
   }else{
      result =0;
   }
   return result;
}
//--------------------------------------------------

void print_new_child(int main_id,int child_id){
   printf("[PID:%d] A new child has been created with PID: %d\n",main_id,child_id);
}
//-----------------------------------------------------------
void go_and_read(int number_of_child,key_t key,int fd1[]){
   char filename[20];
   char temp;
   int local_counter = 0;
   sprintf(filename,"buckets/%d.txt",number_of_child);
   FILE *fp1 = fopen(filename,"r");
   while(fscanf(fp1,"%c",&temp)!=EOF){
      
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
      } else{
      } 
   }  
   fclose(fp1);
   close(fd1[0]);
   write(fd1[1],&local_counter,sizeof(int));
   close(fd1[1]);
}

int creating_child_process(key_t key){
   int result,summation = 0;
   int pid=fork();
   if(pid>0){
      wait(NULL);
      close(fd1[1]);
      read(fd1[0],&local_counter,sizeof(int));
      close(fd1[0]);
      for(int i=0;i<local_counter;i++){
         result = read_message(key); 
         summation =summation + result;         
      }
      msgctl(msgid, IPC_RMID, NULL);
      int pid2=fork();

      if(pid2>0){
         wait(NULL);
         close(fd2[1]);
         read(fd2[0],&local_counter,sizeof(int));
         close(fd2[0]);
         for(int i=0;i<local_counter;i++){
            result = read_message(key); 
            summation =summation + result;            
         }
         msgctl(msgid, IPC_RMID, NULL);
         int pid3=fork();

         if(pid3>0){
            wait(NULL);
            close(fd3[1]);
            read(fd3[0],&local_counter,sizeof(int));
            close(fd3[0]);
            for(int i=0;i<local_counter;i++){
               result = read_message(key); 
               summation =summation + result;             
            }
            msgctl(msgid, IPC_RMID, NULL);
            int pid4=fork();
      
            if(pid4>0){
               wait(NULL);
               close(fd4[1]);
               read(fd4[0],&local_counter,sizeof(int));
               close(fd4[0]);
               for(int i=0;i<local_counter;i++){
                  result = read_message(key); 
                  summation =summation + result;            
               }
               msgctl(msgid, IPC_RMID, NULL);
               int pid5=fork();
               if(pid5>0){
                  wait(NULL);
                  close(fd5[1]);
                  read(fd5[0],&local_counter,sizeof(int));
                  close(fd5[0]);
                  for(int i=0;i<local_counter;i++){
                     result = read_message(key); 
                     summation =summation + result;            
                  }
                  msgctl(msgid, IPC_RMID, NULL);
                  printf("result is =%d\n",summation);
               
               }else if(pid5==0){
                  print_new_child(getppid(),getpid());
                  go_and_read(4,key,fd5);
               }
            }else if(pid4==0){
               print_new_child(getppid(),getpid());
               go_and_read(3,key,fd4);
            }

         }else if(pid3==0){
            print_new_child(getppid(),getpid());
            go_and_read(2,key,fd3);
         }

      }else if(pid2==0){
         print_new_child(getppid(),getpid());
         go_and_read(1,key,fd2);
      }

   }else if(pid==0){
      print_new_child(getppid(),getpid());
      go_and_read(0,key,fd1);
   }
   return 0;
}

int main() {
   key_t key;
   pipe(fd1);
   pipe(fd2);
   pipe(fd3);
   pipe(fd4);
   pipe(fd5);
   key = ftok("progfile", 65);
   creating_child_process(key);
   return 0;

}

//ftok(): is use to generate a unique key.
//msgget(): either returns the message queue identifier for a newly created message queue or returns the identifiers for a queue which exists with the same key value.
//msgsnd(): Data is placed on to a message queue by calling msgsnd().
//msgrcv(): messages are retrieved from a queue.
//msgctl(): It performs various operations on a queue. Generally it is use to destroy message queue.