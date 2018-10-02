//
// bench.cpp
//
// Created by massimo on 8/20/18.
//
#include "bench.h"

#include <utility>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
//#include <ctime>
//#include <ratio>
#include <chrono>
////////////////////////////////////////////////////////////////////////////////
using data_t = C;

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchVector(const size_t containerSize, const bool reserve)
{
  using container = std::vector<data_t>;

  std::chrono::duration<double> pushBackTimeSpan{};
  std::chrono::duration<double> emplaceBackTimeSpan{};

  std::chrono::high_resolution_clock::time_point tDtorStart{};
  std::chrono::high_resolution_clock::time_point tDtorEnd{};
  std::chrono::duration<double> tDtorSpan{};

  {
    container v1;
    container v2;

    std::cout << "\n------------------------>>> vector bench start <<<--------------------------\n";
    if (reserve)
    {
      std::cout << "Reserved space in the vectors for " << containerSize << " elements\n\n";
      v1.reserve(containerSize);
      v2.reserve(containerSize);
    }
    else
    {
      std::cout << "No reserved space is made in the vectors\n\n";
    }

    std::cout << "capacities: v1: " << v1.capacity() << " - v2: " << v2.capacity() << "\n";
    std::cout << "sizes:      v1: " << v1.size() << " - v2: " << v2.size() << "\n\n";

    std::chrono::high_resolution_clock::time_point t1{};
    std::chrono::high_resolution_clock::time_point t2{};
    std::chrono::duration<double> timeSpan{};

    {
      std::cout << "------------------ vector push_back loop ------------------\n\n";
      std::cout << "Pushing back " << containerSize << " elements\n";
      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i{0}; i < containerSize; ++i)
      {
        data_t c(i, "v1");
        v1.push_back(c);
        // this commented line is a bit faster than the above two lines
        //v1.push_back(data_t(i, "v1"));
      }
      t2 = std::chrono::high_resolution_clock::now();

      pushBackTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      auto [objectsCreated_C_v1, objectsAlive_C_v1, objectsDestroyed_C_v1, tooManyDestructions_C_v1] = data_t::getObjectCounters();
      auto [copyConstructions_C_v1, copyAssignments_C_v1, moveConstructions_C_v1, moveAssignments_C_v1] = data_t::getCopyMoveCounters();

      std::cout << "\nvector v1 push_back loop:"
                << "\n--> objects created    "
                << objectsCreated_C_v1
                << "\n--> objects alive      "
                << objectsAlive_C_v1
                << "\n--> objects destroyed  "
                << objectsDestroyed_C_v1
                << "\n--> copy constructions "
                << copyConstructions_C_v1
                << "\n--> copy assignments   "
                << copyAssignments_C_v1
                << "\n--> move constructions "
                << moveConstructions_C_v1
                << "\n--> move assignments   "
                << moveAssignments_C_v1
                << "\nv1 size:     " << v1.size() << "\n"
                << "v1 capacity: " << v1.capacity() << "\n"
                << "push_back loop took " << timeSpan.count() << " seconds\n"
                << "------------------------------------------------\n\n"
                << "----------- vector emplace_back loop -----------\n\n"
                << "Emplacing back " << containerSize << " elements\n";

      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < containerSize; ++i)
      {
        v2.emplace_back(i, "v2");
      }
      t2 = std::chrono::high_resolution_clock::now();

      emplaceBackTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      auto [objectsCreated_C_v2, objectsAlive_C_v2, objectsDestroyed_C_v2, tooManyDestructions_C_v2] = data_t::getObjectCounters();
      auto [copyConstructions_C_v2, copyAssignments_C_v2, moveConstructions_C_v2, moveAssignments_C_v2] = data_t::getCopyMoveCounters();

      std::cout << "\nvector v2 emplace_back loop:"
                << "\n--> objects created    "
                << objectsCreated_C_v2 - objectsCreated_C_v1
                << "\n--> objects alive      "
                << objectsAlive_C_v2 - objectsAlive_C_v1
                << "\n--> objects destroyed  "
                << objectsDestroyed_C_v2 - objectsDestroyed_C_v1
                << "\n--> copy constructions "
                << copyConstructions_C_v2 - copyConstructions_C_v1
                << "\n--> copy assignments   "
                << copyAssignments_C_v2 - copyAssignments_C_v1
                << "\n--> move constructions "
                << moveConstructions_C_v2 - moveConstructions_C_v1
                << "\n--> move assignments   "
                << moveAssignments_C_v2 - moveAssignments_C_v1
                << "\nv2 size:     " << v2.size() << "\n"
                << "v2 capacity: " << v2.capacity() << "\n"
                << "emplace_back loop took " << timeSpan.count() << " seconds.\n"
                << "----------------------------------------------------\n"
                << std::endl;
    }
    std::cout << "---------------------DTOR phase---------------------\n";
    tDtorStart = std::chrono::high_resolution_clock::now();
  }
  tDtorEnd = std::chrono::high_resolution_clock::now();
  tDtorSpan = std::chrono::duration_cast<std::chrono::duration<double>>(tDtorEnd - tDtorStart);

  auto [objectsCreated_C, objectsAlive_C, objectsDestroyed_C, tooManyDestructions_C] = data_t::getObjectCounters();
  auto [copyConstructions_C, copyAssignments_C, moveConstructions_C, moveAssignments_C] = data_t::getCopyMoveCounters();

  std::cout << "--> objects created    "
            << objectsCreated_C
            << "\n--> objects alive      "
            << objectsAlive_C
            << "\n--> objects destroyed  "
            << objectsDestroyed_C
            << "\n--> copy constructions "
            << copyConstructions_C
            << "\n--> copy assignments   "
            << copyAssignments_C
            << "\n--> move constructions "
            << moveConstructions_C
            << "\n--> move assignments   "
            << moveAssignments_C
            << "\n";
  std::cout << "dtor phase took " << tDtorSpan.count() << " seconds.\n";

  data_t::resetCounters();
  std::cout << "\n------------------------>>> vector bench end <<<----------------------------\n" << std::flush;

  return {pushBackTimeSpan, emplaceBackTimeSpan, tDtorSpan};
}  // benchVector

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchVectorTiming(const size_t containerSize, const bool reserve)
{
  using container = std::vector<data_t>;

  std::chrono::duration<double> pushBackTimeSpan{};
  std::chrono::duration<double> emplaceBackTimeSpan{};

  std::chrono::high_resolution_clock::time_point tDtorStart{};
  std::chrono::high_resolution_clock::time_point tDtorEnd{};
  std::chrono::duration<double> tDtorSpan{};

  {
    container v1;
    container v2;

    if (reserve)
    {
      v1.reserve(containerSize);
      v2.reserve(containerSize);
    }

    std::chrono::high_resolution_clock::time_point t1{};
    std::chrono::high_resolution_clock::time_point t2{};

    {
      t1 = std::chrono::high_resolution_clock::now();

      for (size_t i{0}; i < containerSize; ++i)
      {
        data_t c(i, "v1");
        v1.push_back(c);
        // this commented line is a bit faster than the above two lines
        //v1.push_back(data_t(i, "v1"));
      }
      t2 = std::chrono::high_resolution_clock::now();

      pushBackTimeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < containerSize; ++i)
      {
        v2.emplace_back(i, "v2");
      }
      t2 = std::chrono::high_resolution_clock::now();

      emplaceBackTimeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    }
    tDtorStart = std::chrono::high_resolution_clock::now();
  }
  tDtorEnd = std::chrono::high_resolution_clock::now();
  tDtorSpan = std::chrono::duration_cast<std::chrono::duration<double>>(tDtorEnd - tDtorStart);

  data_t::resetCounters();

  return {pushBackTimeSpan, emplaceBackTimeSpan, tDtorSpan};
}  // benchVectorTiming

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchDeque(const size_t containerSize)
{
  using container = std::deque<data_t>;

  std::chrono::duration<double> pushBackTimeSpan{};
  std::chrono::duration<double> emplaceBackTimeSpan{};

  std::chrono::high_resolution_clock::time_point tDtorStart{};
  std::chrono::high_resolution_clock::time_point tDtorEnd{};
  std::chrono::duration<double> tDtorSpan{};

  {
    container d1;
    container d2;

    std::cout << "\n------------------------>>> deque bench start <<<--------------------------\n";

    std::cout << "sizes: d1: " << d1.size() << " - d2: " << d2.size() << "\n\n";

    std::chrono::high_resolution_clock::time_point t1{};
    std::chrono::high_resolution_clock::time_point t2{};
    std::chrono::duration<double> timeSpan{};

    {
      std::cout << "------------------ deque push_back loop ------------------\n\n";
      std::cout << "Pushing back " << containerSize << " elements\n";
      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i{0}; i < containerSize; ++i)
      {
        data_t c(i, "d1");
        d1.push_back(c);
        // this commented line is a bit faster than the above two lines
        //d1.push_back(data_t(i, "d1"));
      }
      t2 = std::chrono::high_resolution_clock::now();

      pushBackTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      auto [objectsCreated_C_v1, objectsAlive_C_v1, objectsDestroyed_C_v1, tooManyDestructions_C_v1] = data_t::getObjectCounters();
      auto [copyConstructions_C_v1, copyAssignments_C_v1, moveConstructions_C_v1, moveAssignments_C_v1] = data_t::getCopyMoveCounters();

      std::cout << "\ndeque d1 push_back loop:"
                << "\n--> objects created    "
                << objectsCreated_C_v1
                << "\n--> objects alive      "
                << objectsAlive_C_v1
                << "\n--> objects destroyed  "
                << objectsDestroyed_C_v1
                << "\n--> copy constructions "
                << copyConstructions_C_v1
                << "\n--> copy assignments   "
                << copyAssignments_C_v1
                << "\n--> move constructions "
                << moveConstructions_C_v1
                << "\n--> move assignments   "
                << moveAssignments_C_v1
                << "\nd1 size:     " << d1.size() << "\n"
                << "push_back loop took " << timeSpan.count() << " seconds\n"
                << "-----------------------------------------------\n\n"
                << "----------- deque emplace_back loop -----------\n\n"
                << "Emplacing back " << containerSize << " elements\n";

      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < containerSize; ++i)
      {
        d2.emplace_back(i, "d2");
      }
      t2 = std::chrono::high_resolution_clock::now();

      emplaceBackTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      auto [objectsCreated_C_v2, objectsAlive_C_v2, objectsDestroyed_C_v2, tooManyDestructions_C_v2] = data_t::getObjectCounters();
      auto [copyConstructions_C_v2, copyAssignments_C_v2, moveConstructions_C_v2, moveAssignments_C_v2] = data_t::getCopyMoveCounters();

      std::cout << "\ndeque d2 emplace_back loop:"
                << "\n--> objects created    "
                << objectsCreated_C_v2 - objectsCreated_C_v1
                << "\n--> objects alive      "
                << objectsAlive_C_v2 - objectsAlive_C_v1
                << "\n--> objects destroyed  "
                << objectsDestroyed_C_v2 - objectsDestroyed_C_v1
                << "\n--> copy constructions "
                << copyConstructions_C_v2 - copyConstructions_C_v1
                << "\n--> copy assignments   "
                << copyAssignments_C_v2 - copyAssignments_C_v1
                << "\n--> move constructions "
                << moveConstructions_C_v2 - moveConstructions_C_v1
                << "\n--> move assignments   "
                << moveAssignments_C_v2 - moveAssignments_C_v1
                << "\nd2 size:     " << d2.size() << "\n"
                << "emplace_back loop took " << timeSpan.count() << " seconds.\n"
                << "----------------------------------------------------\n"
                << std::endl;
    }
    std::cout << "---------------------DTOR phase---------------------\n";
    tDtorStart = std::chrono::high_resolution_clock::now();
  }
  tDtorEnd = std::chrono::high_resolution_clock::now();
  tDtorSpan = std::chrono::duration_cast<std::chrono::duration<double>>(tDtorEnd - tDtorStart);

  auto [objectsCreated_C, objectsAlive_C, objectsDestroyed_C, tooManyDestructions_C] = data_t::getObjectCounters();
  auto [copyConstructions_C, copyAssignments_C, moveConstructions_C, moveAssignments_C] = data_t::getCopyMoveCounters();

  std::cout << "--> objects created    "
            << objectsCreated_C
            << "\n--> objects alive      "
            << objectsAlive_C
            << "\n--> objects destroyed  "
            << objectsDestroyed_C
            << "\n--> copy constructions "
            << copyConstructions_C
            << "\n--> copy assignments   "
            << copyAssignments_C
            << "\n--> move constructions "
            << moveConstructions_C
            << "\n--> move assignments   "
            << moveAssignments_C
            << "\n";
  std::cout << "dtor phase took " << tDtorSpan.count() << " seconds.\n";

  data_t::resetCounters();
  std::cout << "\n------------------------>>> deque bench end <<<----------------------------\n" << std::flush;

  return {pushBackTimeSpan, emplaceBackTimeSpan, tDtorSpan};
}  // benchDeque

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchDequeTiming(const size_t containerSize)
{
  using container = std::deque<data_t>;

  std::chrono::duration<double> pushBackTimeSpan{};
  std::chrono::duration<double> emplaceBackTimeSpan{};

  std::chrono::high_resolution_clock::time_point tDtorStart{};
  std::chrono::high_resolution_clock::time_point tDtorEnd{};
  std::chrono::duration<double> tDtorSpan{};

  {
    container d1;
    container d2;

    std::chrono::high_resolution_clock::time_point t1{};
    std::chrono::high_resolution_clock::time_point t2{};

    {
      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i{0}; i < containerSize; ++i)
      {
        data_t c(i, "d1");
        d1.push_back(c);
        // this commented line is a bit faster than the above two lines
        //d1.push_back(data_t(i, "d1"));
      }
      t2 = std::chrono::high_resolution_clock::now();

      pushBackTimeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < containerSize; ++i)
      {
        d2.emplace_back(i, "d2");
      }
      t2 = std::chrono::high_resolution_clock::now();

      emplaceBackTimeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    }
    tDtorStart = std::chrono::high_resolution_clock::now();
  }
  tDtorEnd = std::chrono::high_resolution_clock::now();
  tDtorSpan = std::chrono::duration_cast<std::chrono::duration<double>>(tDtorEnd - tDtorStart);

  data_t::resetCounters();

  return {pushBackTimeSpan, emplaceBackTimeSpan, tDtorSpan};
}  // benchDequeTiming

