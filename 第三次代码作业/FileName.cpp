
template struct BinNode {
	T data;
	BinNode* parent, * lc, * rc; // 父亲，左孩子和右孩子
	int height; // 高度
	int npl; // 非严格意义下的左式堆
	BinNode(T const& e, BinNode* p = nullptr, BinNode* lc = nullptr, BinNode* rc = nullptr, int h = 0, int l = 1)
		: data(e), parent(p), lc(lc), rc(rc), height(h), npl(l) {}
	int size() { // 当前节点后代总数，亦即以其为根的子树大小
		int s = 1; // 计入本身
		if (lc) s += lc->size(); // 递归计入左子树大小
		if (rc) s += rc->size(); // 递归计入右子树大小
		return s;
	}
	BinNode* insertAsLC(T const& e) { return lc = new BinNode(e, this); }
	BinNode* insertAsRC(T const& e) { return rc = new BinNode(e, this); }

	BinNode* succ() { // 找到节点的直接后继
		BinNode* s = this;
		if (rc) { // 若有右孩子，则直接后继必在右子树中
			s = rc;
			while (HasLChild(*s)) s = s->lc;
		}
		else { // 否则，直接后继在祖先中
			while (IsRChild(*s)) s = s->parent;
			s = s->parent;
		}
		return s;
	}

	template void travLevel(VST& visit) { // 子孙中一层一层地找
		Queue* > Q;
		Q.enqueue(this);
		while (!Q.empty()) {
			BinNode* x = Q.dequeue();
			visit(x->data);
			if (x->lc) Q.enqueue(x->lc);
			if (x->rc) Q.enqueue(x->rc);
		}
	}
};

template using BinNodePosi = BinNode*;

template class BinTree {
protected:
	int _size; // 规模
	BinNodePosi _root; // 根节点

	virtual int updateHeight(BinNodePosi x) { // 更新节点高度
		return x->height = 1 + max(stature(x->lc), stature(x->rc));
	}
	void updateHeightAbove(BinNodePosi x) { // 更新节点及其祖先高度
		while (x) {
			updateHeight(x);
			x = x->parent;
		}
	}
public:
	BinTree() : _size(0), _root(nullptr) {}
	~BinTree() { if (_size > 0) remove(_root); } // 析构函数：递归释放每个节点
	int size() const { return _size; }
	bool empty() const { return !_root; } // 判空
	BinNodePosi root() const { return _root; } // 树根
	BinNodePosi insertAsRoot(T const& e) { _size = 1; return _root = new BinNode(e); } // 插入根节点
	BinNodePosi insertAsLC(BinNodePosi x, T const& e) { // x 身为左孩子，插入 e 作为其左孩子
		_size++; x->insertAsLC(e); updateHeightAbove(x);
		return x->lc;
	}
	BinNodePosi insertAsRC(BinNodePosi x, T const& e) { // x 身为右孩子，插入 e 作为其右孩子
		_size++; x->insertAsRC(e); updateHeightAbove(x);
		return x->rc;
	}
	BinNodePosi attachAsLC(BinNodePosi x, BinTree*& S) { // T 作为 x 左子树接入
		if (x->lc = S->_root) x->lc->parent = x;
		_size += S->_size; updateHeightAbove(x);
		S->_root = nullptr; S->_size = 0; release(S); S = nullptr; // 释放原树
		return x;
	}
	BinNodePosi attachAsRC(BinNodePosi x, BinTree*& S) { // T 作为 x 右子树接入
		if (x->rc = S->_root) x->rc->parent = x;
		_size += S->_size; updateHeightAbove(x);
		S->_root = nullptr; S->_size = 0; release(S); S = nullptr; // 释放原树
		return x;
	}
	int remove(BinNodePosi x) { // 删除以位置 x 处节点为根的子树，返回该子树原先的规模
		FromParentTo(*x) = nullptr; updateHeightAbove(x->parent); // 记录父节点指针，注意要用 FromParentTo 宏定义
		int n = removeAt(x); _size -= n; return n;
	}
	static int removeAt(BinNodePosi x) { // 删除位置 x 处的节点，返回被删除节点数目（0 或 1）
		if (!x) return 0;
		int n = 1 + removeAt(x->lc) + removeAt(x->rc); // 递归删除左右子树
		release(x->data); release(x); return n; // 释放被删除节点，并返回数目
	}

	template void travLevel(VST& visit) { if (_root) _root->travLevel(visit); } // 层次遍历

	template void travPre(VST& visit) { if (_root) _root->travPre(visit); } // 前序遍历

	template void travIn(VST& visit) { if (_root) _root->travIn(visit); } // 中序遍历

	template void travPost(VST& visit) { if (_root) _root->travPost(visit); } // 后序遍历

	bool operator<(BinTree const& t) { return _root && t._root && lt(_root, t._root); } // 比较器（其余自行补充）
	bool operator==(BinTree const& t) { return _root && t._root && (_root == t._root); }
};
```

2. 基于 BinTree 构建 HuffTree

先定义一个结构体用于储存字符和对应的权值：

struct CharFreq {
	char ch;
	int freq;
	CharFreq(char c = '\0', int f = 0) : ch(c), freq(f) {}
	bool operator<(CharFreq const& cf) const { return freq < cf.freq; }
	bool operator==(CharFreq const& cf) const { return freq == cf.freq; }
	bool operator>(CharFreq const& cf) const { return freq > cf.freq; }
};
```

