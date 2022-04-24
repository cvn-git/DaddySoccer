#include "geometry.h"

#include <cmath>
#include <stdexcept>

double LogicalVector::dotProduct(const LogicalVector &other) const
{
    return (vx_ * other.vx_) + (vy_ * other.vy_);
}

double LogicalVector::crossProduct(const LogicalVector &other) const
{
    return (vx_ * other.vy_) - (vy_ * other.vx_);
}

double LogicalVector::length() const
{
    return std::sqrt(dotProduct(*this));
}

double LogicalVector::angle() const
{
    return std::atan2(vy_, vx_);
}

bool LogicalLine::findIntersect(const LogicalLine &other, LogicalPoint *intersect) const
{
    // Reference: https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
    const auto &p = p_;
    const auto &q = other.p_;
    const auto &r = v_;
    const auto &s = other.v_;

    double rcs = r.crossProduct(s);
    if (std::abs(rcs) < 1e-100)
    {
        return false;
    }

    // t = (q − p) × s / (r × s)
    LogicalVector pq(p, q);
    double t = pq.crossProduct(s) / rcs;

    // u = (q − p) × r / (r × s)
    double u = pq.crossProduct(r) / rcs;

    if ((t >= 0) && (t <= 1.0) && (u >= 0) && (u <= 1.0))
    {
        if (intersect != nullptr)
        {
            intersect->x_ = p.x_ + (t * r.vx_);
            intersect->y_ = p.y_ + (t * r.vy_);
        }
        return true;
    }
    else
        return false;
}

LogicalPoint LogicalLine::findSymmetricalPoint(const LogicalPoint &point) const
{
    double len = v_.length();
    LogicalVector vPoint(p_, point);
    double scale = 2.0 * v_.dotProduct(vPoint) / (len * len);

    return LogicalPoint(p_.x_ + (v_.vx_ * scale) - vPoint.vx_,
                        p_.y_ + (v_.vy_ * scale) - vPoint.vy_);
}

bool Wall::bounce(LogicalPoint &prev, LogicalPoint &next, double objectRadius) const
{
    LogicalLine wall(*this);
    if (std::abs(v_.vx_) < 1e-50)
    {
        if ((prev.x_ + objectRadius) < p_.x_)
            wall.p_.x_ -= objectRadius;
        else if ((prev.x_ - objectRadius) > p_.x_)
            wall.p_.x_ += objectRadius;
        else
            ;
    }
    else if (std::abs(v_.vy_) < 1e-50)
    {
        if ((prev.y_ + objectRadius) < p_.x_)
            wall.p_.y_ -= objectRadius;
        else if ((prev.y_ - objectRadius) > p_.y_)
            wall.p_.y_ += objectRadius;
        else
            ;
    }
    else
    {
        throw std::runtime_error("Invalid wall");
        return false;
    }

    if (wall.findIntersect(LogicalLine(prev, next), nullptr))
    {
        prev = wall.findSymmetricalPoint(prev);
        next = wall.findSymmetricalPoint(next);
        return true;
    }
    else
        return false;
}
