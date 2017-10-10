// ConsoleApplication7.cpp : 定义控制台应用程序的入口点。
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

//部门结构体
struct Department
{
	int department_no;       //部门编号（唯一确定值），字符
	int member_limit;          //各部门需要学生数的要求的上限，单个，数值，在[10,15]内；
	int tags_num;			   //部门的特点标签个数 
	string tags[10];             //各部门的特点标签，多个（两个以上），字符；
	int event_schedules_num;     //各部门的常规活动时间段个数 
	long event_schedules[2][50];  //各部门的常规活动时间段，多个（两个以上），字符。
	int join_student_num=0;         //各部门招收的学生数量
	string join_student_no[15];     //各部门招收的学生
	double join_student_point[15];      //各部门招收的学生评分
};

//学生结构体
struct Student
{
	string student_no;        //学生编号（唯一确定值），字符；
	int free_time_num;		 //学生空闲时间段个数 
	long free_time[2][50];    //学生空闲时间段，多个（两个以上），字符；
	bool free_time_used[20][50];  //标记时间段是否被使用
	int tags_num;			 //兴趣标签个数
	string tags[30];          //兴趣标签，多个（两个以上），字符（学生的兴趣标签一定是所有部门特点标签其中的一个）
	int applications_department_num;    //部门意愿个数 
	int applications_department[5];     //每个学生有不多于5个的部门意愿
	int join_Department_num=0;            //当前加入的部门个数
	double point[20] = {0};    //部门对学生的打分
};

//把字符串化为整形
int string_to_int(string str)
{
	stringstream ss;
	int i_number;
	ss << str ;
	ss >> i_number;
	return i_number;
}

// 读取json文件，把数据存入结构体数组
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
			// 获取学生信息
			stuSum = root["students"].size();
			for (int i = 0; i < stuSum; i++)
			{
				// 获取学生空闲时段
				//解析json中的时间格式化为分钟（以星期一00：00为0分钟算起）
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

					//解析前半部分的时间
					string hhour_str = str.substr(dot_loc + 1, front_colon_loc - dot_loc - 1);
					int hhour_int = string_to_int(hhour_str);
					string hminute_str = str.substr(front_colon_loc + 1, wave_loc - front_colon_loc - 1);
					int hminute_int = string_to_int(hminute_str);

					//解析后半部分的时间
					string ehour_str = str.substr(wave_loc + 1, behind_colon_loc - wave_loc - 1);
					int ehour_int = string_to_int(ehour_str);
					string eminute_str = str.substr(behind_colon_loc + 1, str.size() - behind_colon_loc - 1);
					int eminute_int = string_to_int(eminute_str);

					stu[i].free_time[0][j] = Int_Week * 3600 + hhour_int * 60 + hminute_int;
					stu[i].free_time[1][j] = Int_Week * 3600 + ehour_int * 60 + eminute_int;
				}

				// 获取学生学号
				stu[i].student_no =  root["students"][i]["student_no"].asString();

				// 获取学生部门意愿
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

				// 获取学生兴趣标签
				stu[i].tags_num = root["students"][i]["tags"].size();
				for (int j = 0; j < stu[i].tags_num; j++) 
				{
					stu[i].tags[j] = root["students"][i]["tags"][j].asString();
				}
			}

			//输出部门信息
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

					//解析前半部分的时间
					string hhour_str = str.substr(dot_loc + 1, front_colon_loc - dot_loc - 1);
					int hhour_int = string_to_int(hhour_str);
					string hminute_str = str.substr(front_colon_loc + 1, wave_loc - front_colon_loc - 1);
					int hminute_int = string_to_int(hminute_str);

					//解析后半部分的时间
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

// 计算学生在每个部门的得分
void SD_Points(Student *stu, Department *dept)
{
	for (int i = 0; i < stuSum; i++)
	{
		for (int j = 0; j < depSum; j++)
		{
			//部门志愿按顺序分数为50，30，10，7，3
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

			//兴趣标签，一个标签相符得10分,
			for (int k = 0; k < stu[i].tags_num; k++)
			{
				for (int r = 0; r < dept[j].tags_num; r++)
				{
					if (stu[i].tags[k] == dept[j].tags[r])
						stu[i].point[j] += 10;
				}
			}

			//时间重叠，用1小时来考虑，重叠1小时40分，2小时50分，3小时以上55分，如果一次都没有分数置为0
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


// 匹配算法
void matching(Student *stu, Department *dept)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < stuSum; j++)
		{
			if (i < stu[j].applications_department_num)
			{
				int temp = stu[j].applications_department[i];

				if (dept[temp].member_limit > dept[temp].join_student_num)  //如果部门还有人员名额，则学生直接中选
				{
					dept[temp].join_student_no[dept[temp].join_student_num] = stu[j].student_no;
					dept[temp].join_student_point[dept[temp].join_student_num] = stu[j].point[temp];
					dept[temp].join_student_num ++;
					stu[j].join_Department_num++;
				}
				else //如果没有名额的话，与部门中的最低评分的人比较,决定是否替换
				{
					int min_point_index = 0;
					for (int k = 1; k < dept[temp].member_limit; k++)
					{
						if (dept[temp].join_student_point[k] < dept[temp].join_student_point[min_point_index])
						{
							min_point_index = k;
						}
					}

					//部门对于学生的评分高于这个部门中中选的学生的最低评分，替换到这个最低评分的学生
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
	//root为根节点
	Json::Value root;

	//子节点
	Json::Value admitted;

	//未被招收的学生
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

	//部门招收情况
	flag = true;
	for (int i = 0; i < depSum; i++)
	{
		//由于jsoncpp数组为空会输出null，因此将其数组大小置0，则可以输出指定的"[]"格式，而非"[null]"
		if (dep[i].join_student_num != 0)
		{
			flag = false;
			admitted.clear();
			admitted["department_no"] = Json::Value(dep[i].department_no);

			for (int j = 0; j < dep[i].join_student_num; j++)
			{
				admitted["member"].append(dep[i].join_student_no[j]);
			}
			//子节点挂到根节点
			root["admitted"].append(admitted);
		}
	}
	if (flag)
	{
		root["admitted"].resize(0);
	}

	//未招到人的部门
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

	//json格式写入文件
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
