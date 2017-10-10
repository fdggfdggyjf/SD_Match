// ConsoleApplication7.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "json.h"
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;
using namespace Json;

int depSum;
int stuSum;
string all_department_no[30] = { "D001","D002", "D003", "D004", "D005", "D006","D007","D008", "D009", "D010","D011", "D012","D013","D014","D015","D016", "D017", "D018","D019","D020" };

//���Žṹ��
struct Department
{
	int department_no;       //���ű�ţ�Ψһȷ��ֵ�����ַ�
	int member_limit;          //��������Ҫѧ������Ҫ������ޣ���������ֵ����[10,15]�ڣ�
	int tags_num;			   //���ŵ��ص��ǩ���� 
	string tags[10];             //�����ŵ��ص��ǩ��������������ϣ����ַ���
	int event_schedules_num;     //�����ŵĳ���ʱ��θ��� 
	long event_schedules[2][50];  //�����ŵĳ���ʱ��Σ�������������ϣ����ַ���
	int join_student_num=0;         //���������յ�ѧ������
	string join_student_no[15];     //���������յ�ѧ��
	double join_student_point[15];      //���������յ�ѧ������
};

//ѧ���ṹ��
struct Student
{
	string student_no;        //ѧ����ţ�Ψһȷ��ֵ�����ַ���
	int free_time_num;		 //ѧ������ʱ��θ��� 
	long free_time[2][50];    //ѧ������ʱ��Σ�������������ϣ����ַ���
	bool free_time_used[20][50];  //���ʱ����Ƿ�ʹ��
	int tags_num;			 //��Ȥ��ǩ����
	string tags[30];          //��Ȥ��ǩ��������������ϣ����ַ���ѧ������Ȥ��ǩһ�������в����ص��ǩ���е�һ����
	int applications_department_num;    //������Ը���� 
	int applications_department[5];     //ÿ��ѧ���в�����5���Ĳ�����Ը
	int join_Department_num=0;            //��ǰ����Ĳ��Ÿ���
	double point[20] = {0};    //���Ŷ�ѧ���Ĵ��
};

//���ַ�����Ϊ����
int string_to_int(string str)
{
	stringstream ss;
	int i_number;
	ss << str ;
	ss >> i_number;
	return i_number;
}