std::ostream&
operator<<(std::ostream& s, const std::forward_list<data_t>& v)
{
  s.put('[');
  for (auto&& e : v)
  {
    s << "{" << e.s_ << "." << e.x_ << "}, ";
  }
  return s << "\b\b]";
}

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchFrontForwardList(const size_t containerSize)
{
  using container = std::forward_list<data_t>;

  std::chrono::duration<double> pushFrontTimeSpan{};
  std::chrono::duration<double> emplaceFrontTimeSpan{};

  std::chrono::high_resolution_clock::time_point tDtorStart{};
  std::chrono::high_resolution_clock::time_point tDtorEnd{};
  std::chrono::duration<double> tDtorSpan{};

  {
    container fl1;
    container fl2;

    std::cout << "\n------------------------>>> forward_list bench start <<<--------------------------\n";

    std::cout << "sizes: fl1: " << std::distance(fl1.begin(), fl1.end()) << " - fl2: " << std::distance(fl2.begin(), fl2.end()) << "\n\n";

    std::chrono::high_resolution_clock::time_point t1{};
    std::chrono::high_resolution_clock::time_point t2{};
    std::chrono::duration<double> timeSpan{};

    {
      std::cout << "------------------ forward_list push_front loop ------------------\n\n";
      std::cout << "Pushing front " << containerSize << " elements\n";
      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i{0}; i < containerSize; ++i)
      {
        data_t c(i, "fl1");
        fl1.push_front(c);
      }
      t2 = std::chrono::high_resolution_clock::now();

      pushFrontTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      auto [objectsCreated_C_v1, objectsAlive_C_v1, objectsDestroyed_C_v1, tooManyDestructions_C_v1] = data_t::getObjectCounters();
      auto [copyConstructions_C_v1, copyAssignments_C_v1, moveConstructions_C_v1, moveAssignments_C_v1] = data_t::getCopyMoveCounters();

      std::cout << "\nforward_list fl1 push_front loop:"
                << "\n--> objects created    "
                << objectsCreated_C_v1
                << "\n--> objects alive      "
                << objectsAlive_C_v1
                << "\n--> objects destroyed  "
                << objectsDestroyed_C_v1
                << "\n--> copy constructions "
                << copyConstructions_C_v1
                << "\n--> copy assignments   "
                << copyAssignments_C_v1
                << "\n--> move constructions "
                << moveConstructions_C_v1
                << "\n--> move assignments   "
                << moveAssignments_C_v1
                << "\nfl1 size: " << std::distance(fl1.begin(), fl1.end()) << "\n"
                << "push_front loop took " << timeSpan.count() << " seconds\n"
                << "-----------------------------------------------\n\n"
                << "----------- forward_list emplace_front loop -----------\n\n"
                << "Emplacing front " << containerSize << " elements\n";

      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < containerSize; ++i)
      {
        fl2.emplace_front(i, "fl2");
      }
      t2 = std::chrono::high_resolution_clock::now();

      emplaceFrontTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      auto [objectsCreated_C_v2, objectsAlive_C_v2, objectsDestroyed_C_v2, tooManyDestructions_C_v2] = data_t::getObjectCounters();
      auto [copyConstructions_C_v2, copyAssignments_C_v2, moveConstructions_C_v2, moveAssignments_C_v2] = data_t::getCopyMoveCounters();

      std::cout << "\nforward_list fl2 emplace_front loop:"
                << "\n--> objects created    "
                << objectsCreated_C_v2 - objectsCreated_C_v1
                << "\n--> objects alive      "
                << objectsAlive_C_v2 - objectsAlive_C_v1
                << "\n--> objects destroyed  "
                << objectsDestroyed_C_v2 - objectsDestroyed_C_v1
                << "\n--> copy constructions "
                << copyConstructions_C_v2 - copyConstructions_C_v1
                << "\n--> copy assignments   "
                << copyAssignments_C_v2 - copyAssignments_C_v1
                << "\n--> move constructions "
                << moveConstructions_C_v2 - moveConstructions_C_v1
                << "\n--> move assignments   "
                << moveAssignments_C_v2 - moveAssignments_C_v1
                << "\nfl2 size: " << std::distance(fl2.begin(), fl2.end()) << "\n"
                << "emplace_front loop took " << timeSpan.count() << " seconds.\n"
                << "----------------------------------------------------\n"
                << std::endl;
    }
    std::cout << "---------------------DTOR phase---------------------\n";
    tDtorStart = std::chrono::high_resolution_clock::now();
  }
  tDtorEnd = std::chrono::high_resolution_clock::now();
  tDtorSpan = std::chrono::duration_cast<std::chrono::duration<double>>(tDtorEnd - tDtorStart);

  auto [objectsCreated_C, objectsAlive_C, objectsDestroyed_C, tooManyDestructions_C] = data_t::getObjectCounters();
  auto [copyConstructions_C, copyAssignments_C, moveConstructions_C, moveAssignments_C] = data_t::getCopyMoveCounters();

  std::cout << "--> objects created    "
            << objectsCreated_C
            << "\n--> objects alive      "
            << objectsAlive_C
            << "\n--> objects destroyed  "
            << objectsDestroyed_C
            << "\n--> copy constructions "
            << copyConstructions_C
            << "\n--> copy assignments   "
            << copyAssignments_C
            << "\n--> move constructions "
            << moveConstructions_C
            << "\n--> move assignments   "
            << moveAssignments_C
            << "\n";
  std::cout << "dtor phase took " << tDtorSpan.count() << " seconds.\n";

  data_t::resetCounters();
  std::cout << "\n------------------------>>> forward_list bench end <<<----------------------------\n" << std::flush;

  return {pushFrontTimeSpan, emplaceFrontTimeSpan, tDtorSpan};
}  // benchFrontForwardList

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchFrontForwardListTiming(const size_t containerSize)
{
  using container = std::forward_list<data_t>;

  std::chrono::duration<double> pushFrontTimeSpan{};
  std::chrono::duration<double> emplaceFrontTimeSpan{};

  std::chrono::high_resolution_clock::time_point tDtorStart{};
  std::chrono::high_resolution_clock::time_point tDtorEnd{};
  std::chrono::duration<double> tDtorSpan{};

  {
    container fl1;
    container fl2;

    std::chrono::high_resolution_clock::time_point t1{};
    std::chrono::high_resolution_clock::time_point t2{};
    std::chrono::duration<double> timeSpan{};

    {
      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i{0}; i < containerSize; ++i)
      {
        data_t c(i, "fl1");
        fl1.push_front(c);
      }
      t2 = std::chrono::high_resolution_clock::now();

      pushFrontTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < containerSize; ++i)
      {
        fl2.emplace_front(i, "fl2");
      }
      t2 = std::chrono::high_resolution_clock::now();

      emplaceFrontTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    }
    tDtorStart = std::chrono::high_resolution_clock::now();
  }
  tDtorEnd = std::chrono::high_resolution_clock::now();
  tDtorSpan = std::chrono::duration_cast<std::chrono::duration<double>>(tDtorEnd - tDtorStart);

  data_t::resetCounters();

  return {pushFrontTimeSpan, emplaceFrontTimeSpan, tDtorSpan};
}  // benchFrontForwardListTiming

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchEndForwardList(const size_t containerSize)
{
  using container = std::forward_list<data_t>;

  std::chrono::duration<double> insertAfterTimeSpan{};
  std::chrono::duration<double> emplaceAfterTimeSpan{};

  std::chrono::high_resolution_clock::time_point tDtorStart{};
  std::chrono::high_resolution_clock::time_point tDtorEnd{};
  std::chrono::duration<double> tDtorSpan{};

  {
    container fl1;
    container fl2;

    std::cout << "\n------------------------>>> forward_list bench start <<<--------------------------\n";

    std::cout << "sizes: fl1: " << std::distance(fl1.begin(), fl1.end()) << " - fl2: " << std::distance(fl2.begin(), fl2.end()) << "\n\n";

    std::chrono::high_resolution_clock::time_point t1{};
    std::chrono::high_resolution_clock::time_point t2{};
    std::chrono::duration<double> timeSpan{};

    {
      std::cout << "------------------ forward_list insert_after loop ------------------\n\n";
      std::cout << "Inserting after " << containerSize << " elements\n";
      // insert a first dummy element
      fl1.emplace_front(0, "fl1-dummy");
      auto it = fl1.begin();
      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i{0}; i < containerSize; ++i)
      {
        data_t c(i, "fl1");
        it = fl1.insert_after(it, c);
      }
      t2 = std::chrono::high_resolution_clock::now();

      insertAfterTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      auto [objectsCreated_C_v1, objectsAlive_C_v1, objectsDestroyed_C_v1, tooManyDestructions_C_v1] = data_t::getObjectCounters();
      auto [copyConstructions_C_v1, copyAssignments_C_v1, moveConstructions_C_v1, moveAssignments_C_v1] = data_t::getCopyMoveCounters();

      std::cout << "\nforward_list fl1 insert_after loop:"
                << "\n--> objects created    "
                << objectsCreated_C_v1
                << "\n--> objects alive      "
                << objectsAlive_C_v1
                << "\n--> objects destroyed  "
                << objectsDestroyed_C_v1
                << "\n--> copy constructions "
                << copyConstructions_C_v1
                << "\n--> copy assignments   "
                << copyAssignments_C_v1
                << "\n--> move constructions "
                << moveConstructions_C_v1
                << "\n--> move assignments   "
                << moveAssignments_C_v1
                << "\nfl1 size: " << std::distance(fl1.begin(), fl1.end()) << "\n"
                << "insert_after loop took " << timeSpan.count() << " seconds\n"
                << "-----------------------------------------------\n\n"
                << "----------- forward_list emplace_after loop -----------\n\n"
                << "Emplacing after " << containerSize << " elements\n";

      // insert a first dummy element
      fl2.emplace_front(0, "fl2-dummy");
      it = fl2.begin();
      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < containerSize; ++i)
      {
        it = fl2.emplace_after(it, i, "fl2");
      }
      t2 = std::chrono::high_resolution_clock::now();

      emplaceAfterTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      auto [objectsCreated_C_v2, objectsAlive_C_v2, objectsDestroyed_C_v2, tooManyDestructions_C_v2] = data_t::getObjectCounters();
      auto [copyConstructions_C_v2, copyAssignments_C_v2, moveConstructions_C_v2, moveAssignments_C_v2] = data_t::getCopyMoveCounters();

      std::cout << "\nforward_list fl2 emplace_after loop:"
                << "\n--> objects created    "
                << objectsCreated_C_v2 - objectsCreated_C_v1
                << "\n--> objects alive      "
                << objectsAlive_C_v2 - objectsAlive_C_v1
                << "\n--> objects destroyed  "
                << objectsDestroyed_C_v2 - objectsDestroyed_C_v1
                << "\n--> copy constructions "
                << copyConstructions_C_v2 - copyConstructions_C_v1
                << "\n--> copy assignments   "
                << copyAssignments_C_v2 - copyAssignments_C_v1
                << "\n--> move constructions "
                << moveConstructions_C_v2 - moveConstructions_C_v1
                << "\n--> move assignments   "
                << moveAssignments_C_v2 - moveAssignments_C_v1
                << "\nfl2 size: " << std::distance(fl2.begin(), fl2.end()) << "\n"
                << "emplace_after loop took " << timeSpan.count() << " seconds.\n"
                << "----------------------------------------------------\n"
                << std::endl;
    }
    std::cout << "---------------------DTOR phase---------------------\n";
    tDtorStart = std::chrono::high_resolution_clock::now();
  }
  tDtorEnd = std::chrono::high_resolution_clock::now();
  tDtorSpan = std::chrono::duration_cast<std::chrono::duration<double>>(tDtorEnd - tDtorStart);

  auto [objectsCreated_C, objectsAlive_C, objectsDestroyed_C, tooManyDestructions_C] = data_t::getObjectCounters();
  auto [copyConstructions_C, copyAssignments_C, moveConstructions_C, moveAssignments_C] = data_t::getCopyMoveCounters();

  std::cout << "--> objects created    "
            << objectsCreated_C
            << "\n--> objects alive      "
            << objectsAlive_C
            << "\n--> objects destroyed  "
            << objectsDestroyed_C
            << "\n--> copy constructions "
            << copyConstructions_C
            << "\n--> copy assignments   "
            << copyAssignments_C
            << "\n--> move constructions "
            << moveConstructions_C
            << "\n--> move assignments   "
            << moveAssignments_C
            << "\n";
  std::cout << "dtor phase took " << tDtorSpan.count() << " seconds.\n";

  data_t::resetCounters();
  std::cout << "\n------------------------>>> forward_list bench end <<<----------------------------\n" << std::flush;

  return {insertAfterTimeSpan, emplaceAfterTimeSpan, tDtorSpan};
}  // benchEndForwardList

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchEndForwardListTiming(const size_t containerSize)
{
  using container = std::forward_list<data_t>;

  std::chrono::duration<double> insertAfterTimeSpan{};
  std::chrono::duration<double> emplaceAfterTimeSpan{};

  std::chrono::high_resolution_clock::time_point tDtorStart{};
  std::chrono::high_resolution_clock::time_point tDtorEnd{};
  std::chrono::duration<double> tDtorSpan{};

  {
    container fl1;
    container fl2;

    std::chrono::high_resolution_clock::time_point t1{};
    std::chrono::high_resolution_clock::time_point t2{};

    {
      // insert a first dummy element
      fl1.emplace_front(0, "fl1-dummy");
      auto it = fl1.begin();
      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i{0}; i < containerSize; ++i)
      {
        data_t c(i, "fl1");
        it = fl1.insert_after(it, c);
      }
      t2 = std::chrono::high_resolution_clock::now();

      insertAfterTimeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      // insert a first dummy element
      fl2.emplace_front(0, "fl2-dummy");
      it = fl2.begin();
      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < containerSize; ++i)
      {
        it = fl2.emplace_after(it, i, "fl2");
      }
      t2 = std::chrono::high_resolution_clock::now();

      emplaceAfterTimeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    }
    tDtorStart = std::chrono::high_resolution_clock::now();
  }
  tDtorEnd = std::chrono::high_resolution_clock::now();
  tDtorSpan = std::chrono::duration_cast<std::chrono::duration<double>>(tDtorEnd - tDtorStart);

  data_t::resetCounters();

  return {insertAfterTimeSpan, emplaceAfterTimeSpan, tDtorSpan};
}  // benchEndForwardListTiming

