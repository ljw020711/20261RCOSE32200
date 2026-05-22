// #define _CRT_SECURE_NO_WARNINGS

#include <bits/stdc++.h>
using namespace std;


int vac_map[34][34];
int vac_pos_map[34][34];
int new_map[34][34];
int N;
int num_vac;
int tc;
int row, col;

vector<tuple<int, int>> vac;

// 오, 아, 왼, 위
int dx[] = { 0, 1, 0, -1 };
int dy[] = { 1, 0, -1, 0 };

void add_dust()
{
    for (int r = 1; r <= N; r++)
    {
        for (int c = 1; c <= N; c++)
        {
            if (vac_map[r][c] > 0)
            {
                vac_map[r][c] += 5;
            }
        }
    }

    return;
}

void spread_dust()
{
    for (int r = 1; r <= N; r++)
    {
        for (int c = 1; c <= N; c++)
        {
            if (vac_map[r][c] == 0)
            {
                for (int i = 0; i < 4; i++)
                {
                    int nx = r + dx[i];
                    int ny = c + dy[i];

                    if (nx < 1 || nx > N) continue;
                    if (ny < 1 || ny > N) continue;
                    if (vac_map[nx][ny] == -1) continue;

                    new_map[r][c] += vac_map[nx][ny];
                }

                new_map[r][c] /= 10;
            }
        }
    }

    return;
}

void change_map()
{
    for (int r = 1; r <= N; r++)
    {
        for (int c = 1; c <= N; c++)
        {
            if (new_map[r][c] > 0)
            {
                vac_map[r][c] += new_map[r][c];
            }
        }
    }

    memset(new_map, 0, sizeof(new_map));
}

//r, c
tuple<int, int> current_vac;

vector<tuple<int, int, int>> dust_candid;

queue<tuple<int, int>> q;
int bfs_map[34][34];

void run_bfs()
{
    q.push(current_vac);
    bfs_map[get<0>(current_vac)][get<1>(current_vac)] = 1;

    while(!q.empty())
    {
        int x = get<0>(q.front());
        int y = get<1>(q.front());
        q.pop();

        for (int i = 0; i < 4; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx < 1 || nx > N) continue;
            if (ny < 1 || ny > N) continue;
            if (vac_map[nx][ny] == -1 || vac_pos_map[nx][ny] != 0) continue;

            if (bfs_map[nx][ny] == 0)
            {
                bfs_map[nx][ny] = bfs_map[x][y] + 1;
                q.push({ nx, ny });
            }
        }
    }
}

bool compare(tuple<int, int, int> v1, tuple<int, int, int> v2)
{
    int dust1 = get<0>(v1);
    int row1 = get<1>(v1);
    int col1 = get<2>(v1);

    int dust2 = get<0>(v2);
    int row2 = get<1>(v2);
    int col2 = get<2>(v2);

    if (dust1 == dust2)
    {
        if (row1 == row2)
        {
            return col1 < col2;
        }

        return row1 < row2;
    }

    return dust1 < dust2;
}

void move_vac()
{
    for (int j = 0; j < vac.size(); j++)
    {
        current_vac = vac[j];

        run_bfs();

        for (int r = 1; r <= N; r++)
        {
            for (int c = 1; c <= N; c++)
            {
                if (vac_map[r][c] > 0 && bfs_map[r][c] >= 1)
                {
                    dust_candid.push_back({bfs_map[r][c] - 1, r, c});
                }
            }
        }

        sort(dust_candid.begin(), dust_candid.end(), compare);

        if (dust_candid.size() == 0)
        {
            continue;
        }

        vac[j] = { get<1>(dust_candid[0]), get<2>(dust_candid[0]) };

        vac_pos_map[get<0>(current_vac)][get<1>(current_vac)] = 0;

        vac_pos_map[get<1>(dust_candid[0])][get<2>(dust_candid[0])] = j + 1;

        memset(bfs_map, 0, sizeof(bfs_map));
        dust_candid.clear();
    }
}

tuple<int, int> crr_vac;
vector<tuple<int, int>> v;

