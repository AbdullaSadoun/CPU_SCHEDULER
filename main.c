/*
CPU Scheduler 
program that simulates a CPU scheduler. This scheduler is capable
of outputting the result of using various scheduling algorithms
on a set of tasks
by: Abdulla Sadoun
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TASKS 50 // maximum number of tasks
#define MAX_RECORD_LEN  25 // maximum length of a line in input
#define JOB_NAME_LEN 5 // maximum length of a task name
#define SHORTEST_TIME 999999 // maximum time for a task
#define QUANTUM 4 // time quantum for round robin

char delimiters[] = ",\n"; // used to split the input lines to tokens

FILE *fout; // output file pointer

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
    int start=0, end =0, avg_waiting_summer = 0;
    float avg_waiting_time = 0;
 
    fprintf(fout,"FCFS:\n"); // title of the algorithm

    for (int i = 0; i < no_of_records; i++) {
        end += tasks_array[i].burst_time;
        tasks_array[i].waiting_time = start - tasks_array[i].arrival_time;
        avg_waiting_summer += tasks_array[i].waiting_time;
        fprintf(fout, "%s\t%d\t%d\t\n", tasks_array[i].name, start, end);  
        start = end;
    }

    for(int i = 0; i < no_of_records; i++){ // printing waiting times
        fprintf(fout, "Waiting Time %s: %d\n", tasks_array[i].name, tasks_array[i].waiting_time);
    }
    
    avg_waiting_time = (float)avg_waiting_summer/no_of_records;

    fprintf(fout, "Average Waiting Time: %.2f\n", avg_waiting_time);
}
void round_robin(struct tasks *tasks_array, int no_of_records, FILE *fout){
    int time=0, completed=0 , current_task= 0, total_waiting=0 , time_quantum=QUANTUM;
    
    fprintf(fout, "\nRR:\n"); // title of the algorithm

    for (int i = 0; i < no_of_records; i++) { // Initialize remaining times for each task
        tasks_array[i].remaining_time = tasks_array[i].burst_time;
    }

    while (completed < no_of_records) {
        if (tasks_array[current_task].remaining_time > 0) { // process if theres remaining time
            int start_time=time; // save the start time
            
            
            if (tasks_array[current_task].remaining_time > time_quantum) { // give period to process
                time += time_quantum;
                tasks_array[current_task].remaining_time -= time_quantum; 
            } else { // complete the task
                time += tasks_array[current_task].remaining_time;
                int waiting_time = time - tasks_array[current_task].arrival_time - tasks_array[current_task].burst_time;
                total_waiting += waiting_time;
                tasks_array[current_task].waiting_time = waiting_time;
                tasks_array[current_task].remaining_time = 0; // task is marked complete
                completed++; 
            }
            fprintf(fout, "%s\t%d\t%d\n", tasks_array[current_task].name, start_time, time);
        }
        current_task = (current_task + 1) % no_of_records; // move next/loop back
    }
    
    for (int i = 0; i < no_of_records; i++) {
        fprintf(fout, "Waiting Time %s: %d\n", tasks_array[i].name, tasks_array[i].waiting_time);
    }

    float avg_waiting_time = (float)total_waiting / no_of_records;
    fprintf(fout, "Average Waiting Time: %.2f\n", avg_waiting_time);
}
void non_preemptive_priority(struct tasks *tasks_array, int no_of_records, FILE *fout) {
    int time = 0, total_waiting_time = 0, tasks_completed = 0;
    bool completed[MAX_TASKS] = {false}; // make all tasks uncompleted

    fprintf(fout, "\nNSJF:\n"); // title of the algorithm
    
    while (tasks_completed < no_of_records) {
        int shortest_job = -1, shortest_time = SHORTEST_TIME; 
        
        for (int i = 0; i < no_of_records; i++) { // shortest (time) arrived task
            if (!completed[i] && tasks_array[i].arrival_time <= time && tasks_array[i].burst_time < shortest_time) {
                shortest_time = tasks_array[i].burst_time;
                shortest_job = i;
            }
        }

        if (shortest_job == -1) { // no arrived job (impossible in our case but good practice)
            time++; 
        } else { // execute the shortest job
            int start_time = time; // save the start time
            fprintf(fout, "%s\t%d\t%d\n", tasks_array[shortest_job].name, start_time, time + tasks_array[shortest_job].burst_time);
            time += tasks_array[shortest_job].burst_time;
            tasks_array[shortest_job].waiting_time = time - tasks_array[shortest_job].arrival_time - tasks_array[shortest_job].burst_time;
            total_waiting_time += tasks_array[shortest_job].waiting_time;
            completed[shortest_job] = true; // mark as completed
            tasks_completed++;
        }
    }

    float average_waiting_time = (float)total_waiting_time / no_of_records;
    for (int i = 0; i < no_of_records; i++) {
        fprintf(fout, "Waiting Time %s: %d\n", tasks_array[i].name, tasks_array[i].waiting_time);
    }
    fprintf(fout, "Average Waiting Time: %.2f\n", average_waiting_time);
}
void preemptive_priority(struct tasks *tasks_array, int no_of_records, FILE *fout) {
    int total_waiting_time=0, tasks_completed=0, current_time=0, last_start_time=0;
    int previous_task=-1, current_task=-1;

    fprintf(fout, "\nPSJF:\n"); // title of the algorithm

    for (int i = 0; i < no_of_records; i++) { // intialize the tasks
        tasks_array[i].remaining_time = tasks_array[i].burst_time;
        tasks_array[i].waiting_time = -1; 
    }

    while (tasks_completed < no_of_records) { // loop until all tasks are completed
        int index = -1, minimum_remaining_time = SHORTEST_TIME;

        for (int i = 0; i < no_of_records; i++) { // shortest arrived task
            if (tasks_array[i].arrival_time <= current_time && tasks_array[i].remaining_time > 0) {
                if (tasks_array[i].remaining_time < minimum_remaining_time) {
                    minimum_remaining_time = tasks_array[i].remaining_time;
                    index = i;
                }
            }
        }

        if (index == -1) { // no task arrive (again not possible in our case but good practice)
            current_time++;
            continue;
        }

        current_task = index; // save the current task
        if (previous_task != current_task) { // if the task is different from the previous task
            if (previous_task != -1) { // if it's not the first task
                fprintf(fout, "%s %d %d\n", tasks_array[previous_task].name, last_start_time, current_time);
            }
            last_start_time = current_time; // Update last start time for the new task
            previous_task = current_task; // Update previous task
        }

        tasks_array[current_task].remaining_time--; // decrement the remaining time for task
        current_time++; // increment the current time

        if (tasks_array[current_task].remaining_time == 0) { // task is completed
            tasks_completed++;
            if (tasks_array[current_task].waiting_time == -1) { // if the waiting time is not calculated yet
                tasks_array[current_task].waiting_time = current_time - tasks_array[current_task].arrival_time - tasks_array[current_task].burst_time;
                total_waiting_time += tasks_array[current_task].waiting_time;
            }
            if (previous_task != current_task || tasks_completed == no_of_records) { // last task or task is different from previous
                fprintf(fout, "%s %d %d\n", tasks_array[current_task].name, last_start_time, current_time);
            }
        }
    }

    // Print waiting times for each task
    for (int i = 0; i < no_of_records; i++) {
        fprintf(fout, "Waiting Time %s: %d\n", tasks_array[i].name, tasks_array[i].waiting_time);
    }

    // Calculate and print the average waiting time
    float average_waiting_time = (float)total_waiting_time / no_of_records;
    fprintf(fout, "Average Waiting Time: %.2f\n", average_waiting_time);
}

int main(int argc, char *argv[]){

    fout = fopen("Output.txt", "w"); // openning output file
    if (fout == NULL) {
    printf("Cannot open output file\n");
    return 1;
    }

    if (argc < 2) { // command line arguments check
            printf("Please provide a filename as an argument.\n");
            return 1;
        }

        FILE *fin = fopen(argv[1], "r"); // openning input file
        if (fin == NULL) {
            printf("Cannot open file\n");
            return 1;
        }

    char counting_buffer[MAX_RECORD_LEN]; // temporary record place holder (counting)
    char temp_buffer[MAX_RECORD_LEN]; // temporary record place holder (saving)
    int i = 0;

    while(fgets(counting_buffer, MAX_TASKS, fin) != NULL) { 
        i++;
    }

    int no_of_records = i; // save the number of records
    i=0; // reset the counter value
    rewind(fin); // reset file pointer to the beginning of the file

    struct tasks *tasks_array = malloc(no_of_records * sizeof(struct tasks)); // allocating memory for the array of tasks
    if (tasks_array == NULL) {
        printf("Memory allocation failed\n");
        fclose(fin);
        fclose(fout);
        return -1;
    }

    while (fgets(temp_buffer, MAX_RECORD_LEN, fin) != NULL) { // read and save the records
        char *token = strtok(temp_buffer, delimiters);
        strncpy(tasks_array[i].name, token, JOB_NAME_LEN - 1);
        tasks_array[i].name[JOB_NAME_LEN - 1] = '\0'; // null-terminate the string
        tasks_array[i].arrival_time = atoi(strtok(NULL, delimiters)); // str to int
        tasks_array[i].burst_time = atoi(strtok(NULL, delimiters)); // ""
        i++;
    }

    first_come_first_serve(tasks_array, no_of_records, fout);
    round_robin(tasks_array, no_of_records, fout);
    non_preemptive_priority(tasks_array, no_of_records, fout);
    preemptive_priority(tasks_array, no_of_records, fout);

    free(tasks_array); // free the allocated memory
    fclose(fin); // close input file
    fclose(fout); // close output file
    return 0;
}