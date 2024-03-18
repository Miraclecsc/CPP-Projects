/*
编程要求

输入： 英文文本，单个测试用例字母数不超过1000个。
输出： 第一行是输入输出的总字符数（格式：输入字符数:输出字符数）；第二行编码后文本的位长；从第三行开始，每一行的格式为“字母 该字母的哈夫曼编码”（以空格分隔）。

注意：
（1）输出的字母按照字典序（ASCII码由小至大）排列。
（2）参考课件中哈夫曼编码构造方式，哈夫曼树左子节点对应编码为0，右子节点对应编码为1。
（3）哈夫曼树非叶节点对应的字母为“左子节点对应字母+右子节点对应字母”。如节点a与节点bc的父节点对应字母为abc。
（4）构建哈夫曼树时，取最小的两个频率对应的节点构成新树，若多个节点频率相同，则选择对应字典序较小字母的节点。
（5）构建哈夫曼树时，频率较小的节点为左子节点，频率较大的节点为右子节点。若两者频率相同，则字典序较小的字母对应节点为左子节点。

思考

（1）除了文本外，我们还可以输入图像、视频或者音频，事实上，Huffman编码被广泛应用于图像或者音频的压缩中。
（2）进一步了解什么叫无损压缩，什么叫有损压缩。

测试说明

平台会对你编写的代码进行测试：

测试输入：
am go hello
预期输出：
11:52
33
  101
a 1100
e 1101
g 1110
h 1111
l 00
m 100
o 01

开始你的任务吧，祝你成功！
*/
#include "std_lib_facilities.h"
#include <stack>
#include <queue>
using namespace std;

stringstream os;
int text[256] = {};
pair<char, string> ans[256] = {};

struct huffman_node
{
    int alpha;
    int num;
    bool is_leaf;
    bool is_flag;
    huffman_node* left;
    huffman_node* right;
    huffman_node* parent;
    string code = "";
    int length;
};

class cmp
{
public:
    bool operator()(const huffman_node* a, const huffman_node* b)
    {
        if(a->num == b->num)
        {
            return a->alpha > b->alpha;
        }
        return a->num > b->num;
    }
};
priority_queue<huffman_node*, vector<huffman_node*>, cmp> alpha_table;
stack<huffman_node*> dfs;

int main()
{
    string txt;
    getline(cin, txt);

    for(int i = 0; i < txt.size(); i++)
    {
        text[int(txt[i])]++;
    }
    for(int i = 0; i < 256; i++)
    {
        if(text[i] != 0)
        {
            huffman_node* node = new huffman_node;
            node->alpha = i;
            node->num = text[i];
            node->is_leaf = true;
            node->is_flag = false;
            node->left = NULL;
            node->right = NULL;
            node->parent = NULL;
            node->length = 0;
            alpha_table.push(node);
        }
    }
    huffman_node *root;
    while(alpha_table.size() != 1)
    {
        huffman_node* a = alpha_table.top();
        alpha_table.pop();
        huffman_node* b = alpha_table.top();
        alpha_table.pop();
        huffman_node* node = new huffman_node;
        node->alpha = a->alpha;
        node->num = a->num + b->num;
        node->is_leaf = false;
        node->is_flag = false;
        node->left = a;
        node->right = b;
        a->parent = node;
        b->parent = node;
        a->length = b->length = node->length + 1;
        alpha_table.push(node);
        root = node;
    }
    dfs.push(root);
    while(!dfs.empty())
    {
        huffman_node *p = dfs.top();
        dfs.pop();
        p->is_flag = true;
        p->left->code = p->code + "0";
        p->right->code = p->code + "1";
        if(p->left->is_leaf)
        {
            ans[p->left->alpha].first = char(p->left->alpha);
            ans[p->left->alpha].second = p->left->code;
        }
        else
        {
            dfs.push(p->left);
        }
        if(p->right->is_leaf)
        {
            ans[p->right->alpha].first = char(p->right->alpha);
            ans[p->right->alpha].second = p->right->code;
        }
        else
        {
            dfs.push(p->right);
        }
    }
    sort(ans, ans + 256, [](const pair<char, string>& a, const pair<char, string>& b)
    {
        return a.first < b.first;
    });
    int length = 0;
    for(int i = 0; i < 256; i++)
    {
        if(ans[i].first != 0)
        {
            length += ans[i].second.size() * text[int(ans[i].first)];
        }
    }
    os << length;
    for(int i = 0; i < 256; i++)
    {
        if(ans[i].first != 0)
        {
            os <<'\n' << ans[i].first << " " << ans[i].second;
        }
    }
    cout << txt.size() << ":" << os.str().size() << '\n' << os.str();
    while(!alpha_table.empty())
    {
        huffman_node* p = alpha_table.top();
        alpha_table.pop();
        delete p;
    }
    return 0;
}