#include <iostream>
#include <map>
#include <numeric>

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

	void printall() {
		for (auto &n : m_nodes)
			std::cout << "key: " << n.first << " val: " << n.second << std::endl;
	}
};

template<typename T, T Def, typename X = int, typename Y = int, typename... Args>
class Matrix {
	std::map<Y, Vector<T, Def, X>> m_rows;

	class Vec : public Vector<T, Def, X> {
		Matrix* parent;
	public:
		X ind;
		Vec(Matrix* p) : parent(p) {}

		virtual typename Vector<T, Def, X>::Node& AddNode(const X key, const T val) override {
			return parent->AddVector(ind).AddNode(key, val);
		}
	} m_dummy_vec;
public:
	Matrix() : m_dummy_vec(this) {}

	int size() {
		return std::accumulate(m_rows.begin(), m_rows.end(), 0, [](auto s, auto n) { return s + n.second.size();});
	}
	
	void printall() {
		std::cout << "main size " << m_rows.size() << std::endl;
		for (auto &m : m_rows) {
			std::cout << "key " << m.first << " size " << m.second.size() << std::endl;
			m.second.printall();
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

	Vector<T, Def, X>& AddVector(const Y key) {
		return m_rows[key];
	}
};

// template<typename T, T Def, typename X, typename Y, typename Key, typename... Args>
// class Matrix<T, Def, X, Y, Key, Args...> {
//     std::map<Key, Matrix<T, Def, X, Y, Args...>> nodes;
// public:
//     T get_box(int ind) {
//         return 1;
//     }
//     void set_box(int ind, int val) {
//     }
// };

int main() {
	try {

		Vector<int, -5> v;
		std::cout << v[100] << std::endl;
		v[100] = 10;
		std::cout << v[100] << std::endl;
		v[200] = 30;
		std::cout << v[0] << std::endl;
		v.printall();


		Matrix<int, -2> m;
		std::cout << m[0][0] << std::endl;
		std::cout << m[2][2] << std::endl;
		m[2][2] = 5;
		m[2][2] = 6;
		std::cout << m[2][2] << std::endl;
		std::cout << m[3][3] << std::endl;
		m[3][3] = 9;
		std::cout << m[3][3] << std::endl;
		m[3][4] = 10;
		std::cout << m[3][4] << std::endl;
		std::cout << "Matrix size:" << m.size() << std::endl;
		m.printall();

	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
