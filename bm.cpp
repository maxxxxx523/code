#include <bits/stdc++.h>
using namespace std;

vector<int> buildsuffix(const string &p)
{
    int m = static_cast<int>(p.size());
    vector<int> suffix(m, 0);
    if (m == 0)
        return suffix;

    suffix[m - 1] = m;
    for (int i = m - 2; i >= 0; --i)
    {
        int j = i;
        while (j >= 0 && p[j] == p[m - 1 - i + j])
            --j;
        suffix[i] = i - j;
    }
    return suffix;
}

vector<int> buildgs(const string &p)
{
    int m = static_cast<int>(p.size());
    if (m == 0)
        return {};

    vector<int> suffix = buildsuffix(p);
    vector<int> gs(m, m);

    // 情况2：好后缀的某个后缀可匹配模式串前缀
    int j = 0;
    for (int i = m - 1; i >= 0; --i)
    {
        if (suffix[i] == i + 1)
        {
            while (j < m - 1 - i)
            {
                if (gs[j] == m)
                    gs[j] = m - 1 - i;
                ++j;
            }
        }
    }

    // 情况1：存在完全匹配的好后缀
    for (int i = 0; i <= m - 2; ++i)
    {
        gs[m - 1 - suffix[i]] = m - 1 - i;
    }

    gs[m - 1] = 1; // 坏字符发生在最后一位时，至少右移1
    return gs;
}

vector<int> buildbc(const string &p)
{
    vector<int> bc(256, -1); // 全字符集
    for (int i = 0; i < static_cast<int>(p.size()); ++i)
    {
        bc[static_cast<unsigned char>(p[i])] = i;
    }
    return bc;
}

int match(const string &p, const string &t)
{
    int m = static_cast<int>(p.size());
    int n = static_cast<int>(t.size());
    if (m == 0)
        return 0;
    if (n < m)
        return -1;

    vector<int> bc = buildbc(p);
    vector<int> gs = buildgs(p);

    int i = 0;
    while (i <= n - m)
    {
        int j = m - 1;
        while (j >= 0 && p[j] == t[i + j])
            --j;

        if (j < 0)
            return i; // 匹配成功

        int badCharShift = j - bc[static_cast<unsigned char>(t[i + j])];
        int goodSuffixShift = gs[j];
        i += max(1, max(badCharShift, goodSuffixShift));
    }

    return -1; // 未匹配
}

int main()
{
    cout << match("ssc", "abbabcddrhaabcdadddsscadabcddabbcaccabc") << endl;
    return 0;
}