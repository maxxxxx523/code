#include <iostream>
#include <random>
#include <chrono>
#include <vector>
using namespace std;

// 树堆节点结构
template <typename T>
struct Node
{
    T key;
    int priority;
    Node *left, *right;
    int size; // 子树大小，用于求第k大等操作

    Node(const T &k) : key(k), left(nullptr), right(nullptr)
    {
        // 使用mt19937生成高质量随机优先级
        static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        static uniform_int_distribution<int> dist(1, 1e9); // 生成1-1e9范围内的随机数
        priority = dist(rng);
        size = 1;
    }
};

template <typename T>
class Treap
{
private:
    Node<T> *root;

    void updateSize(Node<T> *node)//更新节点的大小
    {
        if (!node)
            return;
        node->size = 1;
        if (node->left)
            node->size += node->left->size;
        if (node->right)
            node->size += node->right->size;
    }

    // 右旋
    Node<T> *rotateRight(Node<T> *p)
    {
        Node<T> *q = p->left;
        p->left = q->right;
        q->right = p;
        updateSize(p);
        updateSize(q);
        return q;
    }

    // 左旋
    Node<T> *rotateLeft(Node<T> *p)
    {
        Node<T> *q = p->right;
        p->right = q->left;
        q->left = p;
        updateSize(p);
        updateSize(q);
        return q;
    }

    Node<T> *insert(Node<T> *node, const T &key)
    {
        if (!node)
            return new Node<T>(key);

        if (key < node->key)//如果小于其根节点的值则插入左子树
        {
            node->left = insert(node->left, key);
            if (node->left->priority > node->priority)
            {
                node = rotateRight(node);
            }
        }
        else if (key > node->key)//如果大于根节点的值则插入右子树
        {
            node->right = insert(node->right, key);
            if (node->right->priority > node->priority)
            {
                node = rotateLeft(node);
            }
        }
        // 如果key已存在，根据需求可以选择不插入或覆盖
        updateSize(node);
        return node;
    }

    // 删除核心递归函数
    Node<T> *remove(Node<T> *node, const T &key)
    {
        if (!node)
            return nullptr;

        if (key < node->key)
        {
            node->left = remove(node->left, key);
        }
        else if (key > node->key)
        {
            node->right = remove(node->right, key);
        }
        else
        {
            // 找到要删除的节点
            if (!node->left && !node->right)
            {
                delete node;
                return nullptr;
            }
            else if (!node->left)
            {
                Node<T> *temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right)
            {
                Node<T> *temp = node->left;
                delete node;
                return temp;
            }
            else
            {
                // 有两个孩子，根据优先级旋转
                if (node->left->priority > node->right->priority)
                {
                    node = rotateRight(node);
                    node->right = remove(node->right, key);
                }
                else
                {
                    node = rotateLeft(node);
                    node->left = remove(node->left, key);
                }
            }
        }
        updateSize(node);
        return node;
    }

    // 查找节点是否存在
    bool search(Node<T> *node, const T &key) const
    {
        if (!node)
            return false;
        if (key == node->key)
            return true;
        if (key < node->key)
            return search(node->left, key);
        return search(node->right, key);
    }

    // 查找最小值
    Node<T> *findMin(Node<T> *node) const
    {
        if (!node)
            return nullptr;
        while (node->left)
            node = node->left;
        return node;
    }

    // 查找最大值
    Node<T> *findMax(Node<T> *node) const
    {
        if (!node)
            return nullptr;
        while (node->right)
            node = node->right;
        return node;
    }

    // 求前驱（小于key的最大值）
    Node<T> *getPredecessor(Node<T> *node, const T &key, Node<T> *best = nullptr) const
    {
        if (!node)
            return best;
        if (node->key < key)
        {
            return getPredecessor(node->right, key, node);
        }
        else
        {
            return getPredecessor(node->left, key, best);
        }
    }

    // 求后继（大于key的最小值）
    Node<T> *getSuccessor(Node<T> *node, const T &key, Node<T> *best = nullptr) const
    {
        if (!node)
            return best;
        if (node->key > key)
        {
            return getSuccessor(node->left, key, node);
        }
        else
        {
            return getSuccessor(node->right, key, best);
        }
    }

