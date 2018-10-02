//
// bench.h
//
// Created by massimo on 8/20/18.
//
#pragma once

#include "object-counter.h"

#include <string>
////////////////////////////////////////////////////////////////////////////////
class C final : public object_factory::object_counter::objectCounter<C>
{
public:
  size_t x_ {};
  std::string s_ {};

  C(const size_t i, std::string s) : x_(i), s_(std::move(s)) {}
};

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchVector(const size_t containerSize, const bool reserve);

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchVectorTiming(const size_t containerSize, const bool reserve);

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchDeque(const size_t containerSize);

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchDequeTiming(const size_t containerSize);

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchFrontForwardList(const size_t containerSize);

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchFrontForwardListTiming(const size_t containerSize);

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchEndForwardList(const size_t containerSize);

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchEndForwardListTiming(const size_t containerSize);

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchFrontList(const size_t containerSize);

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchFrontListTiming(const size_t containerSize);

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchEndList(const size_t containerSize);

std::tuple<std::chrono::duration<double>, std::chrono::duration<double>, std::chrono::duration<double>>
benchEndListTiming(const size_t containerSize);

void
benchTimingVectorNoReservedSpace();

void
benchTimingVectorReservedSpace();

void
benchTimingDeque();

void
benchTimingFrontForwardList();

void
benchTimingEndForwardList();

void
benchTimingFrontList();

void
benchTimingEndList();

void
benchTiming();

void
benchCounters();
