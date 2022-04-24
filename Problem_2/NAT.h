#ifndef __N_A_T__
#define __N_A_T__

#include <string>
#include <bits/stdc++.h>
using namespace std;

// 定义检测人员状态的枚举类型
#ifndef __STATUS_TYPE__
#define __STATUS_TYPE__
enum StatusType {NOTYPE, NEGATIVE, CONFIRMED, SUSPICIOUS, CONTIGUITY, SUBCONTIGUITY, WAITUPLOAD, QUEUING, NOTTEST};
#endif

class NAT
{
public:
    NAT(string Id, bool SingleOrMix);   // 构造函数   
    ~NAT() { }      // 析构函数
    
    string GetSelfId();      // 获取人员代码
    string GetTestingTube();      // 获取试管号
    void WaitTesting(string testTubeId){ testTubeId = testTubeId; }     // 设置等待被检测人员检测试管
    int GetStatus();         // 获取被检测人员当前人员状态
    void TestingResult(int status) { currentStatus = status; }     // 设置检测人员状态
    bool GetSingle();        // 获取是否单人管

private:
    string personSelfId;    // 人员代码
    string testTubeId;      // 测试管编号
    int currentStatus;      // 当前状态
    bool singleOrMix;
};

NAT::NAT(string Id, bool SingleOrMix)
{
    personSelfId = Id;
    singleOrMix = SingleOrMix;
    currentStatus = 8;
}

string NAT::GetTestingTube() { return testTubeId; }
string NAT::GetSelfId() { return personSelfId; }
bool NAT::GetSingle() { return singleOrMix; }
int NAT::GetStatus() { return currentStatus; }

/*****各关键变量定义*****/

queue<int> testingQueue;        // 排队
vector<NAT> testingPerson;      // 记录所有被检测人员的信息
vector<int> preTenPos(10,-1);   // 记录排在某人前面的十人的位置.
vector<vector<int>> postion(9); // 记录各类状态人员的位置.
int singleTestTube = 10000;     // 单人管试管编号
int mixTestTube = 100000;       // 混合管试管编号


/*****功能函数实现*****/

string GetId(int currentPos) { return testingPerson[currentPos].GetSelfId(); }
int GetStatus(int currentPos) { return testingPerson[currentPos].GetStatus(); }

void MixTestTubeInfo(int currentPos)        // 混合管信息登记
{
    
    if (!(rand() % 100))
    {
        for (int i = 0; i < 10;i++)
        {
            testingPerson[currentPos+i].TestingResult(CONFIRMED);
            postion[CONFIRMED].push_back(currentPos+i);
        }
    }
    else if (!(rand() % 10))
    {
        for(int i = 0; i < 10; i++)
        {
            testingPerson[currentPos+i].TestingResult(SUSPICIOUS);
            postion[SUSPICIOUS].push_back(currentPos+i);
        }
        
    }
    else
    {
        for (int i = 0; i < 10; i++)
        {
            testingPerson[currentPos+i].TestingResult(NEGATIVE);
            postion[NEGATIVE].push_back(currentPos+i);
        }
    }
}

void MixTesting(int currentPos)         // 混合检测
{
    string midVar = to_string(mixTestTube);
    midVar[0] = '0';
    testingPerson[currentPos].WaitTesting(midVar.substr(0,4));
    mixTestTube++;
}

void SingleTestTubeInfo(int currentPos)     // 登记单人管信息
{
    if(!(rand() % 100))         // 检测为阳性
    {
        testingPerson[currentPos].TestingResult(CONFIRMED);
        postion[CONFIRMED].push_back(currentPos);
        
        int i = 0;
        while(i < 10 && preTenPos[i] != -1)       // 更改排在其前面十人为密接者
        {
            testingPerson[preTenPos[i]].TestingResult(CONTIGUITY);
            postion[CONTIGUITY].push_back(preTenPos[i]);

            string record = testingPerson[preTenPos[i]].GetSelfId();    // 更新次密接者
            record = record.substr(0,2);
            for(auto pers:testingPerson)
            {
                string cur = pers.GetSelfId();
                cur = cur.substr(0,2);
                int sta = pers.GetStatus();
                if(record == cur && sta != CONFIRMED && sta != CONTIGUITY)
                {
                    pers.TestingResult(SUBCONTIGUITY);
                    postion[SUBCONTIGUITY].push_back(preTenPos[i]);
                }
            }
        }

        testingQueue.push(-1);
        int midVar=testingQueue.front();
        while(midVar!=-1)
        {
            testingPerson[midVar].TestingResult(CONTIGUITY);
            postion[CONTIGUITY].push_back(midVar);
            string record = testingPerson[midVar].GetSelfId();
            record = record.substr(0,2);
            for(auto pers:testingPerson)
            {
                string cur = pers.GetSelfId();
                cur = cur.substr(0,2);
                int sta = pers.GetStatus();
                if(record == cur && sta!=CONFIRMED && sta != CONTIGUITY)
                {
                    pers.TestingResult(SUBCONTIGUITY);
                    postion[SUBCONTIGUITY].push_back(midVar);
                }
            }
            testingQueue.pop();
            testingQueue.push(midVar);
            midVar=testingQueue.front();
        }
        testingQueue.pop();
    }

    else if(!(rand() % 10))       // 检测为可疑
    {
        testingPerson[currentPos].TestingResult(SUSPICIOUS);      //更新该人员信息
        postion[SUSPICIOUS].push_back(currentPos);  //更新postion数组

    }
    
    else // 检测结果为阴
    {
        testingPerson[currentPos].TestingResult(NEGATIVE);    // 更新该人员信息
        postion[NEGATIVE].push_back(currentPos);    //更新postion数组
    }
}

void SingleTesting(int currentPos)      // 单人检测
{
    testingPerson[currentPos].WaitTesting(to_string(singleTestTube));
    singleTestTube++;
}

void TestingResult(int currentPos)
{
    cout << testingPerson[currentPos].GetSelfId() << ": " 
        << testingPerson[currentPos].GetStatus() << endl;
}

void QueueInfo()        // 查看排队情况
{
    testingQueue.push(-1);
    int midVar = testingQueue.front();
    int cnt = 1;
    while(midVar != -1)
    {
        cout << "人员" << cnt++ << "：" << testingPerson[midVar].GetSelfId() << endl;
        testingQueue.pop();
        testingQueue.push(midVar);
        midVar = testingQueue.front();
    }
    testingQueue.pop();
}

void EnQueue(string id,bool method)     // 被检测人员进行排队
{
    NAT midVar = NAT(id,method);
    testingPerson.push_back(midVar);
}

#endif
