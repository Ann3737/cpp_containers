#include <iostream>
#include <initializer_list> // для использования initializer_list
#include <stdexcept>
#include <limits>

namespace s21{

  template <typename T>

  class List {

  private:
    struct Node { // стуртура списка
      T data;
      Node* next; // указатель на след
      Node* prev; // указатель на предыдущий
      Node(const T& value): data(value), next(nullptr), prev(nullptr){}
    };

    Node* head_;  // указатель на стуктуру первую
    Node* tail_;  // последняя
    size_t size_; // размер
  public:

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    //using iterator = T*;
    //using const_iterator = const T*;
    using size_type = size_t;

    /*------- КОНСРУКТОРЫ -------*/
    List(size_type n) : List() {
    for (size_type i = 0; i < n; ++i) {
      push_back(value_type{});  // создаём "пустой" объект типа T
    }
}

    /*конструктор с инициализацией списка*/
    List(std::initializer_list<value_type> const& items) : List() {
      for (const auto& item : items) {
        push_back(item);
      }
    }

    /*конструктор по умолчанию*/
    List(): head_(nullptr), tail_(nullptr), size_(0){}

    ~List(){
      while(head_){
        Node* temp = head_;
        head_ = head_->next;
        delete temp;
      }
    }

    /* конструктор копирования */
    List(const List& other) : List(){ // вызывает конструктор по умолчанию
      Node* current = other->head_; // начинаем с головы чужого списка
      while(current){ // пока есть элементы
        push_back(current->data); // копируем данные
        current = current->next;  // двигаемся дальше
      }
    }

    // — конструктор перемещения
    List(List&& other) noexcept {
      head_ = other.head_;
      tail_ = other.tail_;
      size_ = other.size_;
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    }

    List& operator=(const List& other){ // — копирующее присваивание
      if(this != &other){
        while(head_){
          Node* temp = head_;
          head_ = head_->next;
          delete temp;
        }
        tail_ = nullptr;
        size_ = 0;
         // скопировать элементы из другого списка
        Node* current = other.head_;
        while (current) {
          push_back(current->data);
          current = current->next;
        }
      }
      return *this;
    }

    // — перемещающее присваивание
    List& operator=(List&& other) noexcept {
      if(this != &other){
        while(head_){
          Node* temp = head_;
          head_ = head_->next;
          delete temp;
        }
        tail_ = nullptr;
        size_ = 0;
        head_ = other.head_;  // Просто перекидываем указатели
        tail_ = other.tail_;
        size_ = other.size_;

        other.head_ = nullptr;  // Обнуляем источник
        other.tail_ = nullptr;
        other.size_ = 0;
      }
    }

    /*  ------ МЕТОДЫ ------*/

    /*добавление в конец*/
    void push_back(const T& value){
      Node* newNode = new Node(value); // выделил новую структуру
      if(tail_){ // если последня стуктура не пустая
        tail_->next = newNode;  // добавли в конец
        newNode->prev = tail_;  //  добавли что последняя стала предидущей
        tail_ = newNode;  //  и последняя стала той что мы сделали
      }else {
        head_ = tail_ = newNode; // если пустая то создаем голову и хвост
      }
      ++size_;
    }

    /*удаления с конца*/
    void pop_back(){
      if(tail_){
        if (!tail_) return;  // если список пуст — ничего не делаем
        Node* temp = tail_; // временная станет хвостом
        tail_ = tail_->prev; // хвост становится предыдущем
        if(tail_){  //проверка если список из 1 элемента
          tail_->next = nullptr; // обнулил след указатель
        }else{
          head_ = nullptr; // если 1элемент обнуляем голову
        }
        delete temp; //удаляем сам хвост
        --size_;
      
      }
    }

    /*добавление в начало*/
    void push_front(const T& value){
      Node* newNode = new Node(value);
      if(head_){
        newNode->next = head_;
        head_->prev = newNode;
        head_ = newNode;
      }else {
        head_ = tail_ = newNode;  // Если список пуст
      }
      ++size_;
    }

