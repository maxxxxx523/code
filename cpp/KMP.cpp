#include <bits/stdc++.h>
using namespace std;

vector<int>getnext(const string &t)
{
    int m = t.size();
    vector<int> next(m, -1);
    for (int i = 1; i < m;++i)
    {
        int j = next[i - 1];
        while (j>=0&&t[i]!=t[j+1])
        {
            j = next[j];
        }
        if(t[i]==t[j+1])
        {
            next[i] = j + 1;
        }
        else
        {
            next[i] = -1;
        }
        
    }
    return next;
}

int KMP_match(const string&s,const string &t)
{
    int m = s.size();
    int n = t.size();
    int pos = -1;
    vector<int> next = getnext(t);
    int i = 0, j = 0;
    while (i<m&&j<n)
    {
        if(s[i]==t[j])
        {
            i++;
            j++;
        }
        else if(j>0)
        {
            j = next[j - 1] + 1;
        }
        else
        {
            i++;
        }
    }
    if (j == n)
        pos = i - n;
    return pos;
}

int main()
{
    cout << KMP_match("abcabcabcabcd", "abcabcd") << endl;
    return 0;
}