// ��ȡjson�ļ��������ݴ���ṹ������
void Read_json_txt(Student *stu,Department *dep)
{
	Json::Reader read;
	Json::Value root;
	ifstream is("input_data.txt", ios::binary);
	if (!is.is_open()) 
	{
		cout << "Error opening file\n";
	}
	else 
	{
		if (read.parse(is, root)) 
		{
			// ��ȡѧ����Ϣ
			stuSum = root["students"].size();
			for (int i = 0; i < stuSum; i++)
			{
				// ��ȡѧ������ʱ��
				//����json�е�ʱ���ʽ��Ϊ���ӣ�������һ00��00Ϊ0��������
				stu[i].free_time_num = root["students"][i]["free_time"].size();
				for (int j = 0; j < stu[i].free_time_num; j++)
				{
					string str = root["students"][i]["free_time"][j].asString();
					unsigned int dot_loc = str.find('.');
					unsigned int front_colon_loc = str.find(':');

					unsigned int wave_loc = str.find('~');
					unsigned int behind_colon_loc = str.find(":", wave_loc);

					string Str_Week = str.substr(0, dot_loc);
					
					int Int_Week=0;
					if (Str_Week == "Mon")
					{
						Int_Week = 0;
					}
					else if (Str_Week == "Tues")
					{
						Int_Week = 1;
					}
					else if (Str_Week == "Wed")
					{
						Int_Week = 2;
					}
					else if (Str_Week == "Thur")
					{
						Int_Week = 3;
					}
					else if (Str_Week == "Fri")
					{
						Int_Week = 4;
					}
					else if (Str_Week == "Sat")
					{
						Int_Week = 5;
					}
					else if (Str_Week == "Sun")
					{
						Int_Week = 6;
					}

					//����ǰ�벿�ֵ�ʱ��
					string hhour_str = str.substr(dot_loc + 1, front_colon_loc - dot_loc - 1);
					int hhour_int = string_to_int(hhour_str);
					string hminute_str = str.substr(front_colon_loc + 1, wave_loc - front_colon_loc - 1);
					int hminute_int = string_to_int(hminute_str);

					//������벿�ֵ�ʱ��
					string ehour_str = str.substr(wave_loc + 1, behind_colon_loc - wave_loc - 1);
					int ehour_int = string_to_int(ehour_str);
					string eminute_str = str.substr(behind_colon_loc + 1, str.size() - behind_colon_loc - 1);
					int eminute_int = string_to_int(eminute_str);

					stu[i].free_time[0][j] = Int_Week * 3600 + hhour_int * 60 + hminute_int;
					stu[i].free_time[1][j] = Int_Week * 3600 + ehour_int * 60 + eminute_int;
				}

				// ��ȡѧ��ѧ��
				stu[i].student_no =  root["students"][i]["student_no"].asString();

				// ��ȡѧ��������Ը
				stu[i].applications_department_num = root["students"][i]["applications_department"].size();
				for (int j = 0; j < stu[i].applications_department_num; j++)
				{
					string str_applications_department = root["students"][i]["applications_department"][j].asString();
					for (int k = 0; k < 20; k++)
					{
						if (str_applications_department == all_department_no[k])
							stu[i].applications_department[j] = k;
					}
				}

				// ��ȡѧ����Ȥ��ǩ
				stu[i].tags_num = root["students"][i]["tags"].size();
				for (int j = 0; j < stu[i].tags_num; j++) 
				{
					stu[i].tags[j] = root["students"][i]["tags"][j].asString();
				}
			}

			//���������Ϣ
			depSum = root["departments"].size();
			for (int i = 0; i < depSum; i++)
			{
				dep[i].event_schedules_num = root["departments"][i]["event_schedules"].size();
				for (int j = 0; j < dep[i].event_schedules_num; j++)
				{
					string str = root["departments"][i]["event_schedules"][j].asString();
					unsigned int dot_loc = str.find('.');
					unsigned int front_colon_loc = str.find(':');

					unsigned int wave_loc = str.find('~');
					unsigned int behind_colon_loc = str.find(":", wave_loc);

					string Str_Week = str.substr(0, dot_loc);

					int Int_Week=0;
					if (Str_Week == "Mon")
					{
						Int_Week = 0;
					}
					else if (Str_Week == "Tues")
					{
						Int_Week = 1;
					}
					else if (Str_Week == "Wed")
					{
						Int_Week = 2;
					}
					else if (Str_Week == "Thur")
					{
						Int_Week = 3;
					}
					else if (Str_Week == "Fri")
					{
						Int_Week = 4;
					}
					else if (Str_Week == "Sat")
					{
						Int_Week = 5;
					}
					else if (Str_Week == "Sun")
					{
						Int_Week = 6;
					}

					//����ǰ�벿�ֵ�ʱ��
					string hhour_str = str.substr(dot_loc + 1, front_colon_loc - dot_loc - 1);
					int hhour_int = string_to_int(hhour_str);
					string hminute_str = str.substr(front_colon_loc + 1, wave_loc - front_colon_loc - 1);
					int hminute_int = string_to_int(hminute_str);

					//������벿�ֵ�ʱ��
					string ehour_str = str.substr(wave_loc + 1, behind_colon_loc - wave_loc - 1);
					int ehour_int = string_to_int(ehour_str);
					string eminute_str = str.substr(behind_colon_loc + 1, str.size() - behind_colon_loc - 1);
					int eminute_int = string_to_int(eminute_str);

					dep[i].event_schedules[0][j] = Int_Week * 3600 + hhour_int * 60 + hminute_int;
					dep[i].event_schedules[1][j] = Int_Week * 3600 + ehour_int * 60 + eminute_int;
				}

				dep[i].member_limit = string_to_int(root["departments"][i]["member_limit"].asString());

				string str_department_no = root["departments"][i]["department_no"].asString();
				for (int k = 0; k < 20; k++)
				{
					if (str_department_no == all_department_no[k])
						dep[i].department_no = k;
				}

				dep[i].tags_num = root["departments"][i]["tags"].size();
				for (int j = 0; j < dep[i].tags_num; j++)
				{
					dep[i].tags[j] = root["departments"][i]["tags"][j].asString();
				}
			}

		}

	}
	is.close();
}

