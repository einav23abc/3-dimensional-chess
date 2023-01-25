#ifndef LinkedList_HPP
#define LinkedList_HPP

template <class T> class LinkedListNodeD {
    public:
        LinkedListNodeD( ){
            this->value;
            this->next = nullptr;
            return;
        };
        LinkedListNodeD( T value ){
            this->value = value;
            this->next = nullptr;
            return;
        };
        ~LinkedListNodeD(){
            delete value;
            return;
        };
        
        T getValue(){
            return this->value;
        };
        void setValue( T value ){
            this->value = value;
            return;
        };

        LinkedListNodeD<T>* getNext(){
            return this->next;
        };
        void setNext( LinkedListNodeD<T>* next ){
            this->next = next;
            return;
        };

        static void appendNode( LinkedListNodeD<T>** headptr , LinkedListNodeD<T>* node ){
            if ( *headptr == nullptr ){
                *headptr = node;
                return;
            };
            LinkedListNodeD<T>* current = (*headptr);
            while( current->getNext() != nullptr ){
                current = current->getNext();
            };
            current->setNext( node );
            return;
        };
        
    private:
        T value;
        LinkedListNodeD<T>* next;
};


template <class T> class LinkedListNode {
    public:
        LinkedListNode( ){
            this->value;
            this->next = nullptr;
            return;
        };
        LinkedListNode( T value ){
            this->value = value;
            this->next = nullptr;
            return;
        };
        ~LinkedListNode(){
            return;
        };
        
        T getValue(){
            return this->value;
        };
        void setValue( T value ){
            this->value = value;
            return;
        };

        LinkedListNode<T>* getNext(){
            return this->next;
        };
        void setNext( LinkedListNode<T>* next ){
            this->next = next;
            return;
        };

        static void appendNode( LinkedListNode<T>** headptr , LinkedListNode<T>* node ){
            if ( *headptr == nullptr ){
                *headptr = node;
                return;
            };
            LinkedListNode<T>* current = (*headptr);
            while( current->getNext() != nullptr ){
                current = current->getNext();
            };
            current->setNext( node );
            return;
        };
        
    private:
        T value;
        LinkedListNode<T>* next;
};

#endif