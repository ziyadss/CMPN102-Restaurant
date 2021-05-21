#include <iostream>
#include <fstream>
#include <time.h>
inline int rand(int x, int y)
{
	return rand() % (y - x + 1) + x;
}

inline void inputGen()
{
	srand(time(nullptr));
	std::ofstream OutFile;
	OutFile.open("InputGenTest.txt", std::ios::out);
	OutFile << rand(2, 4) << " " << rand(2, 4) << " " << rand(2, 4) << std::endl;
	OutFile << rand(2, 4) << " " << rand(5, 9) << " " << rand(2, 5) << " " << rand(6, 10) << " " << rand(3, 6) << " " << rand(7, 11) << std::endl;
	OutFile << rand(10, 40) << " " << rand(3, 6) << " " << rand(7, 11) << " " << rand(3, 6) << " " << rand(7, 11) << " " << rand(2, 4) << " " << rand(5, 10) << std::endl;
	OutFile << (float)rand(5, 40) / 100 << " " << rand(5, 15) << std::endl;
	OutFile << rand(13, 25) << " " << rand(7, 15) << std::endl;

	int  M = rand(70, 120);
	OutFile << M << std::endl;
	int j = 1;
	for (int i = 1; i < M + 1; i++)
	{
		int e = rand(0, 7);
		if (e == 1 && i > 4) //Prom
		{
			j = rand(j, j + 3);
			OutFile << "P " << j << " " << rand(i - 5, i) << " " << rand(50, 250) << std::endl;
		}
		else if (e == 2 && i > 4) //Cancel
		{
			j = rand(j, j + 3);
			OutFile << "X " << j << " " << rand(i - 5, i) << std::endl;
		}
		else
		{
			int t = rand(0, 4);
			std::string ty;
			if (t == 1)
				ty = "V ";
			else if (t == 2)
				ty = "G ";
			else ty = "N ";
			j = rand(j, j + 1);
			OutFile << "R " << ty << j << " " << i << " " << rand(5, 80) << " " << rand(25, 500) << std::endl;
		}
	}
	OutFile.close();
}