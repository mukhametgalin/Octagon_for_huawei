
#ifndef OCTAGON__OCTAGON_H_
#define OCTAGON__OCTAGON_H_

#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <assert.h>

class Point {
 public:
  int x{0};
  int y{0};

  Point() : x(0), y(0) {}
  Point(int x, int y) : x(x), y(y) {}

  bool operator==(const Point a) {
    return x == a.x && y == a.y;
  }
};

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

  void normalize() {
    if (empty())
      return;
    for (int i = 0; i < size; ++i) {
      int next = (i + 1) % size;
      int next2 = (i + 2) % size;
      int prev = (i - 1 + size) % size;
      int prev2 = (i - 2 + size) % size;

      if (i % 2 == 0) {
        int first =  2 * (l[next2] + l[prev]);
        int second = l[next] + l[prev];
        int third = 2 * (l[prev2] + l[next]);
        l[i] = std::min({first, second, third, 2 * l[i]});
        l[i] = l[i] / 2 + (l[i] % 2);
      } else {
        int first = l[next2] + 2 * l[prev];
        int second = l[next] + l[prev];
        int third = 2 * l[next] + l[prev2];
        l[i] = std::min({first, second, third, l[i]});
      }
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
    if (dir < 1 || dir > size) {
      throw std::exception();
      // if we want to get limit with number not from [1, size=8], we will get UB.
    }
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
      l.resize(size);
      for (int i = 0 ; i < size; ++i) {
        l[i] = c[i].first * point.x + c[i].second * point.y;
      }
      return;
    }
    for (int i = 0; i < size; ++i) {
      l[i] = std::max(l[i], c[i].first * point.x + c[i].second * point.y);
    }
    normalize();
  }

  void inflate(int inflateParam) {
    emptyCheck();

    if (inflateParam == 0)
      return;

    for (int i = 0; i < size; ++i) {
      int d = inflateParam;
      if (i % 2) {
        d = static_cast<int>(static_cast<double>(d) * std::sqrt(2 ) + (d > 0 ? 1 : -1));
      }
      l[i] += d;
    }
  }

  bool empty() const {

    if(l.size() != static_cast<size_t>(size)) { // our octagon have no limits, so it is empty
      return true;
    }

    if (-l[4] > l[0] || -l[6] > l[2] || -l[5] > l[1] || -l[7] > l[3]) // trivial case
      return true;

    // M, N and L - rectangles from theoretical explanation

    // then I get doubled points of rectangle M that contains diagonal rectangle L
    int y_up = l[1] + l[3];
    int x_right = l[1] + l[7];
    int x_left = -l[3] - l[5];
    int y_down = -l[5] - l[7];

    // there I check that M intersects N
    if (x_left > 2 * l[0]  || -x_right > 2 * l[4]
      || y_down > 2 * l[2] || -y_up > 2 * l[6])
      return true;

    // vertices of N
    Point left_up = Point(-l[4], l[2]);
    Point right_up = Point(l[0], l[2]);
    Point left_down = Point(-l[4], -l[6]);
    Point right_down = Point(l[0], -l[6]);

    return (-right_up.x - right_up.y > l[5]
        || left_down.x + left_down.y > l[1]
        || left_up.x - left_up.y > l[7]
        || -right_down.x + right_down.y > l[3]);
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

Octagon* intersection(const Octagon&, const Octagon&);

bool hasIntersection(const Octagon& first, const Octagon& second) {
  if (first.empty() || second.empty())
    return false;

  Octagon* res = intersection(first, second);

  if (res != nullptr) {
    delete res;
    return true;
  }

  return false;
}

Octagon* intersection(const Octagon& first, const Octagon& second) {
  if (first.empty() || second.empty())
    return nullptr;

  Octagon* answer = new Octagon(first);

  for (int i = 0; i < Octagon::size; ++i) {
    answer->l[i] = std::min(first.l[i], second.l[i]);
  }
  if (answer->empty()) {
    delete answer;
    return nullptr;
  }
  answer->normalize();
  return answer;
}

#endif //OCTAGON__OCTAGON_H_