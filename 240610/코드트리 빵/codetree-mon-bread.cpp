#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

int N, M;
int Map[16][16];
int Visit[16][16];
int dist[16][16];

int PX;
int PY;

int dx[4] = {-1, 0, 0, 1};
int dy[4] = {0, -1, 1, 0};

//각 베이스 캠프 좌표
struct BaseCamp {
    int x;
    int y;
};
vector<BaseCamp> Vector_Camp;

//각 사람이 가고싶어하는 편의점의 좌표와 거리
struct Store {
    int x;
    int y;
    int full;
};
vector<Store> Vector_Store;

//필드에 들어온 사람들의 좌표
struct Person {
    int x;
    int y;
};
vector<Person> Vector_Person;

void Input() {
    cin >> N >> M;
    for(int i = 1; i <= N; i++)
    {
        for(int j = 1; j <= N; j++)
        {
            cin >> Map[i][j];
            if(Map[i][j] == 1)
            {
                Vector_Camp.push_back({i, j});
            }
        }
    }
    for(int i = 1; i <= M; i++)
    {
        int x, y;
        cin >> x >> y;
        Vector_Store.push_back({x, y, false});
    }
}

void Bfs(int x, int y) {

    queue<pair<pair<int, int>, int>> Q;
    Q.push({{x, y}, 0});
    dist[x][y] = 0;

    while(!Q.empty())
    {
        int px = Q.front().first.first;
        int py = Q.front().first.second;
        int time = Q.front().second;
        Q.pop();

        for(int i = 0; i < 4; i++)
        {
            int nx = px + dx[i];
            int ny = py + dy[i];

            if(nx < 1 || ny < 1 || nx > N || ny > N) continue;
            if(Visit[nx][ny] == 1) continue;
            if(dist[nx][ny] != 9999) continue;
;
            dist[nx][ny] = time + 1;
            Q.push({{nx, ny}, time + 1});
        }

    }
    
}

//각 사람의 이동
void Move(int n) {

    int gx = Vector_Store[n].x;
    int gy = Vector_Store[n].y;
    Bfs(gx, gy);

    int px = Vector_Person[n].x;
    int py = Vector_Person[n].y;
    int Min = 9999;

    for(int i = 0; i < 4; i++)
    {
        int nx = px + dx[i];
        int ny = py + dy[i];

        if(nx < 1 || ny < 1 || nx > N || ny > N) continue;
        if(Visit[nx][ny] == 1) continue;
        
        if(dist[nx][ny] < Min)
        {
            Min = dist[nx][ny];
            Vector_Person[n].x = nx;
            Vector_Person[n].y = ny;
        }
    }
}

//모든 편의점이 꽉 차면 true
bool Check_Full() {
    for(auto E : Vector_Store) 
    {
        if(E.full == false) return false;
    }
    return true;
}

//dist 초기화
void Clear_Dist() {
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            dist[i][j] = 9999;
        }
    }
}

int main() {

    int Time = 0;
    Input();

    while(true)
    {

        //1단계 : 필드 위의 사람이 편의점을 향해서 1칸 움직인다.
        for(int i = 0; i < Vector_Person.size(); i++)
        {
            if(Vector_Store[i].full == true) continue;
            Clear_Dist();
            Move(i);
        }

        //2단계 : 각 사람이 편의점에 도착했는지 체크
        for(int i = 0; i < Vector_Person.size(); i++)
        {
            //이미 도착한 편의점은 제외
            if(Vector_Store[i].full == true) continue;

            if(Vector_Person[i].x == Vector_Store[i].x && Vector_Person[i].y == Vector_Store[i].y)
            {
                Vector_Store[i].full = true;
                Visit[Vector_Store[i].x][Vector_Store[i].y] = 1;
            }
        }

        
        if(Check_Full() == true)
        {
            break;
        }


        //3단계
        if(Time < M)
        {
            PX = Vector_Store[Time].x;
            PY = Vector_Store[Time].y;
            
            Clear_Dist();
            Bfs(PX, PY);
            
            int Min = 9999;
            int X;
            int Y;

            for(int i = N; i >= 1; i--)
            {
                for(int j = N; j >= 1; j--)
                {
                    if(Map[i][j] == 1 && dist[i][j] <= Min)
                    {
                        Min = dist[i][j];
                        X = i;
                        Y = j;
                    }
                }
            }

            Vector_Person.push_back({X, Y});
            Visit[X][Y] = 1;
        }



        Time++;
    }

    cout << Time + 1 << '\n';
    return 0;
}