void compute_dir_sum()
{
    int row = get<0>(crr_vac);
    int col = get<1>(crr_vac);

    int sum = 0;

    for (int i = 0; i < 4; i++)
    {
        int left = (i + 3) % 4;
        int right = (i + 1) % 4;

        sum = vac_map[row][col];

        if (row + dx[left] >= 1 && row + dx[left] <= N && col + dy[left] >= 1 && col + dy[left] <= N && vac_map[row + dx[left]][col + dy[left]] > 0)
        {
            if (vac_map[row + dx[left]][col + dy[left]] >= 20)
            {
                sum += 20;
            }
            else
            {
                sum += vac_map[row + dx[left]][col + dy[left]];
            }
        }

        if (row + dx[right] >= 1 && row + dx[right] <= N && col + dy[right] >= 1 && col + dy[right] <= N && vac_map[row + dx[right]][col + dy[right]] > 0)
        {
            if (vac_map[row + dx[right]][col + dy[right]] >= 20)
            {
                sum += 20;
            }
            else
            {
                sum += vac_map[row + dx[right]][col + dy[right]];
            }
        }
        
        if (row + dx[i] >= 1 && row + dx[i] <= N && col + dy[i] >= 1 && col + dy[i] <= N && vac_map[row + dx[i]][col + dy[i]] > 0)
        {
            if (vac_map[row + dx[i]][col + dy[i]] >= 20)
            {
                sum += 20;
            }
            else
            {
                sum += vac_map[row + dx[i]][col + dy[i]];
            }
        }

        v.push_back({ sum, i });
    }
}

bool compare2(tuple<int, int> v1, tuple<int, int> v2)
{
    if (get<0>(v1) == get<0>(v2))
    {
        return get<1>(v1) < get<1>(v2);
    }
    
    return get<0>(v1) > get<0>(v2);
}

void op_clean(int r, int c, int dir)
{
    if (vac_map[r + dx[dir]][c + dy[dir]] > 0)
    {
        vac_map[r + dx[dir]][c + dy[dir]] -= 20;

        if (vac_map[r + dx[dir]][c + dy[dir]] < 0) vac_map[r + dx[dir]][c + dy[dir]] = 0;
    }

    return;
}

void dir_clean(tuple<int, int> v)
{
    int row = get<0>(crr_vac);
    int col = get<1>(crr_vac);

    int i = get<1>(v);
    int left = (i + 3) % 4;
    int right = (i + 1) % 4;

    op_clean(row, col, i);
    op_clean(row, col, left);
    op_clean(row, col, right);

    if (vac_map[row][col] > 0) vac_map[row][col] -= 20;
    if (vac_map[row][col] < 0) vac_map[row][col] = 0;
}

void clean()
{
    for (int j = 0; j < vac.size(); j++)
    {
        crr_vac = vac[j];

        compute_dir_sum();

        sort(v.begin(), v.end(), compare2);

        dir_clean(v[0]);

        v.clear();
    }
}

int main() 
{
    // Please write your code here.
    //freopen("input.txt", "r", stdin);

    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> N >> num_vac >> tc;

    for (int r = 1; r <= N; r++)
    {
        for (int c = 1; c <= N; c++)
        {
            cin >> vac_map[r][c];
        }
    }

    //cout << '\n';
    //for (int r = 1; r <= N; r++)
    //{
    //    for (int c = 1; c <= N; c++)
    //    {
    //        cout << vac_map[r][c] << ' ';
    //    }
    //    cout << '\n';
    //}
    //cout << '\n';

    for (int i = 0; i < num_vac; i++)
    {
        cin >> row >> col;

        vac.push_back({ row, col });

        vac_pos_map[row][col] = i + 1;
    }

    for (int t = 0; t < tc; t++)
    {           
        /*for (int r = 1; r <= N; r++)
        {
            for (int c = 1; c <= N; c++)
            {
                cout << vac_pos_map[r][c] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';*/
        
        // 이동
        move_vac();

        /*for (int r = 1; r <= N; r++)
        {
            for (int c = 1; c <= N; c++)
            {
                cout << vac_pos_map[r][c] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';*/

        //청소
        clean();

        /*for (int r = 1; r <= N; r++)
        {
            for (int c = 1; c <= N; c++)
            {
                cout << vac_map[r][c] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';*/

        add_dust();

        /*for (int r = 1; r <= N; r++)
        {
            for (int c = 1; c <= N; c++)
            {
                cout << vac_map[r][c] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';*/

        spread_dust();

        change_map();

        /*for (int r = 1; r <= N; r++)
        {
            for (int c = 1; c <= N; c++)
            {
                cout << vac_map[r][c] << ' ';
            }
            cout << '\n';
        }*/

        int dust_sum = 0;
        for (int r = 1; r <= N; r++)
        {
            for (int c = 1; c <= N; c++)
            {
                if (vac_map[r][c] > 0)
                {
                    dust_sum += vac_map[r][c];
                }
            }
        }
        cout << dust_sum << '\n';

        if (dust_sum == 0) return 0;
    }

    return 0;
}
