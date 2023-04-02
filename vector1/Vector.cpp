#include<iostream>
#include"Vector.h"
template <typename T> //元素类型
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) { //以数组区间A[lo, hi)为蓝本复制向量
    _elem = new T[_capacity = 2 * (hi - lo)]; _size = 0; //分配空间，规模清零
    while (lo < hi) //A[lo, hi)内的元素逐一
        _elem[_size++] = A[lo++]; //复制至_elem[0, hi - lo)
}

template <typename T> Vector<T>& Vector<T>::operator= (Vector<T> const& V) { //深复制
    if (_elem) delete[] _elem; //释放原有内容
    copyFrom(V._elem, 0, V.size()); //整体复制
    return *this; //返回当前对象的引用，以便链式赋值
}
template <typename T> void Vector<T>::expand() { //向量空间不足时扩容
    if (_size < _capacity) return; //尚未满员时，不必扩容
    if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY; //不低于最小容量
    T* oldElem = _elem;  _elem = new T[_capacity <<= 1]; //容量加倍
    for (int i = 0; i < _size; i++)
        _elem[i] = oldElem[i]; //复制原向量内容（T为基本类型，或已重载赋值操作符'='）
    ​
        delete[] oldElem; //释放原空间
}
template <typename T> void Vector<T>::shrink() { //装填因子过小时压缩向量所占空间
    if (_capacity < DEFAULT_CAPACITY << 1) return; //不致收缩到DEFAULT_CAPACITY以下
    if (_size << 2 > _capacity) return; //以25%为界
    T* oldElem = _elem;  _elem = new T[_capacity >>= 1]; //容量减半
    for (int i = 0; i < _size; i++) _elem[i] = oldElem[i]; //复制原向量内容
    delete[] oldElem; //释放原空间
}

template<typename T>
void merge_sort(T arr[], int len) {
    T* a = arr;
    T* b = new T[len];
    for (int seg = 1; seg < len; seg += seg) {
        for (int start = 0; start < len; start += seg + seg) {
            int low = start, mid = min(start + seg, len), high = min(start + seg + seg, len);
            int k = low;
            int start1 = low, end1 = mid;
            int start2 = mid, end2 = high;
            while (start1 < end1 && start2 < end2)
                b[k++] = a[start1] < a[start2] ? a[start1++] : a[start2++];
            while (start1 < end1)
                b[k++] = a[start1++];
            while (start2 < end2)
                b[k++] = a[start2++];
        }
        T* temp = a;
        a = b;
        b = temp;
    }

    if (a != arr) {
        for (int i = 0; i < len; i++)
            b[i] = a[i];
        b = a;
    }

    delete[] b;
}
template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) {
    //等概率随机置乱区间[lo, hi)
    T* V = _elem + lo; //将子向量_elem[lo, hi)视作另一向量V[0, hi - lo)
    for (Rank i = hi - lo; i > 0; i--) //自后向前
        swap(V[i - 1], V[rand() % i]); //将V[i - 1]与V[0, i)中某一元素随机交换
}
template <typename T> static bool lt(T* a, T* b) { return lt(*a, *b); } //less than
template <typename T> static bool lt(T& a, T& b) { return a < b; } //less than
template <typename T> static bool eq(T* a, T* b) { return eq(*a, *b); } //equal
template <typename T> static bool eq(T& a, T& b) { return a == b; } //equal
template <typename T> //无序向量的顺序查找：返回最后一个元素e的位置；失败时，返回lo - 1
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
    //assert: 0 <= lo < hi <= _size
    while ((lo < hi--) && (e != _elem[hi])); //从后向前，顺序查找
    return hi; //若hi < lo，则意味着失败；否则hi即命中元素的秩
}
//这里的技巧是采用了自后向前，只要第一个满足就可退出
template <typename T> //将e作为秩为r元素插入
Rank Vector<T>::insert(Rank r, T const& e) { //assert: 0 <= r <= size
    expand(); //若有必要，扩容
    for (int i = _size; i > r; i--)
        _elem[i] = _elem[i - 1]; //自后向前，后继元素顺次后移一个单元
    _elem[r] = e; _size++; //置入新元素并更新容量
    return r; //返回秩
}
/**
 * 构造出 [begin, end) 范围的轴点元素
 * @return 轴点元素的最终位置
 */
