#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

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

class Polynimial
{
private:
    bool valid = true;
    vector<pair<int, double>> terms;   
    void init_terms(const string &expression); 
    void sort_terms();
public:
    Polynimial() = default;
    Polynimial(const string &expression);
    Polynimial(const Polynimial &p);
    bool is_valid();
    bool is_zero();
    void print_poly();
    friend Polynimial operator+(const Polynimial &p1, const Polynimial &p2);
    friend Polynimial operator-(const Polynimial &p1, const Polynimial &p2);
    friend Polynimial operator*(const Polynimial &p1, const Polynimial &p2);
    friend pair<Polynimial, Polynimial> operator/(const Polynimial &p1, const Polynimial &p2);
};

void Polynimial::init_terms(const string &expression)
{
    terms.clear();
    int i = 0;
    double base = 1.0;
    bool flag = true;
    double a = 0.0;
    int exp = 0;

    if (expression[i] == '-')
    {
        flag = false;
        ++i;
    }
    for(; i < expression.length(); ++i)
    {
        if(isdigit(expression[i]))
        {
            int num = expression[i] - '0';
            if(base == 1.0)
            {
                a = a * 10 + num;
            }
            else
            {
                a += num * base;
                base /= 10;
            }
        }
        else if(expression[i] == '.')
        {
            if(base == 1.0)
            {
                base /= 10;
            }
            else
            {
                valid = false;
                return;
            }
        }
        else if(expression[i] == 'x')
        {
            if(i == 0 || expression[i - 1] == '+' || expression[i - 1] == '-')
            {
                a = 1.0;
            }
            if(i + 1 < expression.length() && expression[i + 1] == '^')
            {
                i++;
                if(i + 1 == expression.length() || !isdigit(expression[i + 1]))
                {
                    valid = false;
                    return;
                }
                
                while(i + 1 < expression.length() && isdigit(expression[i + 1]))
                {
                    exp = exp * 10 + expression[i + 1] - '0';
                    ++i;
                }
                if(i + 1 < expression.length() && expression[i + 1] != '+' && expression[i + 1] != '-')
                {
                    valid = false;
                    return;
                }
            }
            else
            {
                if(i + 1 < expression.length() && expression[i + 1] != '+' && expression[i + 1] != '-')
                {
                    valid = false;
                    return;
                }
                exp = 1;
            }
        }
        else if(expression[i] == '+' || expression[i] == '-')
        {
            if(i && !isdigit(expression[i - 1]) && expression[i - 1] != 'x')
            {
                valid = false;
                return;
            }
            a = flag ? a : -a;
            terms.emplace_back(make_pair(exp, a));
            flag = expression[i] == '+';
            a = 0.0;
            base = 1.0;
            exp = 0;
        }
        else
        {
            valid = false;
            return;
        }
    }
    a = flag ? a : -a;
    terms.emplace_back(make_pair(exp, a));
}


void Polynimial::sort_terms()
{
    sort(terms.begin(), terms.end(), [](pair<int, double> a, pair<int, double> b) { return a.first > b.first; });
        vector<pair<int, double>> temp;
        temp.clear();
        for (int i = 0; i < terms.size(); ++i)
        {
            int j = i;
            double coeff = terms[i].second;
            while (j + 1 < terms.size() && terms[j + 1].first == terms[i].first)
            {
                ++j;
                coeff += terms[j].second;
            }
            i = j;
            if (coeff < 1e-5 && coeff > -1e-5)
                continue;
            temp.emplace_back(terms[i].first, coeff);
        }
        terms.clear();
        terms = move(temp);
}

Polynimial::Polynimial(const string &expression)
{
    init_terms(expression);
    sort_terms();
}

Polynimial::Polynimial(const Polynimial &p)
{
    terms.clear();
    for(const auto &term: p.terms)
        terms.emplace_back(term);
}
Polynimial operator+(const Polynimial &p1, const Polynimial &p2)
{
    Polynimial sum;
    int i = p1.terms.size() - 1, j = p2.terms.size() - 1;
    while(i >= 0 && j >= 0)
    {
        if(p1.terms[i].first == p2.terms[j].first)
        {
            double coeff = p1.terms[i].second + p2.terms[j].second;
            sum.terms.emplace_back(make_pair(p1.terms[i].first, coeff));
            i--;
            j--;
        }
        else if(p1.terms[i].first < p2.terms[j].first)
        {
            sum.terms.emplace_back(make_pair(p1.terms[i].first, p1.terms[i].second));
            i--;
        }
        else
        {
            sum.terms.emplace_back(make_pair(p2.terms[j].first, p2.terms[j].second));
            j--;
        }
    }
    while(i >= 0)
    {
        sum.terms.emplace_back(make_pair(p1.terms[i].first, p1.terms[i].second));
        i--;
    }
    while(j >= 0)
    {
        sum.terms.emplace_back(make_pair(p2.terms[j].first, p2.terms[j].second));
        j--;
    }
    sum.sort_terms();
    return sum;
}

