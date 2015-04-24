/*
 *  NOTICE
 *  This MUST be compiled using `-std=c++11` or `-std=gnu-c++11` (not tested)
 */

#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

class InvalidIndexException: public exception {

	private:

		int accessX;
		int accessY;

	public:

		InvalidIndexException(int x, int y) {
			accessX = x;
			accessY = y;
		}

		virtual const char* what() {
			return std::string("The position (" + to_string(accessX) + ", " + to_string(accessY) + ") was out of bounds").c_str();
		}

}; 

class Matrix {

	private:

		tuple<int, int> dimensions;
		vector<vector<int>> content;

	public:

		Matrix(int dx, int dy) {
			dimensions = make_tuple(dx, dy);
		}

		int getRowCount() {
			return std::get<0>(dimensions);
		}

		int getColCount() {
			return std::get<1>(dimensions);
		}

		void set(int row, int col, int value) {
			if(col > getColCount() || row > getRowCount()) throw InvalidIndexException(row, col);
			content[row][col] = value;
		}

		int get(int row, int col) {
			if(col > getColCount() || row > getRowCount()) throw InvalidIndexException(row, col);
			return content[row][col];
		}

		int sum() {
			int total = 0;
			vector<vector<int> >::iterator row = content.begin();
			while(row != content.end()) {
				vector<int>::iterator column = row->begin();
				while(column != row->end()) {
					total += *column;
					++column;
				}
				++row;
			}

			return total;
		}

		vector<int> getRow(int row) {
			if(row > getRowCount()) throw InvalidIndexException(0, row);
			return content[row];
		}

		vector<vector<int> > getUpperHalf() {
			const int halfSize = (getRowCount() / 2);
			vector<vector<int> >::iterator row = content.begin();
			vector<vector<int> > half;
			for(int idx = 0; idx < halfSize; ++idx) half.push_back(*row++);
			return half;
		}

		vector<vector<int>> getLowerHalf() {
			const int halfSize = (getRowCount() / 2);
			vector<vector<int> >::iterator row = content.end();
			vector<vector<int> > half;
			for(int idx = 0; idx < halfSize; ++idx) half.push_back(*row--);
			return half;
		}

		void dump(ostream &stream) {
			vector<vector<int>>::iterator row = content.begin();
			while(row != content.end()) {
				vector<int>::iterator col = row->begin();
				while(col != row->end()) stream << *col++ << " ";
				++row;
			}
			stream << endl;
		}

};

int main(int argc, char** argv) {

	++argv;	// Toss out command
	if(argc < 3) { 
		cerr << "Too few arguments. At least two arguments must be specified (rows, cols, values...)" << endl;
		return 1;
	}

	{
		int dimRows	= atoi(*argv++);
		int dimCols	= atoi(*argv++);

		Matrix model(dimRows, dimCols);

		if((argc - 3) != (dimRows * dimCols)) {
			cerr << "Uneven matrix: expected " << (dimRows * dimCols) << " but got " << (argc - 3) << endl;
			return 2;
		}

		for(int row = 0; row < dimRows; ++row) {
			for(int col = 0; col < dimCols; ++col) {
				int positionValue = atoi(*argv++);
				model.set(row, col, positionValue);
			}
		}


		// Matrix output portion	
		model.dump(std::cout);

		cout << "Matrix sum: " << model.sum() << endl;

		vector<vector<int>> half = model.getUpperHalf();

		{
			vector<vector<int>>::iterator row = half.begin();
			while(row != half.end()) {
				vector<int>::iterator col = row->begin();
				while(col != row->end()) cout << *col++ << " " << endl;
				++row;
			}
		}

	}

	return 0;
}

/* vim: */
