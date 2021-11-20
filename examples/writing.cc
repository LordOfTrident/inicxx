/*
 *  inicxx writing example
 */

#include <iostream>
#include <inicxx.hh>

int main() {
	// INI supports more than just char strings, but
	// we want to use a char one
	INI::Structure<char> inis = INI::Sections({
		{INI::Structure::DefaultSection, INI::Section({
			{"Food", "Bar"}
		})},

		{"Fruit", INI::Section({
			{"Foobar", "Foobaz"},
			{"Baz", "Bazbar"}
		})}
	});

	inis["Fruit"]["Baz"] = " Qux "; // [] operator returns INI::Section&
	inis.At("Fruit", "Foobar") = "\"Fred\""; // At(string, string) returns string&

	std::cout
		<< inis.Stringify()
		<< std::endl;

	return 0;
};
