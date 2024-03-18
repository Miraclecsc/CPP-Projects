#include "std_lib_facilities.h"
using namespace std;
int prime[10005] = {};
bool is_prime(int n)
{
    if(n == 1)
        return false;
    for(int i = 2; i <= sqrt(n); i++)
    {
        if(n % i == 0)
            return false;
    }
    return true;
}

int main()
{
    prime[0] = 0;
    prime[1] = 2;
    int num = 3;
    int base = 2;
    while(base <= 10000)
    {
        if(is_prime(num))
            prime[base++] = num;
        num += 2;
    }
    int n, m;
    cin >> n >> m;
    for(int i = n; i <= m; i++)
    {
        cout << prime[i] << " ";
    }
    return 0;
}