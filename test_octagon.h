//
// Created by artur on 07.05.2021.
//

#ifndef OCTAGON__TEST_OCTAGON_H_
#define OCTAGON__TEST_OCTAGON_H_

#include "Octagon.h"
#include <cassert>
#include <chrono>
#include <thread>
#include <random>
std::mt19937 rnd(time(0));


std::vector<Point> getRandomOctagon() {
  std::vector<Point> points;
  int sz = rnd() % 1000;
  for (int i = 0; i < sz; ++i) {
    points.emplace_back((rnd() % 2 ? 1 : -1) * rnd() % int(1e5),
                        (rnd() % 2 ? 1 : -1) * rnd() % int(1e5));
  }
  return points;
}

void constructorsTest() {
  std::cout << "Constructor tests: \n";

  Octagon first = {{1, 1}, {1, 2}, {99, 10}, {-30, -300}};
  Octagon second = first;
  assert(first == second);

  std::cout << "OK! Copy constructor works\n";

  Octagon third;
  try {
    third.vertex(3);
  } catch (EmptyOctagonError) {
    std::cout << "OK! exception safety works\n";
  }

  std::cout << "Now I will check octagon, that contains only 1 point\n";

  for (int j = 0; j < 10000; ++j) {
    third.clear();
    Point p = Point((rnd() % 2 ? 1 : -1) * rnd() % int(1e5), (rnd() % 2 ? 1 : -1) * rnd() % int(1e5));
    third.coverPoint(p);

    for (size_t i = 0; i < Octagon::c.size(); ++i) {
      Point q = p;
      q.x += Octagon::c[i].first;
      q.y += Octagon::c[i].second;

      assert(third.isPointInside(q) == -1);
    }
  }

  std::cout << "OK! one-point-octagon works\n";

  assert(first.isPointInside({1, 1}) >= 0
             && second.isPointInside({1, 1}) >= 0
             && first.isPointInside({1, 2}) >= 0
             && second.isPointInside({1, 2}) >= 0
             && first.isPointInside({99, 10}) >= 0
             && second.isPointInside({99, 10}) >= 0
             && first.isPointInside({-30, -300}) >= 0
             && second.isPointInside({-30, -300}) >= 0);

  std::vector<Point> points = getRandomOctagon();
  Octagon fourth = points;

  for (int i = 0; i < static_cast<int>(points.size()); ++i) {
    assert(fourth.isPointInside(points[i]) >= 0);
  }


  std::cout << "OK! Points constructor works\n";

  Octagon fifth = Octagon(1, 2, 1, 0, -1, 10, -1, 0);
  assert(fifth.limit(6) == -2);

  fifth = Octagon(10, 2, 1, 0, -1, -2, -1, 0);
  assert(fifth.limit(1) == 1);

  std::cout << "OK! Limits constructor works\n";
}

void vertexTest() {

}

void inflateTest() {
  for (int j = 0; j < 1000; ++j) {

    Octagon first = getRandomOctagon();

    if (first.empty())
      continue;
    Octagon copy = first;
    for (int i = 0; i < 5; ++i) {
      int d = rnd() % int(1e5);
      first.inflate(d);
      first.inflate(-d);
      assert(first == copy);
    }
  }
  std::cout << "OK! inflate works\n";
}

void intersectionTest() {

}

void myTestingFunction() {
  constructorsTest();
  vertexTest();
  inflateTest();
  intersectionTest();
}



void yourTests() {

}
#endif //OCTAGON__TEST_OCTAGON_H_