    // 求第k小的元素（k从1开始）
    Node<T> *kthSmallest(Node<T> *node, int k) const
    {
        if (!node || k < 1 || k > node->size)
            return nullptr;
        int leftSize = node->left ? node->left->size : 0;
        if (k <= leftSize)
            return kthSmallest(node->left, k);
        if (k == leftSize + 1)
            return node;
        return kthSmallest(node->right, k - leftSize - 1);
    }

    // 求排名（比key小的元素个数+1）
    int getRank(Node<T> *node, const T &key) const
    {
        if (!node)
            return 1;
        if (key <= node->key)
        {
            return getRank(node->left, key);
        }
        else
        {
            int leftSize = node->left ? node->left->size : 0;
            return leftSize + 1 + getRank(node->right, key);
        }
    }

    // 中序遍历（打印有序序列）
    void inorder(Node<T> *node, vector<T> &result) const
    {
        if (!node)
            return;
        inorder(node->left, result);
        result.push_back(node->key);
        inorder(node->right, result);
    }

    // 清空树
    void clear(Node<T> *node)
    {
        if (!node)
            return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    Treap() : root(nullptr) {}

    ~Treap()
    {
        clear(root);
    }

    void insert(const T &key)
    {
        root = insert(root, key);
    }

    void remove(const T &key)
    {
        root = remove(root, key);
    }

    bool search(const T &key) const
    {
        return search(root, key);
    }

    T getMin() const
    {
        Node<T> *node = findMin(root);
        if (!node)
            throw runtime_error("Treap is empty");
        return node->key;
    }

    T getMax() const
    {
        Node<T> *node = findMax(root);
        if (!node)
            throw runtime_error("Treap is empty");
        return node->key;
    }

    T getPredecessor(const T &key) const
    {
        Node<T> *node = getPredecessor(root, key);
        if (!node)
            throw runtime_error("No predecessor found");
        return node->key;
    }

    T getSuccessor(const T &key) const
    {
        Node<T> *node = getSuccessor(root, key);
        if (!node)
            throw runtime_error("No successor found");
        return node->key;
    }

    T kthSmallest(int k) const
    {
        Node<T> *node = kthSmallest(root, k);
        if (!node)
            throw runtime_error("k is out of range");
        return node->key;
    }

    int getRank(const T &key) const
    {
        if (!search(key))
            throw runtime_error("Key not found");
        return getRank(root, key);
    }

    int size() const
    {
        return root ? root->size : 0;
    }

    bool empty() const
    {
        return root == nullptr;
    }

    vector<T> toVector() const
    {
        vector<T> result;
        inorder(root, result);
        return result;
    }

    void print() const
    {
        vector<T> v = toVector();
        for (const T &val : v)
        {
            cout << val << " ";
        }
        cout << endl;
    }
};

// 测试代码
int main()
{
    Treap<int> treap;

    // 插入元素
    vector<int> nums = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    cout << "插入序列: ";
    for (int num : nums)
    {
        cout << num << " ";
        treap.insert(num);
    }
    cout << endl;

    cout << "中序遍历结果: ";
    treap.print();

    cout << "树的大小: " << treap.size() << endl;
    cout << "最小值: " << treap.getMin() << endl;
    cout << "最大值: " << treap.getMax() << endl;

    // 查找测试
    cout << "\n查找5: " << (treap.search(5) ? "存在" : "不存在") << endl;
    cout << "查找10: " << (treap.search(10) ? "存在" : "不存在") << endl;

    // 前驱后继
    cout << "\n元素6的前驱: " << treap.getPredecessor(6) << endl;
    cout << "元素6的后继: " << treap.getSuccessor(6) << endl;

    // 排名和第k小
    cout << "\n元素4的排名: " << treap.getRank(4) << endl;
    cout << "第3小的元素: " << treap.kthSmallest(3) << endl;

    // 删除测试
    cout << "\n删除元素5: ";
    treap.remove(5);
    treap.print();
    cout << "查找5: " << (treap.search(5) ? "存在" : "不存在") << endl;

    cout << "删除元素2: ";
    treap.remove(2);
    treap.print();

    cout << "\n最终树的大小: " << treap.size() << endl;

    return 0;
}