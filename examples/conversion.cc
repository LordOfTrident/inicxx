/*
 *  inicxx conversion example
 */

#include <iostream>
#include <inicxx.hh>

int main() {
	// INI supports more than just char strings, but
	// we want to use a char one
	INI::Structure<char> inis = INI::Sections({
		{"Fruit", INI::Section({
			{"Foo", "true"}
		})},
	});

	try {
		std::cout
			<< "Foo = "
			<< inis.AsBoolean("Fruit", "Foo")
			<< std::endl;
	} catch (const INI::Exception &error) {
		std::cerr
			<< "Error while converting 'Foo' value to a boolean: "
			<< error.What() << std::endl;

		return 0;
	};

	// Other conversion methods:
	//   INI::Structure::AsNumber(std::string, std::string)
	//     returns a double

	//   INI::Structure::AsInteger(std::string, std::string)
	//     returns an integer

	//   INI::Structure::AsString(std::string, std::string)
	//     returns an std::tring

	return 0;
};
