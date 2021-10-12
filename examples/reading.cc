/*
 *  inicxx reading example
 */

#include <iostream>

/*
 *  If you dont want the library to use
 *  exceptions (for example when the parser
 *  hits a syntax error), you have to
 *  #define the macro INI_DONT_USE_EXCEPTIONS
 *  before you include the header file:
 *    #define INI_DONT_USE_EXCEPTIONS
 */

#include <inicxx.hh>

int main() {
	INI::Structure inis;

	/*
	 *  If you use exceptions, wrap INI::Structure::Parse in a
	 *  try catch, because it can throw an exception. What it
	 *  throws is described below above catch.
	 *
	 *  if you dont use exceptions, INI::Structure::Parse
	 *  will return INI_OK (0) if the parsing was successful
	 *  and INI_ERROR (-1) if the parser encountered an
	 *  error.
	 *    if (inis.Parse("...") == INI_ERROR) {
	 *        // Error encountered while parsing
	 *    };
	 */

	try {

		/*
		 *  A call to INI::Structure::Parse(string) which parses
		 *  the string.
		 *
		 *  To the ones confused by the multi line string and '\n\',
		 *  the first '\n' is for a new line char and the second '\'
		 *  is for the compiler to ignore the new line by which the
		 *  string is split for readability purposes in multiple
		 *  lines, this makes the compiler not error on it.
		 */

		inis.Parse("\
# Comment test                         \n\
Foo = Bar                              \n\
[Fruit]                                \n\
Baz = \" Qux \" ; Set 'Baz' to ' Qux ' \n\
		");

	/*
	 *  INI::Structure::Parse throws an exception of the class
	 *  INI::ParserException, which is derived from the class
	 *  INI::Exception. INI::ParserException holds two
	 *  pieces of information: The number of the line on
	 *  which the error occured and the error message. You
	 *  can get these using the INI::ParserException::Line
	 *  and INI::ParserException::What methods.
	 */

	} catch (const INI::ParserException& Error) {
		std::cout << "Error while parsing on the line " 
			<< Error.Line() << ": " 
			<< Error.What() << std::endl;

		return 0;
	};

	/*
	 *  INI::Structure::Contains() method has two overloads: one
	 *  taking in a single string parameter which is the section
	 *  name you want to check the existance of. The second
	 *  overload has two string parameters, one being the section
	 *  name and the second one being the key name.
	 *  If the second overload finds out that the section does
	 *  not exist, it just returns false.
	 */

	if (not inis.Contains("Fruit")) {
		std::cout << "Section 'Fruit' is missing!" << std::endl;

		return 0;
	};

	if (not inis.Contains("Fruit", "Baz")) {
		std::cout << "Key 'Baz' from section 'Fruit' is missing!" << std::endl;

		return 0;
	};

	/*
	 *  INI::Structure::operator[] overload explained in 'writing.cc'
	 */

	std::cout << "Baz=\"" << inis["Fruit"]["Baz"] << "\"" << std::endl;

	/*
	 *  INI::Structure::Clear() clears the entire sections std::map
	 */
	
	inis.Clear();

	return 0;
};
