#ifndef AWILLI64_MAP_HPP
#define AWILLI64_MAP_HPP

#include <cstdio>
#include <random>

namespace cs540 {
  template <typename Key_T, typename Mapped_T>
  class Map {

    typedef std::pair<const Key_T, Mapped_T> ValueType;
    
    //Forward declerations of nested classes and struct
    class Iterator;
    //class ConstIterator;
    //class ReverseIterator;
    struct Node;
    //************************************

  public:
    //MAP METHOD PROTOTYPES
    //Constructors and Assignment Operator
    //Map            ();
    //Map            (const Map &);
    //Map &operator= (const Map &);
    //Map            (std::intializer_list<std::pair<const Key_T, Mapped_T>>);
    ~Map           ();
    //************************************

    //Size
    size_t size  () const;
    bool   empty () const;
    //************************************

    //Iterators
    //Iterator        begin  ();
    Iterator        end    ();
    //ConstIterator   begin  () const;
    //ConstIterator   end    () const;
    //ReverseIterator rbegin ();
    //ReverseIterator rend   ();
    //************************************

    //Element Access
    Iterator       find        (const Key_T &);
    //ConstIterator  find        (const Key_T &) const;
    //Mapped_T       &at         (const Key_T &);
    //const Mapped_T &at         (const Key_T &) const;
    //Mapped_T       &operator[] (const Key_T &);
    //************************************

    //Modifiers
    std::pair<Iterator, bool> insert (const ValueType &);
    //template <typename IT_T>
    //void                    insert (IT_T range_beg, IT_T range_end);
    //void                    erase  (Iterator pos);
    //void                    erase  (const Key_T &);
    void                    clear  ();
    //************************************

    //Comparison
    //bool operator== (const Map &, const Map &);
    //bool operator!= (const Map &, const Map &);
    //bool operator<  (const Map &, const Map &);
    //************************************


    void print() const;

  private:

   

    static const int MAX_LEVELS = 32;

    Node *head = new Node;
    Node *tail = nullptr;
    size_t numNodes = 0;
    int height = 0;



    struct Node {
      Node();
      Node(const ValueType &valueIn) : value(valueIn) {
	for (int i = 0; i < MAX_LEVELS; ++i) {
	  nextNodes[i] = nullptr;
	  prevNodes[i] = nullptr;
	}
      }

      Node *nextNodes[MAX_LEVELS];
      Node *prevNodes[MAX_LEVELS];
      ValueType value;
    };

    class Iterator {
    public:
      Iterator() : cur(nullptr) {}
      Iterator(Node* nodeIn) : cur(nodeIn) {}

      ValueType &operator*() const;

      friend bool operator==(const Iterator &op1, const Iterator &op2) {
	return (op1.cur == op2.cur);
      }

    private:
      Node *cur;
    };  //end class Iterator



  }; //end class Map

  template <typename Key_T, typename Mapped_T>
  Map<Key_T, Mapped_T>::~Map<Key_T, Mapped_T>() {
    clear();
    delete head;
  }

  template <typename Key_T, typename Mapped_T>
  size_t Map<Key_T, Mapped_T>::size() const {
    return numNodes;
  }

  template <typename Key_T, typename Mapped_T>
  bool Map<Key_T, Mapped_T>::empty() const {
    return (numNodes == 0);
  }

