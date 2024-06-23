#include <Server.hpp>

std::vector<std::string>  ft_split(std::string str, char delimiter)
{
	std::vector<std::string> result;
	std::string word;

	for (size_t i = 0; i < str.length(); ++i)
	{
		if(str[i] != delimiter)
		{
			word += str[i];
		}
		else
		{
			if(!word.empty())
			{
				result.push_back(word);
				word.clear();
			}
		}
	}
	if(!word.empty())
	{
		result.push_back(word);
	}
	return result;
}


//function to remove and split and remove all the spaces
std::vector<std::string> remove_spaces(std::string &str)
{
	std::vector<std::string> result;
	std::string word;

	for (size_t i = 0; i < str.length(); ++i)
	{
		if(str[i] != ' ')
		{
			word += str[i];
		}
		else
		{
			if(!word.empty())
			{
				result.push_back(word);
				word.clear();
			}
		}
	}
	if(!word.empty())
	{
		result.push_back(word);
	}
	return result;
}