#pragma once
#include "ECP.h"

class Sudo
{
public:
	Sudo(void);
	~Sudo(void);

	const std::vector<int>& get_block()const{return block;};

	void fill_in(int i, int j, int num);
	bool solve();

private:
	std::vector<int> block;

	ECP _ecp;
};

