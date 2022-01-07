#include <cstring>
#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#define MAX 5
using namespace std;

int m, s;
struct Fish
{
    int r, c, dir;
};
vector<Fish> fishMap[MAX][MAX], backupMap[MAX][MAX];
int checked[MAX][MAX];
int shark_r, shark_c;
int fdr[9] = {0, 0, -1, -1, -1, 0, 1, 1, 1 };
int fdc[9] = {0, -1, -1, 0, 1, 1, 1, 0, -1};
int sdr[5] = {0, -1, 0, 1, 0 };
int sdc[5] = {0, 0, -1, 0, 1 };
int maxEating;
int tmpRoute[3], route[3];

void copyMap(vector<Fish> A[][MAX], vector<Fish> B[][MAX])
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            A[i][j] = B[i][j];
        }
    }
}
int changeDir(int dir) {
    switch (dir) {
    case 1:
        return 8;
    case 2:
        return 1;
    case 3:
        return 2;
    case 4:
        return 3;
    case 5:
        return 4;
    case 6:
        return 5;
    case 7:
        return 6;
    case 8:
        return 7;
    }
}

//물고기 이동
void moveFish()
{
    //물고기가 이동하면 임시맵에 담아준다.
    vector<Fish>tmpMap[MAX][MAX];
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < fishMap[i][j].size(); ++k)
            {
                int r = fishMap[i][j][k].r;
                int c = fishMap[i][j][k].c;
                int d = fishMap[i][j][k].dir;
                int nr = r;
                int nc = c;
                bool flag = false;
                for (int e = 0; e < 8; ++e)
                {
                    nr = r + fdr[d];
                    nc = c + fdc[d];
                    //범위 벗어나면 방향을 바꾼다.
                    if (nr >= 0 && nc >= 0 && nr < 4 && nc < 4) {
                        if ((nr != shark_r || nc != shark_c)
                            && checked[nr][nc] == 0) {
                            flag = true;
                            break;
                        }
                    }
                    d = changeDir(d);
                }
                if (flag)
                {
                    Fish f = { nr, nc, d };
                    tmpMap[nr][nc].push_back(f);
                }
                else
                {
                    Fish f = { r, c, d };
                    tmpMap[r][c].push_back(f);
                }
            }
        }
    }

    copyMap(fishMap, tmpMap);

}
int routeSimulation()
{
    bool visit[MAX][MAX] = { false, };
    int r = shark_r;
    int c = shark_c;
    int eat = 0;
    for (int i = 0; i < 3; ++i)
    {
        int dir = tmpRoute[i];
        int nr = r + sdr[dir];
        int nc = c + sdc[dir];
        if (nr < 0 || nr >= 4 || nc < 0 || nc >= 4)
            return -1;
        if (visit[nr][nc] == false)
        {
            visit[nr][nc] = true;
            eat += fishMap[nr][nc].size();
        }
        r = nr;
        c = nc;
    }
    return eat;
}
void findRoute(int cnt)
{
    if (cnt == 3)
    {
        int eatNumber = routeSimulation();
        if (eatNumber > maxEating)
        {
            for (int i = 0; i < 3; ++i)
            {
                route[i] = tmpRoute[i];
            }
            maxEating = eatNumber;
        }
        return;
    }
    for (int i = 1; i <= 4; ++i)
    {
        tmpRoute[cnt] = i;
        findRoute(cnt + 1);
    }
}
// 상어 이동
void moveShark(int time)
{
    maxEating = -1;
    findRoute(0);
    vector<Fish> tmpMap[MAX][MAX];
    copyMap(tmpMap, fishMap);

    int r = shark_r;
    int c = shark_c;
    for (int i = 0; i < 3; ++i)
    {
        int dir = route[i];
        int nr = r + sdr[dir];
        int nc = c + sdc[dir];
        if (tmpMap[nr][nc].size() != 0)
        {
            checked[nr][nc] = time;
            tmpMap[nr][nc].clear();
        }
        r = nr;
        c = nc;
        shark_r = r;
        shark_c = c;
    }
    copyMap(fishMap, tmpMap);
}


void removeSmell(int time)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (checked[i][j] == 0)
                continue;
            if (time - checked[i][j] == 2)
                checked[i][j] = 0;
        }
    }
}
void bornFish()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < backupMap[i][j].size(); ++k)
                fishMap[i][j].push_back(backupMap[i][j][k]);
        }
    }
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    //Fish fish;
    cin >> m >> s;
    for (int i = 0; i < m; ++i)
    {
        int a, b, c;
        cin >> a >> b >> c;
        a--, b--;
        fishMap[a][b].push_back({ a, b, c });
    }
    cin >> shark_r >> shark_c;
    shark_r--, shark_c--;


    for(int i =1;i<=s;++i)
    {
        //물고기 복제하기
        copyMap(backupMap, fishMap);
        //물고기 이동
        moveFish();
        //상어 이동
        moveShark(i);
        //냄새없애기
        removeSmell(i);
        //물고기 복제
        bornFish();
    }
    int answer = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            answer += fishMap[i][j].size();
        }
    }
    cout << answer << "\n";
    return 0;
}