#ifndef MASKPOINT_H
#define MASKPOINT_H

#include <unordered_set>
#include <string>

struct MaskPoint
{
  int x;
  int y;

  MaskPoint() {}

  MaskPoint(int x, int y)
      : x(x), y(y) {}

  bool operator==(const MaskPoint& otherPoint) const
  {
      if (this->x == otherPoint.x && this->y == otherPoint.y)
          return true;

      return false;
  }

  struct HashFunction
    {
      size_t operator()(const MaskPoint& point) const
      {
        return std::hash<std::string>()(std::to_string(point.x) + std::to_string(point.y));
      }
    };
};

#endif // MASKPOINT_H
