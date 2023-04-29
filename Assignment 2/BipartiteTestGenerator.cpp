#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    cin >> n;
    cout << n << " " << n*n/4-n/2 << endl;
    for (int i = 0; i < n/2; i++)
    {
        for (int j = n/2; j < n; j++)
        {
            if (j != i + n/2) cout << i + 1 << " " << j + 1 << endl;
        }
    }
}