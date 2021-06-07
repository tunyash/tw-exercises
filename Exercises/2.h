#include <vector>   
#include <string>
#include <boost/graph/adjacency_list.hpp>

class PathDecomposition {

	typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

	std::vector<std::vector<int>> _bags;
	Graph _g;

public:

	struct CorectnessException : public std::exception{
	/*
	Thrown when path-width decomposition is incorrect
	*/
	private:
	public:
		int _error_type;
		int _i, _j, _k, _u;

		CorectnessException(int error_type);
		/*
		@error_type == 0 used when in graph |_g| exists edge (v, u) but in |_bags| there is no bag that contains {v, u}
		@error_type == 1 used when in |_bags| exists vertex |v| that does not belong to [0; |V(_g)|)
		@error_type == 3 used when exists vertex |v| that does not belong to |_bags|
		*/
		CorectnessException(int i, int j, int k, int u);
		/*
		This is used only for error_type == 2
		@error_type == 2 used when _bags[i] and _bags[k] contains vertex |v| but _bags[j] does not.
		i < j < k
		*/
		~CorectnessException() throw() {}
		const char* what() const throw();
	};

	PathDecomposition(std::vector<std::vector<int>> bags, Graph g);
	/*
	Constructor creates path-width decomposition where |path[i]| is i-th bag of decomposition.
	|path| is path-width decompostion of graph |g|.
	@exceptions throw CorrectnessException if decomposition is not correct
	*/

private:

	bool Check();
	/*
		Function checks correctness of path-width decomposition |this->_path| of graph |this->_g|.
		@return true if decomposition is correct false otherwise
	*/


};

