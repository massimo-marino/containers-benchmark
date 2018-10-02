//
// object-counter.h
//
// Created by massimo on 8/21/18.
//
#pragma once

#include <tuple>
#include <mutex>
////////////////////////////////////////////////////////////////////////////////
namespace object_factory::object_counter
{
//
// See:
// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
//
// Each time an object of class T is created, the constructor of objectCounter<T>
// is called, incrementing both the created and alive count.
// Each time an object of class T is destroyed, the alive count is decremented.
// It is important to note that objectCounter<T> and objectCounter<U> are two
// separate classes and this is why they will keep separate counts of T's and U's.
//
// counterType, the type of the counters, MUST be unsigned
// counterType's type is unsigned long by default
template <typename T, typename counterType = unsigned long>
class objectCounter
{
public:
  using objectCounters = std::tuple<counterType, counterType, counterType, bool>;
  using copyMoveCounters = std::tuple<counterType, counterType, counterType, counterType>;

  // default ctor
  objectCounter() noexcept(false)
  {
    std::lock_guard<std::mutex> lg(mtx_);
    ++objectsCreated_;
    ++objectsAlive_;
    if ( checkCounterOverflow() )
    {
      throw std::overflow_error("Object Counters in OVERFLOW");
    }
  }

  // copy ctor
  objectCounter([[maybe_unused]]const objectCounter& rhs) noexcept(false)
  {
    std::lock_guard<std::mutex> lg(mtx_);
    ++copyConstructions_;
    ++objectsCreated_;
    ++objectsAlive_;
    if ( checkCounterOverflow() )
    {
      throw std::overflow_error("Object Counters in OVERFLOW");
    }
  }

  // copy assignment operator=
  objectCounter& operator=([[maybe_unused]] const objectCounter& rhs)
  {
    std::lock_guard<std::mutex> lg(mtx_);
    ++copyAssignments_;
    return *this;
  }

  // move ctor
  objectCounter([[maybe_unused]] objectCounter&& rhs)
  {
    std::lock_guard<std::mutex> lg(mtx_);
    ++moveConstructions_;
    ++objectsCreated_;
    ++objectsAlive_;
    if ( checkCounterOverflow() )
    {
      throw std::overflow_error("Object Counters in OVERFLOW");
    }
  }

  // move assignment operator=
  objectCounter& operator=([[maybe_unused]] objectCounter&& rhs)
  {
    std::lock_guard<std::mutex> lg(mtx_);
    ++moveAssignments_;
    return *this;
  }

  // objects should never be removed through pointers of this type
  virtual
  ~objectCounter() noexcept
  {
    std::lock_guard<std::mutex> lg(mtx_);
    if (   (0 == objectsAlive_) // must be non-zero since we destroy an object
           || (objectsCreated_ != (objectsAlive_ + objectsDestroyed_)) )
    {
      tooManyDestructions_ = true;
    }
    else
    {
      --objectsAlive_;
      ++objectsDestroyed_;
    }
  }

  static
  counterType
  getObjectsCreatedCounter() noexcept
  {
    std::lock_guard<std::mutex> lg(mtx_);
    return objectsCreated_;
  }

  static
  counterType
  getObjectsAliveCounter() noexcept
  {
    std::lock_guard<std::mutex> lg(mtx_);
    return objectsAlive_;
  }

  static
  counterType
  getObjectsDestroyedCounter() noexcept
  {
    std::lock_guard<std::mutex> lg(mtx_);
    return objectsDestroyed_;
  }

  static
  bool
  getTooManyDestructionsFlag() noexcept
  {
    std::lock_guard<std::mutex> lg(mtx_);
    return tooManyDestructions_;
  }

  static
  counterType
  getCopyConstructionsCounter() noexcept
  {
    std::lock_guard<std::mutex> lg(mtx_);
    return copyConstructions_;
  }

  static
  counterType
  getCopyAssignmentsCounter() noexcept
  {
    std::lock_guard<std::mutex> lg(mtx_);
    return copyAssignments_;
  }

  static
  counterType
  getMoveConstructionsCounter() noexcept
  {
    std::lock_guard<std::mutex> lg(mtx_);
    return moveConstructions_;
  }

  static
  counterType
  getMoveAssignmentsCounter() noexcept
  {
    std::lock_guard<std::mutex> lg(mtx_);
    return moveAssignments_;
  }

  static
  auto
  getObjectCounters() noexcept -> objectCounters
  {
    std::lock_guard<std::mutex> lg(mtx_);
    return std::make_tuple(objectsCreated_, objectsAlive_, objectsDestroyed_, tooManyDestructions_);
  }
  static
  auto
  getCopyMoveCounters() noexcept -> copyMoveCounters
  {
    std::lock_guard<std::mutex> lg(mtx_);
    return std::make_tuple(copyConstructions_, copyAssignments_, moveConstructions_, moveAssignments_);
  }

  static
  void
  resetCounters() noexcept
  {
    std::lock_guard<std::mutex> lg(mtx_);
    objectsCreated_ = 0;
    objectsAlive_ = 0;
    objectsDestroyed_ = 0;
    copyConstructions_ = 0;
    copyAssignments_ = 0;
    moveConstructions_ = 0;
    moveAssignments_ = 0;
    tooManyDestructions_ = false;
  }

protected:
  // in a multithreaded process threads can allocate objects of the same class,
  // so static data must be protected with a mutex
  static std::mutex mtx_;
  static counterType objectsCreated_;
  static counterType objectsAlive_;
  static counterType objectsDestroyed_;
  static counterType copyConstructions_;
  static counterType copyAssignments_;
  static counterType moveConstructions_;
  static counterType moveAssignments_;
  static bool tooManyDestructions_;

private:
  static constexpr
  bool
  checkCounterOverflow() noexcept
  {
    return ( (0 == objectsAlive_) ||
             (objectsCreated_ != (objectsAlive_ + objectsDestroyed_)) );
  }
};  // class objectCounter

template <typename T, typename TC>
std::mutex objectCounter<T, TC>::mtx_ {};

template <typename T, typename TC>
TC objectCounter<T, TC>::objectsCreated_ {0};

template <typename T, typename TC>
TC objectCounter<T, TC>::objectsAlive_ {0};

template <typename T, typename TC>
TC objectCounter<T, TC>::objectsDestroyed_ {0};

template <typename T, typename TC>
TC objectCounter<T, TC>::copyConstructions_ {0};

template <typename T, typename TC>
TC objectCounter<T, TC>::copyAssignments_ {0};

template <typename T, typename TC>
TC objectCounter<T, TC>::moveConstructions_ {0};

template <typename T, typename TC>
TC objectCounter<T, TC>::moveAssignments_ {0};

template <typename T, typename TC>
bool objectCounter<T, TC>::tooManyDestructions_ {false};

}  // namespace object_factory::object_counter