// ����ѧ����ÿ�����ŵĵ÷�
void SD_Points(Student *stu, Department *dept)
{
	for (int i = 0; i < stuSum; i++)
	{
		for (int j = 0; j < depSum; j++)
		{
			//����־Ը��˳�����Ϊ50��30��10��7��3
			for (int k = 0; k < stu[i].applications_department_num; k++)
			{
				if (dept[j].department_no == stu[i].applications_department[k])
				{
					if (k == 0)
						stu[i].point[j] += 50;
					else if (k == 1)
						stu[i].point[j] += 30;
					else if (k == 2)
						stu[i].point[j] += 10;
					else if (k == 3)
						stu[i].point[j] += 7;
					else
						stu[i].point[j] += 3;
				}
			}

			//��Ȥ��ǩ��һ����ǩ�����10��,
			for (int k = 0; k < stu[i].tags_num; k++)
			{
				for (int r = 0; r < dept[j].tags_num; r++)
				{
					if (stu[i].tags[k] == dept[j].tags[r])
						stu[i].point[j] += 10;
				}
			}

			//ʱ���ص�����1Сʱ�����ǣ��ص�1Сʱ40�֣�2Сʱ50�֣�3Сʱ����55�֣����һ�ζ�û�з�����Ϊ0
			int time_overlap_count=0;
			for (int k = 0; k < stu[i].free_time_num; k++)
			{
				for(int r = 0; r < dept[j].event_schedules_num; r++)
				{
					int time_overlap = min( dept[j].event_schedules[1][r], stu[i].free_time[1][k]) - max( dept[j].event_schedules[0][r], stu[i].free_time[0][k]);
					if (time_overlap > 0)
						time_overlap_count += time_overlap / 60;
				}
			}
			if (time_overlap_count == 0)
				stu[i].point[j] = 0;
			else  if(time_overlap_count == 1)
				stu[i].point[j] += 40;
			else  if(time_overlap_count == 2)
				stu[i].point[j] += 50;
			else
				stu[i].point[j] += 55;
		}
	}
}


// ƥ���㷨
void matching(Student *stu, Department *dept)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < stuSum; j++)
		{
			if (i < stu[j].applications_department_num)
			{
				int temp = stu[j].applications_department[i];

				if (dept[temp].member_limit > dept[temp].join_student_num)  //������Ż�����Ա�����ѧ��ֱ����ѡ
				{
					dept[temp].join_student_no[dept[temp].join_student_num] = stu[j].student_no;
					dept[temp].join_student_point[dept[temp].join_student_num] = stu[j].point[temp];
					dept[temp].join_student_num ++;
					stu[j].join_Department_num++;
				}
				else //���û������Ļ����벿���е�������ֵ��˱Ƚ�,�����Ƿ��滻
				{
					int min_point_index = 0;
					for (int k = 1; k < dept[temp].member_limit; k++)
					{
						if (dept[temp].join_student_point[k] < dept[temp].join_student_point[min_point_index])
						{
							min_point_index = k;
						}
					}

					//���Ŷ���ѧ�������ָ��������������ѡ��ѧ����������֣��滻�����������ֵ�ѧ��
					if ( dept[temp].join_student_point[min_point_index] < stu[j].point[temp])
					{
						for (int k = 0; k < stuSum; k++)
						{
							if (stu[k].student_no == dept[temp].join_student_no[min_point_index])
							{
								stu[k].join_Department_num--;
								break;
							}
						}
						dept[temp].join_student_point[min_point_index] = stu[j].point[temp];
						dept[temp].join_student_no[min_point_index] = stu[j].student_no;
						stu[j].join_Department_num++;
					}
				}
			}
		}
	}
}

