#include <bits/stdc++.h>

using namespace std;


struct TreeNode
{
	int value;
	vector<TreeNode*> children;
};



int index_for_sortcol = 0;
bool sortcol( const vector<int>& v1,
               const vector<int>& v2 ) {
 return v1[index_for_sortcol] < v2[index_for_sortcol];
}

TreeNode* copy_branch(TreeNode* root)
{
	int i;
	TreeNode* new_node = new TreeNode();
	new_node -> value = root -> value;
	for(i = 0; i < (root -> children).size(); i++)
		(new_node -> children).push_back(copy_branch((root -> children)[i]));
	return new_node;
}

float find_entropy(vector<int>&input_array)
{
	int i, denominator = 0;
	float sum = 0.0;
	for(i = 0; i < input_array.size(); i++)
	{
		// cout << input_array[i] << " ";
		denominator += input_array[i];
	}
	// cout << endl;
	// cout << log2(float(input_array[0]) / denominator) << endl;
	for(i = 0; i < input_array.size(); i++)
	{
		if(input_array[i] != 0)
			sum += (float(input_array[i]) / denominator) * log2(float(input_array[i]) / denominator);
	}
	return -1 * sum;
}

int find_best_attribute(vector< vector<int> >&training_dataset, int target_attribute, vector<int> &current_attributes, vector<int> &no_of_values_in_attribute)
{
	int i, j, k, l, required_attribute = current_attributes[0], flag = 0, sum = 0, flager = 0;
	float entropy_sum = 0.0, entropy = 0.0, miner = 0.0;
	vector<int> count_of_different_target_attributes_in_training_dataset(no_of_values_in_attribute[target_attribute], 0);
	// for(i = 0; i < training_dataset.size(); i++)
	// 	count_of_different_target_attributes_in_training_dataset[training_dataset[i][target_attribute]]++;
	// entropy = find_entropy(count_of_different_target_attributes_in_training_dataset);
	for(i = 0; i < current_attributes.size(); i++)
	{
		entropy_sum = 0.0;
		vector<vector<int> >temp(no_of_values_in_attribute[current_attributes[i]], vector<int>(no_of_values_in_attribute[target_attribute], 0));
	
		for(j = 0; j < training_dataset.size(); j++)
			temp[training_dataset[j][current_attributes[i]]][training_dataset[j][target_attribute]]++;

		for(j = 0; j < temp.size(); j++)
		{
			sum = 0;
			for(k = 0; k < temp[j].size(); k++)
				sum += temp[j][k];
			entropy_sum += (float(sum) / training_dataset.size()) * find_entropy(temp[j]);
			// cout << find_entropy(temp[j]) << endl;
		}
		// cout << entropy_sum << " ";
		if(flager == 0 || entropy_sum < miner)
		{
			miner = entropy_sum;
			required_attribute = current_attributes[i];
			flager = 1;
		}
	}
	// cout << endl;
	return required_attribute;
}

int most_common_value_of_target_attribute(vector< vector<int> >&training_dataset, int target_attribute, vector<int>&no_of_values_in_attribute)
{
	int i, j;
	int most_common_value_index = 0, maxer = 0;
	vector<int> count_of_different_target_attributes_in_training_dataset(no_of_values_in_attribute[target_attribute], 0);
	for(i = 0; i < training_dataset.size(); i++)
		count_of_different_target_attributes_in_training_dataset[training_dataset[i][target_attribute]]++;
	for(i = 0; i < no_of_values_in_attribute[target_attribute]; i++)
	{
		if(count_of_different_target_attributes_in_training_dataset[i] > maxer)
		{
			maxer = count_of_different_target_attributes_in_training_dataset[i];
			most_common_value_index = i;
		}
	}
	return most_common_value_index;
}

