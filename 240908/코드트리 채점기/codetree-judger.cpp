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
quest heap[50005];
int sz = 0;
void heapInit()
{
    sz = 0;
}
void swap(int index_a, int index_b)
{
    quest emp = heap[index_a];
    heap[index_a] = heap[index_b];
    heap[index_b] = emp;
}
bool is_second_prior(quest a, quest b)
{
    if (a.pri > b.pri || ((a.pri == b.pri) && a.insert_time > b.insert_time))
        return true;
    else return false;
}
void heapPush(quest q)
{
    heap[++sz] = q;
    int idx = sz;
    while (idx != 1)
    {
        int par = idx / 2;
        if (is_second_prior(heap[par],heap[idx]))
        {
            //교환이 필요한 경우
            swap(par, idx);
            idx = par;
        }
        else break;
    }
}
quest heapTop()
{
    return heap[1];
}
void heapPop()
{
    heap[1] = heap[sz--];
    int idx = 1;
    while (2 * idx <= sz)
    {
        int lc = 2 * idx, rc = 2 * idx + 1;
        int min_child = lc;
        if (rc <= sz && is_second_prior(heap[lc], heap[rc]))
        {
            min_child = rc;
        }
        if (is_second_prior(heap[min_child], heap[idx])) break;
        swap(min_child, idx);
        idx = min_child;
    }

}
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
    heapInit();

    //cout << "init u0 :" << u0 << '\n';
    //첫 원소 삽입
    heapPush({ 1,u0,0 });
    qurl.insert(u0);

}
void request_mark(quest q)
{
    // 이미 대기열에 존재하는 경우 무시한다
    if (qurl.find(q.url) != qurl.end()) {
        //cout << "대기열에 존재하여" << q.url << "무시\n";
        return;
    }
    
    qurl.insert(q.url);
    heapPush(q);
}
void try_mark(int t)
{
    // 다 가동 중이면 무시
    if (workingJudgercnt == machinescnt) {
        //cout << "다 가동 중\n";
        return;
    }
    //보관함 생성 -> 힙의 top이 채점 불가능한 경우 가능한 것이 나올 때 까지 보관함에 넣어두기 작업 끝나면 다시 힙에 넣기
    vector<quest>store;
    int able = 0; // 하나 성공한 것 나오면 1 됨
    while (able == 0 && sz > 0)
    {
        quest q = heapTop();
        heapPop();
        //cout << "여기 : pri ,url , inserttime:" << q.pri << ' ' << q.url << ' ' << q.insert_time << '\n';
        //cout << "300 url:" << q.url << '\n';
        //1. 해당 task가 진행 중인지 -> 진행 중인 set도 만들어야 겠네
        if (in_judging.find(dom(q.url)) != in_judging.end()) {
            // 탐색중인 경우 스킵
            //cout << "탐색 중이라" << q.url << "skip\n";
            store.push_back(q);
            continue;
        }
       // cout << "there";
        //2. history에서 확인
        mark_unit u = history[dom(q.url)];
        //cout << "his:" << t << ' ' << u.start << ' ' << u.gap << ' ' << u.start + 3 * u.gap << '\n';
        if (t < u.start + 3 * u.gap)
        {
            //history 범위내에 있는 경우 skip
            //cout << "history 범위 내라" << q.url << "skip\n";
            store.push_back(q);
            continue;
        }
        //cout << "here";
        // 통과 가능 -> 이분 탐색으로 가능한 기계 찾기 최소값 찾는 거라 이분 탐색 아니다.
        // 2 .차례대로 배치하고(unused), 1 .중간에 완료 된 기계번호 ordered_set에 넣어서 할당하기
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
        judging_target[m] = { t,q.url };
        in_judging.insert(dom(q.url));
        qurl.erase(q.url);
        able = 1;
     
    }
    //store에 있는 quest들 다시 삽입
    for (int i = 0; i < store.size(); i++)
    {
        //cout << "i:" << i << '\n';
        heapPush(store[i]);
    }
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
    cout << sz << '\n';
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