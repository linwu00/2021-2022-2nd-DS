#include <fstream>
#include <iostream>
using namespace std;

int n, m;
const int MAXN = 1000;
int fa[MAXN], fb[MAXN];

int Find(int x)     // 寻找等价类
{
    int r = x;
    while (fa[r] != r)
    {
        r = fa[r];
    }

    int i = x, j;
    while (i != r)
    {
        j = fa[i];
        fa[i] = r;
        i = j;
    }
    
    return r;
}

void Union(char dgFlag, int a, int b)       // 合并等价类
{
    if (dgFlag == 'S')
        fa[Find(a)] = Find(b);
    else 
    {
        if(fb[a] != a)
            fa[Find(fb[a])] = Find(b);
        
        if(fb[b] != b)
            fa[Find(fb[b])] = Find(a);
        
        fb[b] = a;
        fb[a] = b;
    }
}

int main() 
{
	ifstream in("virus.in");    // 从文件读入数据
    
    in >> n >> m;
    for (int i = 1; i <= n ; i++)       // 初始化
    { 
        fa[i] = i;
        fb[i] = i;
    }
    for (; m > 0; m--)
    {
        char dgFlag;
        in >> dgFlag;
        int a, b;
        in >> a >> b;
        Union(dgFlag, a, b);
    }

    int cnt = 0;
    for(int i = 1; i <= n; i++)
        if(fa[i] == i) 
            cnt++;
    
    ofstream out("virus.out");
    out << cnt << endl;
    out.close();
    in.close();
    
    cout << "分类完成." << endl;
    system("PAUSE");

	  return 0;
}