TreeNode* id3(vector< vector<int> >&training_dataset, int target_attribute, vector<int> &current_attributes, vector<int> &no_of_values_in_attribute, bool create_forest)
{
	int i,j,  flag = 0, best_attribute, num;
	// Checking if all the examples have same output
	int output = training_dataset[0][target_attribute];
	for(i = 0; i < training_dataset.size(); i++)
	{
		if(flag == 0 && training_dataset[i][target_attribute] == output) continue;
		else
		{
			flag = 1;
			break;
		}
	}
	if(flag == 0)		// All the examples have same output
	{
		// cout << training_dataset.size() << "helloooooo" << endl;
		// cout << "hello" << endl;
		TreeNode* leaf = new TreeNode();
		leaf -> value = output;
		return leaf;
	}

	if(current_attributes.size() == 0)
	{
		TreeNode* leaf = new TreeNode();
		leaf -> value = most_common_value_of_target_attribute(training_dataset, target_attribute, no_of_values_in_attribute);
		return leaf;
	}

	if(create_forest == true)
	{
		srand(time(NULL));
		vector<int> random_attributes;
		vector<int> temper_attributes;
		for(i = 0; i < current_attributes.size(); i++)
			temper_attributes.push_back(current_attributes[i]);

		
		for(i = 0; i <= log2(current_attributes.size()); i++)
		{
			num = rand() % temper_attributes.size();
			random_attributes.push_back(temper_attributes[num]);
			temper_attributes.erase(temper_attributes.begin() + num);
		}
		best_attribute = find_best_attribute(training_dataset, target_attribute, random_attributes, no_of_values_in_attribute);

	}
	else
		best_attribute = find_best_attribute(training_dataset, target_attribute, current_attributes, no_of_values_in_attribute);
	// cout << best_attribute << "ye best hai" << endl;
	TreeNode* root = new TreeNode();
	root -> value = best_attribute;

	for(i = 0; i < current_attributes.size(); i++)
	{
		if(current_attributes[i] == best_attribute)
		{
			current_attributes.erase(current_attributes.begin() + i);
			break;
		}
	}
	
	vector<int> new_current_attributes(current_attributes.size(), 0);
	for(i = 0; i < current_attributes.size(); i++)
		new_current_attributes[i] = current_attributes[i];

	// for(i = 0; i < current_attributes.size(); i++)
	// 	cout << current_attributes[i] << " ";
	// cout << endl;

	for(i = 0; i < no_of_values_in_attribute[best_attribute]; i++)
	{
		vector< vector<int> > new_training_dataset;
		for(j = 0; j < training_dataset.size(); j++)
		{
			if(training_dataset[j][best_attribute] == i)			
				new_training_dataset.push_back(training_dataset[j]);
		}
		if(new_training_dataset.size() == 0)
		{
			TreeNode* leaf = new TreeNode();
			leaf -> value = most_common_value_of_target_attribute(training_dataset, target_attribute, no_of_values_in_attribute);
			(root -> children).push_back(leaf);
		}
		else
		{
			// cout << "hello there" << endl;
			// cout << new_training_dataset.size() << "broo" << endl;
			(root -> children).push_back(id3(new_training_dataset, target_attribute, new_current_attributes, no_of_values_in_attribute, create_forest));
			// cout << (root -> children).size() << "blah" << endl;
		}
		// cout << "hello" << endl;
	}
	// cout << (root -> children).size() << "blah" << endl;
	return root;
}

void print_level_order_tree(TreeNode* root, int target_attribute, vector< vector<string> >&possible_values_for_attribute)
{
	int i;
	if((root -> children).size() == 0)
	{
		cout << "helloooooooooo";
		cout << possible_values_for_attribute[target_attribute][root -> value];
		return;
	}
	queue<TreeNode *> q;
	q.push(root);
	while(1)
	{
		int node_count = q.size();
		if(node_count == 0) break;

		while(node_count > 0)
		{
			TreeNode* node = q.front();

			if((node -> children).size() == 0)
				cout << possible_values_for_attribute[target_attribute][node -> value] << " ";
			else
				cout << node -> value << " ";
			q.pop();
			for(i = 0; i < (node -> children).size(); i++)
				q.push((node -> children)[i]);
			node_count--;
		}
		cout << endl;
	}
	return;
}