std::ostream&
operator<<(std::ostream& s, const std::list<data_t>& v)
{
  s.put('[');
  for (auto&& e : v)
  {
    s << "{" << e.s_ << "." << e.x_ << "}, ";
  }
  return s << "\b\b]";
}

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchFrontList(const size_t containerSize)
{
  using container = std::list<data_t>;

  std::chrono::duration<double> pushFrontTimeSpan{};
  std::chrono::duration<double> emplaceFrontTimeSpan{};

  std::chrono::high_resolution_clock::time_point tDtorStart{};
  std::chrono::high_resolution_clock::time_point tDtorEnd{};
  std::chrono::duration<double> tDtorSpan{};

  {
    container l1;
    container l2;

    std::cout << "\n------------------------>>> list bench start <<<--------------------------\n";

    std::cout << "sizes: l1: " << std::distance(l1.begin(), l1.end()) << " - l2: " << std::distance(l2.begin(), l2.end()) << "\n\n";

    std::chrono::high_resolution_clock::time_point t1{};
    std::chrono::high_resolution_clock::time_point t2{};
    std::chrono::duration<double> timeSpan{};

    {
      std::cout << "------------------ list push_front loop ------------------\n\n";
      std::cout << "Pushing front " << containerSize << " elements\n";
      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i{0}; i < containerSize; ++i)
      {
        data_t c(i, "l1");
        l1.push_front(c);
      }
      t2 = std::chrono::high_resolution_clock::now();

      pushFrontTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      auto [objectsCreated_C_v1, objectsAlive_C_v1, objectsDestroyed_C_v1, tooManyDestructions_C_v1] = data_t::getObjectCounters();
      auto [copyConstructions_C_v1, copyAssignments_C_v1, moveConstructions_C_v1, moveAssignments_C_v1] = data_t::getCopyMoveCounters();

      std::cout << "\nlist l1 push_front loop:"
                << "\n--> objects created    "
                << objectsCreated_C_v1
                << "\n--> objects alive      "
                << objectsAlive_C_v1
                << "\n--> objects destroyed  "
                << objectsDestroyed_C_v1
                << "\n--> copy constructions "
                << copyConstructions_C_v1
                << "\n--> copy assignments   "
                << copyAssignments_C_v1
                << "\n--> move constructions "
                << moveConstructions_C_v1
                << "\n--> move assignments   "
                << moveAssignments_C_v1
                << "\nl1 size: " << std::distance(l1.begin(), l1.end()) << "\n"
                << "push_front loop took " << timeSpan.count() << " seconds\n"
                << "-----------------------------------------------\n\n"
                << "----------- list emplace_front loop -----------\n\n"
                << "Emplacing front " << containerSize << " elements\n";

      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < containerSize; ++i)
      {
        l2.emplace_front(i, "l2");
      }
      t2 = std::chrono::high_resolution_clock::now();

      emplaceFrontTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      auto [objectsCreated_C_v2, objectsAlive_C_v2, objectsDestroyed_C_v2, tooManyDestructions_C_v2] = data_t::getObjectCounters();
      auto [copyConstructions_C_v2, copyAssignments_C_v2, moveConstructions_C_v2, moveAssignments_C_v2] = data_t::getCopyMoveCounters();

      std::cout << "\nlist l2 emplace_front loop:"
                << "\n--> objects created    "
                << objectsCreated_C_v2 - objectsCreated_C_v1
                << "\n--> objects alive      "
                << objectsAlive_C_v2 - objectsAlive_C_v1
                << "\n--> objects destroyed  "
                << objectsDestroyed_C_v2 - objectsDestroyed_C_v1
                << "\n--> copy constructions "
                << copyConstructions_C_v2 - copyConstructions_C_v1
                << "\n--> copy assignments   "
                << copyAssignments_C_v2 - copyAssignments_C_v1
                << "\n--> move constructions "
                << moveConstructions_C_v2 - moveConstructions_C_v1
                << "\n--> move assignments   "
                << moveAssignments_C_v2 - moveAssignments_C_v1
                << "\nl2 size: " << std::distance(l2.begin(), l2.end()) << "\n"
                << "emplace_front loop took " << timeSpan.count() << " seconds.\n"
                << "----------------------------------------------------\n"
                << std::endl;
    }
    //std::cout << l1 << "\n";
    //std::cout << l2 << "\n";
    std::cout << "---------------------DTOR phase---------------------\n";
    tDtorStart = std::chrono::high_resolution_clock::now();
  }
  tDtorEnd = std::chrono::high_resolution_clock::now();
  tDtorSpan = std::chrono::duration_cast<std::chrono::duration<double>>(tDtorEnd - tDtorStart);

  auto [objectsCreated_C, objectsAlive_C, objectsDestroyed_C, tooManyDestructions_C] = data_t::getObjectCounters();
  auto [copyConstructions_C, copyAssignments_C, moveConstructions_C, moveAssignments_C] = data_t::getCopyMoveCounters();

  std::cout << "--> objects created    "
            << objectsCreated_C
            << "\n--> objects alive      "
            << objectsAlive_C
            << "\n--> objects destroyed  "
            << objectsDestroyed_C
            << "\n--> copy constructions "
            << copyConstructions_C
            << "\n--> copy assignments   "
            << copyAssignments_C
            << "\n--> move constructions "
            << moveConstructions_C
            << "\n--> move assignments   "
            << moveAssignments_C
            << "\n";
  std::cout << "dtor phase took " << tDtorSpan.count() << " seconds.\n";

  data_t::resetCounters();
  std::cout << "\n------------------------>>> list bench end <<<----------------------------\n" << std::flush;

  return {pushFrontTimeSpan, emplaceFrontTimeSpan, tDtorSpan};
}  // benchFrontList

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchFrontListTiming(const size_t containerSize)
{
  using container = std::list<data_t>;

  std::chrono::duration<double> pushFrontTimeSpan{};
  std::chrono::duration<double> emplaceFrontTimeSpan{};

  std::chrono::high_resolution_clock::time_point tDtorStart{};
  std::chrono::high_resolution_clock::time_point tDtorEnd{};
  std::chrono::duration<double> tDtorSpan{};

  {
    container l1;
    container l2;

    std::chrono::high_resolution_clock::time_point t1{};
    std::chrono::high_resolution_clock::time_point t2{};

    {
      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i{0}; i < containerSize; ++i)
      {
        data_t c(i, "l1");
        l1.push_front(c);
      }
      t2 = std::chrono::high_resolution_clock::now();

      pushFrontTimeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < containerSize; ++i)
      {
        l2.emplace_front(i, "l2");
      }
      t2 = std::chrono::high_resolution_clock::now();

      emplaceFrontTimeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    }
    tDtorStart = std::chrono::high_resolution_clock::now();
  }
  tDtorEnd = std::chrono::high_resolution_clock::now();
  tDtorSpan = std::chrono::duration_cast<std::chrono::duration<double>>(tDtorEnd - tDtorStart);

  data_t::resetCounters();

  return {pushFrontTimeSpan, emplaceFrontTimeSpan, tDtorSpan};
}  // benchFrontListTiming

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchEndList(const size_t containerSize)
{
  using container = std::list<data_t>;

  std::chrono::duration<double> pushBackTimeSpan{};
  std::chrono::duration<double> emplaceBackTimeSpan{};

  std::chrono::high_resolution_clock::time_point tDtorStart{};
  std::chrono::high_resolution_clock::time_point tDtorEnd{};
  std::chrono::duration<double> tDtorSpan{};

  {
    container l1;
    container l2;

    std::cout << "\n------------------------>>> list bench start <<<--------------------------\n";

    std::cout << "sizes: l1: " << std::distance(l1.begin(), l1.end()) << " - l2: " << std::distance(l2.begin(), l2.end()) << "\n\n";

    std::chrono::high_resolution_clock::time_point t1{};
    std::chrono::high_resolution_clock::time_point t2{};
    std::chrono::duration<double> timeSpan{};

    {
      std::cout << "------------------ list push_back loop ------------------\n\n";
      std::cout << "Pushing back " << containerSize << " elements\n";
      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i{0}; i < containerSize; ++i)
      {
        data_t c(i, "l1");
        l1.push_back(c);
      }
      t2 = std::chrono::high_resolution_clock::now();

      pushBackTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      auto [objectsCreated_C_v1, objectsAlive_C_v1, objectsDestroyed_C_v1, tooManyDestructions_C_v1] = data_t::getObjectCounters();
      auto [copyConstructions_C_v1, copyAssignments_C_v1, moveConstructions_C_v1, moveAssignments_C_v1] = data_t::getCopyMoveCounters();

      std::cout << "\nlist l1 push_back loop:"
                << "\n--> objects created    "
                << objectsCreated_C_v1
                << "\n--> objects alive      "
                << objectsAlive_C_v1
                << "\n--> objects destroyed  "
                << objectsDestroyed_C_v1
                << "\n--> copy constructions "
                << copyConstructions_C_v1
                << "\n--> copy assignments   "
                << copyAssignments_C_v1
                << "\n--> move constructions "
                << moveConstructions_C_v1
                << "\n--> move assignments   "
                << moveAssignments_C_v1
                << "\nl1 size: " << std::distance(l1.begin(), l1.end()) << "\n"
                << "push_back loop took " << timeSpan.count() << " seconds\n"
                << "-----------------------------------------------\n\n"
                << "----------- list emplace_back loop -----------\n\n"
                << "Emplacing back " << containerSize << " elements\n";

      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < containerSize; ++i)
      {
        l2.emplace_back(i, "l2");
      }
      t2 = std::chrono::high_resolution_clock::now();

      emplaceBackTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      auto [objectsCreated_C_v2, objectsAlive_C_v2, objectsDestroyed_C_v2, tooManyDestructions_C_v2] = data_t::getObjectCounters();
      auto [copyConstructions_C_v2, copyAssignments_C_v2, moveConstructions_C_v2, moveAssignments_C_v2] = data_t::getCopyMoveCounters();

      std::cout << "\nlist l2 emplace_back loop:"
                << "\n--> objects created    "
                << objectsCreated_C_v2 - objectsCreated_C_v1
                << "\n--> objects alive      "
                << objectsAlive_C_v2 - objectsAlive_C_v1
                << "\n--> objects destroyed  "
                << objectsDestroyed_C_v2 - objectsDestroyed_C_v1
                << "\n--> copy constructions "
                << copyConstructions_C_v2 - copyConstructions_C_v1
                << "\n--> copy assignments   "
                << copyAssignments_C_v2 - copyAssignments_C_v1
                << "\n--> move constructions "
                << moveConstructions_C_v2 - moveConstructions_C_v1
                << "\n--> move assignments   "
                << moveAssignments_C_v2 - moveAssignments_C_v1
                << "\nl2 size: " << std::distance(l2.begin(), l2.end()) << "\n"
                << "emplace_back loop took " << timeSpan.count() << " seconds.\n"
                << "----------------------------------------------------\n"
                << std::endl;
    }
    std::cout << "---------------------DTOR phase---------------------\n";
    tDtorStart = std::chrono::high_resolution_clock::now();
  }
  tDtorEnd = std::chrono::high_resolution_clock::now();
  tDtorSpan = std::chrono::duration_cast<std::chrono::duration<double>>(tDtorEnd - tDtorStart);

  auto [objectsCreated_C, objectsAlive_C, objectsDestroyed_C, tooManyDestructions_C] = data_t::getObjectCounters();
  auto [copyConstructions_C, copyAssignments_C, moveConstructions_C, moveAssignments_C] = data_t::getCopyMoveCounters();

  std::cout << "--> objects created    "
            << objectsCreated_C
            << "\n--> objects alive      "
            << objectsAlive_C
            << "\n--> objects destroyed  "
            << objectsDestroyed_C
            << "\n--> copy constructions "
            << copyConstructions_C
            << "\n--> copy assignments   "
            << copyAssignments_C
            << "\n--> move constructions "
            << moveConstructions_C
            << "\n--> move assignments   "
            << moveAssignments_C
            << "\n";
  std::cout << "dtor phase took " << tDtorSpan.count() << " seconds.\n";

  data_t::resetCounters();
  std::cout << "\n------------------------>>> list bench end <<<----------------------------\n" << std::flush;

  return {pushBackTimeSpan, emplaceBackTimeSpan, tDtorSpan};
}  // benchEndList

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchEndListTiming(const size_t containerSize)
{
  using container = std::list<data_t>;

  std::chrono::duration<double> pushBackTimeSpan{};
  std::chrono::duration<double> emplaceBackTimeSpan{};

  std::chrono::high_resolution_clock::time_point tDtorStart{};
  std::chrono::high_resolution_clock::time_point tDtorEnd{};
  std::chrono::duration<double> tDtorSpan{};

  {
    container l1;
    container l2;

    std::chrono::high_resolution_clock::time_point t1{};
    std::chrono::high_resolution_clock::time_point t2{};
    std::chrono::duration<double> timeSpan{};

    {
      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i{0}; i < containerSize; ++i)
      {
        data_t c(i, "l1");
        l1.push_back(c);
      }
      t2 = std::chrono::high_resolution_clock::now();

      pushBackTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

      t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < containerSize; ++i)
      {
        l2.emplace_back(i, "l2");
      }
      t2 = std::chrono::high_resolution_clock::now();

      emplaceBackTimeSpan = timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    }
    tDtorStart = std::chrono::high_resolution_clock::now();
  }
  tDtorEnd = std::chrono::high_resolution_clock::now();
  tDtorSpan = std::chrono::duration_cast<std::chrono::duration<double>>(tDtorEnd - tDtorStart);

  data_t::resetCounters();

  return {pushBackTimeSpan, emplaceBackTimeSpan, tDtorSpan};
}  // benchEndListTiming

