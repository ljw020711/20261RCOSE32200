#include <bits/stdc++.h>
using namespace std;


int micro_map[20][20];
int N, T;

int r1, c1, r2, c2;

int m[55][4];

void put_micros(int m)
{
	for (int y = c1; y < c2; y++)
	{
		for (int x = r1; x < r2; x++)
		{
			micro_map[y][x] = m;
		}
	}
}

queue<pair<int, int>> q;
int bfs_map[20][20];

int dx[] = { 0, 1, 0, -1 };
int dy[] = { 1, 0, -1, 0 };

void run_bfs()
{
	while (!q.empty())
	{
		int x = q.front().first;
		int y = q.front().second;
		q.pop();

		for (int d = 0; d < 4; d++)
		{
			int nx = x + dx[d];
			int ny = y + dy[d];

			if (nx < 0 || nx > N) continue;
			if (ny < 0 || ny > N) continue;

			if (micro_map[nx][ny] == micro_map[x][y] && bfs_map[nx][ny] == 0)
			{
				q.push({ nx, ny });
				bfs_map[nx][ny] = 1;
			}
		}
	}
}

void map_delete(int i)
{
	for (int x = 0; x < N; x++)
	{
		for (int y = 0; y < N; y++)
		{
			if (micro_map[y][x] == i)
			{
				micro_map[y][x] = 0;
			}
		}
	}
}

void check_if_splits(int i)
{
	memset(bfs_map, 0, sizeof(bfs_map));

	for (int y = 0; y < N; y++)
	{
		int end = 0;
		for (int x = 0; x < N; x++)
		{
			if (micro_map[y][x] == i && bfs_map[y][x] == 0)
			{
				q.push({ y, x });
				bfs_map[y][x] = 1;

				run_bfs();

				//for (int yj = 0; yj < N; yj++)
				//{
				//	for (int xi = 0; xi < N; xi++)
				//	{
				//		cout << bfs_map[yj][xi] << ' ';
				//	}

				//	cout << '\n';
				//}

				end = 1;
				break;
			}
		}

		if (end)
		{
			break;
		}
	}

	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if (micro_map[y][x] == i && bfs_map[y][x] == 0)
			{
				map_delete(i);

				return;
			}
		}
	}
}

vector<pair<int, int>> v;

bool compare(pair<int, int>& v1, pair<int, int>& v2)
{
	if (v1.second == v2.second)
	{
		return v1.first < v2.first;
	}
	
	return v1.second > v2.second;
}

queue<pair<int, int>> new_q;
int new_bfs_map[54][54];

int find_largest()
{
	int idx = -1;
	memset(new_bfs_map, 0, sizeof(new_bfs_map));

	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if (micro_map[y][x] != 0 && new_bfs_map[y][x] == 0)
			{
				new_q.push({ y, x });
				new_bfs_map[y][x] = 1;

				idx++;
				v.push_back({ micro_map[y][x], 1 });

				while (!new_q.empty())
				{
					int qx = new_q.front().second;
					int qy = new_q.front().first;
					new_q.pop();

					for (int d = 0; d < 4; d++)
					{
						int nx = qx + dx[d];
						int ny = qy + dy[d];

						if (nx < 0 || nx >= N) continue;
						if (ny < 0 || ny >= N) continue;

						if (micro_map[ny][nx] == micro_map[qy][qx] && new_bfs_map[ny][nx] == 0)
						{
							new_q.push({ ny, nx });
							new_bfs_map[ny][nx] = 1;

							v[idx].second += 1;
						}
					}
				}

				//cout << '\n';
				//for (int r = 0; r < N; r++)
				//{
				//	for (int c = 0; c < N; c++)
				//	{
				//		cout << new_bfs_map[r][c] << ' ';
				//	}
				//	cout << '\n';
				//}
			}
		}
	}

	sort(v.begin(), v.end(), compare);

	return idx;
}

int newx = 0;
int newy = 0;

queue<pair<int, int>> new_qq;
int newnew_bfs_map[54][54];

int new_map[54][54];

void delete_all(int micro)
{
	for (int i = N - 1; i >= 0; i--)
	{
		for (int j = N - 1; j >= 0; j--)
		{
			if (new_map[j][i] == micro)
			{
				new_map[j][i] = 0;
			}
		}
	}
}

