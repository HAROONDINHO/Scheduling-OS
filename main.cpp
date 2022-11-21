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
    string name;
    int arrival_time;
    int service_time;
    int finish_time;
    int turnaround;
    float wait_time=0;
    float response_rate;
    int FBpriority = 0;
    bool operator<(const process& s)const
    {
        return service_time> s.service_time;
    }

};

string mode;
string policy[30];
int policy_num;
int instants;
int number_of_processes;
process *processes = (process*)malloc(sizeof(process)*20);
class myComparator
{
public:
    bool operator()(const process& s1,const process& s2)
    {
        return s1.service_time<s2.service_time;
    }

};
class myComparator2
{
public:
    bool operator()(const process& s1,const process& s2)
    {
        return s1.response_rate<s2.response_rate;
    }

};

void load_file()
{
    string temp;
    cin >> mode;
    cin >> temp;
    stringstream ss0(temp);
    string word;
    while (!ss0.eof())
    {
        getline(ss0, word, ',');
        policy[policy_num++] = word;
    }
    cin >> instants;
    cin >> number_of_processes;
    for(int i=0; i<number_of_processes; i++)
    {
        string temp;
        cin >> temp;
        stringstream ss1(temp);
        string word;
        string words[3];
        int j = 0;
        while (!ss1.eof())
        {
            getline(ss1, word, ',');
            words[j++] = word;
        }
        processes[i].name = words[0];
        stringstream ss2(words[1]);
        ss2 >> processes[i].arrival_time;
        stringstream ss3(words[2]);
        ss3 >> processes[i].service_time;
    }
}

void trace_init()
{
    for(int i=0; i<number_of_processes; i++)
    {
        processes[i].status=(char *)malloc(sizeof (char)* (instants+50));
        for(int j=0; j<instants; j++)
        {
            processes[i].status[j]=' ';
        }
    }
}

void trace(process *processes,string str)
{
    printf("%-4s  ",str.c_str());
    for(int i = 0; i<=instants; i++)
    {
        printf("%d ",i%10);
    }
    printf("\n");
    for(int f=0; f<(8+(instants)*2); f++)
    {
        printf("-");
    }
    printf("\n");
    for(int x =0; x<number_of_processes; x++)
    {
        printf("%s     |",processes[x].name.c_str());
        for(int j =0; j<instants; j++)
        {
            printf("%c|",processes[x].status[j]);
        }
        if(x!=number_of_processes-1)
        {
            printf("\n");
        }
    }
    printf("\n");
    for(int f=0; f<(8+(instants)*2); f++)
    {
        printf("-");
    }
    printf("\n");
}

void status(process *processes,string str)
{
    float meansum=0;
    printf("%s\nProcess    ",str.c_str());
    for(int x = 0; x<=number_of_processes-1; x++)
    {
        printf("|  %s  ",processes[x].name.c_str());
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
    printf("|%5.2f|\nNormTurn   ",(meansum/number_of_processes));
    meansum=0;
    for(int x = 0; x<=number_of_processes-1; x++)
    {
        printf("|%5.2f",((float)processes[x].turnaround/processes[x].service_time));
        meansum=meansum+(float)processes[x].turnaround/processes[x].service_time;
    }
    printf("|%5.2f|",(float)(meansum/number_of_processes));
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
                    if (processes[x].name==s.name)
                    {
                        processes[x];
                        processes[x].finish_time=time+1;
                        processes[x].turnaround=processes[x].finish_time-processes[x].arrival_time;
                    }
                }

            }
        }

        for(int j=0; j<number_of_processes; j++)
        {
            if (processes[j].name==s.name)
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
                    if (tempb.name==processes[j].name)
                    {
                        processes[j].status[time]='.';
                    }

                }
            }
        }
        time++;

    }
    string name="FCFS";
    if ("trace"==mode)
    {
        trace(processes,name);
    }
    else
    {
        status(processes,name);
    }
}

