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
		int type = 0; // 0 or 1
		int i, j, k;
		std::string _msg = "";
	public:
		CorectnessException(std::string msg) : _msg(msg) {}
		~CorectnessException() throw() {}
		const char* what() const throw() {
			return ("Type of exception is " + to_string(type) 
				+ "\n" + _msg).c_str();
		}
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

