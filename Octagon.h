
#ifndef OCTAGON__OCTAGON_H_
#define OCTAGON__OCTAGON_H_

#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>

class Point {
 public:
  int x{0};
  int y{0};

  Point() : x(0), y(0) {}
  Point(int x, int y) : x(x), y(y) {}

};

Point linesIntersection(int a1, int b1, int c1, int a2, int b2, int c2) {
  //intersection point of 2 lines: a1x + b1y == c1, a2x + b2y = c2.
  return {(b2 * c1 - b1 * c2) / (b2 * a1 - a2 * b1),
          (a1 * c2 - a2 * c1) / (b2 * a1 - a2 * b1)};
}

struct EmptyOctagonError {};

class Octagon {
  friend
  Octagon* intersection(const Octagon&, const Octagon&);

  friend
  bool hasIntersection(const Octagon&, const Octagon&);

  friend
  void  constructorsTest();

 private:
  std::vector<int> l;

  static const std::vector<std::pair<int, int>> c;

  Point localLinesIntersection(int first, int second) { // just for convenience
    return {(l[first] * c[second].second - l[second] * c[first].second)
        / (c[first].first * c[second].second - c[second].first * c[first].second),
            (c[first].first * l[second] - c[second].first * l[first])
        / (c[first].first * c[second].second - c[second].first * c[first].second)};
  }

  void normalize() {
    for (int i = 0; i < size; ++i) {
      int next = (i + 1) % size;
      int next2 = (i + 2) % size;
      int prev = (i - 1 + size) % size;
      int prev2 = (i - 2 + size) % size;
      Point first, second, third;
      first = localLinesIntersection(next, prev2);
      second = localLinesIntersection(prev, next2);
      third = localLinesIntersection(prev, next);
      int first_value = c[i].first * first.x + c[i].second * first.y;
      int second_value = c[i].first * second.x + c[i].second * second.y;
      int third_value = c[i].first * third.x + c[i].second * third.y;
      l[i] = std::min({first_value, second_value, third_value, l[i]});
    }
  }

  void emptyCheck() const {
    if (empty())
      throw EmptyOctagonError();
  }

 public:

  static const int size;

  // constructors:
  Octagon() {}

  Octagon(const Octagon& other) : l(other.l) {}

  Octagon(std::initializer_list<Point> list) : Octagon() {
    if (list.size() == 0)
      return;
    l.resize(size);

    auto first = *list.begin();

    for (int i = 0 ; i < size; ++i) {
      l[i] = c[i].first * first.x + c[i].second * first.y;
    }

    for (auto i : list) {
      this->coverPoint(i);
    }
  }

  Octagon(std::vector<Point> list) : Octagon() { // if we need to construct octagon from implicit list of Points
    if (list.size() == 0)
      return;
    l.resize(size);
    auto first = *list.begin();

    for (int i = 0 ; i < size; ++i) {
      l[i] = c[i].first * first.x + c[i].second * first.y;
    }

    for (auto i : list) {
      this->coverPoint(i);
    }
  }

  Octagon(std::initializer_list<int> limits) : l(limits) {
    normalize();
  }

  Octagon(int limit0, int limit1, int limit2, int limit3,
          int limit4, int limit5, int limit6, int limit7) :
      l({limit0, limit1, limit2, limit3, limit4, limit5, limit6, limit7}) {
    normalize();
  }


  // operators:
  bool operator==(const Octagon& other) const {
    if (empty() || other.empty())
      return false;
    for (int i = 0; i < size; ++i) {

      if (l[i] != other.l[i]) {
        return false;
      }
    }
    return true;
  }

  // describing functions:
  int limit(int dir) const {
    emptyCheck();

    return l[dir - 1];
  }

  Point vertex(int dir) const {
    emptyCheck();

    Point answer = {0, 0};
    dir = (dir + 1) % size;
    int other_dir = (dir + 1) % size;
    if (dir % 2 != 0) {
      std::swap(dir, other_dir);
    }
    if (dir % 4 == 0) {
      answer.x = c[dir].first * l[dir];
      answer.y = c[other_dir].second * (l[other_dir] - c[other_dir].first * answer.x);
    } else {
      answer.y = c[dir].second * l[dir];
      answer.x = c[other_dir].first * (l[other_dir] - c[other_dir].second * answer.y);
    }
    return answer;
  }

  int isPointInside(const Point& point) const {
    if (empty())
      return false;

    for (int i = 0; i < size; ++i) {
      int value = c[i].first * point.x + c[i].second * point.y;
      if (value > l[i])
        return -1;
    }

    for (int i = 0; i < size; ++i) {
      int value = c[i].first * point.x + c[i].second * point.y;
      if (value == l[i])
        return 0;
    }
    return 1;
  }

  bool isEqual(const Octagon& other) const {
    return *this == other;
  }

  // modifying functions:
  void coverPoint(const Point& point) {
    if (empty()) {
      *this = Octagon({point});
      return;
    }
    for (int i = 0; i < size; ++i) {
      l[i] = std::max(l[i], c[i].first * point.x + c[i].second * point.y);
    }
  }

  void inflate(int inflateParam) {
    emptyCheck();

    if (inflateParam == 0)
      return;

    for (int i = 0; i < size; ++i) {
      int d = inflateParam;
      if (i % 2) {
        d = static_cast<int>(d * std::sqrt(2) + (d > 0 ? 1 : -1));
      }
      l[i] += d;
    }
  }

  bool empty() const {
    return l.size() != static_cast<size_t>(size);
  }

  void clear() {
    l.clear();
  }
};

const std::vector<std::pair<int, int>> Octagon::c =
    {{1, 0},
    {1, 1},
    {0, 1},
    {-1, 1},
    {-1, 0},
    {-1, -1},
    {0, -1},
    {1, -1}};

const int Octagon::size = 8;

bool hasIntersection(const Octagon& first, const Octagon& second) {
  if (first.empty() || second.empty())
    return false;

  for (int i = 0; i < Octagon::size; ++i) {
    for (int j = 0; j < Octagon::size; ++j) {
      if (i != j) {
        Point intersection_point = linesIntersection(Octagon::c[i].first, Octagon::c[i].second,
                                                     first.limit(i + 1),
                                                     Octagon::c[j].first, Octagon::c[j].second,
                                                     second.limit(j + 1));

        if (first.isPointInside(intersection_point) >= 0
            && second.isPointInside(intersection_point) >= 0) {
          return true;
        }
      }
    }
  }
  return false;
}

Octagon* intersection(const Octagon& first, const Octagon& second) {
  first.emptyCheck();
  second.emptyCheck();

  if (!hasIntersection(first, second))
    return nullptr;

  Octagon* answer = new Octagon(first);

  for (int i = 0; i < Octagon::size; ++i) {
    answer->l[i] = std::min(first.l[i], second.l[i]);
  }

  return answer;
}

#endif //OCTAGON__OCTAGON_H_