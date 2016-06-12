#include "ECP.h"
#include <algorithm>

ECP::ECP(void)
	:_solver(*this)
{
	clear();
}

ECP::~ECP(void)
{
}

void ECP::clear()
{
	num_elems = 0;
	num_sets = 0;

	for_all([](std::vector<INDEX>& v){v.assign(1, 0);});
}

void ECP::for_all(INDEX_HANDLE h)
{
	h(index_elem);
	h(index_set);
	h(next_in_set);
	h(prev_in_set);
	h(next_in_elem);
	h(prev_in_elem);
}

void ECP::init_index(SIZE ne, SIZE ns, SIZE nn)
{
	num_elems = ne;
	num_sets = ns;

	SIZE n = ne + nn + 1;

	for_all([n](std::vector<INDEX>& v){std::vector<INDEX> tmp(n);v.swap(tmp);});

	for(INDEX i = 0; i <= ne; i++)
	{
		index_elem[i] = i;
		index_set[i] = 0;
		next_in_set[i] = i + 1;
		prev_in_set[i] = (i==0)?ne:(i-1);
		next_in_elem[i] = i;
		prev_in_elem[i] = i;
	}
	next_in_set[ne] = 0;
}

void ECP::init(SIZE ne, const std::vector< std::vector<INDEX> >& sets)
{
	SIZE nn = 0;
	for(const auto& set: sets)
	{
		nn += set.size();
	}

	first_in_set.resize(sets.size());
	
	init_index(ne, sets.size(), nn);
	INDEX t = ne;

	for(INDEX i = 1; i <= sets.size(); i++)
	{
		if(sets[i-1].size() == 0) continue;

		std::vector<INDEX> sorted = sets[i-1];
		std::sort(sorted.begin(), sorted.end());
		INDEX first = t + 1;

		for(INDEX j = 0; j < sorted.size(); j++)
		{
			t++;
			int elem = sorted[j];
			index_elem[t] = elem;
			index_set[t] = i;

			prev_in_elem[t] = prev_in_elem[elem];
			next_in_elem[t] = elem;
			prev_in_elem[elem] = t;
			next_in_elem[prev_in_elem[t]] = t;

			prev_in_set[t] = t - 1;
			next_in_set[t] = t + 1;
		}

		prev_in_set[first] = t;
		next_in_set[t] = first;

		first_in_set[i-1] = first;
	}
}

void ECP::solve(bool get_all_sols /*= false*/)
{
	_solver.init();
	for(const auto& set: chosen_sets)
	{
		_solver.ChooseSet(first_in_set[set-1], true);
	}
	
	_solver.sols.clear();
	_solver.solve_rec(get_all_sols);
	
	sols.swap(_solver.sols);
}

void ECP::choose_a_set(INDEX s)
{
	chosen_sets.push_back(s);
}

void ECP::clear_chosen()
{
	chosen_sets.clear();
}

ECP::solver::solver(const ECP& p)
	:parent(p)
{
}

ECP::solver::~solver(void)
{
	
}

void ECP::solver::init()
{
	ie = parent.index_elem;
	is = parent.index_set;
	pe = parent.prev_in_elem;
	ps = parent.prev_in_set;
	ne = parent.next_in_elem;
	ns = parent.next_in_set;

	iep = ie.data();
	isp = is.data();
	pep = pe.data();
	psp = ps.data();
	nep = ne.data();
	nsp = ns.data();

	fis = parent.first_in_set.data();

	num_elems = parent.num_elems;
	num_sets = parent.num_sets;

	sol_stack.resize(num_sets);
	ssp = sol_stack.data();
	ss_top = 0;
}

bool ECP::solver::solve_rec(bool get_all_sols /*= false*/)
{
	INDEX elem = nsp[0];
	if(elem == 0)
	{
		sols.push_back(std::vector<INDEX>(ssp, ssp + ss_top));
		return !get_all_sols;
	}

	RemoveElem(elem);

	INDEX iter_set;

	iter_set = nep[elem];
	while(iter_set != elem)
	{
		ChooseSet(iter_set);

		if(solve_rec(get_all_sols))
		{
			return true;
		}

		CancelSet(iter_set);

		iter_set = nep[iter_set];
	}

	ResumeElem(elem);
	return false;
}

void ECP::solver::RemoveElem(INDEX elem)
{
	if(psp[nsp[elem]] != elem || nsp[psp[elem]] != elem)
	{
		return;
	}
	psp[nsp[elem]] = psp[elem];
	nsp[psp[elem]] = nsp[elem];

	INDEX iter_set, iter_elem;

	iter_set = nep[elem];
	while(iter_set != elem)
	{
		iter_elem = nsp[iter_set];
		while(iter_elem != iter_set)
		{
			pep[nep[iter_elem]] = pep[iter_elem];
			nep[pep[iter_elem]] = nep[iter_elem];

			iter_elem = nsp[iter_elem];
		}

		iter_set = nep[iter_set];
	}
}

