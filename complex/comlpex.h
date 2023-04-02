#pragma once
typedef int Rank; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）​
template <typename T> class Vector { //向量模板类
protected:
	void Shuffle(std::vector<Complex>& vec);
	int Find(const std::vector<Complex>& vec, const Complex& c);
	void Insert(std::vector<Complex>& vec, const Complex& c);
	bool Delete(std::vector<Complex>& vec, const Complex& c);
	void Unique(std::vector<Complex>& vec);
	void BubbleSort(std::vector<Complex>& vec);
	void MergeSort(std::vector<Complex>& vec, int start, int end);
		int mid = start + (end - start) / 2;
	std::vector<Complex> tmp(end - start + 1);
	int i = start, j = mid + 1, k = 0;
	bool IsSorted(const std::vector<Complex>& vec);
	void TestSort(std::vector<Complex>& vec);
		std::vector<Complex> vec_bubble(vec);






}
;
