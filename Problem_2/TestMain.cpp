#include "NAT.h"
#include <fstream>

const int MAXNUM = 9;
const string TAG[9] = {"空缺","阴性", "确诊", "可疑", "密接", "次密接", "待上传结果", "在排队", "未检测"};

int main()
{
    char c = '@';
    string choice;
    int mixNum, singleNum;
    ifstream in("nucleic_acid_test.in");
    in >> mixNum >> singleNum;

    string mulSelfId[4005], singleSelfId[4005];
    string checkingPersonId;
    string Classification[MAXNUM][100];
    int statusNum[MAXNUM] = {0};
    
    cout << "正在启动核酸检测系统NAT..." << endl;
    system("PAUSE");
    cout << "即将进入检测人员排队状态..." << endl;
    cout << "是否进入排队[yes/no]: ";
    cin >> choice;
    if (choice == "yes")
    {
        for (int i = 0; i < mixNum; i++)
            in >> mulSelfId[i];
            
        for (int i = 0; i < singleNum; i++)
            in >> singleSelfId[i];
        
        for (int i = 0; i < mixNum; i++)
        {
            EnQueue(mulSelfId[i], 0);
            testingQueue.push(i);
        }
        for (int i = 0; i < singleNum; i++)
        {
            EnQueue(singleSelfId[i], 1);
            testingQueue.push(i);
        }
        cout << "已成功排队！" << endl;

        cout << "\n即将进入检测并登记测试管信息状态..." << endl;
        cout << "是否进入该状态[yes/no]: ";
        cin >> choice;
        if (choice == "yes")
        {
            for (int i = 0; i < mixNum / 10; i++)
            {
                for (int j = i*10; j < i*10+10; j++)
                    MixTesting(j);
                MixTestTubeInfo(i*10);
            }
            for (int i = mixNum; i < mixNum + singleNum; i++)
                SingleTesting(i);
            cout << "已成功检测并登记测试管信息！" << endl;
            cout << "混检人数为：" << mixNum << "，" << "单检人数为：" << singleNum << endl;

            cout << "\n即将进入功能查询模块..." << endl;
            cout << "是否进入该模块[yes/no]: ";
            cin >> choice;
            if (choice == "yes")
                while (c != '0')
                {
                    cout << endl << "A. 查看被检测人员排队情况.";
                    cout << endl << "B. 查询各类状态的人员情况.";
                    cout << endl << "C. 查询个人状态.";
                    cout << endl << "0. 退出";
                    cout << endl << "选择功能(A~C): ";
                    cin >> c;
                    switch (c)
                    {                    
                    case 'A':
                        cout << "以下为检测人员的排队情况：" << endl;
                        QueueInfo();
                        break;
                    
                    case 'B':
                        for (int i = 0; i < mixNum+singleNum; i++)
                        {
                            int tag0 = GetStatus(i);
                            statusNum[tag0]++;
                            Classification[tag0][statusNum[tag0]] = GetId(i);
                        }

                        for (int i = 1; i <= 9; i++)
                        {
                            if (statusNum[i] != 0)
                            {
                                int j = 1;
                                cout << TAG[i] << "人员为: ";
                                cout << Classification[i][j];
                                for (j = 2; j <= statusNum[i]; j++)
                                    cout << ", " << Classification[i][j];
                                cout << endl;
                            }
                        }
                        break;
                    
                    case 'C':
                        cout << "请输入所要查询的人员编号: ";
                        cin >> checkingPersonId;
                        int i;
                        for (i=0; i<mixNum+singleNum; i++)
                        {
                            if (checkingPersonId == GetId(i))
                            {
                                cout << "当前查询人员的状态为： " << TAG[GetStatus(i)] << endl;
                                break;
                            }
                        }
                        if (i == mixNum + singleNum)
                            cout << "查无此人." << endl;

                        break;
                    }
                }
            else
                cout << "退出系统..." << endl;
        }
        else
            cout << "退出系统..." << endl;   
    }
    else
    {
        cout << "退出系统..." << endl;
    }
    in.close();     // 关闭文件
        
	  system("PAUSE");

    return 0;
}
