#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;


struct process
{
    char *status;
    char name[10];
    int arrival_time;
    int service_time;
    int finish_time;
    int turnaround;
    bool operator<(const process& s)const{
        return service_time> s.service_time;
    }




};

char mode[10];
int policy;
int instants;
int number_of_processes;
process *processes = (process*)malloc(sizeof(process)*20);
class myComparator{
    public:
        bool operator()(const process& s1,const process& s2){
            return s1.service_time>s2.service_time;
        }
};

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
    for(int i=0; i<number_of_processes; i++)
    {
        processes[i].status=(char *)malloc(sizeof (char)* (instants+50));
        for(int j=0; j<instants; j++)
        {
            processes[i].status[j]=' ';


        }

    }
}
void trace(process *processes,char str[])
{
    printf("%-4s  ",str);
    for(int i = 0; i<=instants; i++)
    {
        printf("%d ",i%10);
    }
    printf("\n------------------------------------------------\n");
    for(int x =0; x<number_of_processes; x++)
    {
        printf("%s     |",processes[x].name);
        for(int j =0; j<instants; j++)
        {
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
        printf("| %2d  ",processes[x].arrival_time);
    }
    printf("|\nService    ");
    for(int x = 0; x<=number_of_processes-1; x++)
    {
        printf("| %2d  ",processes[x].service_time);
    }
    printf("| Mean|");
    printf("\nFinish     ");
    for(int x = 0; x<=number_of_processes-1; x++)
    {
        printf("| %2d  ",processes[x].finish_time);
    }
    printf("|-----|");
    printf("\nTurnaround ");
    for(int x = 0; x<=number_of_processes-1; x++)
    {
        printf("| %2d  ",processes[x].turnaround);
        meansum=meansum+processes[x].turnaround;
    }
    printf("| %2.2f|\nNormTurn   ",(meansum/number_of_processes));
    meansum=0;
    for(int x = 0; x<=number_of_processes-1; x++)
    {
        printf("| %2.2f",((float)processes[x].turnaround/processes[x].service_time));
        meansum=meansum+(float)processes[x].turnaround/processes[x].service_time;
    }
    printf("| %2.2f|",(float)(meansum/number_of_processes));
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
        if((!busy) && (q.empty()))
        {
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
            {
                queue <process> tempq = q;
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
    if (strcmp("trace",mode)==0)
    {
        trace(processes,name);
    }
    else
    {
        stats(processes,name);
    }
}


void RR(process* processes)
{
    queue <process> q;
    process executing;
    process buffer;
    process dummy;
    dummy.name[0]='.';
    bool busy = false;
    int time=0;
    int qt=4;
    buffer=dummy;
    while(time<=instants)
    {

        for(int i=0; i<number_of_processes; i++)
        {
            if(time==processes[i].arrival_time)
            {
                q.push(processes[i]);
            }
        }
        if(strcmp(dummy.name,buffer.name)!=0)
        {
            q.push(buffer);
        }

        if(!q.empty())
        {
            process s = q.front();
            q.pop();
            busy=true;
            executing = s;
            for(int x = 0; x<number_of_processes; x++)
            {
                if(strcmp(processes[x].name,executing.name)==0)
                {
                    for(int p=0; p<qt; p++)
                    {
                        processes[x].status[time+p]='*';
                        executing.service_time=executing.service_time-qt;

                    if(executing.service_time>0)
                    {
                        buffer=executing;

                    }
                    else
                    {
                        buffer=dummy;
                        executing.finish_time=time+p;
                        processes[x].finish_time=time+p;
                        executing.turnaround=executing.finish_time-executing.arrival_time;
                        processes[x].turnaround=processes[x].finish_time-processes[x].arrival_time;
                    }
                    }
                }
                queue <process> tempq = q;

                while(! tempq.empty())
                {
                    process tempb = tempq.front();
                    tempq.pop();
                    for(int k=0; k<number_of_processes; k++)
                    {
                        if ((strcmp(tempb.name,processes[k].name)==0)&&((strcmp(tempb.name,executing.name)!=0)))
                        {
                            processes[k].status[time]='.';


                        }
                    }

                }

            }

        }
        time++;
    }



    char name[]="RR";
    if (strcmp("trace",mode)==0)
    {
        trace(processes,name);
    }
    else
    {
        stats(processes,name);
    }
}


void SPN(process* processes){
    priority_queue<process,vector<process>,myComparator>q ;
    int time = 0;
    process s;
    process dummy;
    bool busy=false;
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
            s = q.top();
            q.pop();
            busy=true;
        }
        if((!busy) && (q.empty()))
        {
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
            {
                priority_queue<process,vector<process>,myComparator>tempq = q;
                while(! tempq.empty())
                {
                    process tempb = tempq.top();
                    tempq.pop();
                    if (strcmp(tempb.name,processes[j].name)==0)
                    {
                        processes[j].status[time]='.';


                    }

                }
            }
        }
        time++;


}    char name[]="SPN";
    if (strcmp("trace",mode)==0)
    {
        trace(processes,name);
    }
    else
    {
        stats(processes,name);
    }
}


void schedule()
{
    //load processes and modes
    load_file();
    //execution

    switch(policy)
    {
    case 1:
        FCFS(processes);
        break;
    case 2:
        RR(processes);
        break;
    case 3 :
        SPN(processes);
        break;
    }
}




int main()
{
    schedule();
    //SPN(processes);
    return 0;
}
