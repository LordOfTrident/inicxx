/*
 *  inicxx writing example
 */

#include <iostream>
#include <inicxx.hh>

int main() {

	/*
	 *  Using the typedef of INI::Sections to let the compiler know
	 *  what is it that we are assigning to the INI::Structure,
	 *  which has a constructor from INI::Sections.
	 *
	 *  INI::Sections and INI::Section typedefs:
	 *    typedef std::map<std::string, std::string> Section;
	 *    typedef std::map<std::string, Section> Sections;
	 */
	 
	INI::Structure inis = INI::Sections({
		{INI_DEFAULT, INI::Section({
			{"Food", "Bar"}
		})},

		{"Fruit", INI::Section({
			{"Foobar", "Foobaz"},
			{"Baz", "Bazbar"}
		})}
	});

	/*
	 *  The [] operator overload returns the Section (INI::Section)
	 *  from which you then index the key.
	 *
	 *  The INI::Structure::At(string, string) method runs the std::map::at
	 *  method first on the sections std::map and then the returned 
	 *  INI::Section.
	 *
	 *  You can also use the overload INI::Structure::At(string), which does
	 *  the same as the [] operator overload but it uses std::map::at
	 *  instead of the [] operator on the sections std::map.
	 *
	 *  Use the one you prefer.
	 */
	 
	inis["Fruit"]["Baz"] = " Qux ";
	inis.At("Fruit", "Foobar") = "\"Fred\"";

	/*
	 *  The INI::Structure::Stringify() method generates an INI text from
	 *  the structure, but it generates '[]' as the default section; be
	 *  aware of that!
	 */

	std::cout << inis.Stringify() << std::endl;

	return 0;
};
