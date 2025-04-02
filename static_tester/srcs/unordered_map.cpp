#include "templates/unordered_associative_container.tpp"
#include "templates/container_cpp11.tpp"
#include "templates/forward_iterator.tpp"
#include <unordered_map>
#include "utils/basic_type.hpp"
#include "unordered_map.hpp"

void test_map()
{
	using Key = basic_type;
	using Mapped = basic_type;
	using ValueType = NAMESPACE::pair<const Key,Mapped>;
	using Hash = std::hash<Key>;
	using Compare = basic_type::compare;
	using Allocator = std::allocator <NAMESPACE::pair<const Key, Mapped> >;
	using X = NAMESPACE::unordered_map<Key, Mapped, Hash, Compare, Allocator>;

	ValueType t = NAMESPACE::pair<const Key, Mapped>(Key(), Key());
	Key k;

	(void)t;
	(void)k;

	container_cpp11<NAMESPACE::unordered_map<int, int>, NAMESPACE::pair<const int, int> >();

	forward_iterator<typename X::iterator, ValueType>(t);
	forward_iterator<typename X::const_iterator, const ValueType>(t);
	forward_iterator<typename X::local_iterator, ValueType>(t);
	forward_iterator<typename X::const_local_iterator, const ValueType>(t);

	unordered_associative_containers<
	        X,
	        Key,
	        Mapped,
			ValueType,
			Hash,
			Compare,
			Allocator,
			true
			>(t, k);

	static_assert(std::is_same<typename NAMESPACE::unordered_map<int, int>::mapped_type, int>::value, "mapped_type is not well defined");
	NAMESPACE::unordered_map<int, int> m;

	(void)m[1];
	static_assert(std::is_same<decltype(m[1]), int& >::value, "wrong return type for operator[]");

	(void)m.at(1);
	static_assert(std::is_same<decltype(m.at(1)), int& >::value, "wrong return type for at()");

	const NAMESPACE::unordered_map<int, int> const_m;
	(void)const_m.at(1);
	static_assert(std::is_same<decltype(const_m.at(1)), const int& >::value, "wrong return type for at()");

}
