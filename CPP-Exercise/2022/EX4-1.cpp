/*
编程要求

（1）读入线性方程组
    程序的输入共有若干行，每一行为一个线性方程（形如3*n1+4*m2=10），识别出方程中的“元”、元的系数等。
（2）利用高斯消去法计算
    将方程组的元的系数组建为一个矩阵，并利用高斯消去法求解。
（3）错误处理
    （a）判断输入的每一行是否是一个规范的多元一次方程。若不规范则跳过该行的方程，对剩下的规范方程（组）进行求解。
    （b）判断不符合标准高斯法求解的方程组，并利用try catch机制报错，具体报错内容见后续说明。
（4）输出结果
    输出分为三部分：
    1. 输入字符数:输出字符数，输出字符数需统计\n等不可见字符，不统计第一行输出。
    2. 合法的方程组及所有元的解（按行输出所有元的解，每个元与解之间用:分隔）。
    （a）输出合法方程组的排序同输入顺序，输出元的解的排序以元（变量名）字典序从小到大排序为准；
    （b）若有唯一解，则保留小数点后4位（直接截断）。例如，某一“元”解为0，则其数值的预期输出为0.0000；
    （c）若方程组无解（例如，0*x1=5），则输出error1；
    （d）若方程组存在多个解（例如，0*x2=0），则输出error2；
    （e）若不存在合法方程（组），则“元的解”这一部分输出为空；
    3. 不能解析为多元一次方程的行，即非法方程式（Illegal Equations）

注意方程式合法性
（1）用户输入的合法表达式仅包含.，+，-，* 符号、数字以及字母a~z，每一项系数为整数或小数；
（2）“元”与“系数”之间以*连接，系数在前，元在后。如5*n2，代表n2是元，5是系数。如果系数为1，则可以省略该系数和*；
（3）“元”的表示为“字母+数字”的形式（例如a0, d30, g20），字母为小写字母a~z，数字为小于100的非负整数；
（4）“元”只能出现在等号左侧，一个方程式中同一个“元”至多出现一次；
（5）等号左边不出现常数项，等号右边只能出现常数项；
（6）计算过程中，忽略不符合标准形式的方程（如x1*x2=10;x1+5=3等）。

测试说明

测试输入:
x1+2*x3=5
x1+x2=1
预期输出:
17:60
Legal Equations:
x1+2*x3=5
x1+x2=1
error2
Illegal Equations:
*/

#include "std_lib_facilities.h"

string init_numbers(double a)
{
    string s = to_string(a);
    for(int i = 0; i < s.length(); ++i)
    {
        if(s[i] == '.')
        {
            s.erase(i + 5);
            break;
        }
    }
    return s;
}


class formulation
{
private:
    string str;
    bool valid = true;

public:
    formulation(const string& s);
    vector<pair<pair<char, int>,double>> veriable;
    double res = 0.0;
    bool is_valid() const {return valid;}
};

formulation::formulation(const string& s) : str(s) 
{
    int pos = 0, i = 0;
    for(pos = 0; pos < str.size(); ++pos)
    {
        if(str[pos] == '=')
        {
            break;
        }
        else if(!isalnum(str[pos]) && str[pos] != '+' && str[pos] != '-' && str[pos] != '*' && str[pos] != '.')
        {
            valid = false;
            return;
        }
        if(pos == str.size() - 1)
        {
            valid = false;
            return;
        }
    }
    double base = 1.0;
    bool flag = true;
    i = str[pos + 1] == '-' ? pos + 2 : pos + 1;
    flag = str[pos + 1] == '-' ? false : true;
    if(i == str.size())
    {
        valid = false;
        return;
    }
    for(; i < str.size(); i++)
    {
        if(!isdigit(str[i]) && str[i] != '.')
        {
            valid = false;
            return;
        }
        else if(str[i] == '.')
        {
            if(base == 1.0)
            {
                base *= 0.1;  
            }
            else
            {
                valid = false;
                return;
            }
        }
        else
        {
            int digit = str[i] - '0';
            if(base == 1.0)
            {
                res = res * 10 + digit;
            }
            else
            {
                res += digit * base;
                base *= 0.1;
            }
        }
    }
    res = flag ? res : -res;
    i = 0;
    flag = true;
    double coeff = 0.0;
    char ch;
    int suffix = 0;
    base = 1.0;
    bool tmp = true;
    if(str[i] == '-')
    {
        flag = false;
        ++i;
    }
    for(; i < pos; i++)
    {
        if(isdigit(str[i]) && tmp)
        {
            int num = str[i] - '0';
            if(base == 1.0)
            {
                coeff = coeff * 10 + num;
            }
            else
            {
                coeff += num * base;
                base *= 0.1;
            }
        }
        else if(str[i] == '.' && tmp)
        {
            if(base == 1.0)
            {
                base *= 0.1;
            }
            else
            {
                valid = false;
                return;
            }
        }

        else if(str[i] == '*' && tmp)
        {
            tmp = false;
        }
        else if(isalpha(str[i]))
        {
            tmp = false;
            if(i == 0 || str[i - 1] == '+' || str[i - 1] == '-')
            {
                coeff = 1.0;
            }
            ch = str[i];
        }
        else if(isdigit(str[i]))
        {
            suffix = suffix * 10 + str[i] - '0';
            coeff = flag ? coeff : -coeff;
        }
        else if(str[i] == '+' || str[i] == '-')
        {
            veriable.push_back(make_pair(make_pair(ch, suffix), coeff));
            tmp = true;
            if(str[i] == '-')
            {
                flag = false;
            }
            else
            {
                flag = true;
            }
            coeff = 0.0;
            base = 1.0;
            suffix = 0;

        }
        else
        {
            valid = false;
            return;
        }  
    }
    veriable.push_back(make_pair(make_pair(ch, suffix), coeff));
    sort(veriable.begin(), veriable.end(), [](pair<pair<char, int>, double> a, pair<pair<char, int>, double> b)
    {
        if(a.first.first == b.first.first)
        {
            return a.first.second < b.first.second;
        }
        else
        {
            return a.first.first < b.first.first;
        }
    }
    );
}




