#include <iostream>
#include <unordered_map>
#include <string.h>
#include<unordered_set>
using namespace std;
int Q, order, N, M, P;
const int MX = 2005;
int d[MX], score[MX]; //인덱스는 0~2000
int sz, total_score;
unordered_map<int, int>rmap;
typedef struct
{
    int total_jump;
    int yplusx;
    int y;
    int x;
    int id;
}rabbit;
rabbit rheap[MX];
rabbit rabbits[MX];
void rInitHeap()
{
    sz = 0;
}
bool rchange(rabbit a, rabbit b)
{
    //a가 우선순위가 낮은 경우 참을 반환해 교환하기
    if (a.total_jump == b.total_jump)
    {
        if (a.yplusx == b.yplusx)
        {
            if (a.y == b.y)
            {
                if (a.x == b.x)
                {
                    return a.id > b.id;
                }
                return a.x > b.x;
            }
            return a.y > b.y;
        }
        return a.yplusx > b.yplusx;
    }
    //cout <<"부모"<< a.id << ' ' <<"자식"<< b.id << "의 점프:" << a.total_jump << ' ' << b.total_jump << '\n';
    return a.total_jump > b.total_jump;
}
void rswap(int a, int b)
{
    rabbit emp = rheap[a];
    rheap[a] = rheap[b];
    rheap[b] = emp;
}
rabbit rheapTop()
{
    return rheap[1];
}
void rheapPush(rabbit r)
{
    rheap[++sz] = r;
    //cout << sz << "번째 heap에" << r.id << "넣었음\n";
    //cout << "top:" << rheapTop().id << '\n';
    int idx = sz;
    while (idx != 1)
    {
        int par = idx / 2;
        if (rchange(rheap[par], rheap[idx]))
        {
            //cout << rheap[par].id << "와" << rheap[idx].id << "교환\n";
            rswap(par, idx);
            idx = par;
        }
        else break;
    }
    //cout << idx << "번째 힙에" << rheap[idx].id << "가 들어감\n";
    auto rrr = rheapTop();
    //cout << "top :" << rrr.id << '\n';
}

