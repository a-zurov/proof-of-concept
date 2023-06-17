
#include <iostream>
#include <limits>
#include <vector>
#include <sstream>

//------------------------------------------------------------------
#include <map>
template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {

	// INSERT YOUR SOLUTION HERE

		if (!(keyBegin < keyEnd)) {

			return;
		}

		if (m_map.empty()) {

			m_map.insert({ keyBegin, val });
			m_map.insert({ keyEnd, m_valBegin });
			return;
		}

		auto itStart = m_map.begin();
		K keyStart = itStart->first;
		auto itFinish = m_map.end();
		K keyFinish = (--itFinish)->first;

		if (keyEnd < keyStart || keyFinish < keyBegin) {

			m_map.insert({ keyBegin, val });
			m_map.insert({ keyEnd, m_valBegin });
			return;
		}

		// keyEnd == keyStart
		if (!(keyEnd < keyStart) && !(keyEnd > keyStart)) {

			m_map.insert({ keyBegin, val });
			return;
		}

		// keyFinish == keyBegin
		if (!(keyFinish < keyBegin) && !(keyFinish > keyBegin)) {

			m_map.insert_or_assign(itFinish, keyBegin, val);
			m_map.insert({ keyEnd, m_valBegin });
			return;
		}

		// keyBegin <= keyStart && keyFinish <= keyEnd
		if (!(keyBegin > keyStart) && !(keyFinish > keyEnd)) {

			m_map.clear();
			m_map.insert({ keyBegin, val });
			m_map.insert({ keyEnd, m_valBegin });
			return;
		}

		// keyBegin <= keyStart && keyEnd < keyFinish
		if (!(keyBegin > keyStart) && (keyEnd < keyFinish)) {

			auto itEnd = m_map.lower_bound(keyEnd);
			V old_val = itEnd->second;
			if (itEnd->first != keyEnd) {
				auto it = itEnd;
				old_val = (--it)->second;
			}

			m_map.erase(itStart, itEnd);
			m_map.insert({ keyBegin, val });
			m_map.insert_or_assign(keyEnd, old_val);
			return;
		}

		// keyStart <= keyBegin && keyEnd < keyFinish
		if (!(keyStart > keyBegin) && (keyEnd < keyFinish)) {

			auto itBegin = m_map.lower_bound(keyBegin);
			auto itEnd = m_map.lower_bound(keyEnd);
			V old_val = itEnd->second;
			if (itEnd->first != keyEnd) {
				auto it = itEnd;
				old_val = (--it)->second;
			}

			if ( itBegin->first < itEnd->first)
				m_map.erase(itBegin, itEnd);
			else m_map.clear();
			m_map.insert_or_assign(keyBegin, val);
			m_map.insert_or_assign(keyEnd, old_val);
			return;
		}

		// keyStart <= keyBegin && keyFinish <= keyEnd
		if (!(keyStart > keyBegin) && !(keyFinish > keyEnd)) {

			auto itBegin = m_map.lower_bound(keyBegin);

			m_map.erase(itBegin, m_map.end());
			m_map.insert_or_assign(keyBegin, val);
			m_map.insert_or_assign(keyEnd, m_valBegin);
		}


		/*
		auto itBegin = m_map.lower_bound(keyBegin);

		if ( keyEnd < keyFinish) {

			auto itEnd = m_map.lower_bound(keyEnd);

			V old_val = itEnd->second;
			if (itEnd->first != keyEnd) {
				auto it = itEnd;
				old_val = (--it)->second;
			}

			if (itBegin->first < itEnd->first)
				m_map.erase(itBegin, itEnd);
			else m_map.clear();

			m_map.insert_or_assign( keyEnd, old_val );
		}
		else {

			m_map.erase(itBegin, m_map.end());

			m_map.insert_or_assign( keyEnd, m_valBegin );
		}

		m_map.insert_or_assign( keyBegin, val );
		*/

	}

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}

	void print() {
		for (const auto& p : m_map) {
			std::cout << p.first << ":" << p.second << ' ';
		}
		std::cout << '\n';
	}
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.

class TestRunner {

	int fail_count_ = 0;

public:
	~TestRunner() {

		if (fail_count_ > 0) {
			std::cerr << fail_count_ << " unit tests failed." << std::endl;
			exit(1);
		}
	}

