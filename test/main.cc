#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	std::string str = "\"{\"\"ok\"\":0,\"\"status\"\":\"\"update not yet implemented\"\"}\"";
	std::string::iterator cur = str.begin();
	std::string::iterator end = str.end();
	unsigned int len = 0;
	unsigned int pos = 0;
	std::string result;
	char c = *cur;
	
	if (c == '"') {
		// get string literal 
		// move further
		cur++;
		pos++;
		do {
again:
			if (cur == end) 
				break;
		
			c = *cur++;
			std::cout << c<< std::endl;
			if (c == '"' && *cur == '"') {
				cur++;
				len += 2;
				goto again;
			}
			len++;			
		} while (c != '"' || cur != end);

		result = str.substr(pos, len);
		std::cout << result << std::endl;	
	}
	
	return 0;
}