然后定义 Huffman 树并继承自 BinTree，实现 Huffman 编码算法：

template
class HuffTree : public BinTree {
protected:
	typedef BinNodePosi HuffNodePosi;
	static void generateCT(Bitmap* code, int length, HuffNodePosi v) { // 在 Huffman 树中从 v 处到根节点路径上的所有节点所处的编码，记录在 Bitmap 中并返回
		if (IsRoot(*v)) return;
		generateCT(code, length + 1, v->parent);
		(IsLChild(*v)) ? code->clear(length) : code->set(length); // 左分支为 0，右分支为 1
	}

public:
	HuffTree(List& freqList) { // 构造函数
		PriorityQueue Q;
		for (int i = 0; i < freqList.size(); i++) {
			CharFreq cf = freqList[i];
			BinNodePosi x = new BinNode(cf);
			this->_size++;
			Q.insert(x); // 将森林的每一个树都做为果实插入优先队列
		}
		while (Q.size() > 1) { // 不断地选出频率最小的两个节点合并，直至只剩一棵树
			HuffNodePosi l = Q.delMax();
			HuffNodePosi r = Q.delMax();
			HuffNodePosi u = new BinNode(CharFreq('\0', l->data.freq + r->data.freq), nullptr, l, r);
			this->_size++;
			l->parent = u;
			r->parent = u;
			Q.insert(u);
		}
		this->_root = Q.delMax(); // 队列中唯一剩余的节点即为根
		generateCT(new Bitmap, 0, this->_root);
	}
	~HuffTree() { release(this->_root); } // 析构函数（递归删除各节点）
	void generateCT(Bitmap* code[]) { // 生成编码（与下面的函数二选一，由于位图很难和 LaTeX 兼容，这里不再输出）
		generateCT(new Bitmap, 0, this->_root);
	}
	void generateCT(HufCode& code) { // 生成编码
		code.resize(256); // 初始化为所有 ASCII 码字符，初始编码长度为 0
		generateCT(&code, 0, this->_root);
	}
	void decode(HufCode const& code, ostream& os) const { // 解码
		HuffNodePosi v = this->_root; // 暂存根节点
		for (int i = 0; i < code.size(); i++) {
			v = code[i] ? v->rc : v->lc; // 根据编码分支方向选取子节点
			if (IsLeaf(*v)) { // 若当前节点为叶节点，则输出其字符，并重置当前节点为根
				os << v->data.ch;
				v = this->_root;
			}
		}
	}
};
```

3. 借助位图类 Bitmap 定义 Huffman 二进制编码串类型 HufCode

class Bitmap { // 按比特进行存储的二进制位图
private:
	char* M;
	int _size;

	void expand(int n) { // 注意这里的实现需要省略
	}

public:
	Bitmap(int n = 8) { M = new char[_size = (n + 7) / 8]; memset(M, 0, _size); }
	Bitmap(char const* file, int n = 8) { // 从指定文件中读取比特图（由于篇幅限制，这里不再给出）
	}
	~Bitmap() { delete[] M; M = nullptr; }
	void set(int i) { expand(i); M[i >> 3] |= (0x80 >> (i & 0x07)); } // 将第 i 位设为 1
	void clear(int i) { expand(i); M[i >> 3] &= ~(0x80 >> (i & 0x07)); } // 将第 i 位设为 0
	bool test(int i) { expand(i); return M[i >> 3] & (0x80 >> (i & 0x07)); } // 测试第 i 位是否为 1
	int size() { return _size * 8; }
	char* bits2string(int n) { // 比特串转化为字符串（同样由于篇幅限制，这里不再给出）
	}
};

typedef Bitmap* HufCode;
```

4. 实现 Huffman 编码算法

void encodeHuff(char const* file) { // Huffman 编码算法
	List freqList = statistics(file); // 字符频度统计
	release(file);
	HuffTree* huffTree = new HuffTree(freqList); // 基于字符频度构建 Huffman 树
	release(freqList);
	HufCode code = new Bitmap; // 分配编码存储空间
	huffTree->generateCT(*code); // 生成 Huffman 编码
	FILE* fout = fopen("output.huff", "wb"); // 输出压缩文件
	fwrite(code, sizeof(Bitmap), 1, fout); // 先将编码存储空间写入文件，便于解压时读入
	fwrite(&huffTree->_size, sizeof(int), 1, fout); // 写入字符总数
	huffTree->encode(file, fout); // 写入编码串
	fclose(fout);
	release(huffTree); // 删除 Huffman 树及其生成的编码