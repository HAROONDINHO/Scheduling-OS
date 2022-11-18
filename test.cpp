#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

struct process
{
    string status;
    string name;
    int arrival_time;
    int service_time;
    int finish_time;
    int turnaround;
    bool operator<(const process& s)const
    {
        return service_time> s.service_time;
    }
};

string mode;
string policy[10];
int policy_num;
int instants;
int number_of_processes;
process *processes = (process*)malloc(sizeof(process)*20);

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
/*if (s1.find(s2) != std::string::npos) {
    std::cout << "found!" << '\n';
}*/
int main()
{
    load_file();
    printf("\n\n\n");
    for(int i=0; i<number_of_processes; i++)
    {
        cout << processes[i].name << "  " << processes[i].arrival_time << "  " << processes[i].service_time << endl;
    }
    printf("\n\n\n");
    for(int i=0; i<policy_num; i++)
    {
        int x;
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
            cout << "rr-" << words[1] << endl;
        }
        else
        {
            stringstream ss(policy[i]);
            ss >> x;
            switch (x)
            {
            case 1:
                cout <<"fcfs"<<endl;
                break;
            case 3:
                cout <<"spn"<<endl;
                break;
            case 4:
                cout <<"srt"<<endl;
                break;
            case 5:
                cout <<"hrrn"<<endl;
                break;
            case 6:
                cout <<"fb1"<<endl;
                break;
            case 7:
                cout <<"fb2"<<endl;
                break;
            case 8:
                cout <<"aging"<<endl;
                break;
            }
        }
    }
    return 0;
}
