#include <iostream>
#include <fstream>  //file operations
#include <string>   //getlline

//////////////////////
//  Ramal Seyidli   //
//  150180901       //
//////////////////////

using namespace std;

struct Node{

    Node();     //constructor
    ~Node(){}       //destructor
    Node* parent;    //parent of node
    Node* left;      //left child of node
    Node* right;    //right child of node
    string name;
    string platform;
    string year_of_release;
    string publisher; //key value
    string color;   
    //Sales* sale;     //points to sales node  
    float na_sales;
    float eu_sales;
    float other_sales;
};

Node::Node(){
    color = "RED";
    parent = NULL;
    left = NULL;
    right = NULL;
    //sale = NULL;    
}


struct Sales{
    float na_sales; //north america
    float eu_sales; //europe
    float other_sales;  //other
};

struct BestSeller{
    string na_bestseller;
    string eu_bestseller;
    string other_bestseller;    //names of publisher which are bestsellers by area
    
    float na_sales;
    float eu_sales;
    float other_sales;
    
    void control(Node * node);
};

void BestSeller::control(Node* input){
    if(this->na_sales < input->na_sales){
        this->na_bestseller = input->publisher;
        this->na_sales = input->na_sales;
    }
    if(this->eu_sales < input->eu_sales){
        this->eu_bestseller = input->publisher;
        this->eu_sales = input->eu_sales;
    }
    if(this->other_sales < input->other_sales){
        this->other_bestseller = input->publisher;
        this->other_sales = input->other_sales;
    }
}

struct RedBlackTree{

    Node* root; //head

    Node* detect(string); //detects the publisher
    void insert(Node * , BestSeller*);  //inserts input to the tre
    void rotateLeft(Node*); //rotates left
    void rotateRight(Node*);    //rotates right
    void rebalance(Node*);  //maintains red black tree features
    void clear(Node*);  //deletes nodes from head toward leafs
    
    void print(string, BestSeller*); //prints decade bestseller info
    void printTree(Node*, int );    //prints tree

    RedBlackTree(); //constructor
    ~RedBlackTree();    //destructor

};

RedBlackTree::RedBlackTree(){
    root = NULL;
}

RedBlackTree::~RedBlackTree(){
    clear(root);
}

void RedBlackTree::clear(Node* input){
    if(input == NULL){
        return;
    }
    clear(input->left);
    clear(input->right);
    delete input;
}



void RedBlackTree::insert(Node* input, BestSeller* best){
    if(root == NULL){
        root = input;
        input->color = "BLACK";
        best->control(input);
        return;
    }

    Node* traverse = root;
    int value = input->publisher.compare(traverse->publisher);
    
    Node* tail = NULL;
    //string small = smaller(input->publisher, traverse->publisher);

    while(traverse != NULL){

        //small = smaller(input->publisher, traverse->publisher);
        if(input->publisher.compare(traverse->publisher) < 0){  //new publisher's name is smaller than traverse's
           
            tail = traverse;
            traverse = traverse->left;
            //traverse left 

        }else if(input->publisher.compare(traverse->publisher) > 0){ //new publisher's name is bigger than traverse's
            
            tail = traverse;
            traverse = traverse->right;
            //traverse right

        }else{
            
            traverse->publisher = input->publisher;
            traverse->name = input->name;
            traverse->year_of_release = input->year_of_release;
            traverse->platform = input->platform;
            traverse->na_sales += input->na_sales;
            traverse->eu_sales += input->eu_sales;
            traverse->other_sales += input->other_sales;
            best->control(traverse);
            delete input;
            return;
        }
        if(value < 0){
            tail->left = input;
            input->parent = tail;

            //new publisher is added to the tree from left 
        }else if(value > 0){
            tail->right = input;
            input->parent = tail;
            //new publisher is added to the tree from right 
        }
        

        rebalance(input);
        best->control(input);
        return;
    }
}

