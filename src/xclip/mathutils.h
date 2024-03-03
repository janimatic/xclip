#pragma once

#include "io.h"

#ifndef _MATHUTILS_H_
#define _MATHUTILS_H_

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES // M_PI
#endif
#include <math.h>

struct XPoint2D {
	f64 x = 0, y = 0;
};

struct XPoint3D {
  f64 x = 0, y = 0, z = 0;
};

// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
struct Quaternion {
  f64 w = 0, x = 0, y = 0, z = 0;
};

struct EulerAngles {
  f64 roll = 0, pitch = 0, yaw = 0;
};

static EulerAngles toEulerAngles(Quaternion q) {
	EulerAngles angles;

	// roll (x-axis rotation)
	f64 sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	f64 cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.roll = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	f64 sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	f64 cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.pitch = 2 * std::atan2(sinp, cosp) - M_PI / 2;

	// yaw (z-axis rotation)
	f64 siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	f64 cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.yaw = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

static Quaternion toQuaternion(f64 roll, f64 pitch, f64 yaw) // roll (x), pitch (y), yaw (z), angles are in radians
{
	// Abbreviations for the various angular functions

	f64 cr = cos(roll * 0.5);
	f64 sr = sin(roll * 0.5);
	f64 cp = cos(pitch * 0.5);
	f64 sp = sin(pitch * 0.5);
	f64 cy = cos(yaw * 0.5);
	f64 sy = sin(yaw * 0.5);

	Quaternion q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}

static f64 radToDeg(f64 radians) {
	return radians * (180.0 / M_PI);
}
static f64 degToRad(f64 degrees) {
  return degrees*(M_PI / 180);
}

static f64 quaternionToRollDegree(f64 w, f64 x, f64 y, f64 z) {
	Quaternion q = { w, x, y, z };
	EulerAngles angles = toEulerAngles(q);
	return radToDeg(angles.roll);
}

static Quaternion rollDegreeToQuaternion(f64 angle) {
  return toQuaternion(degToRad(angle), 0, 0);
}

static XPoint2D convertPoint2DCoordsPixelsToNdc(XPoint2D p, float width, float height) {
	XPoint2D result;
	result.x = (p.x * 2.0 / width) - 1.0;
	result.y = (p.y * 2.0 / height) - 1.0;
	return result;
}

static XPoint2D convertPoint2DCoordsNdcToPixels(XPoint2D p, float width, float height) {
	XPoint2D result;
	result.x = (p.x / 2.0 * width) + 1.0;
	result.y = (p.y / 2.0 * height) + 1.0;
	return result;
}

#endif
