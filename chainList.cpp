#include <iostream>
#include <random>
#include <ctime>

using namespace std;

/*
 * Declare class Chain. This is only to make class ChainNode aware
 * that there will be a definition of class Chain later on.
 */
template <typename T>
class Chain;

// Define class ChainNode
template <typename T> 
class ChainNode{ 
    friend Chain<T>;
    private: 
        T data;
        ChainNode<T> *link;
};

// Define class Chain
template <typename T>
class Chain{
    public:
        Chain(int maxSize = 1000){first = 0;}     // constructor (first = 0 because the chain list is empty in the beginning)
        ~Chain();               // destructor
        bool isEmpty() const {return first == 0;}   // if first = 0 it returns true, meaning that the list is empty
        int Length() const;
        bool Find(int k, T& x) const;
        int Search(const T& x) const;
        Chain<T>& Delete(int k, T& x);
        Chain<T>& Insert(int k, const T& x);
        void Sort(const Chain<T>& x);
        void RemoveDuplicates();
        void Output(ostream& out) const;
    private: 
        ChainNode<T> *first;    // pointer to the  first node
};

// ~Chain()
template <typename T>
Chain<T>::~Chain(){
    // Chain destructor, deletes all nodes in the chain
    ChainNode<T> *next;     // next node
    while (first){
        next = first->link;
        delete first;
        first = next;
    }
}

// Length()
template <typename T>
int Chain<T>::Length() const{
    // return the number of elements in the chain
    ChainNode<T> *current = first;
    int len = 0;
    while (current){
        len++;
        current = current->link;
    }
    return len;
}

// Find()
template <typename T>
bool Chain<T>::Find(int k, T& x) const{
    // set x to the k'th element in the chain
    // return false if no k'th, true otherwise
    if (k < 1) return false;
    ChainNode<T> *current = first;  // current now points at the first element of the list
    int index = 1;                  // index of current
            
    while (index < k && current){   // if there is no next element (we are in the last element), current = 0 so the loop won't be executed again
        current = current->link;
        index ++;
    }
    
    if (current){                   // writing current alone means current != 0
        x = current->data;
        return true;
    }

    return false;                   // we didn't find the k'th element
}

// Search()
template <typename T>
int Chain<T>::Search(const T& x) const{
    // locate x, return the position of x if found
    // return 0 if x is not in the chain
    ChainNode<T> *current = first;
    int index = 1;

    while (current && current->data != x){
        current = current->link;
        index ++;
    }

    if (current){
        return index;   // returns the position in which the number x was found
    }

    return 0;
}

// Delete()
template <class T>
Chain<T>& Chain<T>::Delete(int k, T& x){
    // set x to the k'th element and delete it
    // throw OutOfBounds exception if no k'th element
    if (k < 1 || !first)
        throw this->OutOfBounds();        // no k'th element
    
    // p will eventually point to the k'th node
    ChainNode<T> *p = first;

    // move p to the k'th element and remove from the chain
    if (k == 1)                     // p already at k
        first = first->link;        // remove
    else{                           // use q to get to k-1'st 
        ChainNode<T> *q = first;
        for (int index = 1; index < k - 1 && q; index ++)
            q = q->link;
        if (!q || !q->link)
            throw this->OutOfBounds();    // no k'th
        p = q->link;                // k'th
        q->link = p->link;          // remove from the chain
    }
    // save the k'th element and free node p 
    x = p->data;
    delete p;
    return *this;
}

// Insert()
template <class T>
Chain<T>& Chain<T>::Insert(int k, const T& x){
    // insert x after the k'th element
    // throw OutOfBounds exception if no k'th element exists
    // pass NoMem exception if inadequate space

    // if (k < 0) throw this->OutOfBounds();
    // p will eventually point to the k'th node
    ChainNode<T> *p = first;
    for (int index = 1; index < k && p; index ++)   // move p to k'th
        p = p->link;
    // if (k > 0 && !p) throw this->OutOfBounds();            // no k'th element

    // insert
    ChainNode<T> *y = new ChainNode<T>;
    y->data = x;
    if (k){
        // insert after p
        y->link = p->link;
        p->link = y;
    }
    else{
        // insert as first element
        y->link = first;
        first = y;
    }
    return *this;
} 

// Output()
template <class T>
void Chain<T>::Output(ostream& out) const{
    // insert the stream elements into the stream out
    ChainNode<T> *current;
    for (current = first; current; current = current->link)
        out << current->data << " ";
}

// overload <<
template <class T>
ostream& operator<<(ostream& out, const Chain<T>& x){
    x.Output(out);
    return out;
}

