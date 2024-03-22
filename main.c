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
    int start=0, end =0, avg_waiting_summer = 0;
    float avg_waiting_time = 0;

    for (int i = 0; i < no_of_records; i++) {
        end += tasks_array[i].burst_time;
        tasks_array[i].waiting_time = start - tasks_array[i].arrival_time;
        avg_waiting_summer += tasks_array[i].waiting_time;
        fprintf(fout, "%s\t%d\t%d\t\n", tasks_array[i].name, start, end);  // Note: changed task_name to name as per struct definition
        start = end;
    }

    for(int i = 0; i < no_of_records; i++){ // printing waiting times
        fprintf(fout, "Waiting Time %s: %d\n", tasks_array[i].name, tasks_array[i].waiting_time);
    }
    
    avg_waiting_time = (float)avg_waiting_summer/no_of_records;

    fprintf(fout, "Average Waiting Time: %f\n\n", avg_waiting_time);
}

void round_robin(struct tasks *tasks_array, int no_of_records, FILE *fout){
    int time = 0; // Current time in the simulation
    int completed = 0; // Number of tasks completed
    int current_task = 0; // Index of the current task
    int total_waiting_time = 0; // Total waiting time for calculation of average
    int time_quantum = 4; // Time quantum for the round-robin algorithm
    
    fprintf(fout, "\nRR:\n");

    // Initialize remaining times for each task
    for (int i = 0; i < no_of_records; i++) {
        tasks_array[i].remaining_time = tasks_array[i].burst_time;
    }

    // Continue looping until all tasks are completed
    while (completed < no_of_records) {
        // If the task has remaining time, then it needs to be processed
        if (tasks_array[current_task].remaining_time > 0) {
            // Task is starting or continuing
            fprintf(fout, "%s starts at %d, ", tasks_array[current_task].name, time);
            
            // If the remaining time is more than the time quantum, process it for the duration of the quantum
            // Otherwise, just complete the task
            if (tasks_array[current_task].remaining_time > time_quantum) {
                time += time_quantum;
                tasks_array[current_task].remaining_time -= time_quantum;
            } else {
                time += tasks_array[current_task].remaining_time;
                // Calculate waiting time: current time - arrival time - burst time
                int waiting_time = time - tasks_array[current_task].arrival_time - tasks_array[current_task].burst_time;
                total_waiting_time += waiting_time;
                tasks_array[current_task].waiting_time = waiting_time;
                
                tasks_array[current_task].remaining_time = 0; // Mark task as completed
                completed++; // Increment the number of completed tasks
            }
            
            fprintf(fout, "ends at %d\n", time);
        }
        
        // Move to the next task, cycling back to the start if necessary
        current_task = (current_task + 1) % no_of_records;
    }
    
    for (int i = 0; i < no_of_records; i++) {
        fprintf(fout, "Waiting Time for %s: %d\n", tasks_array[i].name, tasks_array[i].waiting_time);
    }

    // Calculate and print the average waiting time
    float average_waiting_time = (float)total_waiting_time / no_of_records;
    fprintf(fout, "Average Waiting Time: %.2f\n", average_waiting_time);
}

void non_preemptive_priority(struct tasks *tasks_array, int no_of_records, FILE *fout){
}

void preemptive_priority(struct tasks *tasks_array, int no_of_records, FILE *fout){
}

int main(int argc, char *argv[]){
    //FILE *fout 
    fout = fopen("Output.txt", "w");

    if (fout == NULL) {
    printf("Cannot open output file\n");
    return 1;
}
    fprintf(fout, "my mini test");

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
    
    i=0;

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
    round_robin(tasks_array, no_of_records, fout);

    free(tasks_array);
    fclose(fin);
    fclose(fout);
    return 0;
}


