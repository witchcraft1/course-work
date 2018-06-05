#include <cassert>
#include <iostream>

using namespace std;

struct node {
  int value;
  int nodeHeight;
  node *left;
  node *right;
  node(int k) {
    value = k;
    nodeHeight = 0;
    left = right = nullptr;
  }
};

int height(node *Node) { return Node ? Node->nodeHeight : -1; }

void changeHeight(node *Node) {
  int HeightLeft = height(Node->left);
  int HeightRight = height(Node->right);
  Node->nodeHeight = (HeightLeft > HeightRight ? HeightLeft : HeightRight) + 1;
}

int difference_height_right_left_child(node *Node) {
  return height(Node->right) - height(Node->left);
}

node *right_small_rotation(node *&alpha) {
  node *beta = alpha->left;
  alpha->left = beta->right;
  beta->right = alpha;
  changeHeight(alpha);
  changeHeight(beta);
  return beta;
}

node *left_small_rotation(node *&alpha) {
  node *beta = alpha->right;
  alpha->right = beta->left;
  beta->left = alpha;
  changeHeight(alpha);
  changeHeight(beta);
  return beta;
}

node *left_big_rotation(node *&alpha) {
  node *beta = alpha->right;
  node *gamma = beta->left;
  alpha->right = gamma->left;
  beta->left = gamma->right;
  gamma->right = beta;
  gamma->left = alpha;
  changeHeight(alpha);
  changeHeight(beta);
  changeHeight(gamma);
  return gamma;
}

node *right_big_rotation(node *&alpha) {
  node *beta = alpha->left;
  node *gamma = beta->right;
  alpha->left = gamma->right;
  beta->right = gamma->left;
  gamma->right = alpha;
  gamma->left = beta;
  changeHeight(alpha);
  changeHeight(beta);
  changeHeight(gamma);
  return gamma;
}

const int depth = 2;
node *balance(node *&Node) {
  changeHeight(Node);
  if (difference_height_right_left_child(Node) == depth) {
    if (difference_height_right_left_child(Node->right) < 0)
      return left_big_rotation(Node);
    return left_small_rotation(Node);
  }
  if (difference_height_right_left_child(Node) == -depth) {
    if (difference_height_right_left_child(Node->left) > 0)
      return right_big_rotation(Node);
    return right_small_rotation(Node);
  }
  return Node;
}

node *addNode(int elem, node **Tree) {
  if (*Tree == nullptr) {
    *Tree = new node(elem);
    return *Tree;
  }
  if (elem > (*Tree)->value) {
    (*Tree)->right = addNode(elem, &(*Tree)->right);
  } else {
    (*Tree)->left = addNode(elem, &(*Tree)->left);
  }
  *Tree = balance(*Tree);
  return *Tree;
}

void addNode(int elem, node *&Tree) {
  if (Tree == nullptr) {
    Tree = new node(elem);
    return;
  }
  if (elem > Tree->value) {
    addNode(elem, Tree->right);
  } else {
    addNode(elem, Tree->left);
  }
}

node *deleteNode(int elem, node *&Tree) {
  if (Tree == nullptr) {
    return Tree;
  } else {
    if (elem < Tree->value) {
      Tree->left = deleteNode(elem, Tree->left);
    } else if (elem > Tree->value) {
      Tree->right = deleteNode(elem, Tree->right);
    } else {
      bool isTreeLeft = false;
			bool isTreeRight = false;
			if(Tree->left)  isTreeLeft = true;
			if (Tree->right) isTreeRight = true;
				if (!isTreeLeft && !isTreeRight) {
        delete Tree;
        Tree = nullptr;
        return Tree;
      } else if (isTreeRight) {
        node *tmp = Tree->right;
        node *ttmp = Tree->right;
        while (tmp->left) {
          ttmp = tmp;
          tmp = tmp->left;
        }
        if (tmp == ttmp)
          Tree->right = tmp->right;
        else
          ttmp->left = tmp->right;
        Tree->value = tmp->value;
        delete tmp;
        return Tree;
      } else {
        node *tmp = Tree->left;
        node *ttmp = Tree->left;
        while (tmp->right) {
          ttmp = tmp;
          tmp = tmp->right;
        }
        if (tmp == ttmp)
          Tree->left = tmp->left;
        else
          ttmp->right = tmp->left;
        Tree->value = tmp->value;
        delete tmp;
        return Tree;
      }
    }
  }
  Tree = balance(Tree);
  return Tree;
};

