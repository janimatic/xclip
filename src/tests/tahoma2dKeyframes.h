#pragma once
#ifndef _TAHOMA2DKEYFRAMES_H_
#define _TAHOMA2DKEYFRAMES_H_
/// <summary>
/// This is a reduced set of Tahoma2D keyframes api just for testing xclip api... 
/// it does nothing! But just makes sure that the xclip api is handy and easy to use...
/// </summary>
/// <typeparam name="T"></typeparam>

static int tahomaFps = 24;
template <class T>
class TPointT {
public:
  T x, y;

  TPointT() : x(0), y(0){};
  TPointT(T _x, T _y) : x(_x), y(_y){};
  TPointT(const TPointT &point) : x(point.x), y(point.y){};
  inline TPointT &operator=(const TPointT &a) {
    x = a.x;
    y = a.y;
    return *this;
  };

  inline TPointT &operator+=(const TPointT &a) {
    x += a.x;
    y += a.y;
    return *this;
  };
  inline TPointT &operator-=(const TPointT &a) {
    x -= a.x;
    y -= a.y;
    return *this;
  };
  inline TPointT operator+(const TPointT &a) const {
    return TPointT(x + a.x, y + a.y);
  };
  inline TPointT operator-(const TPointT &a) const {
    return TPointT(x - a.x, y - a.y);
  };
  inline TPointT operator-() const { return TPointT(-x, -y); };

  bool operator!=(const TPointT &p) const { return x != p.x || y != p.y; }
};


typedef TPointT<double> TPointD;

class TDoubleKeyframe {
public:
  enum Type {
    None = 0,
    Constant,
    Linear,
    SpeedInOut,
    EaseInOut,
    EaseInOutPercentage,
    Exponential,
    Expression,
    File,
    SimilarShape
  };

  // private:
  Type m_type;
  Type m_prevType;
  double m_frame;
  double m_value;
  bool m_isKeyframe;
  int m_step;
  TPointD m_speedIn, m_speedOut;

public:
  TDoubleKeyframe(double frame = 0, double value = 0);

  bool operator<(const TDoubleKeyframe &k) const { return m_frame < k.m_frame; }
};

TDoubleKeyframe::TDoubleKeyframe(double frame, double value)
    : m_type(Linear)
    , m_frame(frame)
    , m_value(value)
    , m_step(1)
    , m_isKeyframe(false)
    , m_speedIn()
    , m_speedOut() {}
    
static void addKeyframe(double value, TDoubleKeyframe::Type type, int frame) {
  TDoubleKeyframe dkf;
  dkf.m_value = value;
  dkf.m_type  = type;
  dkf.m_frame = frame;
}

static void addKeyframe(double value, TPointD speedIn, TPointD speedOut,
                        TDoubleKeyframe::Type type, int frame) {
  TDoubleKeyframe dkf;
  dkf.m_value = value;
  dkf.m_speedIn  = speedIn;
  dkf.m_speedOut = speedOut;
  dkf.m_type  = type;
  dkf.m_frame = frame;
}

static void addKeyframes(const std::vector<TDoubleKeyframe> &ks) {
  for (auto k : ks) {
    if (k.m_type == TDoubleKeyframe::Type::SpeedInOut || k.m_type == TDoubleKeyframe::Type::EaseInOut || k.m_type == TDoubleKeyframe::Type::EaseInOutPercentage)
      addKeyframe(k.m_value, k.m_speedIn, k.m_speedOut, k.m_type, k.m_frame);
    else
      addKeyframe(k.m_value, k.m_type, k.m_frame);
  }
}

#endif
