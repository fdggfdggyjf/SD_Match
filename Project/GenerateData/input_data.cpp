/*********************************
* 输入数据的生成程序
* 生成 input_data.txt 文件 
********************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <algorithm>
#include <time.h>
#include <sstream>
using namespace std;

const int depSum = 20;
const int stuSum = 300;
string all_tags[10] = {"reading","programming","film","English","music",
							"dance","basketball","chess","running","swimming"};
string freeweek[7] = {"Mon.","Tues.","Wed.","Thurs.","Fri.","Sat.","Sun."};
string freetimes[8] = {"08:00~10:00","9:00~11:00","10:00~12:00","14:00~16~00","15:00~17:00",
							"18:00~20:00","19:00~21:00","20:00~22:00"};
string all_department_no[20] = {"D001","D002", "D003", "D004", "D005", "D006","D007","D008", "D009", "D010",
									"D011", "D012","D013","D014","D015","D016", "D017", "D018","D019","D020" };

//部门结构体
struct Department
{
	string department_no;       //部门编号（唯一确定值），字符
	int member_limit;          //各部门需要学生数的要求的上限，单个，数值，在[10,15]内；
	int tags_num;			   //部门的特点标签个数 
	int tags[10];             //各部门的特点标签，多个（两个以上），字符；
	int event_schedules_num;     //各部门的常规活动时间段个数 
	int event_schedules[7][8];  //各部门的常规活动时间段，多个（两个以上），字符。	
};

//学生结构体
struct Student
{
	string student_no;        //学生编号（唯一确定值），字符；
	int free_time_num;		 //学生空闲时间段个数 
	int free_time[7][8];    //学生空闲时间段，多个（两个以上），字符；	
	int tags_num;			 //兴趣标签个数
	int tags[10];          //兴趣标签，多个（两个以上），字符（学生的兴趣标签一定是所有部门特点标签其中的一个）
	int applications_department_num;    //部门意愿个数 
	int applications_department_flag[20];
	int applications_department[5];     //每个学生有不多于5个的部门意愿
};

//随机生成部门信息
void RandDepartment(Department *dep)
{
	srand((int)time(0));
	for(int i = 0; i < depSum; i++)
	{
		dep[i].department_no = all_department_no[i];
		
		dep[i].member_limit = unsigned(rand()%6)+10;    //部门纳新人数

		//各部门的特点标签生成 
		dep[i].tags_num = unsigned(rand()%9) + 2;
		for(int j = 0; j < dep[i].tags_num; j++)
		{
			int t = unsigned(rand() % 10);
			if(dep[i].tags[t]==1)
				j--;
			else
				dep[i].tags[t] = 1;
		}
		
		//常规活动时间段
		dep[i].event_schedules_num = unsigned(rand()%6) + 2;
		for(int j = 0; j < dep[i].event_schedules_num; j++)
		{
			int t1 = unsigned(rand()%7);
			int t2 = unsigned(rand()%8);
			if(dep[i].event_schedules[t1][t2]==1)
            	j--;
            else
            	dep[i].event_schedules[t1][t2] = 1;
		}
	}
}

//打印生成的部门信息
void PrintDepartment(Department *dep)
{
	cout << "	" << "\"departments\": ["<<endl;
	for (int i = 0;i < depSum;i++)
	{
		//固定活动时间输出 
		cout << "		{" << endl << "			\"event_schedules\": [" << endl;
		for(int j = 0, count = 0; j < 7; j++)
			for(int k = 0; k < 8; k++)
			{
				if( dep[i].event_schedules[j][k] == 1 )
				{
					count ++;
					if(count == dep[i].event_schedules_num)
						cout << "				" << "\"" << freeweek[j] << freetimes[k]<< "\"" << endl;
					else
						cout << "				" << "\"" << freeweek[j] << freetimes[k]<< "\"," << endl;
				}	
			}
		cout << "			]," << endl;

		//各部门需要学生数的要求的上限
		cout << "			\"member_limit\":\"" << dep[i].member_limit << "\"," << endl;

		//部门编号
		cout << "			\"department_no\":\"" << dep[i].department_no << "\"," << endl;


		//部门标签
		cout << "			\"tags\":[" << endl;
		for (int j = 0, count = 0;j < 10;j++)
		{
			if( dep[i].tags[j] )
			{
				count ++;
				if(count == dep[i].tags_num == 1)
					cout << "				" << "\"" << all_tags[j] << "\"" << endl;
				else
					cout << "				" << "\"" << all_tags[j] << "\"," << endl;
			}		
		}
		cout << "			]" << endl;


		if (i == depSum-1)
			cout << "		}" << endl;
		else
			cout << "		}," << endl;
	}
	cout << "	]" << endl<<"}";
}

//随机生成学生信息
void RandStudent(Student *stu)
{
	srand((int)time(0));
	for(int i = 0; i < stuSum; i++)
	{
		stringstream ss;
		string str;
		ss<<(i+1);
    	ss>>str;
    	
		if(i<9)
			stu[i].student_no="03150200" + str;
		else if(i<99)
			stu[i].student_no="0315020" + str;
		else
			stu[i].student_no="031502" + str;

		//每个学生有不多于5个的部门意愿
		stu[i].applications_department_num = unsigned(rand()%6);
		for(int j = 0; j < stu[i].applications_department_num; j++)
		{
			int t = unsigned(rand() % 20);
			if( stu[i].applications_department_flag[t] == 1)
				j--;
			else
			{
				stu[i].applications_department[j] = t;
				stu[i].applications_department_flag[t] = 1;
			}	
		}
		
		//兴趣标签生成
		stu[i].tags_num = rand() % 9 + 2;
		for(int j = 0; j < stu[i].tags_num; j++)
		{
			int t = unsigned(rand() % 10);
			if(stu[i].tags[t] == 1 )
				j--;
			else
				stu[i].tags[t] = 1;
		}
		
		//学生空闲时间段生成
		stu[i].free_time_num = unsigned(rand()%6) + 2;
		for(int j = 0; j < stu[i].free_time_num; j++)
		{
			int t1 = unsigned(rand()%7);
			int t2 = unsigned(rand()%8);
			if(stu[i].free_time[t1][t2] == 1 )
            	j--;
            else
            	stu[i].free_time[t1][t2] = 1;
		}
	}
}

//打印生成的学生信息
void PrintStudent(Student *stu)
{
	cout << "{" << endl<<"	"<<"\"students\":["<<endl;
	for (int i = 0;i < stuSum;i++)
	{	
		//空闲时间输出
		cout << "		{" << endl << "			\"free_time\":[" << endl;
		for(int j = 0, count = 0; j < 7; j++)
			for(int k = 0; k < 8; k++)
			{
				if( stu[i].free_time[j][k] ==1 )
				{
					count ++;
					if( count == stu[i].free_time_num)
						cout << "				" << "\"" << freeweek[j] << freetimes[k]<< "\"" << endl;
					else
						cout << "				" << "\"" << freeweek[j] << freetimes[k]<< "\"," << endl;						
				}
			}
		cout << "			]," << endl;
		
		

		//学号输出 
		cout << "			\"student_no\":\"" << stu[i].student_no <<"\","<<endl;

		//部门志愿输出 
		cout << "			\"applications_department\":[" << endl;
		for (int j = 0, count = 0;j < stu[i].applications_department_num;j++)
		{
			if( j == stu[i].applications_department_num-1)
				cout << "				" << "\"" << all_department_no[stu[i].applications_department[j]] << "\"" << endl;
			else
				cout << "				" << "\"" << all_department_no[stu[i].applications_department[j]] << "\"," << endl;				
		}
		cout << "			]," << endl;

		//标签输出 
		cout << "			\"tags\":[" << endl;
		for (int j = 0, count = 0; j < 10; j++)
		{
			if( stu[i].tags[j] ==1 )
			{
				count ++;
				if( count == stu[i].tags_num )
					cout << "				" << "\"" << all_tags[j] << "\"" << endl;
				else
					cout << "				" << "\"" << all_tags[j] << "\"," << endl;				
			}
		}
		cout << "			]" << endl;


		if(i == stuSum-1)
			cout << "		}" << endl;
		else
			cout << "		}," << endl;
	}
	cout << "	]," << endl;
}

int main()
{
	Student stu[stuSum];
	Department dep[depSum];
	
	for(int i=0; i<depSum; i++)
	{
		memset(dep[i].tags,0,sizeof(dep[i].tags));
		memset(dep[i].event_schedules,0,sizeof(dep[i].event_schedules));
	}
	for(int i=0; i<stuSum; i++)
	{
		memset(stu[i].free_time,0,sizeof(stu[i].free_time));
		memset(stu[i].tags,0,sizeof(stu[i].tags));
		memset(stu[i].applications_department,0,sizeof(stu[i].applications_department));
	}
	
	freopen(".//input_data.txt", "w", stdout);
	
	RandStudent(stu);
	PrintStudent(stu);
	
	RandDepartment(dep); 
	PrintDepartment(dep);
	
	return 0;
}
