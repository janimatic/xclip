#pragma once

#ifndef _STRINGUTILS_H_
#define _STRINGUTILS_H_

#include "IO.h"

class string_utils
{
public:
	//*******	FILTER	**********
	//if (string_utils::wildmatch("bl?h.*", "blah.jpg"))
	static bool wildmatch(const char *wild, const char *line) {
		bool wildcard = 0;
		char* placeholder;
		do
		{
			if ((*wild == *line) || (*wild == '?'))
			{
				line++;
				wild++;
			}
			else if (*wild == '*')
			{
				if (*(++wild) == '\0')
					return true;
				else
				{
					placeholder = (char *)wild;
					wildcard = true;
				}
			}
			else if (wildcard)
			{
				if (wild == placeholder)
					line++;
				else
					wild = placeholder;
			}
			else
				return false;
		} while (*line);
		if (*wild == '\0')
			return true;
		else
			return false;
	}
	//*******	TOKENIZER	**********
	static void tokenize(	const std::string& str,
		std::vector<std::string>& tokens,
					const std::string& delimiters = " ")
	{
		// Skip delimiters at beginning.
		std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
		// Find first "non-delimiter".
		std::string::size_type pos     = str.find_first_of(delimiters, lastPos);
		while (std::string::npos != pos || std::string::npos != lastPos)
		{
			// Found a token, add it to the vector.
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of"
			lastPos = str.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter"
			pos = str.find_first_of(delimiters, lastPos);
		}
	}
	//*******	SEARCH/REPLACE	**********
	static std::string searchAndReplace( const std::string& originalString,	const std::string& searchString, const std::string& replaceString)
	{
		std::string str = originalString;
		if( searchString == replaceString )	{return str;}
		std::string::size_type pos = 0;
		while ( (pos = str.find(searchString, pos)) != std::string::npos ) {
			str.replace( pos, searchString.size(), replaceString );
			pos++;
		}
		return str;
	}
};

#endif
