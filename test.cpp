#include <utility>
#include <string>
#include <unordered_map>
#include <iostream>

void	print_stuff(std::unordered_map<std::string, int> const& foo)
{
	std::cout << "Bucket count: " << foo.bucket_count() << '\n'
		<< "Size: " << foo.size() << '\n'
		<< "Load factor: " << foo.load_factor() << '\n'
		<< "Max load factor: " << foo.max_load_factor() << '\n';
}

int main() {
	std::unordered_map<std::string, int>    foo;

	print_stuff(foo);
	foo["coucou"] = 0;
	print_stuff(foo);
	foo["foobar"] = 1;
	print_stuff(foo);
	foo["fizzbuzz"] = 2;
	print_stuff(foo);
}
