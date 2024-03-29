#include <bits/stdc++.h>
using namespace std;
map<int, int> redundantBits;
int getNumberOfBits(int m)
{
    int r = 0;
    while (pow(2, r) < m + r + 1)
        r++;
    return m + r;
}
vector<int> getBinary(long long int data)
{
    vector<int> bin(log2(data) + 1);
    int i = 0;
    while (data)
    {
        bin[i++] = data % 2;
        data /= 2;
    }
    reverse(bin.begin(), bin.end());
    return bin;
}
int checkAndFixParity(vector<int> bin, int off, int size)
{
    int n = bin.size();
    int count = 0;
    cout << off << " ";
    for (int i = 0; i < size + 1; i++)
    {
        if (((i >> off) & 1) == 1 && bin[i - 1] == 1 && redundantBits.count(i) == 0)
        {
            count++;
            cout << i << " ";
        }
    }
    cout << endl;
    if (count % 2 != 0)
        return 1;
    else
        return 0;
}
int main()
{
    int index = 1;
    int m;
    cin >> m;
    vector<int> bin(m);
    for (int &i : bin)
        cin >> i;
    int size = getNumberOfBits(m);
    int data_index = 0;
    while (index <= size)
        redundantBits[index] = 1, index *= 2;
    index = 1;
    vector<int> dataWithRedundant(size);
    for (int i = 0; i < size; i++)
    {
        if (i != (index - 1))
            dataWithRedundant[i] = bin[data_index++];
        else
            index *= 2;
    }
    index = 1;
    while (index <= size)
    {
        dataWithRedundant[index - 1] = checkAndFixParity(dataWithRedundant, log2(index), size);
        index *= 2;
    }
    for (int i : bin)
        cout << i << " ";
    cout << endl;
    for (int i : dataWithRedundant)
        cout << i << " ";
    cout << endl;
}