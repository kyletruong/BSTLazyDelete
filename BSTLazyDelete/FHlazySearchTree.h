// File FHlazySearchTree.h
// Template definitions for FHsearchTrees, which are general trees
#ifndef FHLAZYSEARCHTREE_H
#define FHLAZYSEARCHTREE_H

// ---------------------- FHlazySearchTreeNode Prototype --------------------------
template <class Comparable>
class FHlazySearchTreeNode
{
public:
   FHlazySearchTreeNode( const Comparable & d = Comparable(),
      FHlazySearchTreeNode *lt = NULL,
      FHlazySearchTreeNode *rt = NULL, bool dltd = false)
      : lftChild(lt), rtChild(rt), data(d), deleted(dltd)
   { }

   FHlazySearchTreeNode *lftChild, *rtChild;
   Comparable data;
   bool deleted;

   // for use only with AVL Trees
   virtual int getHeight() const { return 0; }
   virtual bool setHeight(int height) { return true; }
}; 

// ---------------------- FHlazySearchTree Prototype --------------------------
template <class Comparable>
class FHlazySearchTree
{
protected:
   int mSize; // tracks only undeleted nodes
   FHlazySearchTreeNode<Comparable> *mRoot;
   int mSizeHard; // tracks number of hard nodes, including soft deleted nodes

public:
   FHlazySearchTree() { mSize = 0; mSizeHard = 0; mRoot = NULL; }
   FHlazySearchTree(const FHlazySearchTree &rhs)
      { mRoot = NULL; mSize = 0; mSizeHard = 0; *this = rhs; }
   ~FHlazySearchTree() { clear(); }

   const Comparable &findMin() const;
   const Comparable &findMax() const;
   const Comparable &find(const Comparable &x) const;

   bool empty() const { return (mSize == 0); }
   int size() const { return mSize; }
   int sizeHard() const { return mSizeHard; }
   void clear() { makeEmpty(mRoot); }
   const FHlazySearchTree & operator=(const FHlazySearchTree &rhs);

   bool insert(const Comparable &x);
   bool remove(const Comparable &x);
   bool contains(const Comparable &x) const { return find(mRoot, x) != NULL; }

   template <class Processor>
   void traverse(Processor func) const { traverse(mRoot, func); }
   int showHeight() const { return findHeight(mRoot); }
   void collectGarbage();

protected:
   FHlazySearchTreeNode<Comparable> *clone( FHlazySearchTreeNode<Comparable> *root) const;
   FHlazySearchTreeNode<Comparable> *findMin(FHlazySearchTreeNode<Comparable> *root) const;
   FHlazySearchTreeNode<Comparable> *findMax(FHlazySearchTreeNode<Comparable> *root) const;
   FHlazySearchTreeNode<Comparable> *find(FHlazySearchTreeNode<Comparable> *root,
      const Comparable &x) const;
   bool insert(FHlazySearchTreeNode<Comparable> * &root,
      const Comparable &x);
   bool remove(FHlazySearchTreeNode<Comparable> * &root, const Comparable &x);
   void makeEmpty(FHlazySearchTreeNode<Comparable> * &subtreeToDelete);
   template <class Processor>
   void traverse(FHlazySearchTreeNode<Comparable> *treeNode,
      Processor func, int level = -1) const;
   int findHeight(FHlazySearchTreeNode<Comparable> *treeNode, int height = -1) const;
   bool removeHard(FHlazySearchTreeNode<Comparable> *&root);
   void collectGarbage(FHlazySearchTreeNode<Comparable> *&root);
   
public:
   // for exception throwing
   class EmptyTreeException {};
   class NotFoundException {};
};

// FHlazySearchTree public method definitions -----------------------------
template <class Comparable>
const Comparable & FHlazySearchTree<Comparable>::findMin() const
{
   FHlazySearchTreeNode<Comparable> *resultNode;

   resultNode = findMin(mRoot);
   if (resultNode == NULL)
      throw NotFoundException();
   return resultNode->data;
}

template <class Comparable>
const Comparable & FHlazySearchTree<Comparable>::findMax() const
{
   FHlazySearchTreeNode<Comparable> *resultNode;

   resultNode = findMax(mRoot);
   if (resultNode == NULL)
      throw NotFoundException();
   return resultNode->data;
}

template <class Comparable>
const Comparable &FHlazySearchTree<Comparable>::find(
   const Comparable &x) const
{ 
   FHlazySearchTreeNode<Comparable> *resultNode;
   
   resultNode = find(mRoot, x);
   if (resultNode == NULL)
      throw NotFoundException();
    return resultNode->data;
}

template <class Comparable>
const FHlazySearchTree<Comparable> &FHlazySearchTree<Comparable>::operator=
   (const FHlazySearchTree &rhs)
{
   if (&rhs != this) 
   {
      clear();
      mRoot = clone(rhs.mRoot);
      mSize = rhs.size();
      mSizeHard = rhs.sizeHard();
   }
   return *this;
}

template <class Comparable>
bool FHlazySearchTree<Comparable>::insert(const Comparable &x)
{
   if (insert(mRoot, x))
   {
      mSize++;
      return true;
   }
   return false;
}

template <class Comparable>
bool FHlazySearchTree<Comparable>::remove(const Comparable &x)
{
   if (remove(mRoot, x))
   {
      mSize--;
      return true;
   }
   return false;
}

template <class Comparable>
template <class Processor>
void FHlazySearchTree<Comparable>::traverse( FHlazySearchTreeNode<Comparable> *treeNode,
   Processor func, int level) const
{
   if (treeNode == NULL)
      return;
   // we're not doing anything with level but its there in case we want it
   traverse(treeNode->lftChild, func, level + 1);
   if (!treeNode->deleted)
      func(treeNode->data);
   traverse(treeNode->rtChild, func, level + 1);
}


