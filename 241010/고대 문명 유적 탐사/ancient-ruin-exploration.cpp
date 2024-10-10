#include<iostream>
#include<queue>
using namespace std;
int K, M;
int board[5][5];
int wall[304];
bool visited[5][5];
bool removing[5][5];
int dy[4] = { 0,0,1,-1 };
int dx[4] = { 1,-1,0,0 };
//int bfscnt = 0;
bool isArea(int y, int x)
{
	if (y >= 5 || y < 0 || x >= 5 || x < 0) return false;
	else return true;
}
int bfs(int y, int x)
{
	//bfscnt++;
	queue<pair<int, int>>q;
	q.push({ y,x });
	visited[y][x] = true;


	int cnt = 1;
	while (!q.empty())
	{
		int b = q.front().first;
		int a = q.front().second;
		q.pop();

		for (int p = 0; p < 4; p++)
		{
			int nb = b + dy[p];
			int na = a + dx[p];

			if (!isArea(nb, na) || visited[nb][na] || board[nb][na] != board[b][a]) continue;

			q.push({ nb,na });
			visited[nb][na] = true;
			//removing[nb][na] = true;
			cnt++;
			//cout << "cnt:" << cnt << '\n';
		}
	}
	return cnt;
}
int removing_bfs(int y, int x)
{
	bool removing_visited[5][5] = { 0, };
	queue<pair<int, int>>q;
	q.push({ y,x });
	visited[y][x] = true;
	removing[y][x] = true;

	int cnt = 0;
	while (!q.empty())
	{
		int b = q.front().first;
		int a = q.front().second;
		q.pop();

		for (int p = 0; p < 4; p++)
		{
			int nb = b + dy[p];
			int na = a + dx[p];

			if (!isArea(nb, na) || removing_visited[nb][na] || board[nb][na] != board[b][a]) continue;

			q.push({ nb,na });
			removing_visited[nb][na] = true;
			removing[nb][na] = true;
			cnt++;
		}
	}
	return cnt;
}
void rotate90(int ceny, int cenx)
{
	int emp1 = board[ceny - 1][cenx - 1];
	int emp2 = board[ceny - 1][cenx];

	board[ceny - 1][cenx] = board[ceny][cenx - 1];
	board[ceny - 1][cenx - 1] = board[ceny + 1][cenx - 1];

	board[ceny][cenx - 1] = board[ceny + 1][cenx];
	board[ceny + 1][cenx - 1] = board[ceny + 1][cenx + 1];

	board[ceny + 1][cenx] = board[ceny][cenx + 1];
	board[ceny + 1][cenx + 1] = board[ceny - 1][cenx + 1];

	board[ceny][cenx + 1] = emp2;
	board[ceny - 1][cenx + 1] = emp1;
}
void rotate180(int ceny, int cenx)
{
	int emp1 = board[ceny - 1][cenx - 1];
	int emp2 = board[ceny - 1][cenx];
	int emp3 = board[ceny - 1][cenx + 1];
	int emp4 = board[ceny][cenx + 1];

	board[ceny - 1][cenx - 1] = board[ceny + 1][cenx + 1];
	board[ceny - 1][cenx] = board[ceny + 1][cenx];
	board[ceny - 1][cenx + 1] = board[ceny + 1][cenx - 1];
	board[ceny][cenx + 1] = board[ceny][cenx - 1];

	board[ceny + 1][cenx + 1] = emp1;
	board[ceny + 1][cenx] = emp2;
	board[ceny + 1][cenx - 1] = emp3;
	board[ceny][cenx - 1] = emp4;
}
void rotate270(int ceny, int cenx)
{
	int emp1 = board[ceny - 1][cenx - 1];
	int emp2 = board[ceny - 1][cenx];

	board[ceny - 1][cenx - 1] = board[ceny - 1][cenx + 1];
	board[ceny - 1][cenx] = board[ceny][cenx + 1];

	board[ceny - 1][cenx + 1] = board[ceny + 1][cenx + 1];
	board[ceny][cenx + 1] = board[ceny + 1][cenx];

	board[ceny + 1][cenx + 1] = board[ceny + 1][cenx - 1];
	board[ceny + 1][cenx] = board[ceny][cenx - 1];

	board[ceny + 1][cenx - 1] = emp1;
	board[ceny][cenx - 1] = emp2;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cin >> K >> M;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
			cin >> board[i][j];
	}
	for (int i = 0; i < M; i++)
		cin >> wall[i];

	int fixed_board[5][5];
	int ceny, cenx;
	int max_score;
	int max_ceny, max_cenx, max_rot;
	int ycnt;
	bool suc; // 턴 당 성공 여부
	int turn_score;
	int turn = 0;
	int unused = 0;
	while (++turn <= K)
	{
		//한 턴 안에는 같은 보드에서 시작해야함.
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
				fixed_board[i][j] = board[i][j];
		}
		/*cout << "fix 출력\n";
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
				cout << fixed_board[i][j];
			cout << "\n";
		}*/
		//첫턴 스코어 비교
		suc = false;
		max_score = 0;
		for (int rot = 0; rot < 3; rot++)
		{
		for (int j = 1; j <= 3; j++)
			for (int i = 1; i <= 3; i++)
			{
				ceny = i, cenx = j;

				
					//회전 전에 board 초기화
					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < 5; j++)
							board[i][j] = fixed_board[i][j];
					}

					ycnt = 0;

					if (rot == 0) rotate90(ceny, cenx);
					else if (rot == 1) rotate180(ceny, cenx);
					else rotate270(ceny, cenx);

					
					for (int y = 0; y < 5; y++)
						for (int x = 0; x < 5; x++)
						{
							visited[y][x] = false;
							//removing[y][x] = false;
						}

					for (int y = 0; y < 5; y++)
						for (int x = 0; x < 5; x++)
						{
							if (visited[y][x]) continue;

							int val = bfs(y, x);
							//if (turn == 1) cout << "Val:" << val << '\n';
							if (val >= 3)
							{
								// 성공 시
								ycnt += val;
								suc = true;
							}
						}
					if (ycnt > max_score)
					{
						max_score = ycnt;
						max_ceny = i;
						max_cenx = j;
						max_rot = rot;
					}
			}
		}
		//cout << "suc:" << suc << '\n';
		// 조합 실패
		if (!suc) break;
		// 조합 성공
		//1.성공한 상태로 보드 변화
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
				board[i][j] = fixed_board[i][j];
		}
		if (max_rot == 0) rotate90(max_ceny, max_cenx);
		else if (max_rot == 1) rotate180(max_ceny, max_cenx);
		else rotate270(max_ceny, max_cenx);
		//2. 연쇄 유물 발굴
		int discovered;
		int combi_cnt = 0;
		turn_score = 0;
		
		while (1)
		{
			combi_cnt++;
			discovered = 0;
			for (int y = 0; y < 5; y++)
				for (int x = 0; x < 5; x++)
				{
					visited[y][x] = false;
					removing[y][x] = false;
				}

			for (int y = 0; y < 5; y++)
				for (int x = 0; x < 5; x++)
				{
					if (visited[y][x]) continue;

					int val = bfs(y, x);
					if (val >= 3)
					{
						
						// 성공 시
						//cout << "ceny, cenx, max_rot:" << max_ceny << ' ' << max_cenx << ' ' << max_rot << '\n';
						//cout << "y,x,val:" << y << ' ' << x << ' ' << val << '\n';
						
						turn_score += val;
						suc = true;
						discovered = 1;

						//removing 갱신
						removing_bfs(y, x);
					}
				}
			//갈아 끼우기
			
			for (int x = 0; x < 5; x++)
				for (int y = 4; y >= 0; y--)
				{
					if (removing[y][x])
					{						
						board[y][x] = wall[unused++];
					}
				}
			
			
			if (discovered == 0) break;
		}
		// 유물 가치 출력
		cout << turn_score << ' ';
	}
}