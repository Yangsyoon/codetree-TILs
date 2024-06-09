#include <iostream>
#include <vector>
#include <algorithm>

#define MAX 20

using namespace std;

struct info{
    int cnt,x,y;
};

int dx[8] = {0,-1,-1,-1,0,1,1,1};
int dy[8] = {-1,-1,0,1,1,1,0,-1};

int N,M,K,C;
int map[MAX][MAX]={0,};
int herbicide[MAX][MAX]={0,};
int res =0;

bool comp( info a, info b)
{
    if(a.cnt > b.cnt )  return true;
    else if(a.cnt < b.cnt )  return false;

    if(a.x < b.x )  return true;
    else if(a.x > b.x )  return false;

    if(a.y < b.y )  return true;
    return false;
}

void growthTree()
{
    int addTree[MAX][MAX]={0,};
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(map[i][j] <= 0)  continue;
            int cnt =0;
            for(int k=0;k<8;k+=2){
                int nx = i + dx[k];
                int ny = j + dy[k];

                if( nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
                if( map[nx][ny] <= 0)   continue;
                cnt++;
            }
            addTree[i][j]=cnt;
        }
    }

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            map[i][j] += addTree[i][j];
        }
    }
}

void breedTree()
{
    int addTree[MAX][MAX]={0,};

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(map[i][j] <= 0)  continue;
            int cnt =0;
            for(int k=0;k<8;k+=2){
                int nx = i + dx[k];
                int ny = j + dy[k];

                if( nx < 0 || ny < 0 || nx >= N || ny >= N || map[nx][ny] != 0 || herbicide[nx][ny] != 0) continue;
                
                cnt++;
            }
            for(int k=0;k<8;k+=2){
                int nx = i + dx[k];
                int ny = j + dy[k];

                if( nx < 0 || ny < 0 || nx >= N || ny >= N || map[nx][ny] != 0 || herbicide[nx][ny] != 0) continue;
                addTree[nx][ny]+=map[i][j]/cnt;
            }
        }
    }

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            map[i][j] += addTree[i][j];
        }
    }
}

void dieTree()
{
    vector<info> die;

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if( map[i][j] <= 0)  continue;
            int cnt = map[i][j];
            for(int d=1;d<8;d+=2){
                for(int l=1;l<=K;l++){
                    int nx = i + dx[d]*l;
                    int ny = j + dy[d]*l;
                    if( nx < 0 || ny < 0 || nx >= N || ny >= N || map[nx][ny] <= 0) break;
                    cnt += map[nx][ny];
                }
            }
            if(cnt > 0)   die.push_back({cnt,i,j});
        }
    }
    

    
    if( die.size() == 0)    return ;
    sort(die.begin(), die.end(),comp);
    res += die[0].cnt;
    int x =die[0].x;
    int y =die[0].y;
    
    if( map[x][y] > 0){
        map[x][y] = 0;
        herbicide[x][y] = C;

        for(int d=1;d<8;d+=2){
            for(int l=1;l<=K;l++){
                int nx = x + dx[d]*l;
                int ny = y + dy[d]*l;
                if( nx < 0 || ny < 0 || nx >= N || ny >= N || map[nx][ny] <0 ) break;
                
                herbicide[nx][ny] = C;
                
                if( map[nx][ny] == 0 ){
                    break;
                } else {
                    map[nx][ny] = 0;
                }
            }
        }
    }
    
}

void deleteHerb()
{
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(herbicide[i][j] == 0)    continue;
            herbicide[i][j]-=1;
        }
    }
}

int main(void)
{
    cin >> N >> M >> K >> C;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++)    cin >> map[i][j];
    }

    for(int i=0;i<M;i++){
        growthTree();
        breedTree();
        deleteHerb();
        dieTree();
    }
    cout << res;
    return 0;
}