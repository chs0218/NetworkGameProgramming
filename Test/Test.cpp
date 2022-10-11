#include <iostream>
#include <fstream>

#define BUFSIZE 512

int main()
{
	std::ifstream in{ "tmp4.txt", std::ios::binary};
	std::ofstream out{ "tmp.txt", std::ios::binary };
	in.seekg(0, std::ios::end);
	int len = in.tellg();
	in.seekg(0, std::ios::beg);

	char buf[BUFSIZE] ="\0";
	while (in)
	{
		in.read(buf, BUFSIZE);
		out.write(buf, sizeof(buf));
		std::cout << len << std::endl;
		std::cout << (float)((float)out.tellp() / (float)len) * 100.0f << std::endl;
	}
}