#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>

class Complex {
private:
    double real_;
    double imag_;
public:
    Complex(double real = 0, double imag = 0) : real_(real), imag_(imag) {}

    double real() const { return real_; }
    double imag() const { return imag_; }

    friend bool operator==(const Complex& c1, const Complex& c2) {
        return (c1.real_ == c2.real_) && (c1.imag_ == c2.imag_);
    }

    friend Complex operator+(const Complex& c1, const Complex& c2) {
        return Complex(c1.real_ + c2.real_, c1.imag_ + c2.imag_);
    }

    friend Complex operator-(const Complex& c1, const Complex& c2) {
        return Complex(c1.real_ - c2.real_, c1.imag_ - c2.imag_);
    }

    friend Complex operator*(const Complex& c1, const Complex& c2) {
        return Complex(c1.real_ * c2.real_ - c1.imag_ * c2.imag_, c1.real_ * c2.imag_ + c1.imag_ * c2.real_);
    }

    friend Complex operator/(const Complex& c1, const Complex& c2) {
        double r = c2.real_ * c2.real_ + c2.imag_ * c2.imag_;
        return Complex((c1.real_ * c2.real_ + c1.imag_ * c2.imag_) / r, (c1.imag_ * c2.real_ - c1.real_ * c2.imag_) / r);
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.real_ << "+" << c.imag_ << "i";
        return os;
    }
};

void Shuffle(std::vector<Complex>& vec) {
    srand(time(nullptr));
    for (int i = vec.size() - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(vec[i], vec[j]);
    }
}

int Find(const std::vector<Complex>& vec, const Complex& c) {
    auto it = std::find(vec.begin(), vec.end(), c);
    return (it == vec.end()) ? -1 : std::distance(vec.begin(), it);
}

void Insert(std::vector<Complex>& vec, const Complex& c) {
    vec.push_back(c);
}

bool Delete(std::vector<Complex>& vec, const Complex& c) {
    int pos = Find(vec, c);
    if (pos != -1) {
        vec.erase(vec.begin() + pos);
        return true;
    }
    else {
        return false;
    }
}

void Unique(std::vector<Complex>& vec) {
    std::sort(vec.begin(), vec.end());
    auto it = std::unique(vec.begin(), vec.end());
    vec.erase(it, vec.end());
}
void BubbleSort(std::vector<Complex>& vec) {
    int n = vec.size();
    for (int i = 0; i < n; ++i) {
        bool sorted = true;
        for (int j = 0; j < n - i - 1; ++j) {
            double m1 = std::hypot(vec[j].real_, vec[j].imag_);
            double m2 = std::hypot(vec[j + 1].real_, vec[j + 1].imag_);
            if ((m1 > m2) || ((m1 == m2) && (vec[j].real_ > vec[j + 1].real_))) {
                std::swap(vec[j], vec[j + 1]);
                sorted = false;
            }
        }
        if (sorted) break;
    }
}

void MergeSort(std::vector<Complex>& vec, int start, int end) {
    if (start >= end) return;

    int mid = start + (end - start) / 2;
    MergeSort(vec, start, mid);
    MergeSort(vec, mid + 1, end);

    std::vector<Complex> tmp(end - start + 1);
    int i = start, j = mid + 1, k = 0;
    while (i <= mid && j <= end) {
        double m1 = std::hypot(vec[i].real_, vec[i].imag_);
        double m2 = std::hypot(vec[j].real_, vec[j].imag_);
        if ((m1 < m2) || ((m1 == m2) && (vec[i].real_ < vec[j].real_))) {
            tmp[k++] = vec[i++];
        }
        else {
            tmp[k++] = vec[j++];
        }
    }
    while (i <= mid) {
        tmp[k++] = vec[i++];
    }
    while (j <= end) {
        tmp[k++] = vec[j++];
    }
    std::copy(tmp.begin(), tmp.end(), vec.begin() + start);
}

bool IsSorted(const std::vector<Complex>& vec) {
    for (int i = 1; i < vec.size(); ++i) {
        double m1 = std::hypot(vec[i - 1].real_, vec[i - 1].imag_);
        double m2 = std::hypot(vec[i].real_, vec[i].imag_);
        if ((m1 > m2) || ((m1 == m2) && (vec[i - 1].real_ > vec[i].real_))) {
            return false;
        }
    }
    return true;
}

void TestSort(std::vector<Complex>& vec) {
    std::cout << "Original Vector: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    clock_t start, end;

    std::vector<Complex> vec_bubble(vec);
    start = clock();
    BubbleSort(vec_bubble);
    end = clock();
    std::cout << "BubbleSort: ";
    for (auto it = vec_bubble.begin(); it != vec_bubble.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "Time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << std::endl;

    std::vector<Complex> vec_merge(vec);
    start = clock();
    MergeSort(vec_merge, 0, vec_merge.size() - 1);
    end = clock();
    std::cout << "MergeSort: ";
    for (auto it = vec_merge.begin(); it != vec_merge.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "Time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << std::endl;

    std::cout << std::endl;
}
std::vector<Complex> FindInRange(const std::vector<Complex>& vec, double m1, double m2) {
    std::vector<Complex> result;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        double m = std::hypot(it->real_, it->imag_);
        if ((m >= m1) && (m < m2)) {
            result.push_back(*it);
        }
    }
    return result;
}

void TestFindInRange(const std::vector<Complex>& vec, double m1, double m2) {
    std::cout << "Original Vector: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::vector<Complex> vec_range = FindInRange(vec, m1, m2);
    std::cout << "Range [" << m1 << "," << m2 << "): ";
    for (auto it = vec_range.begin(); it != vec_range.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl << std::endl;
}