void RR(process* processes, int qt)
{
    queue <process> q;
    int qntm = qt;
    process buffer;
    bool buff = false;
    bool busy = false;
    process s;
    process dummy;
    int time = 0;
    int track;
    while (time <= instants)
    {
        for(int i=0; i<number_of_processes; i++)
        {
            if(time==processes[i].arrival_time)
            {
                q.push(processes[i]);
            }
        }
        if(buff)
        {
            q.push(buffer);
            buff = false;
        }
        if((!busy) && (!q.empty()))
        {
            s = q.front();
            q.pop();
            track = qntm;
            busy=true;
        }
        if((!busy) && (q.empty()))
        {
            s=dummy;
        }
        if(busy)
        {
            if(s.service_time>0 && track>0)
            {
                s.service_time--;
                track--;
                if(track==0)
                {
                    busy = false;
                }
            }
            if(s.service_time>0 && !busy)
            {
                buffer = s;
                buff = true;
            }
            if(s.service_time==0)
            {

                busy=false;
                for(int x=0; x<number_of_processes; x++)
                {
                    if (processes[x].name==s.name)
                    {
                        processes[x];
                        processes[x].finish_time=time+1;
                        processes[x].turnaround=processes[x].finish_time-processes[x].arrival_time;
                    }
                }
            }
        }
        for(int j=0; j<number_of_processes; j++)
        {
            if (processes[j].name==s.name)
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
                    if (tempb.name==processes[j].name)
                    {
                        processes[j].status[time]='.';
                    }
                }
            }
        }
        time++;
    }

    string name9 = "RR-";
    std::string qts;
    qts = std::to_string(qntm);
    string name = name9 + qts;
    if ("trace"==mode)
    {
        trace(processes,name);
    }
    else
    {
        status(processes,name);
    }
}


void SRT(process* processes)
{
    priority_queue<process,vector<process>,myComparator>q;
    int qntm = 1;
    process buffer;
    bool buff = false;
    bool busy = false;
    process s;
    process dummy;
    int time = 0;
    int track;
    while (time <= instants)
    {
        for(int i=0; i<number_of_processes; i++)
        {
            if(time==processes[i].arrival_time)
            {
                q.push(processes[i]);
            }
        }
        if(buff)
        {
            q.push(buffer);
            buff = false;
        }
        if((!busy) && (!q.empty()))
        {
            s = q.top();
            q.pop();
            track = qntm;
            busy=true;
        }
        if((!busy) && (q.empty()))
        {
            s=dummy;
        }
        if(busy)
        {
            if(s.service_time>0 && track>0)
            {
                s.service_time--;
                track--;
                if(track==0)
                {
                    busy = false;
                }
            }
            if(s.service_time>0 && !busy)
            {
                buffer = s;
                buff = true;
            }
            if(s.service_time==0)
            {

                busy=false;
                for(int x=0; x<number_of_processes; x++)
                {
                    if (processes[x].name==s.name)
                    {
                        processes[x];
                        processes[x].finish_time=time+1;
                        processes[x].turnaround=processes[x].finish_time-processes[x].arrival_time;
                    }
                }
            }
        }
        for(int j=0; j<number_of_processes; j++)
        {
            if (processes[j].name==s.name)
            {
                processes[j].status[time]='*';
            }
            else
            {
                priority_queue<process,vector<process>,myComparator> tempq = q;
                while(! tempq.empty())
                {
                    process tempb = tempq.top();
                    tempq.pop();
                    if (tempb.name==processes[j].name)
                    {
                        processes[j].status[time]='.';
                    }
                }
            }
        }
        time++;
    }

    string name = "SRT";
    if ("trace"==mode)
    {
        trace(processes,name);
    }
    else
    {
        status(processes,name);
    }
}

void SPN(process* processes)
{
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
                    if (processes[x].name==s.name)
                    {
                        processes[x];
                        processes[x].finish_time=time+1;
                        processes[x].turnaround=processes[x].finish_time-processes[x].arrival_time;
                    }
                }

            }
        }

        for(int j=0; j<number_of_processes; j++)
        {
            if (processes[j].name==s.name)
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
                    if (tempb.name==processes[j].name)
                    {
                        processes[j].status[time]='.';
                    }
                }
            }
        }
        time++;
    }
    string name="SPN";
    if ("trace"==mode)
    {
        trace(processes,name);
    }
    else
    {
        status(processes,name);
    }
}

