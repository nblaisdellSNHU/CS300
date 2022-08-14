//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Nicolas Blaisdell
// Version     : 1.0
// Description : Sorting & Selecting Course Information
//============================================================================//

#include <iostream>
#include "CSVParser.hpp"
#include <algorithm>

using std::cout;
using std::cin;
using std::string;
using std::vector;

string strToLower(string strIn) {
	string newStr = strIn;
	std::transform(newStr.begin(), newStr.end(), newStr.begin(), [](unsigned char c) {
		return static_cast<char>(std::tolower(c));
			});
	return newStr;
}

// define a structure to hold bid information
struct Course {
	string courseID;
	string courseName;
	vector<string> prerequisites;

	Course() {
		prerequisites = vector<string>(0);
	}

	Course(string courseID, string courseName) : Course() {
		this->courseID = courseID;
		this->courseName = courseName;
	}
};

// Internal structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a bid
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

class BinarySearchTree {

private:
	Node* root;
	int size;

	void addNode(Node* node, Course course);
	Node* removeNode(Node* parent, Node* node);
	void inOrder(Node* node);
	void postOrderDelete(Node* node);
public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void Insert(Course bid);
	void InOrder();
	int Size();
	Course Search(string courseID);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
	// FixMe (1): initialize housekeeping variables
	//root is equal to nullptr
	this->root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
	// recurse from root deleting every node
	this->postOrderDelete(this->root);
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
	// FIXME (8) Implement inserting a bid into the tree

	// NB: Using the "BidID" field to see if a new bid object
	//     is larger or smaller than the current node
	while (node != nullptr) {
		// if node is larger then add to left
		if (course.courseID < node->course.courseID) {
			// if no left node
			if (node->left == nullptr) {
				// this node becomes left
				node->left = new Node(course);
				return;
			}
			// else recurse down the left node
			else {
				this->addNode(node->left, course);
				return;
			}
		}
		// else
		else {
			// if no right node
			if (node->right == nullptr) {
				// this node becomes right
				node->right = new Node(course);
				return;
			}
			//else
			else {
				// recurse down the left node
				this->addNode(node->right, course);
				return;
			}
		}
	}
}

Node* BinarySearchTree::removeNode(Node* parent, Node* node) {
	if (node == nullptr) {
		return nullptr;
	}

	// Internal node w/ 2 children
	if (node->left != nullptr && node->right != nullptr) {
		// Find successor and its parent
		Node* sucNode = node->right;
		Node* sucParent = node;
		while (sucNode->left != nullptr) {
			sucParent = sucNode;
			sucNode = sucNode->left;
		}

		// copy the actual value from the successor node
		node->course = sucNode->course;

		// recursively remove the successor
		this->removeNode(sucParent, sucNode);
	}

	// Root node (with 1 or 0 children)
	else if (node == this->root) {
		if (node->left != nullptr) {
			this->root = node->left;
		}
		else {
			this->root = node->right;
		}
	}

	// Internal child w/ left child only
	else if (node->left != nullptr) {
		// replace node w/ node's left child
		if (parent->left == node) {
			parent->left = node->left;
		}
		else {
			parent->right = node->left;
		}
	}

	// Internal child w/ right child only or leaf
	else {
		// replace node w/ node's right child
		if (parent->left == node) {
			parent->left = node->right;
		}
		else {
			parent->right = node->right;
		}
	}

	return node;
}

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourseName(Course course) {
	cout << course.courseID << ", " << course.courseName << "\n";
	return;
}

void BinarySearchTree::inOrder(Node* node) {
	// FixMe (9): Pre order root
	//if node is not equal to null ptr
	if (node != nullptr) {
		//InOrder left
		this->inOrder(node->left);
		//output bidID, title, amount, fund
		displayCourseName(node->course);
		//InOder right
		this->inOrder(node->right);
	}
}

void BinarySearchTree::postOrderDelete(Node* node) {
	if (node != nullptr) {
		this->postOrderDelete(node->left);
		this->postOrderDelete(node->right);
		delete node;
	}
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Course course) {
	// FIXME (5) Implement inserting a bid into the tree

	// if root equarl to null ptr
	if (this->root == nullptr) {
		// root is equal to new node bid
		this->root = new Node(course);
	}
	// else
	else {
		// add Node root and bid
		this->addNode(this->root, course);
	}

	// NB: Increment the size of the BST
	++this->size;
}

/**
 * Search for a bid
 */
Course BinarySearchTree::Search(string courseId) {
	// FIXME (7) Implement searching the tree for a bid

	// set current node equal to root
	Node* currNode = this->root;
	string givenID = strToLower(courseId);
	
	// keep looping downwards until bottom reached or matching bidId found
	while (currNode != nullptr) {
		// if match found, return current bid
		string currNodeID = strToLower(currNode->course.courseID);
		if (currNodeID == givenID) {
			return currNode->course;
		}

		// if bid is smaller than current node then traverse left
		if (givenID < currNodeID) {
			currNode = currNode->left;
		}
		// else larger so traverse right
		else {
			currNode = currNode->right;
		}
	}

	Course course;
	return course;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
	// FixMe (2): In order root
	// call inOrder fuction and pass root 
	this->inOrder(this->root);
}

