#include "Sudo.h"
#include <assert.h>

Sudo::Sudo(void)
{
	std::vector< std::vector<ECP::INDEX> > sudosets;

	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			for(int k = 0; k < 9; k++)
			{
				ECP::INDEX tmp[] = 
				{
					i*9 + j + 1,
					i*9 + k + 82,
					j*9 + k + 163,
					(i/3*3 + j/3)*9 + k + 244
				};
				sudosets.push_back(std::vector<ECP::INDEX>(tmp, tmp + 4));
			}
		}
	}

	block.assign(81, 0);
	_ecp.init(324, sudosets);
}


Sudo::~Sudo(void)
{
}

bool Sudo::solve()
{
	std::vector<bool> check(10);
	//row
	for(int i = 0; i < 9; i++)
	{
		check.assign(10, false);
		for(int j = 0; j < 9; j++)
		{
			int tmp = block[i*9+j];
			if(!check[tmp] || tmp == 0)
			{
				check[tmp] = true;
			}
			else
			{
				return false;
			}
		}
	}

	//col
	for(int i = 0; i < 9; i++)
	{
		check.assign(10, false);
		for(int j = 0; j < 9; j++)
		{
			int tmp = block[j*9+i];
			if(!check[tmp] || tmp == 0)
			{
				check[tmp] = true;
			}
			else
			{
				return false;
			}
		}
	}

	//3x3
	for(int i = 0; i < 9; i++)
	{
		check.assign(10, false);
		for(int j = 0; j < 9; j++)
		{
			int tmp = block[(i/3*3+j/3)*9+(i%3*3+j%3)];
			if(!check[tmp] || tmp == 0)
			{
				check[tmp] = true;
			}
			else
			{
				return false;
			}
		}
	}

	_ecp.solve();

	const auto& sol = _ecp.get_sols();
	if(sol.empty())
	{
		return false;
	}

	for(const auto& line: sol[0])
	{
		int t = line - 1;
		int i = t/81;
		int j = t/9 - i*9;
		int k = t - i*81 - j*9;

		block[i*9+j] = k+1;
	}
	return true;
}

void Sudo::fill_in(int i, int j, int num)
{
	block[i*9+j-10] = num;
	_ecp.choose_a_set(i*81+j*9+num-90);
}