  template <typename Key_T, typename Mapped_T>
  std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool>  Map<Key_T, Mapped_T>::insert(const ValueType &valueIn) {
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> u(0,1);
    int insertHeight;
    bool repeat;
    
    Node *trav = head;
    Node *newNode = new Node(valueIn);
    
    insertHeight = -1;
    repeat = true;
    while (repeat && insertHeight < MAX_LEVELS) {
      repeat = u(e);
      insertHeight++;
    }
    if (insertHeight > height)
      height = insertHeight;
    printf("inserting %d at height %d\n", valueIn.first, insertHeight);
    
    int curLevel = insertHeight;

    //for each level we are inserting at (0 ... insertHeight)
    while (curLevel >= 0) {
      //move along current level until we find correct spot to insert
      while (trav->nextNodes[curLevel] != nullptr && trav->nextNodes[curLevel]->value.first < valueIn.first)
	trav = trav->nextNodes[curLevel];
      
      ++numNodes;

      //make insertion
      Node *tmp = trav->nextNodes[curLevel];
      newNode->prevNodes[curLevel] = trav;
      newNode->nextNodes[curLevel] = tmp;
      
      trav->nextNodes[curLevel] = newNode;
      
      if (tmp != nullptr) 
	tmp->prevNodes[curLevel] = newNode;
      //drop down to next level
	--curLevel;
    } //end while curLevel >= 0

    //The following return statement is UGLY... would like it to be
    // return ( { {newNode}, true} )
    // but won't compile.... anyway to fix?

    std::pair<Map<Key_T, Mapped_T>::Iterator, bool> retPair ({newNode}, true);
    return retPair;
  }
  
  template <typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::end () {
    Iterator retIt;
    return retIt;  
  }

    
  template <typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::find (const Key_T &keyIn) {
    printf("looking for %d\n", keyIn);
    int curHeight = height;
    Node* trav = head;

    
    
    while (curHeight >= 0) {
      
      while (trav->nextNodes[curHeight] != nullptr && trav->nextNodes[curHeight]->value.first < keyIn)
	trav = trav->nextNodes[curHeight];
      if (trav->nextNodes[curHeight] != nullptr && trav->nextNodes[curHeight]->value.first == keyIn) {
	return {trav->nextNodes[curHeight]};
      }
      --curHeight;
    }
    return (end());
  }
  
  template <typename Key_T, typename Mapped_T>
  void Map<Key_T, Mapped_T>::clear() {
  
    Node *cur = head->nextNodes[0];
    Node *prev;

    while (cur != nullptr) {
      prev = cur;
      cur = cur->nextNodes[0];
      delete prev;
    }

    for (int i = 0; i < MAX_LEVELS; ++i) {
      head->nextNodes[i] = nullptr;
      head->prevNodes[i] = nullptr;
    }
    height = 0;
    numNodes = 0;
  }

  template <typename Key_T, typename Mapped_T>
  void Map<Key_T, Mapped_T>::print() const {
    
    int curHeight = height;
    printf("height = %d\n", height);
    while (curHeight >= 0) {
      printf("level %d:", curHeight);
      Node *trav = head->nextNodes[curHeight];
      while(trav != nullptr) {
	printf("-->%d", trav->value.first);
	trav = trav->nextNodes[curHeight];
      }
      /*
	trav = tail;
	while (trav != nullptr) {
	printf("-->%d", trav->key);
	trav = trav->prevNodes[curHeight];
	}
      */
      --curHeight;
      printf("\n");
    }
  }

  template <typename Key_T, typename Mapped_T>
  Map<Key_T, Mapped_T>::Node::Node() {
    for (int i = 0; i < MAX_LEVELS; ++i) {
      nextNodes[i] = nullptr;
      prevNodes[i] = nullptr;
    }
  }

  /*
  template<typename Key_T, typename Mapped_T>
  Map<Key_T, Mapped_T>::Node::Node (const Map<Key_T, Mapped_T>::ValueType &valueIn) : value.first(valueIn.first), value.second(valueIn.second) {
    for (int i = 0; i < MAX_LEVELS; ++i) {
      nextNodes[i] = nullptr;
      prevNodes[i] = nullptr;
    }
    // value.first  = valueIn.first;
    //value.second = valueIn.second;
  }
  */
  /*
  template<typename Key_T, typename Mapped_T>
  Map<Key_T, Mapped_T>::Node::Node(const Node&) {
    
  }
  */


  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ValueType &Map<Key_T, Mapped_T>::Iterator::operator* () const {
    return cur->value;
  }
  
  /*
  template<typename Key_T, typename Mapped_T>
  bool operator==(const typename Map<Key_T, Mapped_T>::Iterator &op1, const typename Map<Key_T, Mapped_T>::Iterator &op2) {
    return (op1.cur == op2.cur);
  }
  */


} //end namespace cs540
#endif
