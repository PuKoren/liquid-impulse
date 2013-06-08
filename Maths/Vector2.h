#ifndef _VECTOR2_H
#define _VECTOR2_H

class Vector2{
	public:
		Vector2();
		Vector2(Vector2 *);
		Vector2(const Vector2&);
		Vector2(float, float);
		Vector2(int, int);
		~Vector2();
		
		float X;
		float Y;

		int Distance(Vector2 *);
		float Length();
		Vector2 Normalize();
		Vector2 operator- (Vector2);
		Vector2 operator+ (Vector2);
		Vector2 operator+= (Vector2);
		Vector2 operator* (Vector2);
		Vector2 operator* (float);
};
#endif