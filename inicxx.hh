#pragma once

#include <string> // std::string
#include <vector> // std::vector
#include <unordered_map> // std::unordered_map
#include <cstdint> // std::int8_t, std::int16_t, std::int32_t, std::int64_t,
                   // std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t,
#include <cstddef> // std::size_t

#define INI_VERSION_MAJOR 1
#define INI_VERSION_MINOR 2
#define INI_VERSION_PATCH 1

namespace INI {
	typedef std::int8_t  i8;
	typedef std::int16_t i16;
	typedef std::int32_t i32;
	typedef std::int64_t i64;

	typedef std::uint8_t  ui8;
	typedef std::uint16_t ui16;
	typedef std::uint32_t ui32;
	typedef std::uint64_t ui64;

	typedef std::size_t word;

	// Typedefs for shorter and readable code
	typedef std::unordered_map<std::string, std::string> Section;
	typedef std::unordered_map<std::string, Section> Sections;

#ifndef INI_DONT_USE_EXCEPTIONS
	class Exception {
		public:
			Exception(const std::string &p_message):
				m_message(p_message)
			{};

			~Exception() {};

			const std::string& What() const {
				return m_message;
			};

		private:
			std::string m_message;
	}; // class Exception

	class ParserException: public Exception {
		public:
			ParserException(const std::string &p_message, word p_line):
				Exception(p_message),
				m_line(p_line)
			{};

			~ParserException() {};

			word Line() const {
				return m_line;
			};

		private:
			word m_line;
	}; // class ParserException
#endif // INI_DONT_USE_EXCEPTIONS

	class Structure {
		public:
			// Constants for defaults
			static constexpr const char DefaultSection[1] = "";

#ifdef INI_DONT_USE_EXCEPTION
			// Constants for exitcodes
			static constexpr const i8 Ok = 0;
			static constexpr const i8 Error = -1;
#endif // INI_DONT_USE_EXCEPTION

#ifdef INI_DONT_USE_EXCEPTIONS
			Structure():
				m_errorMessage(""),
				m_line(0)
			{};
#else // not INI_DONT_USE_EXCEPTIONS
			Structure() {};
#endif // INI_DONT_USE_EXCEPTIONS

			~Structure() {};

#ifdef INI_DONT_USE_EXCEPTIONS
			Structure(const Sections &p_sections):
				m_errorMessage(""),
				m_line(0)
#else // not INI_DONT_USE_EXCEPTIONS
			Structure(const Sections &p_sections)
#endif // INI_DONT_USE_EXCEPTIONS
			{
				m_sections = p_sections;
			};

			Structure& operator=(const Structure &p_structure) {
				m_sections = p_structure.m_sections;

				return *this;
			};

			// Index functions
			Section& operator[](const std::string &p_idx) {
				return m_sections[p_idx];
			};

			Section& At(const std::string &p_idx) {
				return m_sections.at(p_idx);
			};

			std::string& At(
				const std::string &p_idxA,
				const std::string &p_idxB
			) {
				return m_sections.at(p_idxA).at(p_idxB);
			};

			bool Contains(const std::string &p_idx) const {
				return m_sections.count(p_idx);
			};

			bool Contains(
				const std::string &p_idxA,
				const std::string &p_idxB
			) const {
				if (m_sections.count(p_idxA))
					if (m_sections.at(p_idxA).count(p_idxB))
						return true;

				return false;
			};

			void Clear() {
				m_sections.clear();
			};

			void Clear(const std::string &p_idx) {
				m_sections[p_idx].clear();
			};

			// Conversion functions
			double AsNumber(
				const std::string &p_idxA,
				const std::string &p_idxB
			) {
				return std::stod(m_sections[p_idxA][p_idxB]);
			};

			long AsInteger(
				const std::string &p_idxA,
				const std::string &p_idxB
			) {
				return std::stol(m_sections[p_idxA][p_idxB]);
			};

