#include <bits/stdc++.h>
using namespace std;

struct Treenode
{
    Treenode *children[26];
    int count;
    Treenode() : count(0)
    {
        for (int i = 0; i < 26; i++)
            children[i] = nullptr;
    }
};

class Trie
{
private:
    Treenode *root;

    void insert_helper(const string &word)
    {
        Treenode *node = root;
        for (char ch : word)
        {
            int idx = ch - 'a';
            if (!node->children[idx])
            {
                node->children[idx] = new Treenode();
            }
            node = node->children[idx];
        }
        node->count++;
    }

    void collect_helper(Treenode *node, string &current, vector<pair<string, int>> &result)
    {
        if (node->count > 0)
        {
            result.push_back({current, node->count});
        }
        for (int i = 0; i < 26; i++)
        {
            if (node->children[i])
            {
                current.push_back('a' + i);
                collect_helper(node->children[i], current, result);
                current.pop_back();
            }
        }
    }

    void clear(Treenode *node)
    {
        for (int i = 0; i < 26; i++)
        {
            if (node->children[i])
            {
                clear(node->children[i]);
            }
        }
        delete node;
    }

    int dfs(Treenode *p)
    {
        if (p == nullptr)
            return 0;
        vector<int> arr;
        for (int i = 0; i < 26; ++i)
        {
            if (p->children[i])
                arr.push_back(dfs(p->children[i]));
        }
        if (arr.empty())
            return 1;
        return *max_element(arr.begin(), arr.end()) + 1;
    }

    int dfscount(Treenode *p)
    {
        if (p == nullptr)
            return 0;
        int total = p->count;
        for (int i = 0; i < 26; ++i)
        {
            if (p->children[i])
                total += dfscount(p->children[i]);
        }
        return total;
    }

public:
    Trie()
    {
        root = new Treenode;
    }

    void insert(const string &word)
    {
        insert_helper(word);
    }

    void collectwords(string &current, vector<pair<string, int>> &result)
    {
        collect_helper(root, current, result);
    }

    int getMax_len_of_words()
    {
        return dfs(root);
    }

    int getNum_of_words()
    {
        return dfscount(root);
    }

    bool isempty()
    {
        return root == nullptr;
    }

    ~Trie()
    {
        clear(root);
    }
};

class cmp
{
public:
    bool operator()(const pair<string, int> &a, const pair<string, int> &b)
    {
        if (a.second != b.second)
            return a.second > b.second;
        return a.first < b.first;
    }
};

int main()
{
    ifstream infile("in.txt");
    Trie tr;
    string line;

    while (getline(infile, line))
    {
        string word;
        for (char ch : line)
        {
            if (isalpha(ch))
            {
                word += tolower(ch);
            }
            else
            {
                if (!word.empty())
                {
                    tr.insert(word);
                    word.clear();
                }
            }
        }

        if (!word.empty())
        {
            tr.insert(word);
            word.clear();
        }
    }
    infile.close();

    vector<pair<string, int>> result;
    string current;
    tr.collectwords(current, result);

    sort(result.begin(), result.end(), cmp());

    int limit = min(100, (int)result.size());
    for (int i = 0; i < limit; i++)
    {
        cout << result[i].first << " " << result[i].second << endl;
    }
    
    return 0;
}