//
// ctors-and-dtors-examples.cpp
//
// Created by massimo on 8/20/18.
//
#include "ctors-and-dtors-examples.h"

#include <iostream>
#include <memory>
////////////////////////////////////////////////////////////////////////////////

class A {
public:
  A() {
    std::cout << "CTOR A()\n";
  }
  virtual ~A();
  A([[maybe_unused]] const A& rhs) {
    std::cout << "COPY CTOR A()\n";
  }
  A& operator=([[maybe_unused]] const A& rhs) {
    std::cout << "COPY ASSIGNMENT OPERATOR= A()\n";
    return *this;
  }
  A([[maybe_unused]] const A&& rhs) {
    std::cout << "MOVE CTOR A()\n";
  }
  A& operator=([[maybe_unused]] const A&& rhs) {
    std::cout << "MOVE ASSIGNMENT OPERATOR= A()\n";
    return *this;
  }
};

A::~A() {
  std::cout << "DTOR ~A()\n";
}

class B final : public A {
public:
  //B() {  // this line has the same effect of the line below
  B() : A() {
    std::cout << "CTOR B()\n";
  }
  ~B();
  B([[maybe_unused]] const B& rhs) : A(rhs) {
    std::cout << "COPY CTOR B()\n";
  }
  B& operator=([[maybe_unused]] const B& rhs) {
    std::cout << "COPY ASSIGNMENT OPERATOR= B()\n";
    return *this;
  }
  B([[maybe_unused]] const B&& rhs) : A(rhs) {
    std::cout << "MOVE CTOR B()\n";
  }
  B& operator=([[maybe_unused]] const B&& rhs) {
    std::cout << "MOVE ASSIGNMENT OPERATOR= B()\n";
    return *this;
  }
};

B::~B() {
  std::cout << "DTOR ~B()\n";
}

void example_1() {
  std::cout << "--------example 1-------\n";
// this block prints:
//  CTOR A() -> delegating ctor for A is called before class B's ctor and a temporary object is created
//  CTOR B() -> a temporary object B is created
//  COPY/MOVE CTOR A() -> the temporary object B is copy/move-constructed to a;
//                     -> copy/move-ctor for B is not called then the B's temp object will be destroyed
//                     -> copy ctor is called if the move ctor is not defined, otherwise the move ctor is called
//  DTOR ~B()  -> the temporary objects (the sources for the copy/move) are destroyed
//  DTOR ~A()  -> in reverse order
//  --- CTOR phase ended ---
//  DTOR ~A()  --> the object a is destroyed
//  --- DTOR phase ended ---
  {
    A a = B{};
    //A a{B{}};  // the same effect of the line above
    std::cout << "--- CTOR phase ended ---\n";
  }
  std::cout << "--- DTOR phase ended ---\n";
  std::cout << "------------------------\n\n";
}


void example_2() {
  std::cout << "--------example 2-------\n";
// this block prints:
//  CTOR A()  --> new B: delegating ctor for A is called before B's ctor
//  CTOR B()  --> new B: B's ctor
//  --- CTOR phase ended ---
//  DTOR ~B()  --> delete a
//  DTOR ~A()  --> delete a
//  --- DTOR phase ended ---
  {
    A *a {new B};

    std::cout << "--- CTOR phase ended ---\n";

    delete a;

    std::cout << "--- DTOR phase ended ---\n";
  }
  std::cout << "------------------------\n\n";
}

void example_3() {
  std::cout << "--------example 3-------\n";
  // this block prints:
//  CTOR A()  --> new B: delegating ctor for A is called before B's ctor
//  CTOR B()  --> new B: B's ctor
//  --- CTOR phase ended ---
//  DTOR ~B()  --> delete a
//  DTOR ~A()  --> delete a
//  --- DTOR phase ended ---
  {
    std::unique_ptr<A> upa{std::make_unique<B>()};

    std::cout << "--- CTOR phase ended ---\n";
  }
  std::cout << "--- DTOR phase ended ---\n";
  std::cout << "------------------------\n\n";
}

void example_4() {
  std::cout << "--------example 4-------\n";
  // this block prints:
//  CTOR A()  --> new B: delegating ctor for A is called before B's ctor
//  CTOR B()  --> new B: B's ctor
//  --- CTOR phase ended ---
//  DTOR ~B()  --> delete a
//  DTOR ~A()  --> delete a
//  --- DTOR phase ended ---
  {
    std::shared_ptr<A> upa{std::make_shared<B>()};

    std::cout << "--- CTOR phase ended ---\n";
  }
  std::cout << "--- DTOR phase ended ---\n";
  std::cout << "------------------------\n\n";
}

void ctors_and_dtors_examples() {
  example_1();
  example_2();
  example_3();
  example_4();
}
