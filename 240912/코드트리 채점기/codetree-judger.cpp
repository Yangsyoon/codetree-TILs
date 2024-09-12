#include <iostream>
#include <string.h>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <set>
using namespace std;
int Q,order,machinescnt,workingJudgercnt,unused_machine;
unordered_set<string>qurl; // 큐에 있는 url
unordered_set<string>in_judging; // 채점 중인 도메인
set<int>completed_machine; // 머신번호
string dom(string url)
{
    //cout << "dom 시작";
    int i = 0;
    char a[50];
    //cout << "url:" << url << '\n';
    // '/' 나오기 직전까지만 저장
    while (url[i] != '/')
    {
        a[i] = url[i];
        i++;
    }
    a[i] = '\0';
    //cout << " a:"  << a << '\n';
    //cout << "dom 끝";
    return a;
}
typedef struct
{
    int start;
    int gap;
}mark_unit;
unordered_map<string, mark_unit>history; // 도메인이 key
typedef struct
{
    int pri;
    string url;
    int insert_time;
}quest;
typedef struct
{
    int start;
    string url;
}judge_unit;
unordered_map<int, judge_unit>judging_target; // 기계 번호와 quest 대응
quest heap[303][3000];
int total_sz;
int sz[303]; // 각 도메인 별 힙 사이즈
unordered_map<string, int>dmap; //domain을 매핑
void heapInit(int i)
{
    sz[i] = 0;
}
void sswap(int i,int index_a, int index_b)
{
    //cout << "idxa , idxb:" << index_a << ' ' << index_b << '\n';
    quest emp = heap[i][index_a];
    heap[i][index_a] = heap[i][index_b];
    heap[i][index_b] = emp;
    //cout << "idxa , idxb :" << index_a << ' ' << index_b << '\n';
}
bool is_second_prior(quest a, quest b)
{
    if (a.pri > b.pri || ((a.pri == b.pri) && a.insert_time > b.insert_time))
        return true;
    else return false;
}
void heapPush(int i,quest q)
{
    heap[i][++sz[i]] = q;
    int idx = sz[i];
    //cout << "추가된 sz_" << i << ' ' << sz[i] << '\n';
    while (idx != 1)
    {
        //cout << "idx:" << idx << '\n';
        int par = idx / 2;
        //cout << "par:" << par << '\n';
        if (is_second_prior(heap[i][par],heap[i][idx]))
        {
            //cout << "idx, par:" << idx << ' ' << par << '\n';
            //교환이 필요한 경우
            sswap(i,par, idx);
            //cout << "idx , par:" << idx << ' ' << par << '\n';
            idx = par;
            //cout << "idx , par:" << idx << ' ' << par << '\n';
        }
        else break;
    }
    total_sz++;
    //cout << "total_sz 증가:" << total_sz << '\n';
}
quest heapTop(int i)
{
    return heap[i][1];
}
void heapPop(int i)
{
    heap[i][1] = heap[i][sz[i]--];
    //cout << "감소된 sz_" << i << ':' << sz[i] << '\n';
    int idx = 1;
    while (2 * idx <= sz[i])
    {
        int lc = 2 * idx, rc = 2 * idx + 1;
        int min_child = lc;
        if (rc <= sz[i] && is_second_prior(heap[i][lc], heap[i][rc]))
        {
            min_child = rc;
        }
        if (is_second_prior(heap[i][min_child], heap[i][idx])) break;
        swap(min_child, idx);
        idx = min_child;
    }
    total_sz--;
    //cout << "total_sz 감소:" << total_sz << '\n';
}
int unused;
void init(int N, string u0)
{
    qurl.clear();
    in_judging.clear();
    completed_machine.clear();
    history.clear();
    judging_target.clear();
    machinescnt = N; // 기계 개수
    workingJudgercnt = 0;
    unused_machine = 1;
    unused = 1;
    dmap[dom(u0)] = unused;
    heapInit(unused);
    total_sz = 0;
    memset(sz, 0, 301);     
    //cout << "init u0 :" << u0 << '\n';
    //첫 원소 삽입
    heapPush(unused,{1,u0,0 });
    unused++;
    qurl.insert(u0);

}
void request_mark(quest q)
{
    // 이미 대기열에 존재하는 경우 무시한다
    if (qurl.find(q.url) != qurl.end()) {
        //cout << "대기열에 존재하여" << q.url << "무시\n";
        return;
    }
    int x = dmap[dom(q.url)];
    //cout << "dom:" << dom(q.url) << ' ' << "x:" << x << '\n';
    if (x == 0)
    {
        dmap[dom(q.url)] = unused;
        x = unused;
        unused++;
        heapInit(x);
    }

    qurl.insert(q.url);
    heapPush(x,q);
}
void try_mark(int t)
{
    // 다 가동 중이면 무시
    if (workingJudgercnt == machinescnt) {
        //cout << "다 가동 중\n";
        return;
    }
    //unused-1 번째 힙탑까지 확인. issecondprior -> ok -> condition check
    
    int cnt = 0; // 조건 만족해야 비교 대상이 될 자격도 주어짐 
    quest priq;
    int pridx;

    for (int i = 1; i < unused; i++)
    {
        //cout << "i:" << i << '\n';
        //cout << "sz:" << sz[i] << '\n';
        if (sz[i] == 0) continue;
        
        quest q = heapTop(i);
        //cout << "q.url:" << q.url << '\n';
        // cnt == 1이면 second_prior을 충족해야 다음 진행 
        if (cnt == 1 && !is_second_prior(priq, q)) continue;
        //1. 해당 task가 진행 중인지 -> 진행 중인 set도 만들어야 겠네
            if (in_judging.find(dom(q.url)) != in_judging.end()) {
                // 탐색중인 경우 스킵
                //cout << "탐색 중이라" << q.url << "skip\n";
                continue;
            }
            //2. history에서 확인
            mark_unit u = history[dom(q.url)];
            //cout << "his:" << t << ' ' << u.start << ' ' << u.gap << ' ' << u.start + 3 * u.gap << '\n';
            if (t < u.start + 3 * u.gap)
            {
                //history 범위내에 있는 경우 skip
                //cout << "history 범위 내라" << q.url << "skip\n";            
                continue;
            }                  
        priq = q;
        pridx = i;
        cnt = 1;
    }
    if (cnt == 0) return;

    int m;
    if (completed_machine.empty())
    {
        m = unused_machine++;
    }
    else
    {
        auto it = completed_machine.begin();
        m = *it;
        completed_machine.erase(it);
    }
    //cout << "m:" << m << '\n';
    //m은 사용할 기계 번호
    //judging_target 이 필요한 건 start , url
    judging_target[m] = { t,priq.url };
    in_judging.insert(dom(priq.url));
    qurl.erase(priq.url);  
    int x = dmap[dom(priq.url)];
    heapPop(x);
    //cout << priq.url << "지울게요\n";
    
    workingJudgercnt++;

}
void quit_mark(int t, int num)
{
    //t - start가 gap
    // 무시 조건 아직 num을 쓰지 않았거나 썼는데 작동 중이 아니거나(완료에 있거나)
    if (unused_machine <= num || completed_machine.find(num) != completed_machine.end()) {
        //cout << "unused_machine:" << unused_machine << "정지 불가\n";
        return;
    }
    // 정지 성공
    judge_unit ju = judging_target[num];
    int gap = t - ju.start;
    completed_machine.insert(num);
    history[dom(ju.url)] = { ju.start,gap };
    in_judging.erase(dom(ju.url));
    //cout << "ju.url:" << ju.url << '\n';
    //cout << "ju.start:" << ju.start << '\n';
    //cout << "gap:" << gap << '\n';
    workingJudgercnt--;
}
void count_remain()
{
    cout << total_sz << '\n';
}
int main() {
    
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin >> Q;
    int N, t, prior,jnum;
    string url;
    

    while (Q--)
    {     
        cin >> order;
        switch (order) {
        case 100:          
            cin >> N >> url;
            init(N,url);
            //cout << "100 url : " << url << '\n';
            break;
        case 200:
            cin >> t >> prior >> url;
            request_mark({ prior,url,t });
            break;
        case 300:
            cin >> t;
            try_mark(t);
            break;
        case 400:
            cin >> t >> jnum;
            quit_mark(t,jnum);
            break;
        case 500:
            cin >> t;
            count_remain();
            break;
        default:
            break;

        }
       
    }
    return 0;
}