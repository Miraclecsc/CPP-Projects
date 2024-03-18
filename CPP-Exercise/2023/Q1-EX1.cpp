#include "std_lib_facilities.h"
#include <string>
#include <map>
#include <vector>
using namespace std;
class filename
{
private:
    string name;
    string begin;
    int version;
    string end;
    string flag;

public:
    filename() : name(), begin(), version(-1), end(), flag() {}
    filename(string s);

    string get_name() const {return name;}
    string get_begin() const {return begin;}
    int get_version() const {return version;}
    string get_end() const {return end;}
    string get_flag() const {return flag;}

    void debug()
    {
        cout << "name: " << name << endl;
        cout << "begin: " << begin << endl;
        cout << "version: " << version << endl;
        cout << "end: " << end << endl;
        cout << "flag: " << flag << endl;
    }

    ~filename() = default;
};

filename::filename(string s) : name(s), begin(), version(-1), end(), flag()
{
    int pos = -1;
    for(int i = name.size() - 1; i >= 0; --i)
    {
        if(name[i] == '.')
        {
            end = name.substr(i);
            pos = i;
            break;
        }
    }
    if(pos == -1)
    {
        begin = name;
        end = "";
    }
    else
    {
        int pos2 = pos;
        while(pos2 - 1 >= 1 && isdigit(name[pos2 - 1]))
            --pos2;
        if(pos2 - 7 >= 1 && name.substr(pos2 - 7, 7) == "_副本")
        {
            begin = name.substr(0, pos2 - 7);
            if(pos2 != pos)
                version = atoi(name.substr(pos2, pos - pos2 - 3).c_str());
            else
                version = 0;
        }
        else
            begin = name.substr(0, pos);
    }
    flag = begin + end;
}

int main()
{
    string s1, s2;
    getline(cin, s1);
    getline(cin, s2);
    stringstream ss1(s1), ss2(s2);

    vector<filename> v1, v2;
    string temp;
    while(ss1 >> temp)
        v1.push_back(filename(temp));

    while(ss2 >> temp)
        v2.push_back(filename(temp));

    sort(v1.begin(), v1.end(), [](const filename& f1, const filename& f2){return f1.get_name() < f2.get_name();});

    map<string, vector<int>> file_table;
    map<string, int> max_version;

    for(int i = 0; i < v1.size(); i++)
    {
        if(file_table.find(v1[i].get_flag()) == file_table.end())
        {
            vector<int> temp;
            temp.push_back(v1[i].get_version());
            file_table[v1[i].get_flag()] = temp;
        }
        else
        {
            file_table[v1[i].get_flag()].push_back(v1[i].get_version());
        }

        if(max_version.find(v1[i].get_flag()) == max_version.end())
            max_version[v1[i].get_flag()] = v1[i].get_version();
        else
        {
            if(max_version[v1[i].get_flag()] < v1[i].get_version())
                max_version[v1[i].get_flag()] = v1[i].get_version();
        }
    }

    for(auto iter = v2.begin(); iter != v2.end(); ++iter)
    {
        if(file_table.find(iter->get_flag()) == file_table.end())
            continue;
        auto file = file_table[iter->get_flag()];
        if(find(file.begin(), file.end(), iter->get_version()) == file.end())
            continue;
        file_table[iter->get_flag()].push_back(++max_version[iter->get_flag()]);
    }

    vector<string> result;
    for(auto iter = file_table.begin(); iter != file_table.end(); ++iter)
    {
        for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2)
        {
            string mid = "";
            if(*iter2 == 0)
                mid = "_副本";
            else if(*iter2 > 0)
                mid = "_副本" + to_string(*iter2);

            int pos = iter->first.find_last_of('.');
            if(pos > 1000)
                //cout << iter->first << mid << endl;
                result.push_back(iter->first + mid);
            else
                //cout << iter->first.substr(0, pos) << mid << iter->first.substr(pos) << endl;
                result.push_back(iter->first.substr(0, pos) + mid + iter->first.substr(pos));
        }
    }
    sort(result.begin(), result.end());
    for(auto iter = result.begin(); iter != result.end(); ++iter)
        cout << *iter << endl;
    return 0;

}