////////////////////////////////////////////////////////////////////////////////
static const bool RESERVE_SPACE {true};
static const bool DO_NOT_RESERVE_SPACE {false};
static const int PRECISION(6);
static const size_t containersizeMin{10};

void
benchTimingVectorNoReservedSpace()
{
  const size_t containersizeMax{100'000'000};

  size_t containerSize {};
  std::string containerSizeStr{};

  std::chrono::duration<double> pushBackTimeSpan{};
  std::chrono::duration<double> emplaceBackTimeSpan{};
  std::chrono::duration<double> dTorTimeSpan{};

  std::cout << "\n\n"
            << "vector no reserved space\nsize       push_back      emplace_back   dtor-phase\n";
  for (containerSize = containersizeMin; containerSize <= containersizeMax; containerSize *= 10)
  {
    std::tie(pushBackTimeSpan, emplaceBackTimeSpan, dTorTimeSpan) = benchVectorTiming(containerSize,
                                                                                      DO_NOT_RESERVE_SPACE);
    containerSizeStr = std::to_string(containerSize);
    std::cout << containerSizeStr << ":"
              << std::string(10 - containerSizeStr.length(), ' ') << std::scientific << std::setprecision(PRECISION)
              << pushBackTimeSpan.count() << "   " << emplaceBackTimeSpan.count() << "   " << dTorTimeSpan.count() << std::endl;
  }
}  // benchTimingVectorNoReservedSpace

