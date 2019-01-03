#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <cassert>

template<typename T, T Def, typename K = int>
class Vector {
public:
	struct Node {
		Vector* parent;
		T val;
		K ind;

		Node(Vector* p) : parent(p), val(Def) {}
		Node(const T v) : parent(nullptr), val(v) {}

		bool operator== (const T v) {
			return val == v;
		};

		Node& operator= (const T v) {
			if (v == val)
				return *this;
			if (parent == nullptr) {
				val = v;
				return *this;
			}
			return parent->AddNode(ind, v);
		}

		friend std::ostream& operator<<(std::ostream& out, Node node) {
			out << node.val;
			return out;
		}
	};

private:

	std::map<K, Node> m_nodes;
	Node m_dummy_node;

public:
	Vector() : m_dummy_node(this) {}

	Node& operator[] (const K key) {
		auto it = m_nodes.find(key);
		if (it == m_nodes.end()) {
			m_dummy_node.ind = key;
			return m_dummy_node;
		}
		return it->second;
	}

	virtual Node& AddNode(const K key, const T val) {
		m_nodes.insert(std::make_pair(key, Node(val)));
		return m_nodes.at(key);
	}

	int size() {
		return m_nodes.size();
	}

	void PrintAll(const std::string &prefix = "") {
		for (auto &n : m_nodes)
			std::cout << prefix << n.first << n.second << ' ';
	}
};

template<typename T, T Def, typename X = int, typename Y = int, typename... Args>
class Matrix {
public:
	using ContType = std::map<Y, Vector<T, Def, X>>;
private:
	ContType m_rows;

	class Vec : public Vector<T, Def, X> {
		Matrix* parent;
	public:
		X ind;
		Vec(Matrix* p) : parent(p) {}

		virtual typename Vector<T, Def, X>::Node& AddNode(const X key, const T val) override {
			return parent->GetCont()[ind].AddNode(key, val);
		}
	} m_dummy_vec;
public:
	Matrix() : m_dummy_vec(this) {}

	int size() {
		return std::accumulate(m_rows.begin(), m_rows.end(), 0, [](auto s, auto n) { return s + n.second.size();});
	}
	
	void PrintAll(const std::string &prefix = "") {
		for (auto &m : m_rows) {
			std::stringstream ss;
			ss << m.first;
			m.second.PrintAll(ss.str());
			std::cout << std::endl;
		}
	}

	Vector<T, Def, X>& operator[] (const Y key) {
		auto it = m_rows.find(key);
		if (it == m_rows.end()) {
			m_dummy_vec.ind = key;
			return m_dummy_vec;
		}
		return it->second;
	}

	virtual ContType& GetCont() {
		return m_rows;
	}
};

template<typename T, T Def, typename X, typename Y, typename Key, typename... Args>
class Matrix<T, Def, X, Y, Key, Args...> {
public:
	using MatrixType = Matrix<T, Def, X, Y, Args...>;
	using ContType = std::map<Key, MatrixType>;
private:
	ContType m_nodes;

	class Met : public MatrixType {
		Matrix* parent;
	public:
		Key ind;
		Met(Matrix* p) : parent(p) {}

		virtual typename MatrixType::ContType& GetCont() override {
			return parent->GetCont()[ind].GetCont();
		}
	} m_dummy_node;
public:
	Matrix() : m_dummy_node(this) {}

	MatrixType& operator[] (const Y key) {
		auto it = m_nodes.find(key);
		if (it == m_nodes.end()) {
			m_dummy_node.ind = key;
			return m_dummy_node;
		}
		return it->second;
	}

	virtual ContType& GetCont() {
		return m_nodes;
	}
};

int main() {
	try {

		{ // task script
			Matrix<int, 0> m;
			for (int i = 0; i < 10; ++i) {
				m[i][i] = i;
				m[9 - i][i] = i;
			}
			for (int i = 1; i < 9; ++i) {
				for (int j = 1; j < 9; ++j)
					std::cout << m[i][j] << ' ';
				std::cout << std::endl;
			}
			std::cout << m.size() << std::endl;
			m.PrintAll();
		}

		{ // canonical form
			Matrix<int, 0> m;
			((m[100][100]  = 314) = 0) = 217;
			assert(m[100][100] == 217);
		}
		{ // multidimensional matrix
			Matrix<int, 0, int, int, int> m;
			m[100][100][100] = 314;
		}
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
