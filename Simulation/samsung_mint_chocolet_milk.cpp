#include <iostream>
#include <bits/stdc++.h>
using namespace std;

string food[54][54];
int belief[54][54];

int len;

int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

void morning()
{
    for(int r = 0; r < len; r++)
    {
        for(int c = 0; c < len; c++)
        {
            belief[r][c] += 1;
        }
    }

    return ;
}

queue<pair<int, int>> noon_q;
int noon_bfs[54][54];

tuple<int, int, int> leaders[2500];
int bfs_num;

void noon()
{
    memset(leaders, 0, sizeof(leaders));
    memset(noon_bfs, 0, sizeof(noon_bfs));

    bfs_num = 0;
    
    for(int r = 0; r < len; r++)
    {
        for(int c = 0; c < len; c++)
        {
            if(noon_bfs[r][c] == 0)
            {
                bfs_num += 1;
                
                noon_q.push({r, c});
                noon_bfs[r][c] = bfs_num;
                belief[r][c] -= 1;

                int belief_count = 1;
                leaders[bfs_num] = make_tuple(belief[r][c] , r, c);

                while(!noon_q.empty())
                {
                    pair<int, int> student = noon_q.front();
                    int x = student.first;
                    int y = student.second;
                    noon_q.pop();

                    for(int i = 0; i < 4; i++)
                    {
                        int nx = x + dx[i];
                        int ny = y + dy[i];

                        if(nx < 0 || nx >= len) continue;
                        if(ny < 0 || ny >= len) continue;

                        if(noon_bfs[nx][ny] == 0 && food[nx][ny] == food[x][y])
                        {
                            noon_q.push({nx, ny});
                            noon_bfs[nx][ny] = bfs_num;

                            belief[nx][ny] -= 1;
                            belief_count++;

                            auto current_leader = leaders[bfs_num];
                            bool should_update = false;

                            if (belief[nx][ny] > get<0>(current_leader)) {
                                should_update = true;
                            } else if (belief[nx][ny] == get<0>(current_leader)) {
                                if (nx < get<1>(current_leader)) {
                                    should_update = true;
                                } else if (nx == get<1>(current_leader) && ny < get<2>(current_leader)) {
                                    should_update = true;
                                }
                            }

                            if (should_update) {
                                leaders[bfs_num] = make_tuple(belief[nx][ny], nx, ny);
                            }
                        }
                    }
                }

                belief[get<1>(leaders[bfs_num])][get<2>(leaders[bfs_num])] += belief_count;

            }
        }
    }

    return ;
}


int get_group(string s)
{
    sort(s.begin(), s.end());
    
    if("T" == s)
    {
        return 1;
    }
    if("M" == s)
    {
        return 1;
    }
    if("C" == s)
    {
        return 1;
    }

    if("CT" == s)
    {
        return 2;
    }
    if("MT" == s)
    {
        return 2;
    }
    if("CM" == s)
    {
        return 2;
    }

    if("CMT" == s)
    {
        return 3;
    }

    return 0;
}

bool compare(tuple<int, int, int> l1, tuple<int, int, int> l2)
{
    if(get_group(food[get<1>(l1)][get<2>(l1)]) != get_group(food[get<1>(l2)][get<2>(l2)]))
    {
        return get_group(food[get<1>(l1)][get<2>(l1)]) < get_group(food[get<1>(l2)][get<2>(l2)]);
    }

    if(belief[get<1>(l1)][get<2>(l1)] != belief[get<1>(l2)][get<2>(l2)])
    {
        return belief[get<1>(l1)][get<2>(l1)] > belief[get<1>(l2)][get<2>(l2)];
    }

    if(get<1>(l1) != get<1>(l2))
    {
        return get<1>(l1) < get<1>(l2);
    }

    return get<2>(l1) < get<2>(l2);
}

int ddx[] = {-1, 1, 0, 0};
int ddy[] = {0, 0, -1, 1};

void print_b();

int str_contains(string foodin, string current_food)
{
    sort(foodin.begin(), foodin.end());
    sort(current_food.begin(), current_food.end());

    if(foodin.find(current_food) != string::npos || current_food.find(foodin) != string::npos)
    {
        return 1;
    }

    return 0;
}