void
benchTimingVectorReservedSpace()
{
  const size_t containersizeMax{100'000'000};

  size_t containerSize {};
  std::string containerSizeStr{};

  std::chrono::duration<double> pushBackTimeSpan{};
  std::chrono::duration<double> emplaceBackTimeSpan{};
  std::chrono::duration<double> dTorTimeSpan{};

  std::cout << "\n\n"
            << "vector reserved space\nsize       push_back      emplace_back   dtor-phase\n";
  for (containerSize = containersizeMin; containerSize <= containersizeMax; containerSize *= 10)
  {
    std::tie(pushBackTimeSpan, emplaceBackTimeSpan, dTorTimeSpan) = benchVectorTiming(containerSize, RESERVE_SPACE);
    containerSizeStr = std::to_string(containerSize);
    std::cout << containerSize << ":"
              << std::string(10 - containerSizeStr.length(), ' ') << std::scientific << std::setprecision(PRECISION)
              << pushBackTimeSpan.count() << "   " << emplaceBackTimeSpan.count() << "   " << dTorTimeSpan.count() << std::endl;
  }
}  // benchTimingVectorReservedSpace

void
benchTimingDeque()
{
  const size_t containersizeMax{100'000'000};

  size_t containerSize {};
  std::string containerSizeStr{};

  std::chrono::duration<double> pushBackTimeSpan{};
  std::chrono::duration<double> emplaceBackTimeSpan{};
  std::chrono::duration<double> dTorTimeSpan{};

  std::cout << "\n\n"
            << "deque\nsize       push_back      emplace_back   dtor-phase\n";
  for (containerSize = containersizeMin; containerSize <= containersizeMax; containerSize *= 10)
  {
    std::tie(pushBackTimeSpan, emplaceBackTimeSpan, dTorTimeSpan) = benchDequeTiming(containerSize);
    containerSizeStr = std::to_string(containerSize);
    std::cout << containerSize << ":"
              << std::string(10 - containerSizeStr.length(), ' ') << std::scientific << std::setprecision(PRECISION)
              << pushBackTimeSpan.count() << "   " << emplaceBackTimeSpan.count() << "   " << dTorTimeSpan.count() << std::endl;
  }
}  // benchTimingDeque