void HRRN(process* processes)
{
    priority_queue<process,vector<process>,myComparator2>q ;
    priority_queue<process,vector<process>,myComparator2>buffer ;
    priority_queue<process,vector<process>,myComparator2>dummy2 ;
    int time = 0;
    float time2 = 0;
    process s;
    process dummy;
    bool busy=false;
    while(time<instants)
    {
        buffer=dummy2;
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
                    if (processes[x].name==s.name)
                    {
                        processes[x];
                        processes[x].finish_time=time+1;
                        processes[x].turnaround=processes[x].finish_time-processes[x].arrival_time;
                    }
                }

            }
        }

        for(int j=0; j<number_of_processes; j++)
        {
            if (processes[j].name==s.name)
            {
                processes[j].status[time]='*';
            }
            else
            {
                priority_queue<process,vector<process>,myComparator2>tempq = q;
                while(! tempq.empty())
                {
                    process tempb = tempq.top();
                    tempq.pop();
                    if (tempb.name==processes[j].name)
                    {
                        processes[j].status[time]='.';
                        processes[j].wait_time=(time2-processes[j].arrival_time);
                        processes[j].response_rate=((processes[j].wait_time+processes[j].service_time)/processes[j].service_time);
                        buffer.push(processes[j]);



                    }
                }
            }
        }
        time++;
        time2++;
        q=buffer;
    }
    string name="HRRN";
    if ("trace"==mode)
    {
        trace(processes,name);
    }
    else
    {
        status(processes,name);
    }

}

/*void FB_1(process* processes)
{
    queue <process> qs[4];
    queue <process> readys;
    process buffer;
    bool buff = false;
    bool busy = false;
    process s;
    process dummy;
    int time = 0;
    int track;
    while (time < instants)
    {
        for(int i=0; i<number_of_processes; i++)
        {
            if(time==processes[i].arrival_time)
            {
                qs[0].push(processes[i]);
                readys.push(processes[i]);
            }
        }
        if(buff)
        {
            if(readys.empty())
            {
                s = buffer;
                track = 1;
                busy = true;
                buff = false;
            }
            else
            {
                qs[buffer.FBpriority].push(buffer);
                readys.push(buffer);
                buff = false;
            }
        }
        if((!busy))
        {
            for(int i=0; i<4; i++)
            {
                if(!qs[i].empty())
                {
                    s = qs[i].front();
                    s.FBpriority+=1;
                    qs[i].pop();
                    readys.pop();
                    track = 1;
                    busy=true;
                }
            }
        }
        if((!busy))
        {
            bool allempty = true;
            for(int i=0; i<4; i++)
            {
                if(!qs[i].empty())
                {
                    allempty = false;
                    break;
                }
            }
            if(allempty)
            {
                s = dummy;
            }
        }
        if(busy)
        {
            if(s.service_time>0 && track>0)
            {
                s.service_time--;
                track--;
                if(track==0)
                {
                    busy = false;
                }
            }
            if(s.service_time>0 && !busy)
            {
                buffer = s;
                buff = true;
            }
            if(s.service_time==0)
            {

                busy=false;
                for(int x=0; x<number_of_processes; x++)
                {
                    if (processes[x].name==s.name)
                    {
                        processes[x];
                        processes[x].finish_time=time+1;
                        processes[x].turnaround=processes[x].finish_time-processes[x].arrival_time;
                    }
                }
            }
        }
        for(int j=0; j<number_of_processes; j++)
        {
            if (processes[j].name==s.name)
            {
                processes[j].status[time]='*';
            }
            else
            {
                for(int k=0; k<4; k++)
                {
                    queue <process> tempq = qs[k];
                    while(! tempq.empty())
                    {
                        process tempb = tempq.front();
                        tempq.pop();
                        if (tempb.name==processes[j].name)
                        {
                            processes[j].status[time]='.';
                        }
                    }
                }


            }
        }
        time++;
    }

    string name = "FB-1";
    if ("trace"==mode)
    {
        trace(processes,name);
    }
    else
    {
        status(processes,name);
    }
}*/



