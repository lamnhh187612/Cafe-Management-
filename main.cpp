#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <time.h>


using namespace std;

struct Time
{
    int day,month,year;
};

struct Group
{
    int id;
    int num_student;
    vector<string> member;
};

struct Project
{
    int id;
    string description;
    Time due_date;
    Time submission_date;
    string state;
    string specific_state;
    string current_state;
};

struct Group_submission
{
    Group data;
    vector<Project> info;
};

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

bool is_greater_or_equal(Time a,Time b)
{
    if(a.year<b.year){
        return false;
    }
    else if(a.year>b.year){
        return true;
    }
    else if(a.year==b.year){
        if(a.month>b.month){
            return true;
        }
        else if(a.month<b.month){
            return false;
        }
        else if(a.month==b.month){
            if(a.day<b.day){
                return false;
            }
            else if(a.day>=b.day){
                return true;
            }
        }
    }
}

bool if_valid_name(vector<string> arr,string subtext)
{
    int same=0;
    bool result=true;
    for(int i=0;i<arr.size();i++){
            if(arr[i]==subtext){
                result= false;
            }
    }
    return result;
}

void group_information(int* num_group,Group* group,Group_submission* all)
{
    vector<string>people;
    people.clear();
    int option,same;
    do{
        cout<<"------------------------------"<<endl;
        cout<<"1. Input information of group"<<endl;
        cout<<"2. Display information"<<endl;
        cout<<"3. Save group information"<<endl;
        cout<<"4. Load group information"<<endl;
        cout<<"5. Back to main menu"<<endl;
        cout<<"------------------------------"<<endl;
        cout<<"Your option : ";
        cin>>option;
        switch(option)
        {
        case 1:
            {
                string name;

                for(int i=0;i<(*num_group);i++){
                    do{
                        cout<<"Enter the number of member in group "<<i+1<<" : ";
                        cin>>group[i].num_student;
                        cin.ignore();
                        group[i].member.clear();
                        for(int j=0;j<group[i].num_student;j++){

                            cout<<"Enter the name of member "<<j+1<<" : ";
                            getline(cin, name);

                            if(if_valid_name(people,name)==false){
                                cout<<"This student is already in group "<<endl;
                                j--;
                            }
                            else if(if_valid_name(people,name)==true){
                                people.push_back(name);
                                group[i].member.push_back(name);
                            }
                        }
                        cin.ignore();
                    }while(group[i].num_student<1 || (*num_group)%1!=0);
                }
                break;
            }

        case 2:
            {
                cout<<"Group"<<setw(8)<<"Student's name"<<endl;
                for(int i=0;i<(*num_group);i++){
                    for(int j=0;j<group[i].num_student;j++){
                            cout<<i+1<<setw(8)<<group[i].member[j]<<endl;
                    }
                }
                break;
            }

        case 3:
            {
                fstream file;
                file.open("group.txt",ios::out);
                if(file.is_open()){
                    file<<"Group"<<setw(8)<<"Student's name"<<'\n';
                    int i=0;
                    while(!file.eof() && i<(*num_group)){
                        for(int j=0;j<group[i].num_student;j++){
                            file<<i+1<<setw(8)<<group[i].member[j]<<'\n';
                        }
                        i++;
                    }
                }
                else if(file.fail()){
                    cout<<"File cannot open"<<endl;
                }
                cout<<"File has saved"<<endl;
                file.close();
                break;
            }

        case 4:
            {
                fstream file;
                file.open("group.txt",ios::in);
                string line;
                if(file.is_open()){
                    getline(file,line);
                    int i=0;
                    while(!file.eof() && i<(*num_group)){
                        string person;
                        for(int j=0;j<group[i].num_student;j++){
                            file>>all[i].data.id>>person;
                            group[i].member.push_back(person);
                        }
                        i++;
                    }
                }
                cout<<"File has loaded"<<endl;
                file.close();
                break;
            }

        }
    }while(option!=5);
}

void deadline_and_submit(Project* project,int* num_project,Group_submission* all,int* num_group)
{
    int option;
    do{
        cout<<"------------------------------"<<endl;
        cout<<"1. Deadline declaration"<<endl;
        cout<<"2. Submit project"<<endl;
        cout<<"3. Back to main menu"<<endl;
        cout<<"------------------------------"<<endl;
        cout<<"Your option : ";
        cin>>option;
        switch(option)
        {
        case 1:
            {
                cout<<"Deadline declaration : "<<endl;
                for(int i=0;i<(*num_project);i++){
                    all[i].info.clear();
                    cin.ignore();
                    cout<<"  Project "<<i+1<<" : "<<endl;
                    cout<<"  Description : ";
                    getline(cin,project->description);
                    cout<<"  Sumission deadline (seperated by space) : ";
                    cin>>project->due_date.day>>project->due_date.month>>project->due_date.year;
                    cin.ignore();
                    for(int j=0;j<(*num_group);j++){
                        all[j].info.push_back(*project);
                    }
                }
                break;
            }
        case 2:
            {
                cout<<"Submit project (seperated by space) : "<<endl;
                cout<<"Note : Enter three '0' if group not submited yet. "<<endl;
                cin.ignore();
                for(int i=0;i<(*num_group);i++){
                    cout<<"  Group "<<i+1<<" : "<<endl;
                    for(int j=0;j<(*num_project);j++){
                        cout<<"    Project "<<j+1<<" : ";
                        cin>>all[i].info[j].submission_date.day>>all[i].info[j].submission_date.month>>all[i].info[j].submission_date.year;
                        cin.ignore();
                        if(is_greater_or_equal(all[i].info[j].due_date,all[i].info[j].submission_date)==true){
                            all[i].info[j].state="On time";
                        }
                        else if(is_greater_or_equal(all[i].info[j].due_date,all[i].info[j].submission_date)==false){
                            all[i].info[j].state="Late";
                        }
                        else if(all[i].info[j].submission_date.day==0
                                && all[i].info[j].submission_date.month==0
                                && all[i].info[j].submission_date.year==0){
                            all[i].info[j].state="Not yet submited";
                        }
                    }
                }
                break;
            }
        }
    }while(option!=3);

}

