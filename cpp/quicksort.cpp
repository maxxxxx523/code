#include<iostream>
#include<ctime>
using namespace std;
#define N 1000
int a[N];
int get_random(int left,int right)
{
    int i = rand() % (right - left + 1) + left;
    return a[i];
}
void quick_sort(int left,int right)
{
    if(left>=right)
        return;
    //1.选择基准元素
    int p = get_random(left, right);
    //2.数组分三块
    int l = left - 1, i = left, r = right + 1;
    while(i<r)
    {
        if(a[i]<p)
            swap(a[++l], a[i++]);
        else if(a[i]==p)
            i++;
        else
            swap(a[--r], a[i]);
    }
    quick_sort(left, l);
    quick_sort(r, right);
}
int main()
{
    srand(time(0));
    int n;
    cin >> n;
    for (int i = 0; i < n;i++)
    {
        cin >> a[i];
    }
    quick_sort(0, n-1);
    for (int i = 0; i < n;i++)
    {
        cout << a[i] << " ";
    }
    return 0;
}