int str_same(string foodin, string current_food)
{
    sort(foodin.begin(), foodin.end());
    sort(current_food.begin(), current_food.end());

    if(foodin == current_food)
    {
        return 1;
    }

    return 0;
}

string str_add(string foodin, string current_food)
{
    string s = foodin + current_food;
    sort(s.begin(), s.end());
    s.erase(unique(s.begin(), s.end()), s.end());

    return s;
}

int changed[54][54];

void dinner()
{ 
    memset(changed, 0, sizeof(changed));
    
    sort(leaders + 1, leaders + 1 + bfs_num, compare);
    
    for(int i = 1; i <= bfs_num; i++)
    {
        int c_x = get<1>(leaders[i]);
        int c_y = get<2>(leaders[i]);

        if(changed[c_x][c_y] == 1)
        {
            continue;
        }

        // cout << c_x << ' ' << c_y << '\n';

        string current_food = food[c_x][c_y];
        int x = belief[c_x][c_y] - 1;
        int dir = belief[c_x][c_y] % 4;
        belief[c_x][c_y] = 1;

        // cout << dir << '\n';

        int nx = c_x;
        int ny = c_y;

        while(x > 0)
        {
            nx = nx + ddx[dir];
            ny = ny + ddy[dir];

            if(nx < 0 || nx >= len) break;
            if(ny < 0 || ny >= len) break;
            if(str_same(food[nx][ny], current_food)) continue;

            // cout << x << '\n';

            if(x > belief[nx][ny])
            {
                x = x - (belief[nx][ny] + 1);
                belief[nx][ny] += 1;
                food[nx][ny] = current_food;

                changed[nx][ny] = 1;
            }
            else
            {
                belief[nx][ny] += x;
                food[nx][ny] = str_add(food[nx][ny], current_food);
                x = 0;

                changed[nx][ny] = 1;
            }
        }

        // print_b();

        // for(int r = 0; r < len; r++)
        // {
        //     for(int c = 0; c < len; c++)
        //     {
        //         cout << food[r][c] << ' ';
        //     }
        //     cout << '\n';
        // }
    }
}

void print_b()
{
    for(int r = 0; r < len; r++)
    {
        for(int c = 0; c < len; c++)
        {
            cout << belief[r][c] << ' ';
        }

        cout << '\n';
    }

}

int CMT, CT, MT, CM, M, C, T;

int main() {
    // Please write your code here.
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> len;

    int days = 0;
    cin >> days;

    string tmpstr;
    for(int r = 0; r < len; r++)
    {
        cin >> tmpstr;

        for(int c = 0; c < len; c++)
        {
            food[r][c] = tmpstr[c];
        }
    }

    int tmp = 0;

    for(int r = 0; r < len; r++)
    {
        for(int c = 0; c < len; c++)
        {
            cin >> belief[r][c];
        }
    }

    for(int t = 0; t < days; t++)
    {
        morning();
        //print_b();

        noon();
        //print_b();

        dinner();
        //print_b();


        // for(int r = 0; r < len; r++)
        // {
        //     for(int c = 0; c < len; c++)
        //     {
        //         cout << food[r][c] << ' ';
        //     }
        //     cout << '\n';
        // }

        for(int r = 0; r < len; r++)
        {
            for(int c = 0; c < len; c++)
            {
                sort(food[r][c].begin(), food[r][c].end());

                if(food[r][c] == "CMT")
                {
                    CMT += belief[r][c];
                }

                if(food[r][c] == "CT")
                {
                    CT += belief[r][c];
                }

                if(food[r][c] == "MT")
                {
                    MT += belief[r][c];
                }

                if(food[r][c] == "CM")
                {
                    CM += belief[r][c];
                }

                if(food[r][c] == "T")
                {
                    T += belief[r][c];
                }

                if(food[r][c] == "C")
                {
                    C += belief[r][c];
                }

                if(food[r][c] == "M")
                {
                    M += belief[r][c];
                }

            }
        }

        cout << CMT << ' ' << CT << ' ' << MT << ' ' << CM << ' ' << M << ' ' << C << ' ' << T << '\n';

        CMT = 0;
        CT = 0;
        MT = 0;
        CM = 0;
        M = 0;
        C = 0;
        T = 0;

    }

    return 0;
}
