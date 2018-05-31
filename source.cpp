#include <cassert>
#include <iostream>

using namespace std;

struct node {
  int value;      //���� ������ ��������
  int nodeHeight; // ��� ������
  node *left;     // ����� �������
  node *right;    //������ �������
  node(int k) {   //�����������
    value = k, nodeHeight = 0, left = right = nullptr;
  }
};

// node *Tree2 = nullptr;
//��������� ��� ��������������� �������, ��������� � �������.

//������� ������� ���������� ������ ������ ���� ��� ���� ��� -1 ���� ��� ���
int height(node *p) { return p ? p->nodeHeight : -1; }
//������� ������� ������ � ������� �������
int difference_height_right_left_child(node *p) {
  return height(p->right) - height(p->left);
}
//������ ����
void fixHeight(node *p) {
  int HeightLeft = height(p->left);   // p->left->height;
  int HeightRight = height(p->right); // p->right->height;

  p->nodeHeight = (HeightLeft > HeightRight ? HeightLeft : HeightRight) + 1;
}

// ������������ �����. ��� ����� ������������ �������� , ����� ���� 4 : 2
// �����:������ � �����, 2 �������: ������ � �����

// ����� ������ ������� :
node *right_small_rotation(node *&p) {
  node *q = p->left;
  p->left = q->right;
  q->right = p;
  fixHeight(p);
  fixHeight(q);
  return q;
}
//����� ����� �������
node *left_small_rotation(node *&p) {
  node *q = p->right;
  p->right = q->left;
  q->left = p;
  fixHeight(p);
  fixHeight(q);
  return q;
}
//������� ����� �������
node *left_big_rotation(node *&alpha) {
  node *beta = alpha->right;
  node *gamma = beta->left;
  alpha->right = gamma->left;
  beta->left = gamma->right;
  gamma->right = beta;
  gamma->left = alpha;
  fixHeight(alpha);
  fixHeight(beta);
  fixHeight(gamma);
  return gamma;
}
//������� ������ �������
node *right_big_rotation(node *&alpha) {
  node *beta = alpha->left;
  node *gamma = beta->right;
  alpha->left = gamma->right;
  beta->right = gamma->left;
  gamma->right = alpha;
  gamma->left = beta;
  fixHeight(alpha);
  fixHeight(beta);
  fixHeight(gamma);
  return gamma;
}
// ������������ ���� p

node *balance(node *&p) {
  fixHeight(p);
  if (difference_height_right_left_child(p) == 2) {
    if (difference_height_right_left_child(p->right) < 0)
      return left_big_rotation(p); // p->right = right_small_rotation(p->right);
    return left_small_rotation(p);
  }
  if (difference_height_right_left_child(p) == -2) {
    if (difference_height_right_left_child(p->left) > 0)
      return right_big_rotation(p); // p->left = left_small_rotation(p->left);
    return right_small_rotation(p);
  }
  return p; // ������������ �� �����
}

// ������� ����� elem � ������ � ������ Tree
//�������� ������� ��� ���������� ��������� � ������ ����� � �������������
//������
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
  return *Tree; // return balance(Tree);
}
// ��������������� ������� ������� ������������ � ������� ����������� ������
void addNode(int elem, node *&Tree) {
  if (Tree == nullptr) {
    Tree = new node(elem);
    // Tree->value = elem;
    // Tree->left = Tree->right = nullptr;
    // balance(Tree2);
    return;
  }
  if (elem > Tree->value) {
    // Tree->nodeHeight++;
    addNode(elem, Tree->right);
  } else {
    // Tree->nodeHeight++;
    addNode(elem, Tree->left);
  }
}

//�������� ����� elem � ������ �r��
node *deleteNode(int elem, node *&Tree) {
  if (Tree == nullptr) {
    return Tree;
  } else {
    if (elem < Tree->value) {
      Tree->left = deleteNode(elem, Tree->left);

    } else if (elem > Tree->value) {
      Tree->right = deleteNode(elem, Tree->right);

    } else {
      if (Tree->left == nullptr && Tree->right == nullptr) {
        // Tree->data = NULL ;
        delete Tree;
        Tree = nullptr;
        //
        return Tree;
        //
      } else if (Tree->right != nullptr) {
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
        //
        return Tree;
        //
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
        //
        return Tree;
        //
      }
    }
  }
  Tree = balance(Tree);
  return Tree;
};

node *BeforefindNode(node *Tree, int key) {
  if (Tree != nullptr) {

    if (Tree->left->value > key) {
      BeforefindNode(Tree->left, key);
    } else if (Tree->right->value < key) {
      BeforefindNode(Tree->right, key);
    } else {
      return Tree;
    }
  } else
    return nullptr;
}

node *findNode(node *Tree, int key) {
  if (Tree != nullptr) {

    if (Tree->value > key) {
      findNode(Tree->left, key);
    } else if (Tree->value < key) {
      findNode(Tree->right, key);
    } else {
      return Tree;
    }
  } else
    return nullptr;
}

void copy(node *&tree, node *&tree2) {
  if (tree == nullptr)
    return; // cout << "Tree doesn\'t have any element" << endl;

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
  // node * DelNode = findNode(Tree, elem);
  deleteNode(elem, Tree);
  assert(findNode(Tree, elem) == nullptr);
  cout << "test delete node +++" << endl;
}

void test_copy(node *&Tree, node *&Tree2) {
  if (Tree == nullptr && Tree2 == nullptr)
    return; // cout << "Tree doesn\'t have any element" << endl;
  assert(Tree && Tree2);
  assert(Tree->value == Tree2->value);

  // addNode(tree->data, &tree2);
  if (Tree->left || Tree2->left)
    test_copy(Tree->left, Tree2->left);
  if (Tree->right || Tree2->right)
    test_copy(Tree->right, Tree2->right);
}

// ������� ������ �� �����
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
  // node *Tree = NULL;
  node *Tree2 = nullptr;
  node *Tree = nullptr;

  cout << "tree2 : " << endl;
  // test_addNode(567676, Tree2);
  // addNode(5676, Tree2);
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
  // addNode(20, Tree2);
  // node* test = addNode(50, Tree2);
  // show(Tree2, 0);
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

  // addNode(6, Tree2);
  show(Tree2, 0);

  copy(Tree2, Tree);
  cout << " Tree : " << endl;
  show(Tree, 0);
  test_copy(Tree2, Tree);
  cout << "testCopy +++" << endl;

  cout << "==============" << endl;
  deleteNode(50, Tree2);
  deleteNode(100, Tree2);
  deleteNode(150, Tree2);
  show(Tree2, 0);
  cout << Tree2->left->left->right->right->value << endl << endl;
  // cout << "tree : " << endl;
  // addNode(50,Tree);
  // addNode(120, Tree);
  // addNode(150, Tree);
  // addNode(1250, Tree);
  // addNode(3, &Tree);
  // addNode(1, &Tree);
  // show(Tree, 0);
  system("pause");
  return 0;
}