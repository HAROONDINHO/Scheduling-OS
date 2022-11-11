#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;


typedef struct
{
    char name[10];
    int arrival_time;
    int service_time;
    int finish_time;
    int turnaround;
} process;

char mode[10];
int policy;
int instants;
int number_of_processes;
process *processes = (process*)malloc(sizeof(process)*20);

void load_file()
{
    char s[20];
    int p = 0;
    printf("enter input filename: ");
    scanf("%s", s);
    FILE *f=fopen(s, "r");
    if (f!= NULL)
    {
        fscanf(f, "%s\n", mode);
        fscanf(f, "%d\n", &policy);
        fscanf(f, "%d\n", &instants);
        fscanf(f, "%d\n", &number_of_processes);
        while(!feof(f))
        {
            fscanf(f, "%[^,],", processes[p].name);
            fscanf(f, "%d", &processes[p].arrival_time);
            fscanf(f, ",");
            fscanf(f, "%d", &processes[p].service_time);
            fscanf(f, "\n");
            p++;
        }
    }
    else
    {
        printf("INPUT FILE NOT FOUND!\n");
        load_file();
    }
}

void load_stdin()
{
    printf("enter mode [trace-stats]: ");
    scanf("%s", mode);
    printf("\nenter scheduling policy(s) [1:FSCS 2:RR 3:SPN 4:SRT 5:HRRN 6:FB-1 7:FB-2i 8:aging] : ");
    scanf("%d", &policy);
    printf("\nenter number of time instants: ");
    scanf("%d", &instants);
    printf("\nenter number of processes: ");
    scanf("%d", &number_of_processes);
    for(int i=0; i<number_of_processes; i++)
    {
        printf("\nenter process[%d] name: ", i);
        scanf("%s", processes[i].name);
        printf("enter process[%d] arrival time: ", i);
        scanf("%d", &processes[i].arrival_time);
        printf("enter process[%d] service time: ", i);
        scanf("%d", &processes[i].service_time);
    }
}

void load()
{
    int m;
    printf("enter 0 to load from file or 1 to manually enter data: ");
    scanf("%d", &m);
    if (m==0)
    {
        load_file();
    }
    else if(m==1)
    {
        load_stdin();
    }
    else
    {
        printf("WRONG INPUT!\n");
        load();
    }
}

void schedule()
{
    //load processes and modes
    load();

    //execution
    for(int i=0; i<number_of_processes; i++)
    {
        printf("%s,%d,%d\n", processes[i].name, processes[i].arrival_time, processes[i].service_time);
    }
}


int main()
{
    while (true)
    {
        schedule();
        printf("\n\n\n");
    }
    return 0;
}