void rheapPop()
{
    // cout << "popsz:" << sz << '\n';
    rheap[1] = rheap[sz--];
    int idx = 1;
    while (2 * idx <= sz)
    {
        int lc = 2 * idx, rc = 2 * idx + 1;
        int strong_child = lc;
        if (rc <= sz && rchange(rheap[lc], rheap[rc]))
        {
            strong_child = rc;
        }
        if (rchange(rheap[idx], rheap[strong_child]))
        {
            rswap(idx, strong_child);
            idx = strong_child;
            //cout << rheap[idx].id << "와" << rheap[strong_child].id << "교환\n";
        }
        else break;
    }
}
void ready()
{
    rInitHeap();
    rmap.clear();
    memset(d, P + 1, 0);
    memset(score, P + 1, 0);
    total_score = 0;
}
typedef struct
{
    int yplusx;
    int y;
    int x;
}point;
bool mov_cmp(point a, point b)
{
    if (a.yplusx == b.yplusx)
    {
        if (a.y == b.y)
        {
            return a.x < b.x;
        }
        return a.y < b.y;
    }
    return a.yplusx < b.yplusx;
}
bool strong_r_cmp(rabbit a, rabbit b)
{
    if (a.yplusx == b.yplusx)
    {
        if (a.y == b.y)
        {
            if (a.x == b.x)
            {
                return a.id < b.id;
            }
            return a.x < b.x;
        }
        return a.y < b.y;
    }
    return a.yplusx < b.yplusx;
}
void start(int K, int S)
{
    int z;
    rabbit strongest_r;
    unordered_set<int>rabbited;
    int srcnt = 0; // 초기화 위한 장치
    while (K--)
    {// 1 .우선순위 가장 높은 토끼 결정
        rabbit r = rheapTop();
        rheapPop();
        //cout << "점프:" << r.total_jump <<"합:" << r.yplusx << ' ' << r.y << ' ' << r.x << ' ' <<"id:"<< r.id << '\n';
        //cout << "rid:" << r.id << '\n';
        //cout << "남은 대장:" << rheapTop().id << '\n';
        int b = r.y;
        int a = r.x;
        int rid = rmap[r.id];
        int dis = d[rid];

        // 2. 상하좌우 이동
        int strong_dir = 0; // 0,1,2,3 중 하나
        point strong_p, p;
        // [0] 우 - strong_p 에다가 초기화
        strong_p.y = b;
        z = dis - (M - a);
        // 벽에 안 부딪히는 경우
        if (z <= 0)
        {
            strong_p.x = a + dis;
        }
        // 벽에 부딪히는 경우
        else
        {
            if ((z / (M - 1)) % 2 == 0)
            {
                strong_p.x = M - (z % (M - 1));
            }
            else
            {
                strong_p.x = 1 + (z % (M - 1));
            }
        }
        strong_p.yplusx = strong_p.y + strong_p.x;
        // [1] 좌 - 여기서부터는 계산해서 strong_p와 비교하여 초기화
        p.y = b;
        z = dis - (a - 1);
        if (z <= 0)
        {
            p.x = a - dis;
        }
        else
        {
            if ((z / (M - 1)) % 2 == 0)
            {
                p.x = 1 + (z % (M - 1));
            }
            else
            {
                p.x = M - (z % (M - 1));
            }
        }
        p.yplusx = p.y + p.x;
        //비교
        if (mov_cmp(strong_p, p))
            strong_p = p;
        // [2] 상
        p.x = a;
        z = dis - (b - 1);
        if (z <= 0)
        {
            p.y = b - dis;
        }
        else
        {
            if ((z / (N - 1)) % 2 == 0)
            {
                p.y = 1 + (z % (N - 1));
            }
            else
            {
                p.y = N - (z % (N - 1));
            }
        }
        p.yplusx = p.y + p.x;
        if (mov_cmp(strong_p, p))
            strong_p = p;
        // [3] 하
        z = dis - (N - b);
        if (z <= 0)
        {
            p.y = b + dis;
        }
        else
        {
            if ((z / (N - 1)) % 2 == 0)
            {
                p.y = N - (z % (N - 1));
            }
            else
            {
                p.y = 1 + (z % (N - 1));
            }
        }
        p.yplusx = p.y + p.x;
        if (mov_cmp(strong_p, p))
            strong_p = p;
        // strong_p 점 찾아냈음
        // 이동시키기
        rheapPush({ r.total_jump + 1,strong_p.yplusx,strong_p.y,strong_p.x,r.id });
        score[rid] -= strong_p.yplusx;
        //cout << "score_" << rid << ":" << score[rid] << '\n';
        total_score += strong_p.yplusx;
        //cout << "total_score:" << total_score << '\n';
        //cout << r.id << " 토끼가" << strong_p.y << ',' << strong_p.x << "에 위치하여" <<  strong_p.yplusx << "의 점수를 얻는다\n";
        //저장 (이동한 것만 저장하면 된다!)
        rabbits[rid] = { r.total_jump + 1,strong_p.yplusx,strong_p.y,strong_p.x,r.id };
        //id 기억
        rabbited.insert(rid);
    }
    //반복 끝나고 제일 강한 토끼찾기
    strongest_r = rabbits[0];
    for (auto rr : rabbited)
    {
        if (strong_r_cmp(strongest_r, rabbits[rr]))
        {
            strongest_r = rabbits[rr];
        }
    }
   //cout << "최강" << strongest_r.id << "가" << S << "점 얻는다\n";
    score[rmap[strongest_r.id]] += S;

}
void change_dis(int pid, int L)
{
    d[rmap[pid]] *= L;  
    //cout << pid << "의 이동거리는" << d[rmap[pid]] << '\n';
}
void pick()
{
    int maxi = score[1];
    //cout << "score[1]:" << maxi << '\n';
    for (int i = 2; i <= P; i++)
    {
        if (maxi < score[i]) maxi = score[i];
        //cout << "score_"<<i<<':' << score[i] << '\n';
    }
    //cout << "total_score:" << total_score << '\n';
    cout << total_score + maxi << '\n';
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> Q;
    int pid, K, S, L;
    while (Q--)
    {
        cin >> order;
        switch (order) {
        case 100:
            ready();
            cin >> N >> M >> P;
            for (int i = 1; i <= P; i++)
            {
                //거리는 따로 기록 index로
                cin >> pid >> d[i];
                rmap[pid] = i;
                rheapPush({ 0,2,1,1,pid });

                score[i] = 0;
            }
            break;
        case 200:
            cin >> K >> S;
            start(K, S);
            break;
        case 300:
            cin >> pid >> L;
            change_dis(pid, L);
            break;
        case 400:
            pick();
            break;
        }
    }
    return 0;
}