			std::string AsString(
				const std::string &p_idxA,
				const std::string &p_idxB
			) {
				return m_sections[p_idxA][p_idxB];
			};

#ifdef INI_DONT_USE_EXCEPTIONS
			i8 AsBoolean(
				const std::string &p_idxA,
				const std::string &p_idxB
			) {
#else // not INI_DONT_USE_EXCEPTIONS
			bool AsBoolean(
				const std::string &p_idxA,
				const std::string &p_idxB
			) {
#endif // INI_DONT_USE_EXCEPTIONS
				const std::string &val = m_sections[p_idxA][p_idxB];

				if (val == "true")
					return true;
				else if (val == "false")
					return false;

#ifdef INI_DONT_USE_EXCEPTIONS
				m_errorMessage = "Attempt to use ASBoolean() on a non-boolean value";

				return Error;
#else // not INI_DONT_USE_EXCEPTIONS
				throw Exception("Attempt to use AsBoolean() on a non-boolean value");
#endif
			};

#ifdef INI_DONT_USE_EXCEPTIONS
			const std::string &GetErrorMessage() const {
				return m_errorMessage;
			};

			word GetErrorLine() const {
				return m_line;
			};
#endif // INI_DONT_USE_EXCEPTIONS

			std::string Stringify() const {
				std::string stringified = "";

				for (const std::pair<
						const std::string&,
						const Section&
					> &sect : m_sections
				) {
					if (sect.first[0] == ' ' and sect.first.back() == ' ')
						stringified += "[\"" + INIEscape(sect.first) + "\"]\n";
					else
						stringified += "[" + INIEscape(sect.first) + "]\n";

					for (const std::pair<const std::string&, const std::string&> &val : sect.second) {
						if (val.first[0] == ' ' and val.first.back() == ' ')
							stringified += "\"" + INIEscape(val.first) + "\"";
						else
							stringified += INIEscape(val.first);

						stringified += "=";

						if (val.second[0] == ' ' and val.second.back() == ' ')
							stringified += "\"" + INIEscape(val.second) + "\"\n";
						else
							stringified += INIEscape(val.second) + "\n";
					};
				};

				return stringified;
			};

#ifdef INI_DONT_USE_EXCEPTIONS
			ui8 Parse(const std::string &p_text) {
#else // not INI_DONT_USE_EXCEPTIONS
			void Parse(const std::string &p_text) {
#endif // INI_DONT_USE_EXCEPTIONS
				Clear();

				std::string line = "";
				std::string currentSection = DefaultSection;
				word currentLine = 0;

				for (const char &ch : p_text + "\n") {
					switch (ch) {
					case '\n': {
							++ currentLine;
							Trim(line);

							switch (line[0]) {
							// Comment
							case ';': case '#': break;

							// Sections
							case '[': {
									word pos = line.find_first_of("]");

									if (pos == std::string::npos) {
#ifdef INI_DONT_USE_EXCEPTIONS
										m_errorMessage = "Sector name closing expected";

										return Error;
#else // not INI_DONT_USE_EXCEPTIONS
										throw ParserException("Sector name closing expected", currentLine);
#endif // INI_DONT_USE_EXCEPTIONS
									};

									currentSection = line.substr(1, pos - 1);
									Trim(currentSection);
									RemoveApostrophes(currentSection);
								};

								break;

							// Assignment
							default: {
									if (line == "")
										break;

									word pos = 0;

									do {
										pos = line.find_first_of("=", pos + 1);

										if (pos == std::string::npos) {
#ifdef INI_DONT_USE_EXCEPTIONS
											m_errorMessage = "Expected assignment";

											return Error;
#else // not INI_DONT_USE_EXCEPTIONS
											throw ParserException("Expected assignment", currentLine);
#endif // INI_DONT_USE_EXCEPTIONS
										};
									} while (line[pos - 1] == '\\');

									std::string key = line.substr(0, pos);
									std::string value = "";
									line = line.substr(pos + 1);

									if (INIUnescape(std::string(key), key)) {
#ifdef INI_DONT_USE_EXCEPTIONS
										m_errorMessage = "Expected assignment";

										return Error;
#else // not INI_DONT_USE_EXCEPTIONS
										throw ParserException("Expected assignment", currentLine);
#endif // INI_DONT_USE_EXCEPTIONS
									};

									INIUnescape(line, value);

									Trim(key); Trim(value);
									RemoveApostrophes(key); RemoveApostrophes(value);

									if (key == "") {
#ifdef INI_DONT_USE_EXCEPTIONS
										m_errorMessage = "Key name expected";

										return Error;
#else // not INI_DONT_USE_EXCEPTIONS
										throw ParserException("Key name expected", currentLine);
#endif // INI_DONT_USE_EXCEPTIONS
									};

									m_sections[currentSection][key] = value;
								};

								break;
							};

							line = "";
						};

						break;

					default: line += ch; break;
					};
				};

#ifdef INI_DONT_USE_EXCEPTIONS
				return Ok;
#endif // INI_DONT_USE_EXCEPTIONS
			};

		private:
			void Trim(std::string &p_str) {
				word start = p_str.find_first_not_of(" \t");

				if (start == std::string::npos) {
					p_str = "";

					return;
				};

				word end = p_str.find_last_not_of(" \t");

				p_str = p_str.substr(start, end + 1 - start);
			};

			void RemoveApostrophes(std::string &p_str) {
				if (p_str[0] == '"' and p_str.back() == '"')
					p_str = p_str.substr(1, p_str.length() - 2);
			};

			std::string INIEscape(const std::string &p_str) const {
				std::string escaped = "";

				for (word i = 0; i < p_str.length(); ++ i)
					switch (p_str[i]) {
					case '\n': escaped += "\\n"; break;
					case '\r': escaped += "\\r"; break;
					case '\t': escaped += "\\t"; break;
					case '\f': escaped += "\\f"; break;
					case '\b': escaped += "\\b"; break;
					case '\a': escaped += "\\a"; break;
					case '\0': escaped += "\\0"; break;
					case '"': escaped += "\\\""; break;

					default: escaped += p_str[i]; break;
					};

				return escaped;
			};

			bool INIUnescape(const std::string &p_line, std::string &p_str) {
				bool escape = false;
				p_str = "";

				for (const char &ch : p_line) {
					if (escape) {
						escape = false;

						switch (ch) {
						case 'n':  p_str += '\n'; break;
						case 'r':  p_str += '\r'; break;
						case 't':  p_str += '\t'; break;
						case 'f':  p_str += '\f'; break;
						case 'b':  p_str += '\b'; break;
						case 'a':  p_str += '\a'; break;
						case '0':  p_str += '\0'; break;
						case '"':  p_str += '\"'; break;
						case '\\': p_str += '\\'; break;
						case ';':  p_str += ';';  break;
						};
					};

					switch (ch) {
					case ';': case '#': return true;
					case '\\': escape = true; break;
					default: p_str += ch; break;
					};
				};

				return false;
			};

			Sections m_sections;

#ifdef INI_DONT_USE_EXCEPTIONS
			std::string m_errorMessage;
			word m_line;
#endif // INI_DONT_USE_EXCEPTIONS
	}; // class Structure
}; // namespace INI
