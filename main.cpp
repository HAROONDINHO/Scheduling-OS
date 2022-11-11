#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;


typedef struct
{
    int *status;
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
    scanf("%s\n", mode);
    scanf("%d\n", &policy);
    scanf("%d\n", &instants);
    scanf("%d\n", &number_of_processes);
    for(int i=0; i<number_of_processes; i++)
    {
        scanf("%[^,],", processes[i].name);
        scanf("%d", &processes[i].arrival_time);
        scanf(",");
        scanf("%d", &processes[i].service_time);
    }
}


void FCFS(process* processes)
{
    queue <process> q;
    bool busy = false;
    process s;
    int time=0;
    while(time<=instants)
    {
        for(int i=0; i<number_of_processes; i++)
        {
            if (processes[i].arrival_time==time)
            {
                q.push(processes[i]);
            }
        }
        if((!busy) && (!q.empty()))
        {
            s = q.front();
            q.pop();
            busy=true;
        }
        if(busy)
        {
            if(s.service_time>0)
            {
                printf("%s %d->%d\n", s.name, time, time+1);
                s.service_time--;
            }
            if(s.service_time==0)
            {
                busy=false;
                s.finish_time=time+1;
                s.turnaround=s.finish_time-s.arrival_time;
            }
        }
        time++;
    }
}

/*

void RR(process* processes)
{
    queue <process> q;
    process executing;
    int time;
    for (time=0; time<=instants; time++)
    {
        for(int i=0; i<number_of_processes; i++)
        {
            if(time==processes[i].arrival_time)
            {
                q.push(processes[i]);
            }
        }

        if(!q.empty())
        {
            process s = q.front();
            q.pop();
            s.running = true;
            busy=true;
            executing = s;
            printf("%s  %d-->%d\n", executing.name, time, time+1);
            executing.service_time--;
            if(executing.service_time>0)
            {
                q.push(executing);
            }
            else
            {
                executing.finish_time=time;
                executing.turnaround=executing.finish_time-executing.arrival_time;
            }
        }
    }
}*/

void schedule()
{
    //load processes and modes
    load_file();
    printf("\n\n");
    //execution
    FCFS(processes);
}




int main()
{
    schedule();
    return 0;
}
