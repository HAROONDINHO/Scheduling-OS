#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;


typedef struct
{
    char *status;
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
    for(int i=0; i<number_of_processes; i++){
        processes[i].status=(char *)malloc(sizeof (char)* (instants+50));
            for(int j=0; j<instants; j++){
            processes[i].status[j]=' ';


    }

    }
}
void trace(process *processes,char str[])
{
    printf("%s  ",str);
    for(int i = 0;i<=instants;i++){
        printf("%d ",i%10);
    }
    printf("\n------------------------------------------------\n");
    for(int x =0;x<number_of_processes;x++){
        printf("%s     |",processes[x].name);
        for(int j =0;j<instants;j++){
                printf("%c|",processes[x].status[j]);
        }
    }
        printf("\n------------------------------------------------");

}
void stats(process *processes,char str[])
{
    float meansum=0;
    printf("%s\nProcess    |  A  ",str);
    for(int x = 1; x<=number_of_processes-1; x++)
    {
        printf("|  %c  ",processes[x].name[1]);
    }
    printf("|\nArrival    ");
    for(int x = 0; x<=number_of_processes-1; x++)
    {
        printf("|  %--*d",3,processes[x].arrival_time);
    }
    printf("|\nService    ");
    for(int x = 0; x<=number_of_processes-1; x++)
    {
        printf("|  %--*d",3,processes[x].service_time);
    }
    printf("| Mean|");
    printf("\nFinish     ");
    for(int x = 0; x<=number_of_processes-1; x++)
    {
        printf("|  %--*d",3,processes[x].finish_time);
    }
    printf("|-----|");
    printf("\nTurnaround ");
    for(int x = 0; x<=number_of_processes-1; x++)
    {
        printf("|  %--*d",3,processes[x].turnaround);
        meansum=meansum+processes[x].turnaround;
    }
    printf("| %.2f|\nNormTurn   ",(meansum/number_of_processes));
    meansum=0;
    for(int x = 0; x<=number_of_processes-1; x++)
    {
        printf("| %.2f",((float)processes[x].turnaround/processes[x].service_time));
        meansum=meansum+(float)processes[x].turnaround/processes[x].service_time;
    }
    printf("| %.2f|",(float)(meansum/number_of_processes));
}

void FCFS(process* processes)
{
    queue <process> q;
    bool busy = false;
    process s;
    process dummy;
    int time=0;
    while(time<instants)
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
        if((!busy) && (q.empty())){
            s=dummy;
        }
        if(busy)
        {
            if(s.service_time>0)
            {
                //printf("%s %d->%d\n", s.name, time, time+1);
                s.service_time--;
            }

                if(s.service_time==0)
                {

                    busy=false;
                    for(int x=0; x<number_of_processes; x++)
                    {
                        if (strcmp(processes[x].name,s.name)==0)
                        {
                            processes[x];
                            processes[x].finish_time=time+1;
                            processes[x].turnaround=processes[x].finish_time-processes[x].arrival_time;
                        }
                    }

                }
            }

            for(int j=0; j<=number_of_processes; j++)
            {
                if (strcmp(processes[j].name,s.name)==0)
                {
                    processes[j].status[time]='*';

                }
                else
                {queue <process> tempq = q;
                    while(! tempq.empty())
                    {
                        process tempb = tempq.front();
                        tempq.pop();
                        if (strcmp(tempb.name,processes[j].name)==0)
                        {
                            processes[j].status[time]='.';


                        }

                    }
                }
        }
        time++;

    }
            char name[]="FCFS";
            if (strcmp("trace",mode)==0){
                trace(processes,name);
            }
            else{
                stats(processes,name);
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
        //execution
        FCFS(processes);
    }




    int main()
    {
        schedule();
        return 0;
    }
