#include "std_lib_facilities.h"

int main()
{
    /********** Begin **********/
    //int exercise = 1;
    //int exercise = 2;
    int exercise = 3;
    
    if (exercise == 1){
    //第一关执行代码
    //Please fix Question_1.h and Question_1.cpp
	int w, n;
    cin >> w >> n;
    vector<int> man;
    int m = 0;
    for(int i = 0; i < n; i++)
    {
        cin >> m;
        man.push_back(m);
    }
    std::sort(man.begin(), man.end());
    if(man[0] <= 0)
        {cout << "error";return 0;}
    if(man[n - 1] > w)
        {cout << "out";return 0;}
    int ans = 0;
    int i = 0, j = n - 1;
    while(i < j)
    {
        if(man[i] + man[j] <= w)
        {
            ans++;i++;j--;
        }
        else
        {
            ans++;j--;
        }
    }
    if(i == j)
        ans++;
    cout << ans;
    return 0;

    }
    else if(exercise == 2){
    //Please fix Question_2.h and Question_2.cpp  
    	int n = 0;
        cin >> n;
        if(n > 100000)
        {
            cout << "error";
            return 0;
        }
        vector<int> v;  
        int m = 0;
        while(cin >> m)
            {
                if(m < -1000 || m > 1000)
                {
                    cout << "error";
                    return 0;
                }
                v.push_back(m);
            }
        if(v.size() != n)
        {
            cout << "error";
            return 0;
        }
        vector<int> dp(n, 0);
        vector<pair<int, int> > rec(n, make_pair(0, 0));
        dp[0] = v[0];
        rec[0] = make_pair(0, 0);
        for(int i = 1; i < n; i++)
        {
            dp[i] = max(dp[i - 1] + v[i], v[i]);
            if(dp[i] == dp[i - 1] + v[i])
                rec[i] = make_pair(rec[i - 1].first, i);
            else
                rec[i] = make_pair(i, i);
        }
        int ans = -2147483648;
        int loc = 0;
        for(int i = 0; i < n; i++)
        {
            if(dp[i] > ans)
            {
                ans = dp[i];
                loc = i;
            }
        }
        cout << ans << " " << rec[loc].first + 1<< " " << rec[loc].second + 1;
    	return 0;
    }
    else if(exercise == 3){
    //Please fix Question_3.h and Question_3.cpp
   	string s;
    	getline (cin,s);

	int n = s.length();
    vector<vector<string>> dp(n, vector<string>(n, ""));

    // Base cases: single characters are palindromes
    for (int i = 0; i < n; i++) {
        //dp[i][i] = s[i];
        dp[i][i]='\0';
    }

    // Fill the table
    for (int length = 2; length <= n; length++) {
        for (int i = 0; i <= n - length; i++) {
            int j = i + length - 1;
            if (s[i] == s[j]) {
                dp[i][j] = s[i] + dp[i + 1][j - 1] + s[j];
            } else {
                dp[i][j] = dp[i + 1][j].length() >= dp[i][j - 1].length() ? dp[i + 1][j] : dp[i][j - 1];
            }
        }
    }

    cout << dp[0][n - 1] << endl;
    }
    else
	cout<<"error";

    /********** End **********/
    return 0;
}

