#include <bits/stdc++.h>
using namespace std;

int N, num_p;
int carmap[55][55];

int num, height, width, col;

queue<pair<int, int>> q;
int bfs_map[55][55];

vector<tuple<int, int, int>> inputs;


void put_bag()
{
    for (int r = 1; r <= height; r++)
    {
        for (int c = 1; c <= width; c++)
        {
            carmap[r][col + c - 1] = num;
        }
    }
}

int dx[] = { 0, 1, 0, -1 };
int dy[] = { 1, 0, -1, 0 };

int newmap[55][55];

int ret_r = 0;
int ret_c = 0;

void find_drop(int cr, int cc, int target)
{
    int h = 0;
    int w = 0;

    if (target == 37)
    {
        int c = 0;
    }

    int i = 0;
    for (i = 0; i < inputs.size(); i++)
    {
        if (get<0>(inputs[i]) == target)
        {
            h = get<1>(inputs[i]);
            w = get<2>(inputs[i]);
        }
    }

    ret_r = cr;
    ret_c = cc;

    for (int r = cr + 1; r <= N; r++)
    {
        int failed = 0;
        for (int nr = 0; nr < h; nr++)
        {
            for (int nc = 0; nc < w; nc++)
            {
                if (newmap[r - nr][cc - nc] != 0 && newmap[r - nr][cc - nc] != target)
                {
                    failed++;
                }
            }
        }

        if (failed)
        {
            failed = 0;
            return;
        }

        ret_r = r;
        ret_c = cc;
    }
}

void bfs_drop()
{
    memset(bfs_map, 0, sizeof(bfs_map));
    memset(newmap, 0, sizeof(newmap));
    
    for (int r = N; r > 0; r--)
    {
        for (int c = N; c > 0; c--)
        {
            if (carmap[r][c] != 0 && bfs_map[r][c] == 0)
            {
                q.push({ r, c });
                bfs_map[r][c] = carmap[r][c];

                find_drop(r, c, carmap[r][c]);

                int r_diff = ret_r - r;
                int c_diff = ret_c - c;

                if (ret_r == 0 && ret_c == 0)
                {
                    r_diff = 0;
                    c_diff = 0;
                }

                while (!q.empty())
                {
                    int x = q.front().first;
                    int y = q.front().second;
                    q.pop();

                    newmap[x + r_diff][y + c_diff] = carmap[x][y];

                    for (int i = 0; i < 4; i++)
                    {
                        int nx = x + dx[i];
                        int ny = y + dy[i];

                        if (nx < 1 || nx > N) continue;
                        if (ny < 1 || ny > N) continue;

                        if (carmap[nx][ny] == carmap[x][y] && bfs_map[nx][ny] == 0)
                        {
                            q.push({ nx, ny });
                            bfs_map[nx][ny] = bfs_map[x][y];
                        }
                    }
                }
            }
        }
    }

    //cout << "newmap:\n";
    //for (int r = 1; r <= N; r++)
    //{
    //    for (int c = 1; c <= N; c++)
    //    {
    //        cout << newmap[r][c] << ' ';
    //    }
    //    cout << '\n';
    //}

    memcpy(carmap, newmap, sizeof(newmap));

    //cout << "carmap:\n";
    //for (int r = 1; r <= N; r++)
    //{
    //    for (int c = 1; c <= N; c++)
    //    {
    //        cout << carmap[r][c] << ' ';
    //    }
    //    cout << '\n';
    //}

    return;
}

bool map_empty()
{
    for (int r = 1; r <= N; r++)
    {
        for (int c = 1; c <= N; c++)
        {
            if (carmap[r][c] != 0)
            {
                return false;
            }
        }
    }
    return true;
}

bool compare(tuple<int, int, int> c1, tuple<int, int, int> c2)
{
    return get<0>(c1) < get<0>(c2);
}

vector<int> delete_candid;

int find_missing()
{
    for (int i = 0; i < inputs.size(); i++)
    {
        int found = 0;
        for (int j = 0; j < delete_candid.size(); j++)
        {
            if (get<0>(inputs[i]) == delete_candid[j])
            {
                found = 1;
                break;
            }
        }

        if (found == 0)
        {
            int return_val = get<0>(inputs[i]);
            
            inputs.erase(inputs.begin() + i, inputs.begin() + i + 1);
            
            return return_val;
        }
    }
}

void delete_map(int target)
{
    cout << target << '\n';

    if (target == 82)
    {
        int a = 0;
    }
    
    for (int r = 1; r <= N; r++)
    {
        for (int c = 1; c <= N; c++)
        {
            if (carmap[r][c] == target)
            {
                carmap[r][c] = 0;
            }
        }
    }
}

void left_pull()
{
    for (int r = 1; r <= N; r++)
    {
        int blocked = 0;
        int non_deleted = -1;

        for (int c = 1; c <= N; c++)
        {
            if (carmap[r][c] != 0)
            {
                if (blocked == 0)
                {
                    blocked = 1;
                    non_deleted = carmap[r][c];
                }
                else
                {
                    if (non_deleted == carmap[r][c])
                    {
                        continue;
                    }

                    delete_candid.push_back(carmap[r][c]);
                }
            }
        }
    }

    sort(delete_candid.begin(), delete_candid.end());
    delete_candid.erase(unique(delete_candid.begin(), delete_candid.end()), delete_candid.end());

    int delete_num = find_missing();
    delete_map(delete_num);

    //cout << "carmap:\n";
    //for (int r = 1; r <= N; r++)
    //{
    //    for (int c = 1; c <= N; c++)
    //    {
    //        cout << carmap[r][c] << ' ';
    //    }
    //    cout << '\n';
    //}

    bfs_drop();

    delete_candid.clear();
}

void right_pull()
{
    for (int r = 1; r <= N; r++)
    {
        int blocked = 0;
        int non_deleted = -1;

        for (int c = N; c > 0; c--)
        {
            if (carmap[r][c] != 0)
            {
                if (blocked == 0)
                {
                    blocked = 1;
                    non_deleted = carmap[r][c];
                }
                else
                {
                    if (non_deleted == carmap[r][c])
                    {
                        continue;
                    }

                    delete_candid.push_back(carmap[r][c]);
                }
            }
        }
    }

    sort(delete_candid.begin(), delete_candid.end());
    delete_candid.erase(unique(delete_candid.begin(), delete_candid.end()), delete_candid.end());

    int delete_num = find_missing();
    delete_map(delete_num);

    bfs_drop();

    delete_candid.clear();
}

int main() {
    // Please write your code here.
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> N >> num_p;

    for (int i = 1; i <= num_p; i++)
    {
        cin >> num >> height >> width >> col;

        inputs.push_back({ num, height, width });

        put_bag();

        bfs_drop();
    }
    sort(inputs.begin(), inputs.end(), compare);

    while (!map_empty())
    {
        left_pull();

        if (map_empty())
        {
            break;
        }

        right_pull();
    }

    return 0;
}
