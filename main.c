/*
CPU Scheduler 
program that simulates a CPU scheduler. This scheduler is capable
of using varied scheduling algorithms to schedule a
group of computation tasks.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_JOBS 50
#define MAX_RECORD_LEN  25
#define JOB_NAME_LEN 5
char delimiters[] = ",\n";

FILE *fout;

struct tasks {

    /*from file*/
    char name[JOB_NAME_LEN];
    int arrival_time;
    int burst_time;

    /*calculated*/
    int waiting_time;
    int remaining_time;
    /*continue onwards*/
};


void first_come_first_serve(struct tasks *tasks_array, int no_of_records, FILE *fout){
    /* implemeting first come first serve algorithm to scheduel */
    fprintf(fout,"FCFS:\n");
    int start=0, end =0, avg_waiting_time = 0;

    for (int i = 0; i < no_of_records; i++) {
        end += tasks_array[i].burst_time;
        tasks_array[i].waiting_time = start - tasks_array[i].arrival_time;
        avg_waiting_time += tasks_array[i].waiting_time;
        fprintf(fout, "%s\t%d\t%d\t\n", tasks_array[i].name, start, end);  // Note: changed task_name to name as per struct definition
        start = end;
    }

    for(int i = 0; i < no_of_records; i++){ // printing waiting times
        fprintf(fout, "Waiting Time %s: %d\n", tasks_array[i].name, tasks_array[i].waiting_time);
    }
    
    fprintf(fout, "Avarege Waiting Time: %f\n\n", (avg_waiting_time/no_of_records));
}

void round_robin(struct tasks *tasks_array, int no_of_records, FILE *fout){
}

void non_preemptive_priority(struct tasks *tasks_array, int no_of_records, FILE *fout){
}

void preemptive_priority(struct tasks *tasks_array, int no_of_records, FILE *fout){
}

int main(int argc, char *argv[]){
    //FILE *fout 
    fout = fopen("Output.txt", "w");

    /* check for command line arguments */
    if (argc < 2) {
            printf("Please provide a filename as an argument.\n");
            return 1;
        }

        FILE *fin = fopen(argv[1], "r");
        if (fin == NULL) {
            printf("Cannot open file\n");
            return 1;
        }

    /*reading the list*/
    int i = 0;
    char temp_record_holder[MAX_RECORD_LEN];

    while(fgets(temp_record_holder, MAX_JOBS, fin) != NULL) { 
        //fprintf(fout, "%s", temp_record_holder); // test
        i++;
    }
    int no_of_records = i;
    i=0;
    rewind(fin); // reset file pointer to the beginning of the file
    /*allocating memory for the array of jobs*/
    


   //struct tasks *tasks_array;

    struct tasks *tasks_array = malloc(no_of_records * sizeof(struct tasks));
    if (tasks_array == NULL) {
        printf("Memory allocation failed\n");
        fclose(fin);
        fclose(fout);
        return -1;
    }

    char temp_buffer[MAX_RECORD_LEN];
    //char delimiters[] = ",\n";

    while (fgets(temp_buffer, MAX_RECORD_LEN, fin) != NULL) {
        char *token = strtok(temp_buffer, delimiters);

        strncpy(tasks_array[i].name, token, JOB_NAME_LEN - 1);
        tasks_array[i].name[JOB_NAME_LEN - 1] = '\0'; // Ensure null termination
        //strcpy(tasks_array[index].name, token);
        tasks_array[i].arrival_time = atoi(strtok(NULL, delimiters));
        tasks_array[i].burst_time = atoi(strtok(NULL, delimiters));
        // Initialize other task attributes as needed
        fprintf(fout, "%s %d %d\n", tasks_array[i].name, tasks_array[i].arrival_time, tasks_array[i].burst_time); // test
        i++;
        
    }


    printf("total of %d records\n", no_of_records); // test

    first_come_first_serve(tasks_array, no_of_records, fout);

    free(tasks_array);
    fclose(fin);
    fclose(fout);
    return 0;
}


