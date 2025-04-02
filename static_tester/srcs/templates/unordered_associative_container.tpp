#ifndef MONKEY_TESTER_ASSOCIATIVE_CONTAINER_TPP
# define MONKEY_TESTER_ASSOCIATIVE_CONTAINER_TPP
# include <type_traits>
# ifndef NAMESPACE
# define NAMESPACE std
# endif
# include "utility.hpp"
# include <utility>
#include "../utils/dummy_iterators.hpp"
#include "assignable.tpp"

template <class X,
		class	Key,
		class	Mapped,
		class	ValueType,
		class	Hash,
		class	Pred,
		class	Allocator,
		bool	uniq>
void	unordered_associative_containers(const ValueType t, const Key k)
{
	(void)t;
	(void)k;
	static_assert(std::is_same_v<typename X::key_type, Key>, "key_type is not well defined");
	static_assert(std::is_destructible_v<typename X::key_type>, "key_type is not destructible");

	static_assert(std::is_same_v<typename X::mapped_type, Mapped>, "mapped_type is not well defined");
	static_assert(std::is_destructible_v<typename X::mapped_type>, "mapped_type is not destructible");

	static_assert(std::is_same_v<typename X::hasher, Hash>, "hasher is not well defined");
	static_assert(std::is_same_v<std::size_t, typename std::result_of<Hash(Key)>::type>, "hasher(key_type) does not return std::size_t");

	static_assert(std::is_same_v<typename X::key_equal, Pred>, "key_equal is not well defined");
	static_assert(std::is_convertible_v<typename std::result_of<Pred(Key, Key)>::type, bool>, "key_equal does not return a type implicitely convertible to bool");

	static_assert(std::is_same_v<typename X::iterator::iterator_category,	typename X::local_iterator::iterator_category>,	"local_iterator does not have the same typedefs as iterator");
	static_assert(std::is_same_v<typename X::iterator::value_type,			typename X::local_iterator::value_type>,		"local_iterator does not have the same typedefs as iterator");
	static_assert(std::is_same_v<typename X::iterator::difference_type,		typename X::local_iterator::difference_type>,	"local_iterator does not have the same typedefs as iterator");
	static_assert(std::is_same_v<typename X::iterator::pointer,				typename X::local_iterator::pointer>,			"local_iterator does not have the same typedefs as iterator");
	static_assert(std::is_same_v<typename X::iterator::reference,			typename X::local_iterator::reference>,			"local_iterator does not have the same typedefs as iterator");

	static_assert(std::is_same_v<typename X::const_iterator::iterator_category,	typename X::const_local_iterator::iterator_category>,	"local_iterator does not have the same typedefs as iterator");
	static_assert(std::is_same_v<typename X::const_iterator::value_type,		typename X::const_local_iterator::value_type>,			"local_iterator does not have the same typedefs as iterator");
	static_assert(std::is_same_v<typename X::const_iterator::difference_type,	typename X::const_local_iterator::difference_type>,		"local_iterator does not have the same typedefs as iterator");
	static_assert(std::is_same_v<typename X::const_iterator::pointer,			typename X::const_local_iterator::pointer>,				"local_iterator does not have the same typedefs as iterator");
	static_assert(std::is_same_v<typename X::const_iterator::reference,			typename X::const_local_iterator::reference>,			"local_iterator does not have the same typedefs as iterator");

	dummy_input_iterator<ValueType> i(t), j(t);
	Hash hf;
	Pred eq;
	typename X::size_type n;
	{
		(void)X(n, hf, eq);
		X a(n, hf, eq);
	}
	{
		(void)X(n, hf);
		X a(n, hf);
	}
	{
		(void)X(n);
		X a(n);
	}
	{
		(void)X();
		X a;
	}
	{
		(void)X(i, j, n, hf, eq);
		X a(i, j, n, hf, eq);
	}
	{
		(void)X(i, j, n, hf);
		X a(i, j, n, hf);
	}
	{
		(void)X(i, j, n);
		X a(i, j, n);
	}
	{
		(void)X(i, j);
		X a(i, j);
	}
	{
		X b;
		(void)X(b);
		X a(b);
	}
	{
		X a, b;
		a = b;
	}

	X a(n, hf, eq);
	const X const_a(n, hf, eq);

	const_a.hash_function();
	static_assert(std::is_same_v<typename std::result_of<decltype(&X::hash_function)(X)>::type, typename X::hasher>, "hash_function() does not return a hasher object");
	const_a.key_eq();
	static_assert(std::is_same_v<typename std::result_of<decltype(&X::key_eq)(X)>::type, typename X::key_equal>, "key_eq() does not return a key_equal object");

	a.insert(t);
	static_assert(std::is_same_v<decltype(a.insert(t)), typename std::conditional_t<uniq, NAMESPACE::pair<typename X::iterator, bool>, typename X::iterator>>, "Wrong return type for insert(value)");
	a.insert(a.begin(), t);
	a.insert(a.cbegin(), t);
	static_assert(std::is_same_v<decltype(a.insert(a.begin(), t)), typename X::iterator>, "wrong return type for insert(it, val)");
	static_assert(std::is_same_v<decltype(a.insert(a.cbegin(), t)), typename X::iterator>, "wrong return type for insert(it, val)");
	a.insert(i, j);
	static_assert(std::is_same_v<decltype(a.insert(i, j)), void>, "wrong return type for insert(it1, it2)");
	a.erase(k);
	static_assert(std::is_same_v<decltype(a.erase(k)), typename X::size_type>, "wrong return type for erase(key)");

	// these test will fail with std::map since the return type of erase change between c++98 and c++11 and the tester is compiled in c++11.
	a.erase(a.begin());
	a.erase(a.cbegin());
	static_assert(std::is_same_v<decltype(a.erase(a.begin())), typename X::iterator>, "wrong return type for erase(it)");
	static_assert(std::is_same_v<decltype(a.erase(a.cbegin())), typename X::iterator>, "wrong return type for erase(it)");

	a.erase(a.begin(), a.end());
	a.erase(a.cbegin(), a.cend());
	static_assert(std::is_same_v<decltype(a.erase(a.begin(), a.end())), typename X::iterator>, "wrong return type for erase(p1, p2)");
	static_assert(std::is_same_v<decltype(a.erase(a.cbegin(), a.cend())), typename X::iterator>, "wrong return type for erase(q1, q2)");

	a.clear();
	static_assert(std::is_same_v<decltype(a.clear()), void>, "wromnt return type for clear()");

	a.find(k);
	const_a.find(k);
	static_assert(std::is_same<decltype(a.find(k)), typename X::iterator>::value, "wrong return type for find");
	static_assert(std::is_same<decltype(const_a.find(k)), typename X::const_iterator>::value, "wrong return type for find on const container");

	a.count(k);
	const_a.count(k);
	static_assert(std::is_same_v<decltype(a.count(k)), typename X::size_type>, "wrong return type for count()");
	static_assert(std::is_same_v<decltype(const_a.count(k)), typename X::size_type>, "wrong return type for count() on const container");

	a.equal_range(k);
	const_a.equal_range(k);
	static_assert(std::is_same<decltype(a.equal_range(k)), NAMESPACE::pair<typename X::iterator, typename X::iterator> >::value, "wrong return type for equal_range");
	static_assert(std::is_same<decltype(const_a.equal_range(k)), NAMESPACE::pair<typename X::const_iterator, typename X::const_iterator> >::value, "wrong return type for equal_range on const container");

	a.bucket_count();
	const_a.bucket_count();
	static_assert(std::is_same_v<decltype(a.bucket_count()), typename X::size_type>, "wrong return type for bucket_count()");
	static_assert(std::is_same_v<decltype(const_a.bucket_count()), typename X::size_type>, "wrong return type for bucket_count() on const container");

	a.max_bucket_count();
	const_a.max_bucket_count();
	static_assert(std::is_same_v<decltype(a.max_bucket_count()), typename X::size_type>, "wrong return type for max_bucket_count()");
	static_assert(std::is_same_v<decltype(const_a.max_bucket_count()), typename X::size_type>, "wrong return type for max_bucket_count() on const container");

	a.bucket(k);
	const_a.bucket(k);
	static_assert(std::is_same_v<decltype(a.bucket(k)), typename X::size_type>, "wrong return type for bucket()");
	static_assert(std::is_same_v<decltype(const_a.bucket(k)), typename X::size_type>, "wrong return type for bucket() on const container");

	a.bucket_size(n);
	const_a.bucket_size(n);
	static_assert(std::is_same_v<decltype(a.bucket_size(n)), typename X::size_type>, "wrong return type for bucket_size()");
	static_assert(std::is_same_v<decltype(const_a.bucket_size(n)), typename X::size_type>, "wrong return type for bucket_size() on const container");

	a.begin(n);
	const_a.begin(n);
	static_assert(std::is_same_v<decltype(a.begin(n)), typename X::local_iterator>, "wrong return type for begin()");
	static_assert(std::is_same_v<decltype(const_a.begin(n)), typename X::const_local_iterator>, "wrong return type for begin() on const container");

	a.end(n);
	const_a.end(n);
	static_assert(std::is_same_v<decltype(a.end(n)), typename X::local_iterator>, "wrong return type for end()");
	static_assert(std::is_same_v<decltype(const_a.end(n)), typename X::const_local_iterator>, "wrong return type for end() on const container");

	a.cbegin(n);
	const_a.cbegin(n);
	static_assert(std::is_same_v<decltype(a.cbegin(n)), typename X::const_local_iterator>, "wrong return type for cbegin()");
	static_assert(std::is_same_v<decltype(const_a.cbegin(n)), typename X::const_local_iterator>, "wrong return type for cbegin() on const container");

	a.cend(n);
	const_a.cend(n);
	static_assert(std::is_same_v<decltype(a.cend(n)), typename X::const_local_iterator>, "wrong return type for cend()");
	static_assert(std::is_same_v<decltype(const_a.cend(n)), typename X::const_local_iterator>, "wrong return type for cend() on const container");

	a.load_factor();
	const_a.load_factor();
	static_assert(std::is_same_v<decltype(a.load_factor()), float>, "wrong return type for load_factor()");
	static_assert(std::is_same_v<decltype(const_a.load_factor()), float>, "wrong return type for load_factor() on const container");

	a.max_load_factor();
	const_a.max_load_factor();
	static_assert(std::is_same_v<decltype(a.max_load_factor()), float>, "wrong return type for max_load_factor()");
	static_assert(std::is_same_v<decltype(const_a.max_load_factor()), float>, "wrong return type for max_load_factor() on const container");

	a.max_load_factor(1.0f);
	static_assert(std::is_same_v<decltype(a.max_load_factor(1.0f)), void>, "wrong return type for max_load_factor(z)");

	a.rehash(n);
	static_assert(std::is_same_v<decltype(a.rehash(n)), void>, "wrong return type for rehash()");

	a.reserve(n);
	static_assert(std::is_same_v<decltype(a.reserve(n)), void>, "wrong return type for reserve()");
}

#endif
