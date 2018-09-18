//
//  main.cpp
//
//  Created by Kyle Truong on 5/10/18.
//  Copyright © 2018 Kyle Truong. All rights reserved.
//

// Binary Search Tree w/ Lazy Deletion Client
#include <iostream>
#include <string>
#include <stack>
#include "FHlazySearchTree.h"
using namespace std;

template <typename Object>
class PrintObject
{
public:
   void operator()(Object obj)
   {
      cout << obj << " ";
   }
};

int main()
{
   int k;
   FHlazySearchTree<int> searchTree;
   PrintObject<int> intPrinter;

   searchTree.traverse(intPrinter);

   cout << "\ninitial size: " << searchTree.size() << endl;
   searchTree.insert(50);
   searchTree.insert(20);
   searchTree.insert(30);
   searchTree.insert(70);
   searchTree.insert(10);
   searchTree.insert(60);

   cout << "After populating -- traversal and sizes: \n";
   searchTree.traverse(intPrinter);
   cout << "\ntree 1 size: " << searchTree.size()
   << "  Hard size: " << searchTree.sizeHard() << endl;

   cout << "Collecting garbage on new tree - should be no garbage." << endl;
   searchTree.collectGarbage();
   cout << "tree 1 size: " << searchTree.size()
   << "  Hard size: " << searchTree.sizeHard() << endl;

   // test assignment operator
   FHlazySearchTree<int> searchTree2 = searchTree;

   cout << "\n\nAttempting 1 removal: \n";
   if (searchTree.remove(20))
      cout << "removed " << 20 << endl;
   cout << "tree 1 size: " << searchTree.size()
   << "  Hard size: " << searchTree.sizeHard() << endl;

   cout << "Collecting Garbage - should clean 1 item. " << endl;
   searchTree.collectGarbage();
   cout << "tree 1 size: " << searchTree.size()
   << "  Hard size: " << searchTree.sizeHard() << endl;

   cout << "Collecting Garbage again - no change expected. " << endl;
   searchTree.collectGarbage();
   cout << "tree 1 size: " << searchTree.size()
   << "  Hard size: " << searchTree.sizeHard() << endl;

   // test soft insertion and deletion:

   cout << "Adding 'hard' 22 - should see new sizes. " << endl;
   searchTree.insert(22);
   searchTree.traverse(intPrinter);
   cout << "\ntree 1 size: " << searchTree.size()
   << "  Hard size: " << searchTree.sizeHard() << endl;

   cout << "\nAfter soft removal. " << endl;
   searchTree.remove(22);
   searchTree.traverse(intPrinter);
   cout << "\ntree 1 size: " << searchTree.size()
   << "  Hard size: " << searchTree.sizeHard() << endl;

   cout << "Repeating soft removal. Should see no change. " << endl;
   searchTree.remove(22);
   searchTree.traverse(intPrinter);
   cout << "\ntree 1 size: " << searchTree.size()
   << "  Hard size: " << searchTree.sizeHard() << endl;

   cout << "Soft insertion. Hard size should not change. " << endl;
   searchTree.insert(22);
   searchTree.traverse(intPrinter);
   cout << "\ntree 1 size: " << searchTree.size()
   << "  Hard size: " << searchTree.sizeHard() << endl;

   cout << "\n\nAttempting 100 removals: \n";
   for (k = 100; k > 0; k--)
   {
      if (searchTree.remove(k))
         cout << "removed " << k << endl;
   }
   searchTree.collectGarbage();

   cout << "\nsearchTree now:\n";
   searchTree.traverse(intPrinter);
   cout << "\ntree 1 size: " << searchTree.size()
   << "  Hard size: " << searchTree.sizeHard() << endl;

   searchTree2.insert(500);
   searchTree2.insert(200);
   searchTree2.insert(300);
   searchTree2.insert(700);
   searchTree2.insert(100);
   searchTree2.insert(600);
   cout << "\nsearchTree2:\n";
   searchTree2.traverse(intPrinter);
   cout << "\ntree 2 size: " << searchTree2.size()
   << "  Hard size: " << searchTree2.sizeHard() << endl;

   cout << "\n*************** Additional Testing ***************\n" << endl;
   FHlazySearchTree<int> myTree;
   // Root
   myTree.insert(7);

   // Left subtree
   myTree.insert(4);
   myTree.insert(6);
   myTree.insert(5);
   myTree.insert(2);
   myTree.insert(1);
   myTree.insert(3);

   // Right subutree
   myTree.insert(15);
   myTree.insert(16);
   myTree.insert(14);
   myTree.insert(13);

   cout << "After populating - traversal and sizes: \n";
   myTree.traverse(intPrinter);
   cout << "\nmyTree size: " << myTree.size()
   << "  Hard size: " << myTree.sizeHard() << endl;

   cout << "\nMinimum: " << myTree.findMin() << endl;
   cout << "Maximum: " << myTree.findMax() << endl;

   cout << "\nMinimum after some soft deletions:" << endl;
   myTree.remove(1);
   myTree.remove(2);
   cout << myTree.findMin() << endl;

   myTree.remove(3);
   myTree.remove(4);
   myTree.remove(5);
   myTree.remove(6);
   cout << "\nAfter deleting root's left subtree - traversal and sizes: \n";
   myTree.traverse(intPrinter);
   cout << "\nmyTree size: " << myTree.size()
   << "  Hard size: " << myTree.sizeHard() << endl;

   cout << "\nMinimum after deleting root's left subtree - should return root:";
   cout << endl << myTree.findMin() << endl;

   cout <<"\nMinimum after deletion of left subtree AND root:" << endl;
   myTree.remove(7);
   cout << myTree.findMin() << endl;

   cout << "\nmyTree sizes after soft deletion of entire tree:" << endl;
   myTree.remove(15);
   myTree.remove(16);
   myTree.remove(14);
   myTree.remove(13);
   cout << "Soft size: " << myTree.size()
   << "  Hard size: " << myTree.sizeHard() << endl;

   cout << "\nfindMin() after soft deletion of entire tree" <<
   " - should throw exception:" << endl;
   try {
      myTree.findMin();
   }
   catch (...) {
      cout << "*** Error: Tree is empty ***" << endl << endl;
   }

   cout << "Testing garbage collection:" << endl;
   myTree.collectGarbage();
   cout << "Soft size: " << myTree.size()
   << "  Hard size: " << myTree.sizeHard() << endl;

   cout << "\nfindMin() after HARD deletion of entire tree" <<
   " - should throw exception:" << endl;
   try {
      myTree.findMin();
   }
   catch (...) {
      cout << "*** Error: Tree is empty ***" << endl << endl;
   }

   return 0;
}

