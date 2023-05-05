
template struct BinNode {
	T data;
	BinNode* parent, * lc, * rc; // ���ף����Ӻ��Һ���
	int height; // �߶�
	int npl; // ���ϸ������µ���ʽ��
	BinNode(T const& e, BinNode* p = nullptr, BinNode* lc = nullptr, BinNode* rc = nullptr, int h = 0, int l = 1)
		: data(e), parent(p), lc(lc), rc(rc), height(h), npl(l) {}
	int size() { // ��ǰ�ڵ����������༴����Ϊ����������С
		int s = 1; // ���뱾��
		if (lc) s += lc->size(); // �ݹ������������С
		if (rc) s += rc->size(); // �ݹ������������С
		return s;
	}
	BinNode* insertAsLC(T const& e) { return lc = new BinNode(e, this); }
	BinNode* insertAsRC(T const& e) { return rc = new BinNode(e, this); }

	BinNode* succ() { // �ҵ��ڵ��ֱ�Ӻ��
		BinNode* s = this;
		if (rc) { // �����Һ��ӣ���ֱ�Ӻ�̱�����������
			s = rc;
			while (HasLChild(*s)) s = s->lc;
		}
		else { // ����ֱ�Ӻ����������
			while (IsRChild(*s)) s = s->parent;
			s = s->parent;
		}
		return s;
	}

	template void travLevel(VST& visit) { // ������һ��һ�����
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
	int _size; // ��ģ
	BinNodePosi _root; // ���ڵ�

	virtual int updateHeight(BinNodePosi x) { // ���½ڵ�߶�
		return x->height = 1 + max(stature(x->lc), stature(x->rc));
	}
	void updateHeightAbove(BinNodePosi x) { // ���½ڵ㼰�����ȸ߶�
		while (x) {
			updateHeight(x);
			x = x->parent;
		}
	}
public:
	BinTree() : _size(0), _root(nullptr) {}
	~BinTree() { if (_size > 0) remove(_root); } // �����������ݹ��ͷ�ÿ���ڵ�
	int size() const { return _size; }
	bool empty() const { return !_root; } // �п�
	BinNodePosi root() const { return _root; } // ����
	BinNodePosi insertAsRoot(T const& e) { _size = 1; return _root = new BinNode(e); } // ������ڵ�
	BinNodePosi insertAsLC(BinNodePosi x, T const& e) { // x ��Ϊ���ӣ����� e ��Ϊ������
		_size++; x->insertAsLC(e); updateHeightAbove(x);
		return x->lc;
	}
	BinNodePosi insertAsRC(BinNodePosi x, T const& e) { // x ��Ϊ�Һ��ӣ����� e ��Ϊ���Һ���
		_size++; x->insertAsRC(e); updateHeightAbove(x);
		return x->rc;
	}
	BinNodePosi attachAsLC(BinNodePosi x, BinTree*& S) { // T ��Ϊ x ����������
		if (x->lc = S->_root) x->lc->parent = x;
		_size += S->_size; updateHeightAbove(x);
		S->_root = nullptr; S->_size = 0; release(S); S = nullptr; // �ͷ�ԭ��
		return x;
	}
	BinNodePosi attachAsRC(BinNodePosi x, BinTree*& S) { // T ��Ϊ x ����������
		if (x->rc = S->_root) x->rc->parent = x;
		_size += S->_size; updateHeightAbove(x);
		S->_root = nullptr; S->_size = 0; release(S); S = nullptr; // �ͷ�ԭ��
		return x;
	}
	int remove(BinNodePosi x) { // ɾ����λ�� x ���ڵ�Ϊ�������������ظ�����ԭ�ȵĹ�ģ
		FromParentTo(*x) = nullptr; updateHeightAbove(x->parent); // ��¼���ڵ�ָ�룬ע��Ҫ�� FromParentTo �궨��
		int n = removeAt(x); _size -= n; return n;
	}
	static int removeAt(BinNodePosi x) { // ɾ��λ�� x ���Ľڵ㣬���ر�ɾ���ڵ���Ŀ��0 �� 1��
		if (!x) return 0;
		int n = 1 + removeAt(x->lc) + removeAt(x->rc); // �ݹ�ɾ����������
		release(x->data); release(x); return n; // �ͷű�ɾ���ڵ㣬��������Ŀ
	}

	template void travLevel(VST& visit) { if (_root) _root->travLevel(visit); } // ��α���

	template void travPre(VST& visit) { if (_root) _root->travPre(visit); } // ǰ�����

	template void travIn(VST& visit) { if (_root) _root->travIn(visit); } // �������

	template void travPost(VST& visit) { if (_root) _root->travPost(visit); } // �������

	bool operator<(BinTree const& t) { return _root && t._root && lt(_root, t._root); } // �Ƚ������������в��䣩
	bool operator==(BinTree const& t) { return _root && t._root && (_root == t._root); }
};
```

2. ���� BinTree ���� HuffTree

�ȶ���һ���ṹ�����ڴ����ַ��Ͷ�Ӧ��Ȩֵ��

struct CharFreq {
	char ch;
	int freq;
	CharFreq(char c = '\0', int f = 0) : ch(c), freq(f) {}
	bool operator<(CharFreq const& cf) const { return freq < cf.freq; }
	bool operator==(CharFreq const& cf) const { return freq == cf.freq; }
	bool operator>(CharFreq const& cf) const { return freq > cf.freq; }
};
```

Ȼ���� Huffman �����̳��� BinTree��ʵ�� Huffman �����㷨��

template
class HuffTree : public BinTree {
protected:
	typedef BinNodePosi HuffNodePosi;
	static void generateCT(Bitmap* code, int length, HuffNodePosi v) { // �� Huffman ���д� v �������ڵ�·���ϵ����нڵ������ı��룬��¼�� Bitmap �в�����
		if (IsRoot(*v)) return;
		generateCT(code, length + 1, v->parent);
		(IsLChild(*v)) ? code->clear(length) : code->set(length); // ���֧Ϊ 0���ҷ�֧Ϊ 1
	}

public:
	HuffTree(List& freqList) { // ���캯��
		PriorityQueue Q;
		for (int i = 0; i < freqList.size(); i++) {
			CharFreq cf = freqList[i];
			BinNodePosi x = new BinNode(cf);
			this->_size++;
			Q.insert(x); // ��ɭ�ֵ�ÿһ��������Ϊ��ʵ�������ȶ���
		}
		while (Q.size() > 1) { // ���ϵ�ѡ��Ƶ����С�������ڵ�ϲ���ֱ��ֻʣһ����
			HuffNodePosi l = Q.delMax();
			HuffNodePosi r = Q.delMax();
			HuffNodePosi u = new BinNode(CharFreq('\0', l->data.freq + r->data.freq), nullptr, l, r);
			this->_size++;
			l->parent = u;
			r->parent = u;
			Q.insert(u);
		}
		this->_root = Q.delMax(); // ������Ψһʣ��Ľڵ㼴Ϊ��
		generateCT(new Bitmap, 0, this->_root);
	}
	~HuffTree() { release(this->_root); } // �����������ݹ�ɾ�����ڵ㣩
	void generateCT(Bitmap* code[]) { // ���ɱ��루������ĺ�����ѡһ������λͼ���Ѻ� LaTeX ���ݣ����ﲻ�������
		generateCT(new Bitmap, 0, this->_root);
	}
	void generateCT(HufCode& code) { // ���ɱ���
		code.resize(256); // ��ʼ��Ϊ���� ASCII ���ַ�����ʼ���볤��Ϊ 0
		generateCT(&code, 0, this->_root);
	}
	void decode(HufCode const& code, ostream& os) const { // ����
		HuffNodePosi v = this->_root; // �ݴ���ڵ�
		for (int i = 0; i < code.size(); i++) {
			v = code[i] ? v->rc : v->lc; // ���ݱ����֧����ѡȡ�ӽڵ�
			if (IsLeaf(*v)) { // ����ǰ�ڵ�ΪҶ�ڵ㣬��������ַ��������õ�ǰ�ڵ�Ϊ��
				os << v->data.ch;
				v = this->_root;
			}
		}
	}
};
```

3. ����λͼ�� Bitmap ���� Huffman �����Ʊ��봮���� HufCode

class Bitmap { // �����ؽ��д洢�Ķ�����λͼ
private:
	char* M;
	int _size;

	void expand(int n) { // ע�������ʵ����Ҫʡ��
	}

public:
	Bitmap(int n = 8) { M = new char[_size = (n + 7) / 8]; memset(M, 0, _size); }
	Bitmap(char const* file, int n = 8) { // ��ָ���ļ��ж�ȡ����ͼ������ƪ�����ƣ����ﲻ�ٸ�����
	}
	~Bitmap() { delete[] M; M = nullptr; }
	void set(int i) { expand(i); M[i >> 3] |= (0x80 >> (i & 0x07)); } // ���� i λ��Ϊ 1
	void clear(int i) { expand(i); M[i >> 3] &= ~(0x80 >> (i & 0x07)); } // ���� i λ��Ϊ 0
	bool test(int i) { expand(i); return M[i >> 3] & (0x80 >> (i & 0x07)); } // ���Ե� i λ�Ƿ�Ϊ 1
	int size() { return _size * 8; }
	char* bits2string(int n) { // ���ش�ת��Ϊ�ַ�����ͬ������ƪ�����ƣ����ﲻ�ٸ�����
	}
};

typedef Bitmap* HufCode;
```

4. ʵ�� Huffman �����㷨

void encodeHuff(char const* file) { // Huffman �����㷨
	List freqList = statistics(file); // �ַ�Ƶ��ͳ��
	release(file);
	HuffTree* huffTree = new HuffTree(freqList); // �����ַ�Ƶ�ȹ��� Huffman ��
	release(freqList);
	HufCode code = new Bitmap; // �������洢�ռ�
	huffTree->generateCT(*code); // ���� Huffman ����
	FILE* fout = fopen("output.huff", "wb"); // ���ѹ���ļ�
	fwrite(code, sizeof(Bitmap), 1, fout); // �Ƚ�����洢�ռ�д���ļ������ڽ�ѹʱ����
	fwrite(&huffTree->_size, sizeof(int), 1, fout); // д���ַ�����
	huffTree->encode(file, fout); // д����봮
	fclose(fout);
	release(huffTree); // ɾ�� Huffman ���������ɵı���