    /*удаления с головы*/
    void pop_front(){
      if (!head_) return;
      Node* temp = head_;
      head_ = head_->next;
      if(head_){
        head_->prev = nullptr;
      }else {
        tail_ = nullptr;
      }
      delete temp;
      --size_;

    }
    // Метод для печати списка
    void print() {
      Node* current = head_; // новый указатель на голову
      while (current) {       // пока указатель на что то указывает
        std::cout << current->data << " ";  // принтуем от начала с пробелом
        current = current->next;  // присваеваем след элемент списка
      }
      std::cout << std::endl; // конец строки
    }
    
    /* ---- МЕТОДЫ ---- */

    /* проверка на пустоту */
    bool empty() const {
      return size_ == 0;
    }
    
    size_type max_size() const {
      return std::numeric_limits<size_type>::max() / sizeof(Node);
    } 
    /* возвращает длину списка */
    size_type size() const {
      return size_;
    }

    /*Очищает список*/
    void clear() {
      while (head_) {
        Node* temp = head_;
        head_ = head_->next;
        delete temp;
      }
      tail_ = nullptr;
      size_ = 0;
    }

    /*ссылка на первый элемент*/
    reference front() {
      if (!head_) {
        throw std::out_of_range("List is empty");
      }
      return head_->data;
    }
    
    /*ссылка на первый элемент*/
    const_reference front() const {
      if (!head_) {
        throw std::out_of_range("List is empty");
      }
      return head_->data;
    }

    /*ссылка на последний элемент*/
    reference back() {
      if (!tail_) {
        throw std::out_of_range("List is empty");
      }
      return tail_->data;
    }

    /*ссылка на последний элемент*/
    const_reference back() const {
      if (!tail_) {
        throw std::out_of_range("List is empty");
      }
      return tail_->data;
    }

    class iterator {
      friend class List<T>;

      private:

      /**/
      Node* current_;  // текущий узел
      
      public:

      /**/
      iterator(Node* node) : current_(node) {}
      // Разыменование — получаем значение
      T& operator*() const { return current_->data; }     // *it — получить данные

      /**/
      iterator& operator++() {
        current_ = current_->next;
        return *this;
      }

      /**/
      iterator& operator--() {
        current_ = current_->prev;
        return *this;
      }

      /**/
      bool operator!=(const iterator& other) const {      // сравнение
        return current_ != other.current_;
      }
      bool operator==(const iterator& other) const {
        return current_ == other.current_;
      }
    };

    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(nullptr ); }
    iterator insert(iterator pos, const_reference value){
      
      Node* newNode = new Node(value); // указатель на структуру и выделили стуктуре память
      Node* pos_node = pos.current_;  // получаем указатель на текущий узел
      newNode->next = pos_node; // в новой структуре указываем что след это иетратор
      if(pos_node){
        newNode->prev = pos_node->prev;
        if(pos_node->prev){
          pos_node->prev->next = newNode;
        }else {
          head_ = newNode;
        }
        pos_node->prev = newNode;
      }else {
        // Вставка в конец (pos == end())
        newNode->prev = tail_;
        if (tail_) tail_->next = newNode;
        tail_ = newNode;
        if (!head_) head_ = newNode;
      }
      ++size_;
      return iterator(newNode);
        
    }

    /*
       ---- ИТЕРАТОРЫ ----)
🔁 Итераторы (обязательно):
 iterator begin()

 iterator end()

 const_iterator begin() const

 const_iterator end() const

Для этого нужно реализовать собственный итератор, потому что T* не подходит для List (у тебя Node*, а не массив в памяти).

➕ Метод вставки по итератору:
 iterator insert(const_iterator pos, const_reference value)

❌ Метод удаления по итератору:
 iterator erase(const_iterator pos)

    🔁 Операции обмена и слияния:
 void swap(List& other)

 void merge(List& other)

 void splice(const_iterator pos, List& other)

 void reverse()

 void unique()

 void sort()
    */
  
  };
}

/*
int main(){
  s21::List<int> myList = {1, 2, 3, 4};
  myList.print();
  myList.push_front(99);
  myList.print();

  return 0;
}*/