template<typename T>
bool Vector<T>::bubble(Rank lo, Rank hi) {//扫描交换
    bool sorted = true;
    while (++lo < hi)
        if (_elem[lo - 1] > _elem[lo]) {
            sorted = false;
            swap(_elem[lo - 1], _elem[lo]);
        }
    return sorted;
}
template<typename T>//起泡排序
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
    while (!bubble(lo, hi--));
}
template<typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
    if (hi - lo < 2) return;
    int mi = (lo + hi) / 2;
    mergeSort(lo, mi);
    mergeSort(mi, hi);
    merge(lo, mi, hi);
}
template<typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
    if (hi - lo < 2) return;
    int mi = (lo + hi) / 2;
    mergeSort(lo, mi);
    mergeSort(mi, hi);
    merge(lo, mi, hi);
}
template <typename T> Rank Vector<T>::remove(Rank lo, Rank hi) { //0 <= lo <= hi <= n
    if (lo == hi) return 0; //出于效率考虑，单独处理退化情况
    while (hi < _size) _elem[lo++] = _elem[hi++]; //后缀[hi, _size)顺次前移hi-lo位
    _size = lo; shrink(); //更新规模，lo=_size之后的内容无需清零；如必要，则缩容
    //若有必要，则缩容
    return hi - lo; //返回被删除元素的数目

}template < typename T> //将e插入至[r]
Rank Vector<T>::insert(Rank r, T const& e) { //0 <= r <= size
    expand(); //如必要，先扩容
    for (Rank i = _size; r < i; i--) //自后向前，后继元素
        _elem[i] = _elem[i - 1]; //顺次后移一个单元
    _elem[r] = e; _size++; //置入新元素并更新容量
    return r; //返回秩

}

template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) { //随机置乱区间[lo, hi)
    T* V = _elem + lo; //将子向量_elem[lo, hi)视作另一向量V[0, hi - lo)
    for (Rank i = hi - lo; 1 < i; --i) //自后向前
        swap(V[i - 1], V[rand() % i]); //将V[i - 1]与V[0, i)中某一元素随机交换

}




template <typename T> Rank Vector<T>::uniquify() { //有序向量重复元素剔除算法（高效版）
    Rank i = 0, j = 0; //各对互异“相邻”元素的秩
    while (++j < _size) //逐一扫描，直至末元素
        if (_elem[i] != _elem[j]) //跳过雷同者
            _elem[++i] = _elem[j]; //发现不同元素时，向前移至紧邻于前者右侧
    _size = ++i; shrink(); //直接截除尾部多余元素
    return j - i; //向量规模变化量，即被删除元素总数

}

template<typename T>
Rank Vector<T>::deduplicate() {
    int oldSize = _size;
    Rank i = 0;
    while (i < _size - 1) {
        Rank j;
        j = i + 1;
        while (j < _size) {
            if ((_elem[i].getreal() == _elem[j].getreal()) && (_elem[i].getimag() == _elem[j].getimag())) {
                remove(j, j + 1);
            }
            else {
                j++;
            }
        }
        i++;
    }
    return oldSize - _size; //被删除元素总数
}

template <typename T> void Vector<T>::traverse(void (*visit)(T&)) //利用函数指针机制的遍历
{
    for (int i = 0; i < _size; i++) visit(_elem[i]);
}

template <typename T> template < typename VST> //元素类型、操作器
void Vector<T>::traverse(VST& visit) //借助函数对象机制
{
    for (Rank i = 0; i < _size; i++) visit(_elem[i]);
}
