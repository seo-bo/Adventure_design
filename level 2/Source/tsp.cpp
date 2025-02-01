#include "sub.h"

void init_dp() 
{
    int max_mask = 1 << count;
    for (int i = 0; i < max_mask; i++) 
    {
        for (int j = 0; j < count; j++)
        {
            dp[i][j] = 1e9;
            dp_parent[i][j] = -1; // 부모 노드 초기화
        }
    }
    dp[1][0] = 0;
}

void dp_algorithm() 
{
    int full_mask = (1 << count) - 1;
    for (int mask = 1; mask <= full_mask; ++mask) 
    {
        for (int last = 0; last < count; ++last)
        {
            if (!(mask & (1 << last)))
            {
                continue;
            }
            if (mask == (1 << last))
            {
                if (last == 0)
                {
                    dp[mask][last] = 0;
                }
                else
                {
                    dp[mask][last] = 1e9;
                }
                continue;
            }
            for (int prev = 0; prev < count; ++prev)
            {
                if (prev == last || !(mask & (1 << prev)))
                {
                    continue;
                }
                long long temp = dp[mask ^ (1 << last)][prev] + distance[prev][last];
                if (dp[mask][last] > temp) 
                {
                    dp[mask][last] = temp;
                    dp_parent[mask][last] = prev;
                }
            }
        }
    }
    dp_mini = 1e9;
    int last_city = -1;
    for (int i = 0; i < count; ++i) 
    {
        long long temp = dp[full_mask][i] + distance[i][0];
        if (dp_mini > temp)
        {
            dp_mini = temp;
            last_city = i;
        }
    }
    if (last_city != -1)
    {
        get_dp_path(last_city);
    }
}

int* get_dp_path(int last_city) 
{
    int mask = (1 << count) - 1;
    int last = last_city;
    dp_head = 0;
    path_dp[dp_head++] = 0; 
    while (mask != (1 << 0))
    {
        path_dp[dp_head++] = last;
        int temp = dp_parent[mask][last];
        if (temp == -1)
        {
            break;
        }
        mask = mask ^ (1 << last);
        last = temp;
    }
    std::reverse(path_dp, path_dp + dp_head);
    return path_dp;
}

void init_sa() 
{
    temper = 1000.0;
    sa_mini = 1e9;
    for (int i = 0; i < count; i++) 
    {
        current[i] = i;
    }
    current_dis = 0.0;
    for (int i = 0; i < count - 1; i++) 
    {
        current_dis += distance[current[i]][current[i + 1]];
    }
    current_dis += distance[current[count - 1]][current[0]];
}

void cal_sa_step() 
{
    if (temper <= 1e-3)
    {
        return;
    }
    int pre = rand() % count;
    int next = rand() % count;
    if (pre == next)
    {
        return;
    }
    if (pre > next)
    {
        std::swap(pre, next);
    }
    inversion(pre, next);
    long long new_dis = 0;
    for (int i = 0; i < count - 1; i++)
    {
        new_dis += distance[current[i]][current[i + 1]];
    }
    new_dis += distance[current[count - 1]][current[0]];
    long long delta = new_dis - current_dis;
    if (delta < 0 || (exp(-delta / temper) > ((double)rand() / RAND_MAX))) 
    {
        current_dis = new_dis;
        if (current_dis < sa_mini)
        {
            sa_mini = current_dis;
            memcpy(path_sa, current, sizeof(int) * count);
        }
    }
    else
    {
        inversion(pre, next);
    }
    temper *= cooling;
}

void init_bf()
{
    bf_mini = 1e9;
    make_permutation();
    bf_head = 0;
}

bool cal_bf_step()
{
    if (!next_permutation(permutation + 1, permutation + count))
    {
        bf_head = -1; // 모든 순열 탐색 완료
        return false;
    }
    long long total_dis = 0;
    for (int i = 0; i < count - 1; i++) 
    {
        total_dis += distance[permutation[i]][permutation[i + 1]];
    }
    total_dis += distance[permutation[count - 1]][permutation[0]];
    if (total_dis < bf_mini)
    {
        bf_mini = total_dis;
        memcpy(path_bf, permutation, sizeof(int) * count);
        bf_head = count; // 경로 길이 업데이트
        return true; // 새로운 최소 거리 발견
    }
    return false; // 최소 거리 갱신 없음
}

int* get_bf_path()
{
    return path_bf;
}