void ECP::solver::ResumeElem(INDEX elem)
{
	if(psp[nsp[elem]] == elem && nsp[psp[elem]] == elem)
	{
		return;
	}
	psp[nsp[elem]] = elem;
	nsp[psp[elem]] = elem;

	INDEX iter_set, iter_elem;

	iter_set = pep[elem];

	while(iter_set != elem)
	{
		iter_elem = nsp[iter_set];
		while(iter_elem != iter_set)
		{
			pep[nep[iter_elem]] = iter_elem;
			nep[pep[iter_elem]] = iter_elem;

			iter_elem = nsp[iter_elem];
		}
		iter_set = pep[iter_set];
	}
}

void ECP::solver::ChooseSet(INDEX node_in_set, bool remove_first /* = false */)
{
	INDEX iter_elem;
	ssp[ss_top] = isp[node_in_set];
	ss_top++;

	if(remove_first)
	{
		RemoveElem(iep[node_in_set]);
	}

	iter_elem = nsp[node_in_set];
	while(iter_elem != node_in_set)
	{
		RemoveElem(iep[iter_elem]);
		iter_elem = nsp[iter_elem];
	}
}

// void ECP::solver::ChooseSet2(INDEX node_in_set, bool remove_first /* = false */)
// {
// 	INDEX iter_elem;
// 	ssp[ss_top] = node_in_set;
// 	ss_top++;
// 
// 	if(remove_first)
// 	{
// 		RemoveElem(iep[node_in_set]);
// 	}
// 
// 	iter_elem = nsp[node_in_set];
// 	while(iter_elem != node_in_set)
// 	{
// 		RemoveElem(iep[iter_elem]);
// 		iter_elem = nsp[iter_elem];
// 	}
// }


void ECP::solver::CancelSet(INDEX node_in_set, bool resume_first /* = false */)
{
	INDEX iter_elem;
	ss_top--;

	iter_elem = psp[node_in_set];
	while(iter_elem != node_in_set)
	{
		ResumeElem(iep[iter_elem]);
		iter_elem = psp[iter_elem];
	}

	
	if(resume_first)
	{
		ResumeElem(iep[node_in_set]);
	}
}

bool ECP::solver::solve_ite(bool get_all_sols /*= false*/)
{
	if(nsp[0] == 0)
	{
		return false;
	}

	for(INDEX i = 0; i < ss_top; i++)
	{
		ssp[i] = fis[ssp[i]-1];
	}

	INDEX first_elem = nsp[0];

	INDEX cur_node = 0;
	while(cur_node != first_elem)
	{
		if(cur_node == 0)
		{
			if(nsp[0] == 0)
			{
				sols.push_back(std::vector<INDEX>(ssp, ssp + ss_top));
				if(!get_all_sols)
				{
					break;
				}
			}

			cur_node = nsp[0];
		}

		cur_node = nep[cur_node];
		if(cur_node <= num_elems)
		{
			if(cur_node == first_elem)
			{
				break;
			}

			ss_top--;
			cur_node = ssp[ss_top];
			//CancelSet(cur_node, nep[cur_node] == iep[cur_node]);
			INDEX iter_in_set;

			bool is_last = (nep[cur_node] == iep[cur_node]);
			iter_in_set = psp[cur_node];

			while((is_last) || (iter_in_set != cur_node))
			{
				//ResumeElem(iep[iter_in_set]);
				INDEX elem = iep[iter_in_set];
				if(psp[nsp[elem]] != elem || nsp[psp[elem]] != elem)
				{
					psp[nsp[elem]] = elem;
					nsp[psp[elem]] = elem;

					INDEX iter_set, iter_elem;

					iter_set = pep[elem];

					while(iter_set != elem)
					{
						iter_elem = nsp[iter_set];
						while(iter_elem != iter_set)
						{
							pep[nep[iter_elem]] = iter_elem;
							nep[pep[iter_elem]] = iter_elem;

							iter_elem = nsp[iter_elem];
						}
						iter_set = pep[iter_set];
					}
				}

				if(iter_in_set == cur_node) 
				{
					break;
				}
				iter_in_set = psp[iter_in_set];
			}
		}
		else
		{

			//ChooseSet2(cur_node, pep[cur_node] == iep[cur_node]);
			ssp[ss_top] = cur_node;
			ss_top++;
			
			INDEX iter_elem;

			bool is_first = (pep[cur_node] == iep[cur_node]);
			iter_elem = is_first?cur_node:nsp[cur_node];

			while((is_first) || (iter_elem != cur_node))
			{
				is_first = false;
				//RemoveElem(iep[iter_elem]);
				INDEX elem = iep[iter_elem];
				if(psp[nsp[elem]] == elem && nsp[psp[elem]] == elem)
				{
					psp[nsp[elem]] = psp[elem];
					nsp[psp[elem]] = nsp[elem];

					INDEX iter_set, iter_elem;

					iter_set = nep[elem];
					while(iter_set != elem)
					{
						iter_elem = nsp[iter_set];
						while(iter_elem != iter_set)
						{
							pep[nep[iter_elem]] = pep[iter_elem];
							nep[pep[iter_elem]] = nep[iter_elem];

							iter_elem = nsp[iter_elem];
						}

						iter_set = nep[iter_set];
					}
				}

				iter_elem = nsp[iter_elem];
			}

			cur_node = 0;
		}
	}

	for(auto& sol: sols)
	{
		for(INDEX i = 0; i < sol.size(); i++)
		{
			sol[i] = isp[sol[i]];
		}
	}

	return false;
}
