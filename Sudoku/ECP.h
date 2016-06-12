#pragma once
#include <vector>
#include <functional>

class ECP
{
public:
	typedef unsigned int SIZE;
	typedef unsigned int INDEX;

	ECP(void);
	virtual ~ECP(void);

	void init(SIZE ne, const std::vector< std::vector<INDEX> >& sets);
	void clear();

	void solve(bool get_all_sols = false);

	const std::vector< std::vector<INDEX> >& get_sols()const{return sols;};

	void choose_a_set(INDEX s);
	void clear_chosen();

private:
	typedef std::function<void(std::vector<INDEX>&)> INDEX_HANDLE;

	SIZE num_sets, num_elems;
	std::vector<INDEX> index_elem, index_set;
	std::vector<INDEX> next_in_elem, prev_in_elem;
	std::vector<INDEX> next_in_set, prev_in_set;
	
	void for_all(INDEX_HANDLE h);
	void init_index(SIZE ne, SIZE ns, SIZE nn);

	std::vector< std::vector<INDEX> > sols;

	std::vector<INDEX> first_in_set;

	std::vector<INDEX> chosen_sets;

	class solver
	{
	public:
		solver(const ECP& p);
		virtual ~solver(void);

		void init();

		bool solve_rec(bool get_all_sols = false);
		bool solve_ite(bool get_all_sols = false);
		std::vector< std::vector<INDEX> > sols;

		void ChooseSet(INDEX node_in_set, bool remove_first = false);
		void CancelSet(INDEX node_in_set, bool resume_first = false);
	private:
		const ECP& parent;

		SIZE num_sets, num_elems;
		std::vector<INDEX> ie, is, pe, ps, ne, ns;

		INDEX *iep, *isp, *pep, *psp, *nep, *nsp;

		const INDEX* fis;

		std::vector<INDEX> sol_stack;
		INDEX *ssp;
		SIZE ss_top;

		void RemoveElem(INDEX elem);
		void ResumeElem(INDEX elem);
/*		void ChooseSet2(INDEX node_in_set, bool remove_first / * = false * /);*/
	}_solver;
};