void
benchTimingFrontForwardList()
{
  const size_t containersizeMax{100'000'000};

  size_t containerSize {};
  std::string containerSizeStr{};

  std::chrono::duration<double> pushFrontTimeSpan{};
  std::chrono::duration<double> emplaceFrontTimeSpan{};
  std::chrono::duration<double> dTorTimeSpan{};

  std::cout << "\n\n"
            << "forward_list\nsize       push_front     emplace_front  dtor-phase\n";
  for (containerSize = containersizeMin; containerSize <= containersizeMax; containerSize *= 10)
  {
    std::tie(pushFrontTimeSpan, emplaceFrontTimeSpan, dTorTimeSpan) = benchFrontForwardListTiming(containerSize);
    containerSizeStr = std::to_string(containerSize);
    std::cout << containerSize << ":"
              << std::string(10 - containerSizeStr.length(), ' ') << std::scientific << std::setprecision(PRECISION)
              << pushFrontTimeSpan.count() << "   " << emplaceFrontTimeSpan.count() << "   " << dTorTimeSpan.count() << std::endl;
  }
}  // benchTimingFrontForwardList

void
benchTimingEndForwardList()
{
  const size_t containersizeMax{100'000'000};

  size_t containerSize {};
  std::string containerSizeStr{};

  std::chrono::duration<double> insertAfterTimeSpan{};
  std::chrono::duration<double> emplaceAfterTimeSpan{};
  std::chrono::duration<double> dTorTimeSpan{};

  std::cout << "\n\n"
            << "forward_list\nsize       insert_after   emplace_after  dtor-phase\n";
  for (containerSize = containersizeMin; containerSize <= containersizeMax; containerSize *= 10)
  {
    std::tie(insertAfterTimeSpan, emplaceAfterTimeSpan, dTorTimeSpan) = benchEndForwardListTiming(containerSize);
    containerSizeStr = std::to_string(containerSize);
    std::cout << containerSize << ":"
              << std::string(10 - containerSizeStr.length(), ' ') << std::scientific << std::setprecision(PRECISION)
              << insertAfterTimeSpan.count() << "   " << emplaceAfterTimeSpan.count() << "   " << dTorTimeSpan.count() << std::endl;
  }
}  // benchTimingEndForwardList

