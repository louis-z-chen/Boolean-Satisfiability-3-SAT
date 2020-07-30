#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include "util.h"

using namespace std;

bool parse(char* filename,vector<vector<int>>& clause_contents,
			map<int,int>& variable_values, int& num_vars, int& num_clauses);

bool parse_out(char* filename, vector<vector<int>>& clause_contents,
			map<int,int>& variable_values);

int one_clause_bool(vector<vector<int>>& clause_content,
					map<int,int>& variable_values, int clause_index);

int all_clause_bool(vector<vector<int>>& clause_content,
					map<int,int>& variable_values);

int main(int argc, char* argv[]){
	if(argc < 3){
		cerr << "Usage ./sat_solver_checker test_file your_output_file" << endl;
	}
	vector<vector<int>> clause_contents;
	map<int, int> variable_values;
	int num_vars = 0;
	int num_clauses = 0;

	// Parse the file
	// 0 = false
	// 1 = true
	// -1 = unassigned
	if(parse(argv[1], clause_contents, variable_values,
			 num_vars, num_clauses) == false){
		cerr << "Unable to open input file" << endl;
		return 1;
	}

	if(parse_out(argv[2], clause_contents, variable_values) == false){
		cerr << "Unable to open input file" << endl;
		return 1;
	}
	
	bool temp = all_clause_bool(clause_contents, variable_values);
	if(temp){
		cout << "your code works" << endl;
	}
	else{
		cout << "FUCKKKKKK" << endl;
	}

	return 0;
}

bool parse(char* filename, vector<vector<int>>& clause_contents,
			map<int,int>& variable_values, int& num_vars, int& num_clauses){
	//read in input file
	ifstream ifile(filename);
	if(ifile.fail()){
		return false;
	}
	//make variable values
	string whole_line;
	while(getline(ifile,whole_line)){
		whole_line = trim(whole_line);
		if(whole_line[0] == 'p'){
			break;
		}
	}
	stringstream ss(whole_line);
	string misc;
	for(int i = 0; i < 2; i++){
		ss >> misc;
	}
	ss >> num_vars;
	ss >> num_clauses;
	int unassigned = -1;
	for(int i = 1; i <= num_vars; i++){
		variable_values.insert({ i, unassigned});
	}
	//make clauses
	for(int i = 0; i < num_clauses; i++){
		getline(ifile,whole_line);
		whole_line = trim(whole_line);
		if(whole_line.empty() == false){
			stringstream ss(whole_line);
			int num;
			vector<int> temp;
			while(ss >> num){
				if(num == 0){
					break;
				}
				temp.push_back(num);
			}
			clause_contents.push_back(temp);
		}
	}
	return true;
}

bool parse_out(char* filename, vector<vector<int>>& clause_contents,
			map<int,int>& variable_values){
	ifstream ifile2(filename);
	if(ifile2.fail()){
		return false;
	}
	string whole_line;
	while(getline(ifile2,whole_line)){
		stringstream ss(trim(whole_line));
		int idx = 0;
		int val = 0;
		string misc;
		ss >> idx;
		ss >> misc;
		ss >> val;
		variable_values[idx] = val;
	}
	return true;
}

int one_clause_bool(vector<vector<int>>& clause_content,
					map<int,int>& variable_values, int clause_index){
	int vector_size = clause_content[clause_index].size();
	vector<int> clause_values;
	for(int i = 0; i < vector_size; i++){
		bool invert = false;
		int temp_var_index = clause_content[clause_index][i];
		if(temp_var_index < 0){
			invert = true;
		}
		temp_var_index = abs(temp_var_index);
		map<int,int>::iterator it = variable_values.find(temp_var_index);
		int temp_var_val = it -> second;
		if(invert == true){
			if(temp_var_val == 0){
				clause_values.push_back(1);
			}
			else if(temp_var_val == 1){
				clause_values.push_back(0);
			}
			else{
				clause_values.push_back(-1);
			}
		}
		else{
			clause_values.push_back(temp_var_val);
		}
	}

	int num_variables = clause_values.size();
	int current_clause_state = 0;
	int num_false = 0;
	for(int i = 0; i < num_variables; i++){
		if(clause_values[i] == 1){
			current_clause_state = 1;
			return current_clause_state;
		}
		else if(clause_values[i] == 0){
			num_false++;
		}
	}
	if(num_false != num_variables){
		current_clause_state = -1;
	}
	return current_clause_state;
}

int all_clause_bool(vector<vector<int>>& clause_content,
					map<int,int>& variable_values){
	int clause_count = clause_content.size();
	vector<int> all_clause;
	for(int i = 0; i < clause_count; i++){
		int clause_bool = one_clause_bool(clause_content,
									 variable_values, i);
		all_clause.push_back(clause_bool);
	}
	int all_clause_status = 0;
	int num_true = 0;
	for(unsigned int i = 0; i < all_clause.size(); i++){
		if(all_clause[i] == 0){
			all_clause_status = 0;
			return all_clause_status;
		}
		else if(all_clause[i] == 1){
			num_true++;
		}
	}
	if(num_true != clause_count){
		all_clause_status = -1;
		return all_clause_status;
	}
	else{
		all_clause_status = 1;
		return all_clause_status;
	}
}