// FHlazySearchTree private method definitions -----------------------------
template <class Comparable>
FHlazySearchTreeNode<Comparable> *FHlazySearchTree<Comparable>::clone(
   FHlazySearchTreeNode<Comparable> *root) const
{
   FHlazySearchTreeNode<Comparable> *newNode;
   if (root == NULL)
      return NULL;

   newNode =  new FHlazySearchTreeNode<Comparable>(
      root->data, 
      clone(root->lftChild), clone(root->rtChild));
   return newNode;
}

template <class Comparable>
FHlazySearchTreeNode<Comparable> *FHlazySearchTree<Comparable>::findMin(
   FHlazySearchTreeNode<Comparable> *root) const
{
   FHlazySearchTreeNode<Comparable> *returnNode = NULL;

   if (root == NULL)
      return NULL;

   if (root->lftChild == NULL) {
      if (!root->deleted)
         return root;
      else
         return findMin(root->rtChild);
   }

   returnNode = findMin(root->lftChild);

   if (returnNode == NULL) {
      if (!root->deleted)
         return root;
      else
         return findMin(root->rtChild);
   }

   return returnNode;
}

template <class Comparable>
FHlazySearchTreeNode<Comparable> *FHlazySearchTree<Comparable>::findMax(
   FHlazySearchTreeNode<Comparable> *root) const
{
   FHlazySearchTreeNode<Comparable> *returnNode = NULL;

   if (root == NULL)
      return NULL;

   if (root->rtChild == NULL) {
      if (!root->deleted)
         return root;
      else
         return findMax(root->lftChild);
   }

   returnNode = findMax(root->rtChild);

   if (returnNode == NULL) {
      if (!root->deleted)
         return root;
      else
         return findMax(root->lftChild);
   }

   return returnNode;
}

template <class Comparable>
FHlazySearchTreeNode<Comparable>* FHlazySearchTree<Comparable>::find(
   FHlazySearchTreeNode<Comparable> *root, const Comparable &x) const
{
   if (root == NULL)
      return NULL;

   if (x < root->data)
      return find(root->lftChild, x);
   if (root->data < x)
      return find(root->rtChild, x);

   // found data, check if soft deleted
   if (root->deleted == true)
      return NULL;
   return root;
}

template <class Comparable>
bool FHlazySearchTree<Comparable>::insert(
   FHlazySearchTreeNode<Comparable> * &root, const Comparable &x)
{
   if (root == NULL)
   {
      root = new FHlazySearchTreeNode<Comparable>(x, NULL, NULL);
      mSizeHard++;
      return true;
   }
   else if (x < root->data)
      return insert(root->lftChild, x);
   else if (root->data < x)
      return insert(root->rtChild, x);

   // inserting node that has been marked soft deleted
   if (root->deleted) {
      root->deleted = false;
      return true;
   }
   return false; // duplicate
}

template <class Comparable>
bool FHlazySearchTree<Comparable>::removeHard(FHlazySearchTreeNode<Comparable> *&root) {
   if (root == NULL)
      return false;

   // Postorder traversal
   removeHard(root->lftChild);
   removeHard(root->rtChild);

   if (root->deleted) {
      if (root->lftChild != NULL && root->rtChild != NULL)
      {
         FHlazySearchTreeNode<Comparable> *minNode = findMin(root->rtChild);
         root->data = minNode->data;
         root->deleted = false;
         minNode->deleted = true;
         removeHard(root->rtChild);
      }
      else
      {
         FHlazySearchTreeNode<Comparable> *nodeToRemove = root;
         root = (root->lftChild != NULL)? root->lftChild : root->rtChild;
         delete nodeToRemove;
         mSizeHard--;
      }
   }
   return true;
}

template <class Comparable>
void FHlazySearchTree<Comparable>::collectGarbage(FHlazySearchTreeNode<Comparable> *&root) {
   removeHard(root);
}

template <class Comparable>
void FHlazySearchTree<Comparable>::collectGarbage() {
   collectGarbage(mRoot);
}

template <class Comparable>
bool FHlazySearchTree<Comparable>::remove(
   FHlazySearchTreeNode<Comparable> * &root, const Comparable &x)
{
   if (root == NULL)
      return false;

   if (x < root->data)
      return remove(root->lftChild, x);
   if (root->data < x)
      return remove(root->rtChild, x);

   // found the node
   if (root->deleted)
      return false;
   root->deleted = true;
   return true;
}

template <class Comparable>
void FHlazySearchTree<Comparable>::makeEmpty(
   FHlazySearchTreeNode<Comparable> * &subtreeToDelete)
{
   if (subtreeToDelete == NULL)
      return;

   // remove children
   makeEmpty(subtreeToDelete->lftChild);
   makeEmpty(subtreeToDelete->rtChild);

   // clear client's pointer
   delete subtreeToDelete;
   subtreeToDelete = NULL;
   --mSize;
   --mSizeHard;
}

template <class Comparable>
int FHlazySearchTree<Comparable>::findHeight( FHlazySearchTreeNode<Comparable> *treeNode,
   int height ) const
{
   int leftHeight, rightHeight;

   if (treeNode == NULL)
      return height;
   height++;
   leftHeight = findHeight(treeNode->lftChild, height);
   rightHeight = findHeight(treeNode->rtChild, height);
   return (leftHeight > rightHeight)? leftHeight : rightHeight;
}
#endif
