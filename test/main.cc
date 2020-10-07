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
		len++;
		cur++;	
		do {
again:
			if (cur == end) goto do_string;
			c = *cur;
			if (c == '"') {
				len++;
				cur++;
				if (cur == end) goto do_string;
				char next = *cur;
				if (next == '"') {
					len++;
					cur++;
					goto again;
				} else {
					// return back
					break;
				}
			} else {
				len++;
				cur++;
			}
		} while (c != '"');
do_string:
		std::cout << str.substr(pos, len) << std::endl;
	}
	
	return 0;
}
