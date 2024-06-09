#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

int N, M, K;

int Map[21][21];
int Lab[21][21];
int Visit[21][21];

int dx[4] = { 1, -1, 0, 0 };
int dy[4] = { 0, 0, 1, -1 };

struct Block {
    int x;
    int y;
};

vector<Block> Team[6];
int Person[6];
int Point[6];

void Input() {
    cin >> N >> M >> K;
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            cin >> Map[i][j];
}

//팀 구역 탐색 : 머리 -> 꼬리 방향
void Label(int x, int y, int L) {
    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx < 1 || ny < 1 || nx > N || ny > N) continue;
        if (Visit[nx][ny] == 1 || Map[nx][ny] == 0) continue;

        //가장 처음 탐색할 때 무조건 2가 있는 방향으로 DFS 진행한다.
        //Vector.size() 를 활용
        //자연스럽게 머리 -> 꼬리 방향으로 Vector에 삽입된다.
        if(Team[L].size() == 1 && Map[nx][ny] != 2) continue;

        //꼬리가 나오는 곳까지 탐색하면 현재까지의 Vector.size()가 인구
        if(Map[nx][ny] == 3) Person[L] = Team[L].size() + 1;

        Visit[nx][ny] = 1;
        Team[L].push_back({ nx, ny });
        Label(nx, ny, L);
    }
}

//팀 생성
void Make_Team() {
    int L = 1;
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
        {
            if (Map[i][j] == 1 && Visit[i][j] == 0)
            {
                Visit[i][j] = 1;
                Team[L].push_back({ i, j });
                Label(i, j, L);
                L++;
            }
        }

    for (int i = 1; i < L; i++)
    {
        for (int j = 0; j < Team[i].size(); j++)
        {
            Lab[Team[i][j].x][Team[i][j].y] = i;
        }
    }
    memset(Visit, 0, sizeof(Visit));
}

//팀 회전
void Rotation() {
    for (int n = 1; n <= M; n++)
    {
        int sz = Team[n].size();
        int tx = Team[n][sz - 1].x;
        int ty = Team[n][sz - 1].y;

        for (int i = sz - 1; i >= 1; i--)
        {
            Team[n][i].x = Team[n][i - 1].x;
            Team[n][i].y = Team[n][i - 1].y;
        }
        Team[n][0].x = tx;
        Team[n][0].y = ty;

        //Map 동기화
        for (int i = 0; i < Team[n].size(); i++)
        {
            if(i == 0) 
                Map[Team[n][i].x][Team[n][i].y] = 1;
            else if(i > 0 && i < Person[n] - 1) 
                Map[Team[n][i].x][Team[n][i].y] = 2;
            else if(i == Person[n] - 1) 
                Map[Team[n][i].x][Team[n][i].y] = 3;
            else 
                Map[Team[n][i].x][Team[n][i].y] = 4;
        }

    }
}


void State(int L, int x, int y) {

    for (int i = 0; i < Team[L].size(); i++)
    {
        if (Team[L][i].x == x && Team[L][i].y == y)
        {
            Point[L] += (i + 1) * (i + 1);
        }
    }

    reverse(Team[L].begin(), Team[L].begin() + Person[L]);
    reverse(Team[L].begin() + Person[L], Team[L].end());
    //좌표 역순 
    for(int i = 0; i < Team[L].size(); i++)
    {
        if(i == 0) 
            Map[Team[L][i].x][Team[L][i].y] = 1;
        else if(i > 0 && i < Person[L] - 1) 
            Map[Team[L][i].x][Team[L][i].y] = 2;
        else if(i == Person[L] - 1) 
            Map[Team[L][i].x][Team[L][i].y] = 3;
        else 
            Map[Team[L][i].x][Team[L][i].y] = 4;
    }
}

void Ball(int Round) {

    int R = Round % (4 * N);
    if (R >= 1 && R <= N)
    {
        for (int i = 1; i <= N; i++)
        {
            if (Map[R][i] == 0 || Map[R][i] == 4) continue;
            State(Lab[R][i], R, i);
            break;
        }
    }
    
    if (R > N && R <= 2 * N)
    {
        for (int i = N; i >= 1; i--)
        {
            if (Map[i][R - N] == 0 || Map[i][R - N] == 4) continue;
            
            State(Lab[i][R - N], i, R - N);
            break;
        }
    }
    
    if (R > 2 * N && R <= 3 * N)
    {
        for (int i = N; i >= 1; i--)
        {
            if (Map[3 * N - R + 1][i] == 0 || Map[3 * N - R + 1][i] == 4) continue;
            State(Lab[3 * N - R + 1][i], 3 * N - R + 1, i);
            break;
        }
    }
    
    if(R > 3 * N)
    {
        for (int i = 1; i <= N; i++)
        {
            if (Map[i][4 * N - R + 1] == 0 || Map[i][4 * N - R + 1] == 4) continue;
            State(Lab[i][4 * N - R + 1], i, 4 * N - R + 1);
            break;
        }
    }
    
    if(R == 0)
    {
        for (int i = 1; i <= N; i++)
        {
            if (Map[i][1] == 0 || Map[i][1] == 4) continue;
            State(Lab[i][1], i, 1);
            break;
        }
    }
}

int Make_Ans() {
    int sum = 0;
    for (int i = 1; i <= M; i++)
    {
        sum += Point[i];
    }
    return sum;
}

int main() {
    Input();
    Make_Team();

    int Time = 1;
    while (Time <= K)
    {
        Rotation();
        Ball(Time);
        Time++;
    }
    cout << Make_Ans() << endl;
}