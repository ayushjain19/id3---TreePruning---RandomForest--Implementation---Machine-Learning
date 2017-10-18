# Decision Tree Learning

This directory contains code for implementation for **id3 Decision Tree** algorithm, followed by **Tree Pruning or Reduced Error Pruning**, which is then followed by the **Random Forest** implementation.

The code is general for any dataset. Its just that you need to provide the input in a particular manner described below:

1. Command line input:
    - Give no.of attributes.
    - The number of possible values each attribute can take (if it is a continious valued attribute, just give the input as -1).
    - Give all the possible inputs that can be given for each attribute.
    - At the end, Give the number of examples on which the training is to be done.
  
    *I have included a file named "input.txt" which contains the command line input for the dataset information provided [here](https://archive.ics.uci.edu/ml/datasets/Census+Income) and the corresponding dataset is provided [here](https://archive.ics.uci.edu/ml/machine-learning-databases/adult/)*
  
2. File input:
    - There are two file inputs:
      - Training data file: File containing the training data.
      - Testing data file: File containing the testing data.
    - Both files should have one dataset per line, each line containing values for different attributes separated by ", "
  
    *I have included two files named "testing_data.txt" and "training_data.txt" following the above format for the same dataset linked above*
  
  
Broadly, a total of three tasks are being performed for training the dataset:

1. **id3 Decision Tree algorithm:**
  - *Pseudocode:*
    - ID3 (Examples, Target_Attribute, Attributes)
    - Create a root node for the tree
    - If all examples are positive, Return the single-node tree Root, with label = +.
    - If all examples are negative, Return the single-node tree Root, with label = -.
    - If number of predicting attributes is empty, then Return the single node tree Root, with label = most common value of the target attribute in the examples.
    - Otherwise Begin
        - A ← The Attribute that best classifies examples.
        - Decision Tree attribute for Root = A.
        - For each possible value, vi, of A,
            - Add a new tree branch below Root, corresponding to the test A = vi.
            - Let Examples(vi) be the subset of examples that have the value vi for A
            - If Examples(vi) is empty
                - Then below this new branch add a leaf node with label = most common target value in the examples
            - Else below this new branch add the subtree ID3 (Examples(vi), Target_Attribute, Attributes – {A})
    - End
    - Return Root


2. **Reduced Error Pruning:**
  - *Pseudocode:*
    - Perform reverse level order traversal for the tree.
    - For each of the node encountered:
        - For every possible value in the target attribute:
            - Place the value in place of the current node.
            - If the accuracy increases:
                - Keep the value at that node and the tree gets modified.
            - Else:
                - Keep the tree as it is.
    
    
3. **Random Forest:**
  - *Pseudocode:*
      - For forming each tree:
          - Randomly sample N data points from the dataset of N data points(data points in the final sample may repeat or may not even occur once).
          - Run id3 algorithm on this dataset with the following modification in every id3 function call:
              - The number of attributes sampled is log(p) where p is the total number of remaining attributes.
      - Final output of the random forest is the mode of the outputs of the individual trees