void RedBlackTree::rebalance(Node* input){  //to maintain red black tree features

    Node* parent = NULL;
    Node* grandparent = NULL;
    Node* uncle = NULL; //sibling of parent

    if(input != root && input->color == "RED" && input->parent->color == "RED"){    //security
        
        grandparent = input->parent->parent;
        parent = input->parent;

        if(grandparent != NULL && parent == grandparent->left){ // input is child of the left child

            uncle = grandparent->right;

            if(uncle != NULL && uncle->color == "RED"){ //when uncle is red
                grandparent->color = "RED";
                uncle->color = "BLACK";
                parent->color = "BLACK";
                input = grandparent;
            }else{

                if(input == parent->right){ //input is right child

                    rotateLeft(parent);
                    input = parent;
                    parent = input->parent;
                    rotateRight(grandparent);
                    parent->color = "BLACK";
                    grandparent->color = "RED";
                    input = parent;

                }else if(input == parent->left){    //input is left child
                    
                    rotateRight(grandparent);
                    parent->color = "BLACK";
                    grandparent->color = "RED";
                    input = parent;

                }
            }
        }else if(grandparent != NULL){  // input is child of the right child

            uncle = grandparent->left;

            if(uncle != NULL && uncle->color == "RED"){

                grandparent->color = "RED";
                uncle->color = "BLACK";
                parent->color = "BLACK";
                input = grandparent;

            }else{

                if(input == parent->left){

                    rotateRight(parent);
                    input = parent;
                    parent = input->parent;
                    rotateLeft(grandparent);
                    parent->color = "BLACK";
                    grandparent->color = "RED";
                    input = parent;

                }else if (input == parent->right){

                    rotateLeft(grandparent);
                    parent->color = "BLACK";
                    grandparent->color = "RED";
                    input = parent;
                }
            }
        }

        root->color = "BLACK";
        rebalance(input);
    }
    
}
    //right rotation
void RedBlackTree::rotateRight(Node* input){
    Node* l = input->left;
    input->left = input->left->right;

    if(input->left != NULL){

        input->left->parent = input;

    }
    l->parent = input->parent;

    if(input->parent == NULL){
        root = l;
    }else if(input == input->parent->right){
        input->parent->right = l;
    }else if (input == input->parent->left){
        input->parent->left = l;
    }

    l->right = input;
    input->parent = l;
}
    //left rotation
void RedBlackTree::rotateLeft(Node* input){
    Node* r = input->right;
    input->right = input->right->left;

    if(input->right != NULL){

        input->right->parent = input;

    }
    r->parent = input->parent;

    if(input->parent == NULL){
        root = r;
    }else if(input == input->parent->right){
        input->parent->right = r;
    }else if (input == input->parent->left){
        input->parent->left = r;
    }

    r->left = input;
    input->parent = r;
}

void RedBlackTree::print(string decade, BestSeller* best){
    
    cout << "End of the " << decade << " year" << endl;
    cout << "Best seller in North America: " << best->na_bestseller << " - " << best->na_sales << " million" << endl; 
    cout << "Best seller in Europe: " << best->eu_bestseller << " - " << best->eu_sales << " million" << endl;
    cout << "Best seller rest of the World: " << best->other_bestseller << " - " << best->other_sales << " million" << endl;  

}

void RedBlackTree::printTree(Node* input, int depth){

    if(input == NULL){  
        return;
    }

    for(int i = 0; i < depth; i++ ){    //to represent depth
        cout << "-";
    }

    cout << "(" << input->color << ")" << input->publisher << endl;

    printTree(input->left, ++depth);    //recurisive
    printTree(input->right, ++depth);
}


int main(int argc, char** argv){

    //input file operations

    string fileName;

    if(argc > 1){
        fileName = argv[1];
    }else{
        cerr << "Please enter a file name" << endl;
        return 1;
    }

    ifstream file;
    file.open(fileName);

    if(!file.is_open()){
		cerr << "File cannot be opened" << endl;
        return 1;
	}

    string line;
	getline(file, line);
	string tableHeader = line; //to skip header


    RedBlackTree* rbt = new RedBlackTree;

    BestSeller* best = new BestSeller;

    string year;        

    while(file.peek() != EOF){
    //for(int i = 0; i < 54; i++){
        Node* node = new Node;

        
        getline(file, line, ',');
        node->name = line;

        getline(file, line, ',');
        node->platform = line;

        getline(file, line, ',');
        

        if((line == "1991" || line == "2001" || line == "2011")&& line != year){
            rbt->print(year, best);
            
        }
        
        node->year_of_release = line;
        year = line;

        getline(file, line, ',');
        node->publisher = line;

        getline(file, line, ',');   //converting to float value
        node->na_sales = stof(line);

        getline(file, line, ',');   //converting to float value
        node->eu_sales = stof(line);

        getline(file, line, '\n');  //converting to float value
        node->other_sales = stof(line);

        rbt->insert(node, best);    //insertion

        

    }
    //rbt->print("1981", best);
    rbt->print("2020", best);   //printing final decade results
    rbt->printTree(rbt->root, 0);   //printing tree

    file.close();

    delete best, rbt;



}