void
benchTimingFrontList()
{
  const size_t containersizeMax{100'000'000};

  size_t containerSize {};
  std::string containerSizeStr{};

  std::chrono::duration<double> pushFrontTimeSpan{};
  std::chrono::duration<double> emplaceFrontTimeSpan{};
  std::chrono::duration<double> dTorTimeSpan{};

  std::cout << "\n\n"
            << "list\nsize       push_front     emplace_front  dtor-phase\n";
  for (containerSize = containersizeMin; containerSize <= containersizeMax; containerSize *= 10)
  {
    std::tie(pushFrontTimeSpan, emplaceFrontTimeSpan, dTorTimeSpan) = benchFrontListTiming(containerSize);
    containerSizeStr = std::to_string(containerSize);
    std::cout << containerSize << ":"
              << std::string(10 - containerSizeStr.length(), ' ') << std::scientific << std::setprecision(PRECISION)
              << pushFrontTimeSpan.count() << "   " << emplaceFrontTimeSpan.count() << "   " << dTorTimeSpan.count() << std::endl;
  }
}  // benchTimingFrontList

void
benchTimingEndList()
{
  const size_t containersizeMax{100'000'000};

  size_t containerSize {};
  std::string containerSizeStr{};

  std::chrono::duration<double> pushBackTimeSpan{};
  std::chrono::duration<double> emplaceBackTimeSpan{};
  std::chrono::duration<double> dTorTimeSpan{};

  std::cout << "\n\n"
            << "list\nsize       push_back      emplace_back   dtor-phase\n";
  for (containerSize = containersizeMin; containerSize <= containersizeMax; containerSize *= 10)
  {
    std::tie(pushBackTimeSpan, emplaceBackTimeSpan, dTorTimeSpan) = benchEndListTiming(containerSize);
    containerSizeStr = std::to_string(containerSize);
    std::cout << containerSize << ":"
              << std::string(10 - containerSizeStr.length(), ' ') << std::scientific << std::setprecision(PRECISION)
              << pushBackTimeSpan.count() << "   " << emplaceBackTimeSpan.count() << "   " << dTorTimeSpan.count() << std::endl;
  }
}  // benchTimingEndList

void
benchTiming()
{
  benchTimingVectorNoReservedSpace();
  benchTimingVectorReservedSpace();
  benchTimingDeque();
  benchTimingFrontForwardList();
  benchTimingEndForwardList();
  benchTimingFrontList();
  benchTimingEndList();
}  // benchTiming

void benchCounters()
{
  const size_t containerSize{50'000'000};

  benchVector(containerSize, DO_NOT_RESERVE_SPACE);
  benchVector(containerSize, RESERVE_SPACE);
  benchDeque(containerSize);
  benchFrontForwardList(containerSize);
  benchEndForwardList(containerSize);
  benchFrontList(containerSize);
  benchEndList(containerSize);
}  // benchCounters
