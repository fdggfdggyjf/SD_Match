/*********************************
* �������ݵ����ɳ���
* ���� input_data.txt �ļ� 
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

//���Žṹ��
struct Department
{
	string department_no;       //���ű�ţ�Ψһȷ��ֵ�����ַ�
	int member_limit;          //��������Ҫѧ������Ҫ������ޣ���������ֵ����[10,15]�ڣ�
	int tags_num;			   //���ŵ��ص��ǩ���� 
	int tags[10];             //�����ŵ��ص��ǩ��������������ϣ����ַ���
	int event_schedules_num;     //�����ŵĳ���ʱ��θ��� 
	int event_schedules[7][8];  //�����ŵĳ���ʱ��Σ�������������ϣ����ַ���	
};

//ѧ���ṹ��
struct Student
{
	string student_no;        //ѧ����ţ�Ψһȷ��ֵ�����ַ���
	int free_time_num;		 //ѧ������ʱ��θ��� 
	int free_time[7][8];    //ѧ������ʱ��Σ�������������ϣ����ַ���	
	int tags_num;			 //��Ȥ��ǩ����
	int tags[10];          //��Ȥ��ǩ��������������ϣ����ַ���ѧ������Ȥ��ǩһ�������в����ص��ǩ���е�һ����
	int applications_department_num;    //������Ը���� 
	int applications_department_flag[20];
	int applications_department[5];     //ÿ��ѧ���в�����5���Ĳ�����Ը
};

//������ɲ�����Ϣ
void RandDepartment(Department *dep)
{
	srand((int)time(0));
	for(int i = 0; i < depSum; i++)
	{
		dep[i].department_no = all_department_no[i];
		
		dep[i].member_limit = unsigned(rand()%6)+10;    //������������

		//�����ŵ��ص��ǩ���� 
		dep[i].tags_num = unsigned(rand()%9) + 2;
		for(int j = 0; j < dep[i].tags_num; j++)
		{
			int t = unsigned(rand() % 10);
			if(dep[i].tags[t]==1)
				j--;
			else
				dep[i].tags[t] = 1;
		}
		
		//����ʱ���
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

//��ӡ���ɵĲ�����Ϣ
void PrintDepartment(Department *dep)
{
	cout << "	" << "\"departments\": ["<<endl;
	for (int i = 0;i < depSum;i++)
	{
		//�̶��ʱ����� 
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

		//��������Ҫѧ������Ҫ�������
		cout << "			\"member_limit\":\"" << dep[i].member_limit << "\"," << endl;

		//���ű��
		cout << "			\"department_no\":\"" << dep[i].department_no << "\"," << endl;


		//���ű�ǩ
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

//�������ѧ����Ϣ
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

		//ÿ��ѧ���в�����5���Ĳ�����Ը
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
		
		//��Ȥ��ǩ����
		stu[i].tags_num = rand() % 9 + 2;
		for(int j = 0; j < stu[i].tags_num; j++)
		{
			int t = unsigned(rand() % 10);
			if(stu[i].tags[t] == 1 )
				j--;
			else
				stu[i].tags[t] = 1;
		}
		
		//ѧ������ʱ�������
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

//��ӡ���ɵ�ѧ����Ϣ
void PrintStudent(Student *stu)
{
	cout << "{" << endl<<"	"<<"\"students\":["<<endl;
	for (int i = 0;i < stuSum;i++)
	{	
		//����ʱ�����
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
		
		

		//ѧ����� 
		cout << "			\"student_no\":\"" << stu[i].student_no <<"\","<<endl;

		//����־Ը��� 
		cout << "			\"applications_department\":[" << endl;
		for (int j = 0, count = 0;j < stu[i].applications_department_num;j++)
		{
			if( j == stu[i].applications_department_num-1)
				cout << "				" << "\"" << all_department_no[stu[i].applications_department[j]] << "\"" << endl;
			else
				cout << "				" << "\"" << all_department_no[stu[i].applications_department[j]] << "\"," << endl;				
		}
		cout << "			]," << endl;

		//��ǩ��� 
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