void Statistic(int* num_project,int* num_group,Group_submission* all)
{
    int option;
    do{
        cout<<"------------------------------"<<endl;
        cout<<"1. The state of submission of a project for groups"<<endl;
        cout<<"2. The state of submission of all projects of a group"<<endl;
        cout<<"3. Back to main menu"<<endl;
        cout<<"------------------------------"<<endl;
        cout<<"Your option : ";
        cin>>option;
        switch(option)
        {
        case 1:
            {
                int id;
                cout<<"Input a project number : ";
                cin>>id;
                for(int i=0;i<(*num_group);i++){
                    cout<<"  Group "<<i+1<<" : "<<all[i].info[id-1].state<<endl;
                }
                break;
            }
        case 2:
            {
                int id;
                cout<<"Input a group number : ";
                cin>>id;
                for(int i=0;i<(*num_project);i++){
                    cout<<"  Project "<<i+1<<" : "<<all[id-1].info[i].state<<endl;
                }
                break;
            }
        }

    }while(option!=3);
}


void Overall_Statistic(Time &specific, Time &current, Group_submission* all, int* num_project, int* num_group)
{
    char choice;
    cout<<"between the beginning of the course to a specific date(S) or to current time(C)? - ";
    cin>>choice;
    if(choice=='s' || choice=='S'){
        cout<<"Enter a specific date (seperated by space) : ";
        cin>>specific.day>>specific.month>>specific.year;
        for(int i=0;i<(*num_group);i++){
            for(int j=0;j<(*num_project);j++){
                if(is_greater_or_equal(specific,all[i].info[j].submission_date)==true){
                    all[i].info[j].specific_state="Submited";
                    cout<<"  Project "<<i+1<<" : "<<all[i].info[j].specific_state<<endl;
                }
                else if(is_greater_or_equal(specific,all[i].info[j].submission_date)==false){
                    all[i].info[j].specific_state="Not yet submited";
                    cout<<"  Project "<<i+1<<" : "<<all[i].info[j].specific_state<<endl;
                }
            }
        }
    }
    else if(choice=='c' || choice=='C'){
        string now=currentDateTime();
        current.day=stoi(now.substr(0, 4));
        current.month=stoi(now.substr(5, 2));
        current.day=stoi(now.substr(8, 2));
        for(int i=0;i<(*num_group);i++){
            for(int j=0;j<(*num_project);j++){
                if(is_greater_or_equal(current,all[i].info[j].submission_date)==true){
                    all[i].info[j].current_state="Submited";
                    cout<<"  Project "<<i+1<<" : "<<all[i].info[j].current_state<<endl;
                }
                else if(is_greater_or_equal(current,all[i].info[j].submission_date)==false){
                    all[i].info[j].current_state="Not yet submited";
                    cout<<"  Project "<<i+1<<" : "<<all[i].info[j].current_state<<endl;
                }
            }
        }
    }
}

int main()
{
    //1
    Time lam,dat;
    lam.day=24;
    lam.month=3;
    lam.year=2021;

    dat.day=24;
    dat.month=6;
    dat.year=2021;

    cout<<is_greater_or_equal(lam,dat)<<endl;
    Project lan;
    lan.state="ba la ga";
    cout<<lan.state<<endl;


    int total_group;
    do{
                    cout<<"Enter the number of groups in the class : ";
                    cin>>total_group;
                }while(total_group<1 || total_group%1!=0);
    Group* b=new Group[total_group];
    Group_submission* c=new Group_submission[total_group];
    int* d=&total_group;
    group_information(d,b,c);

    //2

    int total_project;
    //add inputvalidation
    cout<<"Enter the number of projects for all course : ";
    cin>>total_project;
    Project* e=new Project[total_project];
    int* g= &total_project;
    deadline_and_submit(e,g,c,d);
    cout<<"ex :"<<endl;
    cout<<c[0].info[0].description<<endl;
    cout<<c[0].info[0].submission_date.day<<endl;
    Statistic(g,d,c);
    Time l,k;
    Overall_Statistic(l, k, c, g, d);
    delete [] b;
    delete [] c;
    delete [] e;

    return 0;
}