	template <class TestFunc>
	void RunTest(TestFunc func, const std::string& test_name) {

		try {
			func();
			std::cerr << test_name << " OK\n";
		}
		catch (std::runtime_error& ex) {
			++fail_count_;
			std::cerr << test_name << " FAIL:\n" << ex.what() << '\n';
		}
	}
};

#define RUN_TEST(tr, func) \
    tr.RunTest(func, #func)

template< typename Key, typename T>
void AssignTogether(std::vector<T>& v1, interval_map<Key, T>& map, Key keyStart, Key keyEnd, T val) {

	map.assign(keyStart, keyEnd, val);

	std::fill(v1.begin() + keyStart, v1.begin() + keyEnd, val);

	int j = 0;
	for (auto& item : v1) {
		std::cout << j++ << ':' << item << ' ';
	}
	std::cout << '\n';
}

template<class T>
void AssertEqual2(std::ostringstream& os, const T& t, const T& u) {

	if (t != u) {
		os << t << " != " << u;
		throw std::runtime_error(os.str());
	}
	os << t;
}


template< typename Key, typename T>
void Compare(const std::vector<T>& v, interval_map<Key, T>& map, Key keyStart) {

	std::ostringstream os;
	for (unsigned i = 0; i < v.size(); ++i) {
		os << i << ':';
		AssertEqual2(os, v.at(i), map[keyStart]);
		++keyStart;
		os << ' ';
	}
}

void TestComparison2() {

	for (int N = 1; N < 40; ++N) {

		interval_map<int, int> imap(std::numeric_limits<int>::lowest());
		std::vector<int> v1(N, std::numeric_limits<int>::lowest());

		int keyBegin, keyEnd;

		for (int j = 0; j < 100; ++j) {

			keyBegin = rand() % N;
			keyEnd = rand() % N;

			if (keyEnd == keyBegin) ++keyEnd;
			else if (keyEnd < keyBegin) {
				std::swap(keyBegin, keyEnd);
			}

			std::cout << '[' << keyBegin << ',' << keyEnd << ") = " << j << '\n';

			AssignTogether(v1, imap, keyBegin, keyEnd, j);

			imap.print();
		}

		Compare(v1, imap, 0);
	}
}

int main()
{
	interval_map<int, int> imap(std::numeric_limits<int>::lowest());

	imap.assign(3, 7, 10);

	std::cout << "imap.assign(3, 7, 10)\n";
	for (int j = 0; j < 10; ++j) {
		std::cout << j << ':' << imap[j] << ' ';
	}
	std::cout << '\n';

	imap.assign(1, 3, 30);
	std::cout << "imap.assign(1, 3, 30)\n";
	for (int j = 0; j < 10; ++j) {
		std::cout << j << ':' << imap[j] << ' ';
	}
	std::cout << '\n';

	imap.assign(7, 9, 40);
	std::cout << "imap.assign(7, 9, 40)\n";
	for (int j = 0; j < 10; ++j) {
		std::cout << j << ':' << imap[j] << ' ';
	}
	std::cout << '\n';


	imap.assign(2, 5, 50);
	std::cout << "imap.assign(2, 5, 50)\n";
	for (int j = 0; j < 10; ++j) {
		std::cout << j << ':' << imap[j] << ' ';
	}
	std::cout << '\n';

	imap.assign(0, 7, 60);
	std::cout << "imap.assign(0, 7, 60)\n";
	for (int j = 0; j < 10; ++j) {
		std::cout << j << ':' << imap[j] << ' ';
	}
	std::cout << '\n';


	imap.assign(4, 9, 70);
	std::cout << "imap.assign(4, 9, 70)\n";
	for (int j = 0; j < 10; ++j) {
		std::cout << j << ':' << imap[j] << ' ';
	}
	std::cout << '\n';

	/*
	interval_map<int, int> imap2(std::numeric_limits<int>::lowest());

	imap2.assign(0, 3, 2);
	imap2.assign(3, 4, 5);

	for (int j = 0; j < 10; ++j) {
		std::cout << j << ':' << imap2[j] << ' ';
	}
	std::cout << '\n';

	imap2.assign(1, 2, 6);

	TestRunner tr;
	RUN_TEST(tr, TestComparison2);
	*/
}
