#include "Vector2.h"
#include <cmath>

Vector2::Vector2(Vector2 *v){
	this->X = v->X;
	this->Y = v->Y;
}
Vector2::Vector2(){
	this->X = 0;
	this->Y = 0;
}
Vector2::Vector2(const Vector2 &v){
	this->X = v.X;
	this->Y = v.Y;
}
Vector2::~Vector2(){

}
Vector2::Vector2(int x, int y){
	this->X = (float)x;
	this->Y = (float)y;
}

Vector2::Vector2(float x, float y){
	this->X = x;
	this->Y = y;
}

/*
* Compute the Euclidian distance between two Vector2->
* Distance is sqrt((v1->X - v2->X)² + (v1->Y - v2->Y)²)
*/
int Vector2::Distance(Vector2 *v){
	return (int)sqrt((float)(this->X - v->X)*(this->X - v->X) + (this->Y - v->Y) * (this->Y - v->Y));
}

/*
* Compute the length of a vector
* sqrt(x²+y²)
*/
float Vector2::Length(){
	return sqrt((float)(this->X*this->X + this->Y*this->Y));
}

/*
* Normalize a vector
* Vector2(X/Length, Y/Length)
*/
Vector2 Vector2::Normalize(){
	return Vector2(this->X/this->Length(), this->Y/this->Length());
}

Vector2 Vector2::operator-(Vector2 v){
	return Vector2(this->X - v.X, this->Y - v.Y);
}

Vector2 Vector2::operator+(Vector2 v){
	return Vector2(this->X + v.X, this->Y + v.Y);
}

Vector2 Vector2::operator+=(Vector2 v){
	return Vector2(this->X + v.X, this->Y + v.Y);
}

Vector2 Vector2::operator*(Vector2 v){
	return Vector2(this->X * v.X, this->Y * v.Y);
}

Vector2 Vector2::operator*(float f){
	return Vector2(this->X * f, this->Y * f);
}