/*
 *  inicxx reading example
 */

#include <iostream>
#include <inicxx.hh>

int main() {
	INI::Structure<char> inis;

	try {
		// Using some C++ tricks to simulate a multi line string
		inis.Parse("\
# Comment test                         \n\
Foo = Bar                              \n\
[Fruit]                                \n\
Baz = \" Qux \" ; Set 'Baz' to ' Qux ' \n\
		");
	} catch (const INI::ParserException &error) {
		std::cerr
			<< "Error while parsing on the line "
			<< error.Line() << ": "
			<< error.What() << std::endl;

		return 0;
	};

	if (not inis.Contains("Fruit")) { // overload 1
		std::cerr
			<< "Section 'Fruit' is missing!"
			<< std::endl;

		return 0;
	} else if (not inis.Contains("Fruit", "Baz")) { // overload 2
		std::cerr
			<< "Key 'Baz' from section 'Fruit' is missing!"
			<< std::endl;

		return 0;
	};

	std::cout
		<< "Baz=\"" << inis["Fruit"]["Baz"]
		<< "\"" << std::endl;

	inis.Clear(); // Clear all sections

	return 0;
};