vector<vector<int> > handle_continuous_valued_input(vector< vector<int> >&dataset, int no_of_attributes,int target_attribute, vector<int> &no_of_values_in_attribute, vector<int>&threshold_for_continuous_valued_input)
{
	int i, j, flager = 0, k, l, sum;
	float mid, miner, entropy_sum;
	vector< vector<int> >temp_dataset;
	for(i = 0; i < dataset.size(); i++)
		temp_dataset.push_back(dataset[i]);
	if(temp_dataset.size() == 0) return temp_dataset;
	for(i = 0; i < no_of_attributes - 1; i++)
	{
		miner = 0.0;
		flager = 0;
		if(no_of_values_in_attribute[i] != -1)
			continue;

		index_for_sortcol = i;
		sort(temp_dataset.begin(), temp_dataset.end(), sortcol);
		cout << "Handling continuous valued input for attribute no.: " << i << endl;
		for(j = 1; j < temp_dataset.size(); j++)
		{
			if(temp_dataset[j-1][target_attribute] != temp_dataset[j][target_attribute])
			{
				entropy_sum = 0.0;
				if(temp_dataset[j-1][i] == -1 || temp_dataset[j][i] == -1) continue;
				
				mid = (temp_dataset[j-1][i] + temp_dataset[j][i]) / 2;
				vector< vector<int> >temp(2, vector<int>(no_of_values_in_attribute[target_attribute], 0));
				for(k = 0; k < temp_dataset.size(); k++)
				{
					if(temp_dataset[k][i] != -1)
					{
						if(temp_dataset[k][i] <= mid)
							temp[0][temp_dataset[k][target_attribute]]++;
						else
							temp[1][temp_dataset[k][target_attribute]]++;
					}
				}

				for(k = 0; k < temp.size(); k++)
				{
					sum = 0;
					for(l = 0; l < temp[k].size(); l++)
						sum += temp[k][l];
					entropy_sum += (float(sum) / temp_dataset.size()) * find_entropy(temp[k]);
				}
		
				// cout << entropy_sum << " ";
				if(flager == 0 || entropy_sum < miner)
				{
					miner = entropy_sum;
					threshold_for_continuous_valued_input[i] = mid;
					flager = 1;
				}
			}
		}
		for(j = 0; j < temp_dataset.size(); j++)
		{
			if(temp_dataset[j][i] != -1)
			{
				if(temp_dataset[j][i] <= threshold_for_continuous_valued_input[i])
					temp_dataset[j][i] = 0;
				else
					temp_dataset[j][i] = 1;
			}
		}
		no_of_values_in_attribute[i] = 2;
		// possible_values_for_attribute[i].push_back("a");
		// possible_values_for_attribute[i].push_back("b");
	}
	return temp_dataset;
}


vector< vector<int> > predict_missing_values(vector< vector<int> >&training_dataset, int no_of_attributes, int target_attribute, vector<int> &no_of_values_in_attribute)
{
	int i, j, k, l, flag, value_index, maxer;
	for(i = 0; i < no_of_attributes-1; i++)
	{
		cout << "Predicting missing values in attribute no.: " << i << endl;
		vector< vector<int> > temp(no_of_values_in_attribute[target_attribute], vector<int>(no_of_values_in_attribute[i], 0));
		for(j = 0; j < training_dataset.size(); j++)
		{
			if(training_dataset[j][i] != -1)
				temp[training_dataset[j][target_attribute]][training_dataset[j][i]]++;
		}
		vector<int> index_with_max_occurance(temp.size(), 0);
		for(j = 0; j < temp.size(); j++)
		{
			maxer = -1;
			for(k = 0; k < temp[j].size(); k++)
			{
				if(temp[j][k] > maxer)
				{
					maxer = temp[j][k];
					value_index = k;
				}
			}
			index_with_max_occurance[j] = value_index;
		}
		for(j = 0; j < training_dataset.size(); j++)
		{
			if(training_dataset[j][i] == -1)
				training_dataset[j][i] = index_with_max_occurance[training_dataset[j][target_attribute]];
		}
	}
	return training_dataset;
}


vector< vector<int> >create_testing_matrix(TreeNode* root, vector< vector<int> >&testing_data, vector<int>&no_of_values_in_attribute, int target_attribute)
{
	int i, j, target_value_from_testing_example, target_value_from_tree, branch_no, attribute;
	TreeNode* node = new TreeNode();
	vector< vector<int> >testing_matrix(no_of_values_in_attribute[target_attribute], vector<int>(no_of_values_in_attribute[target_attribute], 0));

	for(i = 0; i < testing_data.size(); i++)
	{
		target_value_from_testing_example = testing_data[i][target_attribute];
		node = root;
		while((node -> children).size() > 0)
		{
			branch_no = testing_data[i][node -> value];
			node = (node -> children)[branch_no];
		}
		target_value_from_tree = node -> value;
		testing_matrix[target_value_from_tree][target_value_from_testing_example]++;
	}
	return testing_matrix;
}


