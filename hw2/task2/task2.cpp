#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include"../../../vector/stack.h"
using namespace std;
// 定义队列数据结构
class Queue {
private:
    vector<int> data;
    int p_start;
public:
    Queue() { p_start = 0; }
    bool empty() { return p_start >= data.size(); }
    int front() { return data[p_start]; }
    void push(int val) { data.push_back(val); }
    void pop() { p_start++; }
};
// 计算矩形最大面积的函数
int largestRectangleArea(vector<int>& heights) {
    int n = heights.size();
    if (n == 0) return 0;
    if (n == 1) return heights[0];
    // 定义左右指针和最大面积
    int left, right, area = 0;
    // 定义单调递增栈
    stack<int> s;
    // 遍历每个柱子
    for (int i = 0; i < n; i++) {
        while (!s.empty() && heights[s.top()] > heights[i]) {
            // 如果栈顶元素大于当前元素，则弹出栈顶元素并计算面积
            int h = heights[s.top()];
            s.pop();
            left = s.empty() ? -1 : s.top();
            right = i;
            area = max(area, h * (right - left - 1));
        }
        s.push(i);
    }
    // 处理栈中剩余元素
    while (!s.empty()) {
        int h = heights[s.top()];
        s.pop();
        left = s.empty() ? -1 : s.top();
        right = n;
        area = max(area, h * (right - left - 1));
    }
    return area;
}

// 随机生成高度数组
vector<int> generateHeights(int n) {
    vector<int> heights(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 105);
    for (int i = 0; i < n; i++) {
        heights[i] = dis(gen);
    }
    return heights;
}

int main() {
    // 随机生成10组数据进行测试
    for (int i = 0; i < 10; i++) {
        vector<int> heights = generateHeights(rand() % 10 + 1);
        cout << "测试项目 " << i + 1 << ": " << endl;
        for (int j = 0; j < heights.size(); j++) {
            cout << heights[j] << " ";
        }
        cout << endl;
        int area = largestRectangleArea(heights);
        cout << "最大矩形面积: " << area << endl;
        cout << endl;

    }
    return 0;
}