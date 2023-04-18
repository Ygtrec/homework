#include "../../../vector/stack.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define N_OPTR 9 //运算符总数
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //运算符集合
//加、减、乘、除、乘方、阶乘、左括号、右括号、起始符与终止符

const char OPSET[N_OPTR] = { '+', '-', '*', '/', '^', '!', '(', ')', '\0' };
const char pri[N_OPTR][N_OPTR] = { //运算符优先等级 [栈顶] [当前]
    /*              |-------------------- 当 前 运 算 符 --------------------| */
    /*              +      -      *      /      ^      !      (      )      \0 */
    /* --  + */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
    /* |   - */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
    /* 栈  * */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
    /* 顶  / */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
    /* 运  ^ */    '>',   '>',   '>',   '>',   '>',   '<',   '<',   '>',   '>',
    /* 算  ! */    '>',   '>',   '>',   '>',   '>',   '>',   ' ',   '>',   '>',
    /* 符  ( */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   '=',   ' ',
    /* |   ) */    ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',
    /* -- \0 */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   ' ',   '='
};


void append(char*& rpn, float opnd)
{
    int n = strlen(rpn);
    char buf[64];
    if (opnd != (float)(int)opnd)
        sprintf(buf, "%.2f ", opnd);
    else
        sprintf(buf, "%d ", (int)opnd);
    rpn = (char*)realloc(rpn, sizeof(char) * (n + strlen(buf) + 1));
    strcat(rpn, buf);
}
void append(char*& rpn, char optr)
{
    int n = strlen(rpn);
    rpn = (char*)(rpn, sizeof(char) * (n + 3));
    sprintf(rpn + n, "%c ", optr);
    rpn[n + 2] = '\n';
}



float readNumber(char*& S, stack<float>& opnd) {
    float x = 0.0;
    bool point = false;//判断是否为小数
    int i = 0;//计小数点后的位数
    while (isdigit(*S) || *S == '.') {
        if (*S != '.') {
            if (point == false) 	x = x * 10 + (*(S++) - 48);//减去48,ASCII转成数字
            if (point == true) { x = x * 10 + (*(S++) - 48); i++; }
        }
        else { S++; point = true; }
    }
    int temp = 1;
    for (int j = 0; j < i; j++) temp *= 10;
    x = x / temp;
    opnd.push(x);//压入操作数栈
    return x;
}



char orderBetween(char& e, char& s)
{
    int i, j;
    for (i = 0; i < N_OPTR; ++i)
        if (e == OPSET[i])break;
    for (j = 0; j < N_OPTR; ++j)
        if (s == OPSET[j])break;
    return pri[i][j];
}

float calcu(float p1, char op, float p2 = 0)
{
    switch (op) {
    case '+':return p1 + p2;
    case '-':return p1 - p2;
    case '*':return p1 * p2;
    case '/':return p1 / p2;
    case '^': {
        float c = 1;
        while (p2--) c = c * p1;
        return c;
    }
    case '!': {
        if (p1 == 0)return 1;
        else return p1 * calcu(p1 - 1, '!');//递归求阶乘
    }
    }
}


float evaluate(char* S, char*& RPN)
{
    stack<float> opnd;
    stack<char> optr;
    optr.push('\0');
    while (!optr.empty())
    {
        if (isdigit(*S))
        {
            readNumber(S, opnd);
            append(RPN, opnd.top());
        }
        else
            switch (orderBetween(optr.top(), *S))
            {
            case '<':
                optr.push(*S);
                S++;
                break;
            case '=':
                optr.pop();
                S++;
                break;
            case '>':
            {
                char op = optr.pop();
                append(RPN, op);
                if (op == '!')
                {
                    float pOpnd = opnd.pop();
                    opnd.push(calcu(op, pOpnd));
                }
                else
                {
                    float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop();
                    opnd.push(calcu(pOpnd1, op, pOpnd2));
                }
                break;
            }
            default:
                exit(-1);
            }
    }
    return opnd.pop();
}


int main() {

    cout << "输入算式: ";
    char sr[100], * rpn = new char[100];
    cin >> sr;
    cout << evaluate(sr, rpn) << endl;
    delete[] rpn;
    
    return 0;
}