float find_accuracy(TreeNode* root, vector< vector<int> >&testing_data, vector<int>&no_of_values_in_attribute, int target_attribute)
{
	int i, j, sum_for_accuracy = 0, total_matrix_sum = 0;
	vector< vector<int> >testing_matrix;
	testing_matrix = create_testing_matrix(root, testing_data, no_of_values_in_attribute, target_attribute);
	for(i = 0; i < no_of_values_in_attribute[target_attribute]; i++)
	{
		for(j = 0; j < no_of_values_in_attribute[target_attribute]; j++)
		{
			if(i == j) sum_for_accuracy += testing_matrix[i][j];
			total_matrix_sum += testing_matrix[i][j];
		}
	}
	float accuracy = (float)sum_for_accuracy / (float)total_matrix_sum;
	return accuracy;
}

// Basically implementing reverse level order traversal in the following function
TreeNode* reduced_error_pruning(TreeNode* root, vector< vector<int> >&testing_data, vector<int>&no_of_values_in_attribute, int target_attribute)
{
	int i, target_index, temp_value;
	float accuracy, max_accuracy;
	TreeNode* node = new TreeNode();
	node = root;
	stack <TreeNode*> s;
	queue <TreeNode*> q;
	q.push(node);

	while(q.empty() == false)
	{
		node = q.front();
		q.pop();
		s.push(node);

		for(i = (node -> children).size() - 1; i >= 0; i--)
			q.push((node -> children)[i]);
	}
	max_accuracy = find_accuracy(root, testing_data, no_of_values_in_attribute, target_attribute);
	while(s.empty() == false)
	{
		target_index = -1;
		node = s.top();
		TreeNode* temp = new TreeNode();
		vector<TreeNode*> children_array;
		temp_value = node -> value;
		for(i = 0; i < (node -> children).size(); i++)
			children_array.push_back((node -> children)[i]);
		
		// temp = copy_branch(node);
		for(i = 0; i < no_of_values_in_attribute[target_attribute]; i++)
		{
			node -> value = i;
			(node -> children).erase((node -> children).begin(), (node -> children).end());
			accuracy = find_accuracy(root, testing_data, no_of_values_in_attribute, target_attribute);
			if(accuracy > max_accuracy)
			{
				max_accuracy = accuracy;
				target_index = i;
			}
		}
		if(target_index != -1)
			node -> value = target_index;
		else
		{
			node -> value = temp_value;
			for(i = 0; i < children_array.size(); i++)
				(node -> children).push_back(children_array[i]);
		}
		s.pop();
	}
	return root;
}


vector< vector<int> > handle_continuous_valued_testing_data(vector< vector<int> >&testing_data, vector<int>&threshold_for_continuous_valued_input, int no_of_attributes)
{
	int i, j;
	for(i = 0; i < no_of_attributes - 1; i++)
	{
		if(threshold_for_continuous_valued_input[i] != -1)
		{
			for(j = 0; j < testing_data.size(); j++)
			{
				if(testing_data[j][i] != -1)
				{
					if(testing_data[j][i] <= threshold_for_continuous_valued_input[i])
						testing_data[j][i] = 0;
					else
						testing_data[j][i] = 1;
				}
			}
			cout << endl;
		}
	}
	cout << "returning" << endl;
	return testing_data;
}

vector< vector<int> > map_dataset(vector<vector<string> >&original_dataset , vector< vector<string> >&possible_values_for_attribute, vector<int> &previous_no_of_values_in_attribute)
{
	int i, j, k;
	vector< vector<int> > modified_dataset;
	for(i = 0; i < original_dataset.size(); i++)
	{
		vector<int>data_row;
		for(j = 0; j < original_dataset[i].size(); j++)
		{
			if(original_dataset[i][j] == "?")
			{
				data_row.push_back(-1);
				continue;
			}
			if(previous_no_of_values_in_attribute[j] == -1)
			{
				int result;
				stringstream(original_dataset[i][j]) >> result;
				data_row.push_back(result);
				continue;
			}
			string temp = original_dataset[i][j];
			for(k = 0; k < possible_values_for_attribute[j].size(); k++)
			{
				if(temp == possible_values_for_attribute[j][k])
				{
					data_row.push_back(k);
					break;
				}
			}
		}
		modified_dataset.push_back(data_row);
	}
	return modified_dataset;
}


int find_mode(vector<int> &arr)
{
    int i;
    int n = arr.size();
    for(i = 0; i< n; i++)
        arr[arr[i]%n] += n;
    int max = arr[0], result = 0;
    for (i = 1; i < n; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
            result = i;
        }
    }
    return result;
}