void fill_map(int micro)
{
	//if (micro == 38)
	//{
	//	int b = 0;
	//}

	int i = 0;
	int j = 0;
	int out = 0;

	for (j = 0; j < N; j++)	//y
	{
		for (i = 0; i < N; i++)	//x
		{
			if (micro_map[j][i] == micro && newnew_bfs_map[j][i] == 0)
			{
				out++;
				break;
			}
		}
		if (out)
		{
			break;
		}
	}

	for (int xs = 0; xs < N; xs++)
	{
		for (int ys = 0; ys < N; ys++)
		{
			int failed = 0;
			if (new_map[ys][xs] == 0)
			{
				new_qq.push({ j, i });
				newnew_bfs_map[j][i] = micro;

				int x_diff = xs - i;
				int y_diff = ys - j;

				while (!new_qq.empty())
				{
					int x = new_qq.front().second;
					int y = new_qq.front().first;
					new_qq.pop();

					int nxs = x + x_diff;
					int nys = y + y_diff;

					if ((nxs >= N) || (nys >= N) || (nxs < 0) || (nys < 0) || (new_map[nys][nxs] != 0))
					{
						failed = 1;
						delete_all(micro);

						memset(newnew_bfs_map, 0, sizeof(newnew_bfs_map));

						while (!new_qq.empty())
						{
							new_qq.pop();
						}

						break;
					}

					new_map[nys][nxs] = micro;

					for (int d = 0; d < 4; d++)
					{
						int nx = x + dx[d];
						int ny = y + dy[d];

						if (nx < 0 || nx >= N)
						{
							continue;
						}
						if (ny < 0 || ny >= N)
						{
							continue;
						}

						if ((micro_map[ny][nx] == micro) && (newnew_bfs_map[ny][nx] == 0))
						{
							new_qq.push({ ny, nx });
							newnew_bfs_map[ny][nx] = micro;
						}
					}
				}
				if (failed)
				{
					continue;
				}

				//cout << "new_map:\n";
				//for (int r = 0; r < N; r++)
				//{
				//	for (int c = 0; c < N; c++)
				//	{
				//		cout << new_map[r][c] << ' ';
				//	}
				//	cout << '\n';
				//}

				return;
			}
		}
	}

}

void change_map()
{
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			micro_map[y][x] = new_map[y][x];
		}
	}

	memset(new_map, 0, sizeof(new_map));
}

void move_map()
{
	int idx = find_largest();

	for (int i = 0; i <= idx; i++)
	{
		//cout << v[i].first << '\n';

		fill_map(v[i].first);

		//for (int y = 0; y < N; y++)
		//{
		//	for (int x = 0; x < N; x++)
		//	{
		//		cout << new_map[y][x] << ' ';
		//	}
		//	cout << '\n';
		//}
		memset(newnew_bfs_map, 0, sizeof(newnew_bfs_map));
	}

	v.clear();
}

vector<pair<int, int>> pairs;
int areas[55];

void exp_result()
{
	int current = 0;
	int pairs_num = 0;
	
	for (int y = 0; y < N; y++)
	{
		current = micro_map[y][0];
		
		for (int x = 0; x < N; x++)
		{
			if (micro_map[y][x] != current)
			{
				pairs.push_back({ current, micro_map[y][x] });
				current = micro_map[y][x];

				pairs_num++;
			}
		}
	}

	for (int x = 0; x < N; x++)
	{
		current = micro_map[0][x];

		for (int y = 0; y < N; y++)
		{
			if (micro_map[y][x] != current)
			{
				pairs.push_back({ current, micro_map[y][x] });
				current = micro_map[y][x];

				pairs_num++;
			}
		}
	}

	for (int i = 0; i < pairs_num; i++)
	{
		if (pairs[i].first > pairs[i].second)
		{
			int tmp = pairs[i].first;
			pairs[i].first = pairs[i].second;
			pairs[i].second = tmp;
		}
	}

	sort(pairs.begin(), pairs.end());

	pairs.erase(unique(pairs.begin(), pairs.end()), pairs.end());

	for (int x = 0; x < N; x++)
	{
		for (int y = 0; y < N; y++)
		{
			areas[micro_map[y][x]]++;
		}
	}

	areas[0] = 0;

	int result = 0;

	for (int i = 0; i < pairs.size(); i++)
	{
		result += (areas[pairs[i].first] * areas[pairs[i].second]);
	}

	cout << result << '\n';

	memset(areas, 0, sizeof(areas));
	pairs.clear();
}

int main()
{
	cin >> N >> T;

	for (int i = 1; i <= T; i++)
	{
		cin >> r1 >> c1 >> r2 >> c2;

		put_micros(i);

		//cout << "put:\n";
		//for (int y = 0; y < N; y++)
		//{
		//	for (int x = 0; x < N; x++)
		//	{
		//		cout << micro_map[y][x] << ' ';
		//	}
		//	cout << '\n';
		//}

		for (int j = 1; j <= i; j++)
		{
			check_if_splits(j);
		}

		//cout << "splits:\n";
		//for (int y = 0; y < N; y++)
		//{
		//	for (int x = 0; x < N; x++)
		//	{
		//		cout << micro_map[y][x] << ' ';
		//	}
		//	cout << '\n';
		//}

		move_map();

		change_map();


		//cout << "change_map:\n";
		//for (int y = 0; y < N; y++)
		//{
		//	for (int x = 0; x < N; x++)
		//	{
		//		cout << micro_map[y][x] << ' ';
		//	}

		//	cout << '\n';
		//}


		exp_result();
	}
}
