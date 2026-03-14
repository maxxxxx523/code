#include <bits/stdc++.h>
using namespace std;

map<string, int> calc_order = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};

vector<string> to_tokens(const string &s)
{
    vector<string> result;
    int i = 0;
    int n = s.length();

    while (i < n)
    {
        if (isspace(s[i]))
        {
            i++;
            continue;
        }
        if (isdigit(s[i]))
        {
            string num;
            while (i < n && isdigit(s[i]))
            {
                num += s[i];
                i++;
            }
            result.push_back(num);
        }
        else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' ||
                 s[i] == '(' || s[i] == ')')
        {
            result.push_back(string(1, s[i]));
            i++;
        }
        else
        {
            cout << "NO" << endl;
            exit(0);
        }
    }
    return result;
}
vector<string> to_RPN(const string &s)
{
    vector<string> tokens = to_tokens(s);
    vector<string> RPN;
    stack<string> st;
    int n = tokens.size();
    int i = 0;
    for (auto i : tokens)
    {
        if (i == "(")
        {
            st.push(i);
        }
        else if (i == ")")
        {
            if(st.empty())
            {
                cout << "NO" << endl;
                exit(0);
            }
            while (st.top() != "(" && !st.empty())
            {
                RPN.push_back(st.top());
                st.pop();
            }
            if (!st.empty() && st.top() == "(")
                st.pop();
            else
            {
                cout << "NO" << endl;
                exit(0);
            }
        }
        else if (i == "+" || i == "-" || i == "*" || i == "/")
        {
            while (!st.empty() && st.top() != "(" && calc_order[st.top()] >= calc_order[i])
            {
                RPN.push_back(st.top());
                st.pop();
            }
            st.push(i);
        }
        else
        {
            RPN.push_back(i);
        }
    }
    while (!st.empty())
    {
        RPN.push_back(st.top());
        st.pop();
    }
    if(RPN.empty())
    {
        cout << "NO" << endl;
        exit(0);
    }
    return RPN;
}

int calc(vector<string> rpn)
{
    stack<string> st;
    for (auto it : rpn)
    {
        if (it == "+" || it == "-" || it == "*" || it == "/" && !st.empty())
        {
            if(st.size()<2)
            {
                cout << "NO" << endl;
                exit(0);
            }
            int b = stoi(st.top());
            st.pop();
            int a = stoi(st.top());
            st.pop();
            if (it == "+")
                st.push(to_string(a + b));
            else if (it == "-")
            {
                st.push(to_string(a - b));
            }
            else if (it == "*")
            {
                st.push(to_string(a * b));
            }
            else if (it == "/")
            {
                st.push(to_string(a / b));
            }
        }
        else
        {
            st.push(it);
        }
    }
    int ans = stoi(st.top());
    return ans;
}

int main()
{
    string s;
    getline(cin, s);
    int n = s.size();
    if (s[n - 1] == '#')
    {
        s.pop_back();
        vector<string> vec = to_RPN(s);
        int ans = calc(vec);
        cout << ans << endl;
    }
    else
    {
        cout << "NO" << endl;
        return 0;
    }
    return 0;
}