void random_forest(vector< vector<int> > &modified_dataset, vector< vector<int> > &testing_dataset, vector<int>&no_of_values_in_attribute, int target_attribute, int no_of_attributes)
{
	int forest_length = 5, correct = 0, branch_no;
	vector< vector<int> >target_value_from_tree(modified_dataset.size(), vector<int>(forest_length, 0));
	vector<int> mode_value(modified_dataset.size(), 0);
	srand(time(NULL));
	int i, num, j;
	int data_length = modified_dataset.size();
	vector< vector<int> >random_dataset;
	vector<int> current_attributes(no_of_attributes-1, 0);
	vector<TreeNode*> forest;
	TreeNode* node = new TreeNode();
	// cout << "ok here" << endl;
	for(i = 0; i < no_of_attributes - 1; i++)
	 	current_attributes[i] = i;
	for(i = 0; i < forest_length; i++)
	{
		for(j = 0; j < data_length; j++)
		{
			num = rand() % data_length;
			random_dataset.push_back(modified_dataset[num]);
		}
		vector<int> current_attributes(no_of_attributes-1, 0);
		for(j = 0; j < no_of_attributes - 1; j++)
	 		current_attributes[i] = j;
		forest.push_back(id3(modified_dataset, no_of_attributes - 1, current_attributes, no_of_values_in_attribute, true));
	}
	// cout << "ok here too" << endl;
	for(i = 0; i < testing_dataset.size(); i++)
	{
		for(j = 0; j < forest_length; j++)
		{
			node = forest[j];
			while((node -> children).size() > 0)
			{
				branch_no = testing_dataset[i][node -> value];
				node = (node -> children)[branch_no];
			}
			target_value_from_tree[i][j] = node -> value;
		}
		mode_value[i] = find_mode(target_value_from_tree[i]);
	}
	// cout << "ok here too too" << endl;
	for(i = 0; i < testing_dataset.size(); i++)
	{
		if(mode_value[i] == testing_dataset[i][target_attribute])
			correct++;
	}
	float accuracy = (float)correct / testing_dataset.size();
	cout << "Accuracy for the random forest: " << accuracy << endl;
	cout << endl;
}