node *BeforefindNode(node *Tree, int key) {
  if (Tree == nullptr) return nullptr;
  else{
     if (Tree->left->value == key) {
     return Tree;
    } else if (Tree->right->value < key) {
      BeforefindNode(Tree->right, key);
    } else {
       BeforefindNode(Tree->left, key);
    }
  } 
}

node *findNode(node *Tree, int key) {
  if (Tree == nullptr) return nullptr;
  else{
     if (Tree->value == key) {
       return Tree;
    } else if (Tree->value < key) {
       findNode(Tree->right, key);
    } else {
       findNode(Tree->left, key);
    }
  } 
}

void copy(node *&tree, node *&tree2) {
  if (tree == nullptr)
    return;
  addNode(tree->value, tree2);
  if (tree->left)
    copy(tree->left, tree2);
  if (tree->right)
    copy(tree->right, tree2);
}

void test_addNode(int elem, node *Tree) {
  addNode(elem, &Tree);
  node *InsertNode = findNode(Tree, elem);
  assert(InsertNode->value == elem);
  cout << "Test add +" << endl;
}

void test_deleteNode(int elem, node *&Tree) {
  deleteNode(elem, Tree);
  assert(findNode(Tree, elem) == nullptr);
  cout << "test delete node +++" << endl;
}

void test_copy(node *&Tree, node *&Tree2) {
  if (Tree == nullptr && Tree2 == nullptr)
    return;
  assert(Tree && Tree2);
  assert(Tree->value == Tree2->value);
  if (Tree->left || Tree2->left)
    test_copy(Tree->left, Tree2->left);
  if (Tree->right || Tree2->right)
    test_copy(Tree->right, Tree2->right);
}

void show(node *tree, int spaces) {
  if (!tree)
    cout << "Tree doesn\'t exist" << endl;
  while (tree) {
    if (tree->right) {
      show(tree->right, spaces + 5);
    }
    for (int i = 0; i < spaces; i++)
      cout << ' ';
    cout << tree->value << endl;
    tree = tree->left;
    spaces += 5;
  }
};

int main() {
  node *Tree2 = nullptr;
  node *Tree = nullptr;
  cout << "tree2 : " << endl;
  show(Tree2, 0);
  addNode(100, &Tree2);
  addNode(150, &Tree2);
  test_addNode(23, Tree2);
  test_addNode(27, Tree2);
  test_deleteNode(27, Tree2);
  show(Tree2, 0);
  cout << endl;
  addNode(250, &Tree2);
  addNode(125, &Tree2);
  addNode(50, &Tree2);
  addNode(30, &Tree2);
  addNode(10, &Tree2);
  addNode(5, &Tree2);
  addNode(3, &Tree2);
  addNode(1, &Tree2);
  addNode(500, &Tree2);
  addNode(5000, &Tree2);
  addNode(50000, &Tree2);
  addNode(7, &Tree2);
  addNode(8, &Tree2);
  addNode(10, &Tree2);
  show(Tree2, 0);
  copy(Tree2, Tree);
  cout << " Tree : " << endl;
  show(Tree, 0);
  test_copy(Tree2, Tree);
  cout << "testCopy +++" << endl;
  cout << "==============" << endl;
  deleteNode(50, Tree2);
  deleteNode(100, Tree2);
  show(Tree2, 0);
  cout << Tree2->left->left->right->right->value << endl << endl;
  return 0;
}
