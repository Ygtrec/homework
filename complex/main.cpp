int main() {
    std::vector<Complex> vec = { Complex(1, 2), Complex(-1, 2), Complex(0, 1),
                                Complex(1, 2), Complex(2, -1), Complex(0, 1) };
    std::cout << "Original Vector: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    Shuffle(vec);
    std::cout << "Shuffled Vector: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    Complex c1(1, 2);
    int pos = Find(vec, c1);
    std::cout << "Find " << c1 << " at position " << pos << std::endl;

    Complex c2(0, 1);
    Insert(vec, c2);
    std::cout << "After Insert " << c2 << ": ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    Complex c3(1, 2);
    bool deleted = Delete(vec, c3);
    std::cout << "After Delete " << c3 << ": ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    Unique(vec);
    std::cout << "After Unique: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}
void BubbleSort(std::vector<Complex>& vec) {
    int n = vec.size();
    for (int i = 0; i < n; ++i) {
        bool sorted = true;
        for (int j = 0; j < n - i - 1; ++j) {
            double m1 = std::hypot(vec[j].real_, vec[j].imag_);
            double m2 = std::hypot(vec[j+1].real_, vec[j+1].imag_);
            if ((m1 > m2) || ((m1 == m2) && (vec[j].real_ > vec[j+1].real_))) {
                std::swap(vec[j], vec[j+1]);
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
    MergeSort(vec, mid+1, end);

    std::vector<Complex> tmp(end - start + 1);
    int i = start, j = mid+1, k = 0;
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
        double m1 = std::hypot(vec[i-1].real_, vec[i-1].imag_);
        double m2 = std::hypot(vec[i].real_, vec[i].imag_);
        if ((m1 > m2) || ((m1 == m2) && (vec[i-1].real_ > vec[i].real_))) {
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



int main() {
    std::vector<Complex> vec = { Complex(1, 2), Complex(-1, 2), Complex(0, 1),
                                Complex(1, 2), Complex(2, -1), Complex(0, 1) };
    TestSort(vec);

    srand(time(nullptr));
    std::vector<Complex> vec2;
    for (int i = 0; i < 10000; ++i) {
        double real = rand() / double(RAND_MAX);
        double imag = rand() / double(RAND_MAX);
        vec2.push_back(Complex(real, imag));
    }
    TestSort(vec2);

    std::reverse(vec2.begin(), vec2.end());
    TestSort(vec2);

    return 0;
}


int main() {
    std::vector<Complex> vec = { Complex(1, 2), Complex(-1, 2), Complex(0, 1), Complex(2, 0),
                                Complex(1, 1), Complex(-2, -1), Complex(-1, 0), Complex(0, -1) };
    TestFindInRange(vec, 0, 1);
    TestFindInRange(vec, 1, 2);
    TestFindInRange(vec, 2, 3);

    return 0;
}