void write_json_txt(Student *stu, Department *dep)
{
	ofstream outfile;
	outfile.open("output_data.txt");

	outfile << "{" << endl;
	outfile << "    " << "\"unlucky_student\": [" << endl;
	bool flag = true;
	for (int i = 0; i < stuSum; i++)
	{
		if (stu[i].join_Department_num == 0)
		{
			if (flag)
			{
				flag = false;
				outfile << "        " << "\"" << stu[i].student_no << "\"";
			}
			else
			{
				outfile << ",\n" << "        " << "\"" << stu[i].student_no << "\"";
			}
		}
	}
	outfile << "\n    ]," << endl;

	outfile << "    " << "\"admitted\": [" << endl;
	flag = true;
	for (int i = 0; i < depSum; i++)
	{
		if (dep[i].join_student_num != 0)
		{
			if (flag)
			{
				outfile << "        {" << endl;
				flag = false;
			}
			else
			{
				outfile << ",\n" << "        {" << endl;
			}
			outfile << "            \"member\": [" << endl;
			bool flag1 = true;

			for (int j = 0; j < dep[i].join_student_num; j++)
			{
				if (flag1)
				{
					outfile << "                \"" << dep[i].join_student_no[j] << "\"";
					flag1 = false;
				}
				else
				{
					outfile << ",\n" << "                \"" << dep[i].join_student_no[j] << "\"";
				}
			}
			outfile << "\n" << "            ]," << endl;
			outfile << "            \"department_no\": \"" << all_department_no[dep[i].department_no] << "\"" << endl;
			outfile << "        }";
		}
	}
	outfile <<"\n"<< "    ]," << endl;

	outfile << "    \"unlucky_department\": [" << endl;
	flag = true;
	for (int i = 0; i < depSum; i++)
	{
		if (dep[i].join_student_num == 0)
		{
			if (flag)
			{
				outfile << "        \"" << dep[i].department_no << "\"";
				flag = false;
			}
			else
			{
				outfile << ",\n" << "        \"" << dep[i].department_no << "\"";
			}
		}
	}
	outfile << "\n" << "    ]" << endl;
	outfile << "}" << endl;

	outfile.close();
}

/*
	//rootΪ���ڵ�
	Json::Value root;

	//�ӽڵ�
	Json::Value admitted;

	//δ�����յ�ѧ��
	bool flag = true;
	for (int i = 0; i < stuSum; i++)
	{
		if (stu[i].join_Department_num == 0)
		{
			flag = false;
			root["unlucky_student"].append(stu[i].student_no);
		}
	}
	if (flag)
	{
		root["unlucky_student"].resize(0);
	}

	//�����������
	flag = true;
	for (int i = 0; i < depSum; i++)
	{
		//����jsoncpp����Ϊ�ջ����null����˽��������С��0����������ָ����"[]"��ʽ������"[null]"
		if (dep[i].join_student_num != 0)
		{
			flag = false;
			admitted.clear();
			admitted["department_no"] = Json::Value(dep[i].department_no);

			for (int j = 0; j < dep[i].join_student_num; j++)
			{
				admitted["member"].append(dep[i].join_student_no[j]);
			}
			//�ӽڵ�ҵ����ڵ�
			root["admitted"].append(admitted);
		}
	}
	if (flag)
	{
		root["admitted"].resize(0);
	}

	//δ�е��˵Ĳ���
	flag = true;
	for (int i = 0; i < depSum; i++)
	{
		if (dep[i].join_student_num == 0)
		{
			flag = false;
			root["unlucky_department"].append(dep[i].department_no);
		}
	}
	if (flag)
	{
		root["unlucky_department"].resize(0);
	}

	//json��ʽд���ļ�
	StyledWriter sw;
	ofstream os;
	os.open("output_data.txt");
	//os << styled_writer.write(root);
	os << root;
	os.close();
}
*/

int main()
{
	Student stu[300];
	Department dep[20];
	Read_json_txt(stu, dep);

	SD_Points(stu, dep);
	
	matching(stu, dep);

	write_json_txt(stu, dep);
	
	system("pause");
	return 0;
}