void FB_1(process *processes)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q; //pair of priority level and process index
    unordered_map<int,int>remainingServiceTime; //map from process index to the remaining service time
    int j=0;
    if(processes[0].arrival_time == 0)
    {
        q.push(make_pair(0,j));
        remainingServiceTime[j]=processes[j].service_time;
        j++;
    }
    for(int time =0; time<instants; time++)
    {
        if(!q.empty())
        {
            int priorityLevel = q.top().first;
            int processIndex =q.top().second;
            int arrivalTime = processes[processIndex].arrival_time;
            int serviceTime = processes[processIndex].service_time;
            q.pop();
            while(j<number_of_processes && processes[j].arrival_time==time+1)
            {
                q.push(make_pair(0,j));
                remainingServiceTime[j]=processes[j].service_time;
                j++;
            }
            remainingServiceTime[processIndex]--;
            processes[processIndex].status[time]='*';
            if(remainingServiceTime[processIndex]==0)
            {
                processes[processIndex].finish_time=time+1;
                processes[processIndex].turnaround = (processes[processIndex].finish_time - arrivalTime);
            }
            else
            {
                if(q.size()>=1)
                    q.push(make_pair(priorityLevel+1,processIndex));
                else
                    q.push(make_pair(priorityLevel,processIndex));
            }
        }
        while(j<number_of_processes && processes[j].arrival_time==time+1)
        {
            q.push(make_pair(0,j));
            remainingServiceTime[j]=processes[j].service_time;
            j++;
        }
    }

    for(int i=0; i<number_of_processes; i++)
    {
        for(int j=processes[i].arrival_time; j<processes[i].finish_time; j++)
        {
            if(processes[i].status[j]!='*')
            {
                processes[i].status[j]='.';
            }
        }
    }

    string name = "FB-1";
    if ("trace"==mode)
    {
        trace(processes,name);
    }
    else
    {
        status(processes,name);
    }
}


void FB_2i(process *processes)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q; //pair of priority level and process index
    unordered_map<int,int>remainingServiceTime; //map from process index to the remaining service time
    int j=0;
    if(processes[0].arrival_time==0)
    {
        q.push(make_pair(0,j));
        remainingServiceTime[j]=processes[j].service_time;
        j++;
    }
    for(int time =0; time<instants; time++)
    {
        if(!q.empty())
        {
            int priorityLevel = q.top().first;
            int processIndex =q.top().second;
            int arrivalTime = processes[processIndex].arrival_time;
            int serviceTime = processes[processIndex].service_time;
            q.pop();
            while(j<number_of_processes && processes[j].arrival_time<=time+1)
            {
                q.push(make_pair(0,j));
                remainingServiceTime[j]=processes[j].service_time;
                j++;
            }

            int currentQuantum = pow(2,priorityLevel);
            int temp = time;
            while(currentQuantum && remainingServiceTime[processIndex])
            {
                currentQuantum--;
                remainingServiceTime[processIndex]--;
                processes[processIndex].status[temp++] = '*';
            }

            if(remainingServiceTime[processIndex]==0)
            {
                processes[processIndex].finish_time = temp;
                processes[processIndex].turnaround = (processes[processIndex].finish_time - arrivalTime);
            }
            else
            {
                if(q.size()>=1)
                    q.push(make_pair(priorityLevel+1,processIndex));
                else
                    q.push(make_pair(priorityLevel,processIndex));
            }
            time = temp-1;
        }
        while(j<number_of_processes && processes[j].arrival_time<=time+1)
        {
            q.push(make_pair(0,j));
            remainingServiceTime[j]=processes[j].service_time;
            j++;
        }
    }


    for(int i=0; i<number_of_processes; i++)
    {
        for(int j=processes[i].arrival_time; j<processes[i].finish_time; j++)
        {
            if(processes[i].status[j]!='*')
            {
                processes[i].status[j]='.';
            }
        }
    }

    string name = "FB-2i";
    if ("trace"==mode)
    {
        trace(processes,name);
    }
    else
    {
        status(processes,name);
    }
}


void schedule()
{
    //load processes and modes
    load_file();
    //execution
    for(int i=0; i<policy_num; i++)
    {
        trace_init();
        int x;
        int qntm;
        if (policy[i].find('-') != std::string::npos)
        {
            stringstream ss(policy[i]);
            string word;
            string words[2];
            int j = 0;
            while (!ss.eof())
            {
                getline(ss, word, '-');
                words[j++] = word;
            }
            stringstream toint(words[0]);
            toint >> x;
            stringstream toint2(words[1]);
            toint2 >> qntm;
        }


        stringstream ss(policy[i]);
        ss >> x;
        switch (x)
        {
        case 1:
            FCFS(processes);
            break;
        case 2:
            RR(processes, qntm);
            break;
        case 3:
            SPN(processes);
            break;
        case 4:
            SRT(processes);
            break;
        case 5:
            HRRN(processes);
            break;
        case 6:
            FB_1(processes);
            break;
        case 7:
            FB_2i(processes);
            break;
        case 8:
            cout <<"aging"<<endl;
            break;
        }

        printf("\n\n");
    }
}




int main()
{
    schedule();
    return 0;
}
