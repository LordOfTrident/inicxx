/*
 *  inicxx - A C++ INI file parser and writer
 *  Version 1.2.3
 *  Github: https://github.com/LordOfTrident/inicxx
 *
 *  Examples under examples/ folder, read README.md on
 *  how to compile them
 */

#ifndef __INICXX_HH_HEADER_GUARD__
#define __INICXX_HH_HEADER_GUARD__

#include <string> // std::string, std::basic_string
#include <unordered_map> // std::unordered_map
#include <cstdint> // std::int8_t, std::int16_t, std::int32_t, std::int64_t,
                   // std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t
#include <cstddef> // std::size_t

#define INI_VERSION_MAJOR 1
#define INI_VERSION_MINOR 2
#define INI_VERSION_PATCH 3

namespace INI {
	// Type aliases for shorter and readable code
	using s8  = std::int8_t;
	using s16 = std::int16_t;
	using s32 = std::int32_t;
	using s64 = std::int64_t;

	using u8  = std::uint8_t;
	using u16 = std::uint16_t;
	using u32 = std::uint32_t;
	using u64 = std::uint64_t;

	using usize = size_t;

	using Section  = std::unordered_map<std::string, std::string>;
	using Sections = std::unordered_map<std::string, Section>;

	class Exception {
		public:
			Exception(const std::string &p_message):
				m_message(p_message)
			{};

			~Exception() {};

			const std::string& What() const {
				return m_message;
			};

		protected:
			std::string m_message;
	}; // class Exception

	class ParserException: public Exception {
		public:
			ParserException(
				const std::string &p_message,
				usize p_line
			):
				Exception(p_message),
				m_line(p_line)
			{};

			~ParserException() {};

			usize Line() const {
				return m_line;
			};

		protected:
			usize m_line;
	}; // class ParserException

	template <class CharT>
	class Structure {
	private:
		using string = std:basic_string<CharT>;

	public:
		// Constants for defaults
		static constexpr const char
			DefaultSection[1] = "";

		Structure() {};

		Structure(const Sections &p_sections) {
			m_sections = p_sections;
		};

		~Structure() {};

		Structure& operator=(const Structure &p_structure) {
			m_sections = p_structure.m_sections;

			return *this;
		};

		// Index functions
		Section& operator[](const string &p_idx) {
			return m_sections[p_idx];
		};

		Section& At(const string &p_idx) {
			return m_sections.at(p_idx);
		};

		string& At(
			const string &p_idxA,
			const string &p_idxB
		) {
			return m_sections.at(p_idxA).at(p_idxB);
		};

		bool Contains(const string &p_idx) const {
			return m_sections.count(p_idx);
		};

		bool Contains(
			const string &p_idxA,
			const string &p_idxB
		) const {
			if (m_sections.count(p_idxA))
				if (m_sections.at(p_idxA).count(p_idxB))
					return true;

			return false;
		};

		void Clear() {
			m_sections.clear();
		};

		void Clear(const string &p_idx) {
			m_sections[p_idx].clear();
		};

		// Conversion functions
		double AsNumber(
			const string &p_idxA,
			const string &p_idxB
		) {
			return std::stod(m_sections[p_idxA][p_idxB]);
		};

		long AsInteger(
			const string &p_idxA,
			const string &p_idxB
		) {
			return std::stol(m_sections[p_idxA][p_idxB]);
		};

		string AsString(
			const string &p_idxA,
			const string &p_idxB
		) {
			return m_sections[p_idxA][p_idxB];
		};

		bool AsBoolean(
			const string &p_idxA,
			const string &p_idxB
		) {
			const string &val = m_sections[p_idxA][p_idxB];

			if (val == "true")
				return true;
			else if (val == "false")
				return false;

			throw Exception("Attempt to use AsBoolean() on a non-boolean value");
		};

		string Stringify() const {
			string stringified = "";

			for (const std::pair<
					const string&,
					const Section&
				> &sect : m_sections
			) {
				if (sect.first[0] == ' ' and sect.first.back() == ' ')
					stringified += "[\"" + INIEscape(sect.first) + "\"]\n";
				else
					stringified += "[" + INIEscape(sect.first) + "]\n";

				for (
					const std::pair<
						const string&, const string&
					> &val :
					sect.second
				) {
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

		void Parse(const string &p_text) {
			Clear();

			string line = "";
			string currentSection = DefaultSection;
			usize currentLine = 0;

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
								usize pos = line.find_first_of("]");

								if (pos == string::npos)
									throw ParserException(
										"Sector name closing expected",
										currentLine
									);

								currentSection = line.substr(1, pos - 1);
								Trim(currentSection);
								RemoveApostrophes(currentSection);
							};

							break;

						// Assignment
						default: {
								if (line == "")
									break;

								usize pos = 0;

								do {
									pos = line.find_first_of("=", pos + 1);

									if (pos == string::npos)
										throw ParserException(
											"Expected assignment",
											currentLine
										);
								} while (line[pos - 1] == '\\');

								string key = line.substr(0, pos);
								string value = "";
								line = line.substr(pos + 1);

								if (INIUnescape(string(key), key))
									throw ParserException(
										"Expected assignment",
										currentLine
									);

								INIUnescape(line, value);

								Trim(key); Trim(value);
								RemoveApostrophes(key);
								RemoveApostrophes(value);

								if (key == "")
									throw ParserException(
										"Key name expected",
										currentLine
									);

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
		};

	private:
		void Trim(string &p_str) {
			usize start = p_str.find_first_not_of(" \t");

			if (start == string::npos) {
				p_str = "";

				return;
			};

			usize end = p_str.find_last_not_of(" \t");

			p_str = p_str.substr(start, end + 1 - start);
		};

		void RemoveApostrophes(string &p_str) {
			if (p_str[0] == '"' and p_str.back() == '"')
				p_str = p_str.substr(1, p_str.length() - 2);
		};

		string INIEscape(const string &p_str) const {
			string escaped = "";

			for (usize i = 0; i < p_str.length(); ++ i)
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

		bool INIUnescape(
			const string &p_line,
			string &p_str
		) {
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
	}; // class Structure
}; // namespace INI

#endif // __INI_HH_HEADER_GUARD__
