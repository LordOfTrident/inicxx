/*
 *  inicxx conversion example
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

	/*
	 *  You can find the explanation of this in writing.cc
	 */

	INI::Structure inis = INI::Sections({
		{"Fruit", INI::Section({
			{"Foo", "true"}
		})},
	});

	/*
	 *  If you use exceptions, wrap INI::Structure::AsBoolean in
	 *  a try catch, because it can throw an exception. What it
	 *  throws is described below above catch.
	 *
	 *  if you dont use exceptions, INI::Structure::AsBoolean
	 *  will return the boolean (true/false) if the conversion
	 *  was successful and INI_ERROR (-1) if the parser encountered
	 *  an error.
	 *    if (inis.AsBoolean("Foo", "Bar") == INI_ERROR) {
	 *        // Error encountered while converting
	 *    };
	 */

	try {

		/*
		 *  INI::Structure::AsBoolean(string, string) takes in
		 *  2 parameters, first one being the sector and the
		 *  second one being the key of which the value you want
		 *  to convert into a boolean type.
		 *
		 *  This is the only conversion method to throw a custom
		 *  exception class.
		 */

		std::cout << "Foo = " << inis.AsBoolean("Fruit", "Foo") << std::endl;

	/*
	 *  INI::Structure::AsBoolean() throws an exception of the
	 *  class INI::Exception which holds one piece of information:
	 *  the error message. You can get it using the
	 *  INI::Exception::What method.
	 */

	} catch (const INI::Exception &error) {
		std::cerr
			<< "Error while converting 'Foo' value to a boolean: "
			<< error.What() << std::endl;

		return 0;
	};

	/*
	 *  There are other conversion methods too:
	 *    INI::Structure::AsNumber(string, string)
	 *    INI::Structure::AsInteger(string, string)
	 *    INI::Structure::AsString(string, string)
	 *
	 *  AsNumber turns the value into a double and returns
	 *  it. It uses the standard function std::stod, so it
	 *  will throw a standard exception class on error.
	 *
	 *  AsInteger turns the value into an integer and returns
	 *  it (the type is size_t). Just like AsNumber, it uses
	 *  a standard function std::stol, so it will throw on
	 *  an error.
	 *
	 *  AsString will simply just return the value, since it
	 *  is a string by default.
	 */

	return 0;
};