class Equations
{
private:
    stringstream strInput;
    stringstream strOutput;
    vector<formulation> form;
    vector<string> legal_form;
    vector<string> illegal_form;

    vector<pair<char, int>> veri_list;
    vector<double> res_list;
    double **array;

    int error = 0;
    bool flag = true;

public:
    Equations();
    ~Equations();

    void sort_list(int i)
    {
        if(array[i][i] != 0)
            return;
        for(int j = i + 1; j < form.size(); ++j)
        {
            if(array[j][i])
            {
                swap(array[i], array[j]);
                return;
            }
        }
    }

    void remove_list(int i)
    {
        if(array[i][i] == 0)
        {
            flag = false;
            return;
        }
        double ratio = array[i][i];
        for(int j = i; j <= veri_list.size(); j++)
            array[i][j] /= ratio;
        for(int j = i + 1; j < form.size(); j++)
        {
            ratio = array[j][i];
            if(ratio == 0)
                continue;
            for(int k = i; k <= veri_list.size(); k++)
            {
                array[j][k] -= ratio * array[i][k];
            }
        }
    }

    void solve_equations()
    {
        for(int i = 0; i < veri_list.size(); ++i)
        {
            res_list.push_back(0.0);
        }
        for(int i = form.size() - 1; i >= 0; --i)
        {
            if(array[i][i] == 0 && array[i][veri_list.size()] != 0)
            {
                error = 1;
                return;
            }
            else if(array[i][i] == 0 && array[i][veri_list.size()] == 0)
                continue;
            double ratio = array[i][i];
            for(int j = i; j <= veri_list.size(); ++j)
            {
                array[i][j] /= ratio;
            }
            for(int j = i + 1; j < veri_list.size(); j++)
            {
                if(array[i][j] != 0)
                {
                    error = 2;
                }
            }
            res_list[i] = array[i][veri_list.size()];
            for(int j = i - 1; j >= 0; --j)
            {
                ratio = array[j][i];
                for(int k = i; k <= veri_list.size(); ++k)
                {
                    array[j][k] -= ratio * array[i][k];
                }
            }
        }
    }

    void print_res()
    {
        strOutput << "Legal Equations:\n";
        for(int i = 0; i < legal_form.size(); ++i)
        {
            strOutput << legal_form[i] << endl;
        }
        switch(error)
        {
            case 0:
                for(int i = 0; i < veri_list.size(); ++i)
                {
                    strOutput << veri_list[i].first << veri_list[i].second << ":" << init_numbers(res_list[i]) << endl;
                }
                break;
            case 1:
                strOutput << "error1\n";
                break;
            case 2:
                strOutput << "error2\n";
                break;
        }
        strOutput << "Illegal Equations:";
        for(int i = 0; i < illegal_form.size(); ++i)
        {
            strOutput << '\n' << illegal_form[i];
        }
        cout << strInput.str().length() - 1 << ":" << strOutput.str().length() << '\n' << strOutput.str();
    }
};

Equations::Equations()
{
    string s;
    while(cin >> s)
    {
        strInput << s << endl;
        formulation f(s);
        if(f.is_valid())
        {
            form.push_back(f);
            legal_form.push_back(s);
            for(int i = 0; i < f.veriable.size(); i++)
            {
                veri_list.push_back(f.veriable[i].first);
            }
        }
        else
        {
            illegal_form.push_back(s);
        }
    }
    sort(veri_list.begin(), veri_list.end(), [](pair<char, int> a, pair<char, int> b) 
    {
        if(a.first == b.first) 
            return a.second < b.second;
        else 
            return a.first < b.first; 
    }
    );
    auto it = unique(veri_list.begin(), veri_list.end());
    veri_list.erase(it, veri_list.end());

    array = new double*[form.size()];
    for(int i = 0; i < form.size(); ++i)
    {
        array[i] = new double[veri_list.size() + 1];
        int k = 0;
        for(int j = 0; j < form[i].veriable.size(); ++j)
        {
            while(k < veri_list.size() && veri_list[k] != form[i].veriable[j].first)
            {
                ++k;
            }
            array[i][k] = form[i].veriable[j].second;
        }
        array[i][veri_list.size()] = form[i].res;
    }
    for(int i = 0; i < form.size(); i++)
    {
        sort_list(i);
        remove_list(i);
    }
    solve_equations();
    print_res();
}

Equations::~Equations()
{
    for(int i = 0; i < form.size(); ++i)
    {
        delete [] array[i];
    }
    delete [] array;
}


int main()
{
    Equations Equation;
    return 0;
}