int main()
{
	int i, j;
	int no_of_attributes, no_of_examples, no_of_training_examples, no_of_pruning_examples;
	string input, str;
	vector< vector<string> >possible_values_for_attribute;
	vector< vector<string> >original_dataset;
	vector< vector<int> >modified_dataset;
	vector< vector<int> >training_dataset;
	vector< vector<int> >pruning_dataset;
	vector< vector<string> >testing_dataset;
	vector< vector<int> >modified_testing_dataset;
	float final_accuracy;

	// cout << "Enter no of attributes: " << endl;
	cin >> no_of_attributes;
	vector<int> no_of_values_in_attribute(no_of_attributes);
	vector<int> previous_no_of_values_in_attribute(no_of_attributes);
	vector<int> threshold_for_continuous_valued_input(no_of_attributes, -1);
	// no_of_attributes includes target attribute as well
	// cout << "Enter no of values in each attribute: " << endl;
	for(i = 0; i < no_of_attributes; i++)
	{
		// Give input for no_of_values_in_attribute[i] as -1 if the attribute is a continuous valued
		cin >> no_of_values_in_attribute[i];
		previous_no_of_values_in_attribute[i] = no_of_values_in_attribute[i];
	}

	for(i = 0; i < no_of_attributes; i++)
	{
		// If the attribute is not continuous
		vector<string> taking_input;
		if(no_of_values_in_attribute[i] != -1)
		{
			// cout << "Enter all the values for attribute ";
			// cout << i;
			// cout << ":" << endl;
			for(j = 0; j < no_of_values_in_attribute[i]; j++)
			{
				cin >> input;
				taking_input.push_back(input);
			}
		}
		possible_values_for_attribute.push_back(taking_input);
	}

	// cout << "Enter the number of training examples" << endl;
	cin >> no_of_examples;
	
	
	ifstream fin;
	ofstream fout;
	fin.open("training_data.txt");
	fout.open("training_data_temp.txt");
	while(getline(fin, str))
	{
		for(i = 0; i < str.size(); i++)
		{
			if(str[i] != ' ' && str[i] != '.')
				fout << str[i];
		}
		fout << endl;
	}
	fin.close();
	fout.close();
	fin.open("training_data_temp.txt");
	while(fin)
	{
		string s;
	    if (!getline( fin, s )) break;

	    istringstream ss( s );
	    vector <string> record;

	    while (ss)
	    {
	      string s;
	      if (!getline( ss, s, ',' )) break;
	      record.push_back( s );
	    }
	    original_dataset.push_back(record);
	}
	fin.close();

	// Reading testing data
	fin.open("testing_data.txt");
	fout.open("testing_data_temp.txt");
	while(getline(fin, str))
	{
		for(i = 0; i < str.size(); i++)
		{
			if(str[i] != ' ' && str[i] != '.')
				fout << str[i];
		}
		fout << endl;
	}
	fin.close();
	fout.close();
	fin.open("testing_data_temp.txt");
	while(fin)
	{
		string s;
	    if (!getline( fin, s )) break;

	    istringstream ss( s );
	    vector <string> record;

	    while (ss)
	    {
	      string s;
	      if (!getline( ss, s, ',' )) break;
	      record.push_back( s );
	    }
	    testing_dataset.push_back(record);
	}
	fin.close();

	modified_dataset = map_dataset(original_dataset, possible_values_for_attribute, no_of_values_in_attribute);

	// cout << "hi"<< endl;
	modified_dataset = handle_continuous_valued_input(modified_dataset, no_of_attributes, no_of_attributes - 1, no_of_values_in_attribute, threshold_for_continuous_valued_input);
	// cout << "hello there"<< endl;
	

	modified_dataset = predict_missing_values(modified_dataset, no_of_attributes, no_of_attributes - 1, no_of_values_in_attribute);


	no_of_training_examples = (3*no_of_examples)/4;
	// no_of_training_examples = no_of_examples;			// Remove this line when you add the file
	for(i = 0; i < no_of_training_examples; i++)
		training_dataset.push_back(modified_dataset[i]);

	for(i = no_of_training_examples; i < no_of_examples; i++)
		pruning_dataset.push_back(modified_dataset[i]);

	vector<int> current_attributes(no_of_attributes-1, 0);
	for(i = 0; i < no_of_attributes - 1; i++)
	 	current_attributes[i] = i;

	TreeNode* root = new TreeNode();
	// id3(Examples, target_attribute, Attributes)
	root = id3(training_dataset, no_of_attributes - 1, current_attributes, no_of_values_in_attribute, false);
	cout << endl << endl;
	// print_level_order_tree(root, no_of_attributes - 1, possible_values_for_attribute);

	cout << "Accuracy of the original tree on training data: ";
	final_accuracy = find_accuracy(root, training_dataset, no_of_values_in_attribute, no_of_attributes-1);
	// final_recall = find_recall(root, training_dataset, no_of_values_in_attribute, no_of_attributes-1);
	cout << final_accuracy;
	cout << endl;
	




	modified_testing_dataset = map_dataset(testing_dataset, possible_values_for_attribute, previous_no_of_values_in_attribute);

	modified_testing_dataset = handle_continuous_valued_testing_data(modified_testing_dataset, threshold_for_continuous_valued_input, no_of_attributes);
	
	modified_testing_dataset = predict_missing_values(modified_testing_dataset, no_of_attributes, no_of_attributes-1, no_of_values_in_attribute);

	cout << "Accuracy of the original tree on testing data: ";
	cout << find_accuracy(root, modified_testing_dataset, no_of_values_in_attribute, no_of_attributes-1) << endl;
	cout << endl;



	// Pruning
	TreeNode* pruned_tree = new TreeNode();
	pruned_tree = reduced_error_pruning(root, pruning_dataset, no_of_values_in_attribute, no_of_attributes-1);

	// print_level_order_tree(pruned_tree, no_of_attributes - 1, possible_values_for_attribute);
	cout << endl;
	cout << "Accuracy of the pruned tree on pruning dataset: ";
	final_accuracy = find_accuracy(pruned_tree, pruning_dataset, no_of_values_in_attribute, no_of_attributes-1);
	cout << final_accuracy << endl;


	cout << "Accuracy of the pruned tree on testing dataset: ";
	cout << find_accuracy(pruned_tree, modified_testing_dataset, no_of_values_in_attribute, no_of_attributes-1) << endl;


	random_forest(modified_dataset, modified_testing_dataset, no_of_values_in_attribute, no_of_attributes-1, no_of_attributes);


	return 0;
}