int BinarySearchTree::Size() {
	return this->size;
}

void displayCoursePrerequisites(Course course) {
	cout << "Prerequisites: ";
	for (int i = 0; i < course.prerequisites.size(); ++i) {
		cout << course.prerequisites.at(i);
		if (i != course.prerequisites.size() - 1) {
			cout << ", ";
		}
	}
}

void displayCoursePrerequisitesGood(Course course, BinarySearchTree* bst) {
	cout << "Prerequisites: \n";
	for (int i = 0; i < course.prerequisites.size(); ++i) {
		cout << "  ";
		Course prereqCourse = bst->Search(course.prerequisites.at(i));
		displayCourseName(prereqCourse);
	}
}

void displayCourseInfo(Course course) {
	displayCourseName(course);
	displayCoursePrerequisites(course);
}

void displayCourseInfoEnhanced(Course course, BinarySearchTree *bst) {
	displayCourseName(course);
	displayCoursePrerequisitesGood(course, bst);
}


void displayMenu() {
	cout << "\n\n";
	
	cout << "    1. Load Data Structure" << ".\n";
	cout << "    2. Print Course List" << ".\n";
	cout << "    3. Print Course" << ".\n";
	cout << "    9. Exit" << ".";

	cout << "\n\n";
}

// This function will take in the user's input and validate it to make sure that
// the value they entered is a valid option for this program
// In this program, the value must be a number, and that number must be 1, 2, or 3.
bool isValidSelection(std::string userInput) {
	if (userInput.length() != 1) {
		return false;
	}

	if (!(userInput == "1" || userInput == "2" || userInput == "3" || userInput == "9")) {
		return false;
	}

	return true;
}

int getUserInput() {
	cout << "What would you like to do? ";

	// First, store the user's input as a string, in case they enter something
	// other than an integer, which would cause an error when storing into
	// an integer
	std::string userInput = "";
	getline(cin, userInput);

	// Then, validate the string input before converting to integer
	if (!isValidSelection(userInput)) {
		cout << userInput << " is not a valid option.";
		return -1; // Will allow for 'invalid selection in switch statement'
	}

	// Lastly, if we make it here, the input has been validated and confirmed
	// to be a number, so let's convert it to an integer here and return it
	return std::stoi(userInput);
}

void loadCourseData(string csvPath, BinarySearchTree* bst) {
	cout << "Loading CSV file " << csvPath << "\n";

	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);

	try {
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// Create a data structure and add to the collection of bids
			Course course;
			course.courseID = file[i][0];
			course.courseName = file[i][1];

			// Then, for any prerequisites, add those to the vector in our
			// Course object. This will handle for any number of prerequisites,
			// not just two.
			for (int j = 2; j < file[i].size(); ++j) {
				string prereqID = file[i][j];
				if (prereqID != "") {
					course.prerequisites.emplace_back(prereqID);
				}
			}

			//cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

			// push this bid to the end
			bst->Insert(course);
		}
	}
	catch (csv::Error& e) {
		std::cerr << e.what() << std::endl;
	}
}

void printCourseList(BinarySearchTree* bst) {
	cout << "Here is a sample schedule:\n\n";
	bst->InOrder();
}

void printCourseInfo(BinarySearchTree* bst) {
	cout << "What course do you want to know about? ";
	
	string courseID = "";
	getline(cin, courseID);

	Course courseToFind = bst->Search(courseID);
	if (courseToFind.courseID == "") {
		cout << "No course found.";
	}
	else {
		//displayCourseInfo(courseToFind); 
		displayCourseInfoEnhanced(courseToFind, bst);
	}
}

int main()
{
	// Welcome message
	cout << "Welcome to the course planner.";

	string pathCoursesCSV = "..\\program_input.csv";
	BinarySearchTree* bst = new BinarySearchTree();

	int userChoice = 0;
	while (userChoice != 9) {
		// display the menu to the user
		displayMenu();

		userChoice = getUserInput();
		switch (userChoice) {
			case 1: // 1: Load Data Structure
				loadCourseData(pathCoursesCSV, bst);
				cout << bst->Size() << " courses loaded.";

				break;
			case 2: // 2: Print Course List (sorted)				
				// "Here is a sample schedule:\n\n"
				printCourseList(bst);
				break;
			case 3: // 3: Print Course (courseID given)
				printCourseInfo(bst);
				break;
			case 9:  // 9: Exit Program (do nothing, and exit loop on next iteration)
			default: // Invalid input
				break;
		}
	}

	// Exit message
	cout << "Thank you for using the course planner.\n";

	// Any last cleanup for our data structures
	delete bst;
}