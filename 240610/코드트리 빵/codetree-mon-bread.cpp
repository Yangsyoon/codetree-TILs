#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstring>
#define INF 987654321
using namespace std;

typedef struct _point{
    int x;
    int y;
}point;

int mat[21][21];
point man[31];
point conv[31];
vector <point> basecamp;
int dx[4] = {-1,0,1,0};
int dy[4] = {0,-1,0,1};
int N,m,t = 0;
int a,b,fin = 0;

bool vis[21][21];
queue<pair<point,int>> q;

int findRoot(point dest, int x, int y){
    q = queue<pair<point,int>>();    
    memset(vis,0,sizeof(vis));
    if(x == dest.x && y == dest.y)
        return 0;
    point newPoint;
    newPoint.x = x; newPoint.y =y ;
    vis[newPoint.x][newPoint.y] = true;
    q.push(make_pair(newPoint,0));

    while(!q.empty()){
        point cur = q.front().first;
        int dist = q.front().second; q.pop();
        for(int k = 0 ; k < 4 ; k++){
            if(mat[cur.x + dx[k]][cur.y + dy[k]] != 2 && !vis[cur.x + dx[k]][cur.y + dy[k]]){
                if(cur.x + dx[k] == dest.x && cur.y + dy[k] == dest.y)
                    return dist + 1;
                vis[cur.x + dx[k]][cur.y + dy[k]] = true;
                newPoint.x = cur.x + dx[k];
                newPoint.y = cur.y + dy[k];
                q.push(make_pair(newPoint,dist+1));
            }
        }
    }
    return INF;
}

void moveOneWay(){
    for(int i = 1; i < min(t,m + 1); i++){
        point cur = man[i];
        int bestdist = INF;
        int bestdir = -1;
        if(cur.x != -1){
            for(int k = 0 ; k < 4 ;k++){
                int dist;
                if(mat[cur.x + dx[k]][cur.y + dy[k]] != 2){
                    dist = findRoot(conv[i],cur.x + dx[k],cur.y + dy[k]);
                    if(dist < bestdist){
                        bestdist = dist;
                        bestdir = k;
                    }
                }
            }
            man[i].x += dx[bestdir];
            man[i].y += dy[bestdir];
            if(man[i].x == conv[i].x && man[i].y == conv[i].y){
                mat[man[i].x][man[i].y] = 2;
                man[i].x = -1;
                man[i].y = -1;
                fin++;
            }
        }
    }
}

void setNewMan(int tm){
    point cur = conv[tm];
    int dist = INF;
    int idx = 0;
    for(int i = 0 ; i < basecamp.size(); i++){
        int bcdist = findRoot(basecamp[i],cur.x,cur.y);
        if(bcdist < dist){
            dist = bcdist;
            idx = i;
        }  
    }
    man[tm].x = basecamp[idx].x;
    man[tm].y = basecamp[idx].y;
    mat[basecamp[idx].x][basecamp[idx].y] = 2;
    basecamp.erase(basecamp.begin() + idx);
}

void simul(){
    while(fin < m){
        t++;
        if(t > 1)
            moveOneWay();     
        if(t <= m){
            setNewMan(t);
        }
    }
}

int main (){
    ios::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    cin >> N >> m;
    for(int i = 0 ; i <= N + 1 ;i++){
        mat[i][0] = 2;
        mat[0][i] = 2;
        mat[N+1][i] = 2;
        mat[i][N+1] = 2;
    }
    for(int i = 1 ; i <= N ;i++){
        for(int j = 1 ; j <= N ;j++){
            cin >> mat[i][j];
            if(mat[i][j] == 1){           
                point newPoint; 
                newPoint.x = i ; newPoint.y = j;
                basecamp.push_back(newPoint);
            }
        }
    }
    for(int i = 1 ; i <= m ;i++){
        cin >> a >> b;
        conv[i].x = a;
        conv[i].y = b;
    }
    simul();
    cout << t;
    return 0;
}