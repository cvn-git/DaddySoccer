#ifndef DADDYS_SOCCER_GEOMETRY_H
#define DADDYS_SOCCER_GEOMETRY_H

struct LogicalPoint
{
    double x_ = 0.0;
    double y_ = 0.0;
    LogicalPoint() {}
    LogicalPoint(double x, double y) : x_(x), y_(y) {}
};

struct LogicalVector
{
    double vx_ = 0.0;
    double vy_ = 0.0;
    LogicalVector() {}
    LogicalVector(double vx, double vy) : vx_(vx), vy_(vy) {}
    LogicalVector(const LogicalPoint &start, const LogicalPoint &end) : vx_(end.x_ - start.x_), vy_(end.y_ - start.y_) {}
    double dotProduct(const LogicalVector &other) const;
    double crossProduct(const LogicalVector &other) const;
    double length() const;
    double angle() const;
};

struct LogicalLine
{
    LogicalPoint p_;
    LogicalVector v_;
    LogicalLine() {}
    LogicalLine(const LogicalPoint &p, const LogicalVector &v) : p_(p), v_(v) {}
    LogicalLine(const LogicalPoint &p1, const LogicalPoint &p2) : p_(p1), v_(p1, p2) {}
    bool findIntersect(const LogicalLine &other, LogicalPoint *intersect) const;
    LogicalPoint findSymmetricalPoint(const LogicalPoint &point) const;
};

class Wall : public LogicalLine
{
public:
    using LogicalLine::LogicalLine;
    bool bounce(LogicalPoint &prev, LogicalPoint &next, double objectRadius) const;
};

#endif  // DADDYS_SOCCER_GEOMETRY_H