Polynimial operator-(const Polynimial &p1, const Polynimial &p2)
{
    Polynimial diff;
    int i = p1.terms.size() - 1, j = p2.terms.size() - 1;
    while(i >= 0 && j >= 0)
    {
        if(p1.terms[i].first == p2.terms[j].first)
        {
            double coeff = p1.terms[i].second - p2.terms[j].second;
            diff.terms.emplace_back(make_pair(p1.terms[i].first, coeff));
            i--;
            j--;
        }
        else if(p1.terms[i].first < p2.terms[j].first)
        {
            diff.terms.emplace_back(make_pair(p1.terms[i].first, p1.terms[i].second));
            i--;
        }
        else
        {
            diff.terms.emplace_back(make_pair(p2.terms[j].first, -p2.terms[j].second));
            j--;
        }
    }
    while(i >= 0)
    {
        diff.terms.emplace_back(make_pair(p1.terms[i].first, p1.terms[i].second));
        i--;
    }
    while(j >= 0)
    {
        diff.terms.emplace_back(make_pair(p2.terms[j].first, -p2.terms[j].second));
        j--;
    }
    diff.sort_terms();
    return diff;
}

Polynimial operator*(const Polynimial &p1, const Polynimial &p2)
{
    Polynimial prod;
    for(const auto &term1 : p1.terms)
    {
        for(const auto &term2 : p2.terms)
        {
            double coeff = term1.second * term2.second;
            int exp = term1.first + term2.first;
            prod.terms.emplace_back(make_pair(exp, coeff));
        }
    }
    prod.sort_terms();
    return prod;
}

pair<Polynimial, Polynimial> operator/(const Polynimial &p1, const Polynimial &p2)
{
    if(p1.terms[0].first < p2.terms[0].first)
    {
        return make_pair(Polynimial(), p1);
    }
    Polynimial quotient, remainder(p1);
    while(!remainder.terms.empty() && remainder.terms[0].first >= p2.terms[0].first)
    {
        int delta = remainder.terms[0].first - p2.terms[0].first;
        double r = remainder.terms[0].second / p2.terms[0].second;
        quotient.terms.emplace_back(make_pair(delta, r));
        for(const auto &term : p2.terms)
        {
            remainder.terms.emplace_back(make_pair(term.first + delta, term.second * (-r)));
        }
        remainder.sort_terms();
    }
    quotient.sort_terms();
    return make_pair(quotient, remainder);
}

bool Polynimial::is_valid()
{
    return valid;
}

bool Polynimial::is_zero()
{
    if(terms.empty())
        return true;
    return false;
}


void Polynimial::print_poly()
{
    if(terms.empty())
    {
        cout << "0.0000\n";
        return;
    }
    for(int i = 0; i < terms.size(); i++)
    {
        string s = init_numbers(terms[i].second);
        if(i && s[0] != '-')
        {
            s = "+" + s;
        }
        if(terms[i].first == 0)
            cout << s;
        else if(terms[i].first == 1)
            cout << s << "x";
        else
            cout << s << "x^" << terms[i].first;
    }
    cout << endl;
}

int main()
{
    string exp1, exp2;
    getline(cin, exp1);
    getline(cin, exp2);

    Polynimial p1(exp1), p2(exp2);

    if(!p1.is_valid() || !p2.is_valid())
    {
        if(!p1.is_valid())
            cout << "error\n";
        else
            p1.print_poly();
        if(!p2.is_valid())
            cout << "error\n";
        else
            p2.print_poly();
        cout << "error\nerror\nerror\nerror\nerror\n";
        return 0;
    }

    p1.print_poly();
    p2.print_poly();

    Polynimial add = p1 + p2;
    Polynimial sub = p1 - p2;
    Polynimial mul = p1 * p2;

    add.print_poly();
    sub.print_poly();
    mul.print_poly();

    if(p2.is_zero())
    {
        cout << "error\nerror\n";
        return 0;
    }    
    pair<Polynimial, Polynimial> div = p1 / p2;
    div.first.print_poly();
    div.second.print_poly();
    return 0;
}