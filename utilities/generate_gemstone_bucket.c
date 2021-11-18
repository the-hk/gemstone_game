#include <stdio.h>
#include <stdlib.h>

#define BUCKET_SIZE 100
#define PROB_DIAMOND 0.001
#define PROB_SAPPHIRE 0.003
#define PROB_EMERALD 0.009
#define PROB_RUBY 0.05
//Initialize a global char array excluding the gems
char dirt_alphabet[] = "abcfghijklmnopqtuvwxyz"; // excluding d, r, s, e
int dirt_alphabet_length = 0;
char get_a_stone();
int write_bucket_file(char* bucket, int bucket_no);
int generate_bucket_string(char* bucket);

int main(int argc, char *argv[]) {
    int seed = 0;
    int bucket_file_count = 0;
    char bucket[BUCKET_SIZE + 1];
    dirt_alphabet_length = sizeof(dirt_alphabet)-1;
    /* get seed value from makefile env variable */
    seed = atoi(argv[1]);
    bucket_file_count = atoi(argv[2]);
    /* Intializes random number generator */
    srand((unsigned) seed);
    for (int i=0; i<bucket_file_count; i++){
        generate_bucket_string(bucket);
        write_bucket_file(bucket, i);
    }

    return 0;
}

int write_bucket_file(char* bucket, int bucket_no){
    FILE *fp;
    char file_name[1024];
    sprintf(file_name,"buckets/%1d.txt",bucket_no);
    fp = fopen(file_name, "w");
    fprintf(fp, "%s", bucket);
    fclose(fp);
    return 0;
}

// fill the bucket
int generate_bucket_string(char* bucket){
    for(int i = 0; i < BUCKET_SIZE; i++){
        bucket[i] = get_a_stone();
    }
    bucket[BUCKET_SIZE] = '\0'; // Null terminated string, for printing purposes
    return 0;
}

/* Returns either dirt or occasionally a gemstone depending on the probabilities */
char get_a_stone(void){
    int rand_index=0;
    int r=0;
    char return_char = '.';
    r = rand();
    if (r<RAND_MAX*PROB_DIAMOND){
        //printf("Diamond!!!");
        return_char = 'd';
    }
    else if(r<RAND_MAX*(PROB_DIAMOND + PROB_SAPPHIRE)){
        ///printf("Sapphire!!!");
        return_char = 's';
    }
    else if(r<RAND_MAX*(PROB_DIAMOND + PROB_SAPPHIRE + PROB_EMERALD)){
        //printf("Emerald!!!");
        return_char = 'e';
    }
    else if(r<RAND_MAX*(PROB_DIAMOND + PROB_SAPPHIRE + PROB_EMERALD +PROB_RUBY)){
        //printf("Ruby!!!");
        return_char = 'r';
    }
    else {
        /* Generate dirt*/
        rand_index = r % dirt_alphabet_length;
        return_char = dirt_alphabet[rand_index];
    }
    

    return return_char;
}