// function used to remove duplicates from the lists we are creating randomly
template <class T>
void Chain<T>::RemoveDuplicates(){
    ChainNode<T> *p = first;
    ChainNode<T> *p2, *dup;

    // pick elements one by one
    while (p != NULL && p->link != NULL){
        p2 = p;

        // compare the picked element with the rest of the elements
        while (p2->link != NULL){
            // if duplicate then delete it
            if (p->data == p2->link->data){
                dup = p2->link;
                p2->link = p2->link->link;
                delete(dup);
            }
            else 
                p2 = p2->link;
        }
        p = p->link;
    } 
}

// function used to sort the list given as a parameter (using bubble-sort)
template <typename T>
void Chain<T>::Sort(const Chain<T>& x){
    ChainNode<T> *cur1 = first;
    ChainNode<T> *cur2 = first;

    for (int i = 0; i < x.Length(); i++){
        for (int j = 0; j < x.Length() - 1; j++){
            if (cur1->data < cur2->data){
                int temp = cur1->data;
                cur1->data = cur2->data;
                cur2->data = temp;
            }
            cur2 = cur2->link;
        }
        cur2 = first;
        cur1 = first->link;
        for (int k = 0; k < i; k++){
            cur1 = cur1->link;
        }
    }    
}

// main function 
int main(int argc, char *argv[]){

    // initialize a random number generator 
    default_random_engine generator(time(0));
    
    // ask the user how many lists he wants to create
    int lists_nr;
    cout << "\nHow many lists do you want to create? Type the number: " << endl;
    cin >> lists_nr;
    cout << "Creating " << lists_nr << " chain lists now!\n" << endl;

    // create a ChainList that will contain other ChainLists
    Chain<Chain<int>> ChainList_List(lists_nr);

    // creating the lists
    for (int i = 0; i < lists_nr; i++){
        
        // define the size of the list (the numbers given can be changed)
        uniform_int_distribution<int> list_size_distribution(25, 50); 
        int first_try = list_size_distribution(generator);
        int random_list_size = list_size_distribution(generator);

        Chain<int> ChainList(random_list_size);

        // adding data to each node of the list that has been created
        for (int j = 0; j < random_list_size; j++){
            
            // define the range of the data elements in each node (the numbers given can be changed)
            uniform_int_distribution<int> data_element_distribution(0, 50); 
            int first_try = data_element_distribution(generator);
            int random_data_element = data_element_distribution(generator);

            ChainList.Insert(j, random_data_element);
        }

        // insert the list that was created in the ChainList_List
        ChainList_List.Insert(i, ChainList);        
    }   

    cout << "These are the lists that were created without any changes: " << "\n" << endl;
    for (int index = 0; index < lists_nr; index ++){
        Chain<int> z;
        ChainList_List.Find(index + 1, z);
        cout << "List " << index + 1 << ": " << z << endl; 
    }

    // -----RUNNING THE ALGORITHM-----

    cout << "\n" << "Now we are going to implement an algorithm in order to solve the problem!" << "\n" << endl;

    // create the new output list in which we will save the elements we are looking for
    Chain<int> OutputList(1000);

    cout << "These are the lists that were created after removing their duplicate elements: " << "\n" << endl;
    // remove the duplicates from the lists we created 
    for (int x = 0;  x < lists_nr;  x++){
        Chain<int> y;
        ChainList_List.Find(x + 1, y);
        y.RemoveDuplicates();
        cout << "List " << x + 1 << ": " << y << endl; 
    }

    // search for an element in all the lists
    // if the element exists in more than half of the lists, add it to the output list, otherwise do nothing

    // outer loop for the lists 
    for (int lists_index = 0; lists_index < lists_nr; lists_index ++){
        
        // find the current list
        Chain<int> current_list;
        ChainList_List.Find(lists_index + 1, current_list);

        // inside loop for each element
        for (int element_index = 0; element_index < current_list.Length(); element_index ++){

            int occ_nr = 0;     // occurence number for the current element

            // find the value of the current element
            int current_element; 
            current_list.Find(element_index + 1, current_element);

            // search if the current element exists in the previous lists saved in the ChainList_List
            for (int idx = 1; idx < element_index; idx ++){
                
                Chain<int> check_list;
                ChainList_List.Find(idx, check_list);

                if (check_list.Search(current_element) != 0){       // function Search() returns 0 if the element was not found
                    occ_nr += 1;
                }
                
            }

            // search if the current element exists in the next lists saved in the ChainList_List
            for (int idx = element_index; idx < lists_nr; idx ++){
                
                Chain<int> check_list;
                ChainList_List.Find(idx + 1, check_list);

                if (check_list.Search(current_element) != 0){       // function Search() returns 0 if the element was not found
                    occ_nr += 1;
                }
                
            }

            // if the element we are searching for finally exists in more than half of the lists that were initially created, add it to the output list
            if (occ_nr > lists_nr/2){
                OutputList.Insert(lists_index, current_element);
            }
        }
    }

    OutputList.RemoveDuplicates();
    OutputList.Sort(OutputList);

    cout << "\nFinal OutputList after removing its duplicate items and sorting it: " << OutputList << endl; 
    
    return 0;
}