/* ---------------------- Run --------------------------

 initial size: 0
 After populating -- traversal and sizes:
 10 20 30 50 60 70
 tree 1 size: 6  Hard size: 6
 Collecting garbage on new tree - should be no garbage.
 tree 1 size: 6  Hard size: 6


 Attempting 1 removal:
 removed 20
 tree 1 size: 5  Hard size: 6
 Collecting Garbage - should clean 1 item.
 tree 1 size: 5  Hard size: 5
 Collecting Garbage again - no change expected.
 tree 1 size: 5  Hard size: 5
 Adding 'hard' 22 - should see new sizes.
 10 22 30 50 60 70
 tree 1 size: 6  Hard size: 6

 After soft removal.
 10 30 50 60 70
 tree 1 size: 5  Hard size: 6
 Repeating soft removal. Should see no change.
 10 30 50 60 70
 tree 1 size: 5  Hard size: 6
 Soft insertion. Hard size should not change.
 10 22 30 50 60 70
 tree 1 size: 6  Hard size: 6


 Attempting 100 removals:
 removed 70
 removed 60
 removed 50
 removed 30
 removed 22
 removed 10

 searchTree now:

 tree 1 size: 0  Hard size: 0

 searchTree2:
 10 20 30 50 60 70 100 200 300 500 600 700
 tree 2 size: 12  Hard size: 12

 *************** Additional Testing ***************

 After populating - traversal and sizes:
 1 2 3 4 5 6 7 13 14 15 16
 myTree size: 11  Hard size: 11

 Minimum: 1
 Maximum: 16

 Minimum after some soft deletions:
 3

 After deleting root's left subtree - traversal and sizes:
 7 13 14 15 16
 myTree size: 5  Hard size: 11

 Minimum after deleting root's left subtree - should return root:
 7

 Minimum after deletion of left subtree AND root:
 13

 myTree sizes after soft deletion of entire tree:
 Soft size: 0  Hard size: 11

 findMin() after soft deletion of entire tree - should throw exception:
 *** Error: Tree is empty ***

 Testing garbage collection:
 Soft size: 0  Hard size: 0

 findMin() after HARD deletion of entire tree - should throw exception:
 *** Error: Tree is empty ***

 Program ended with exit code: 0

 ---------------------------------------------------------------------- */
