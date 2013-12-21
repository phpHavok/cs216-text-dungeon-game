#ifndef RECT_H_
#define RECT_H_

template<class T>
class Rect {
	public:
		Rect( T x, T y, T w, T h );
		virtual ~Rect();
		virtual void setX( T x );
		virtual T getX() const;
		virtual void setY( T y );
		virtual T getY() const;
		virtual void setW( T w );
		virtual T getW() const;
		virtual void setH( T h );
		virtual T getH() const;
	private:
		T mX;
		T mY;
		T mW;
		T mH;
};

template<class T>
Rect<T>::Rect( T x, T y, T w, T h )
	: mX( x ), mY( y ), mW( w ), mH( h )
{};

template<class T>
Rect<T>::~Rect() {}

template<class T>
void Rect<T>::setX( T x ) {
	mX = x;
}

template<class T>
T Rect<T>::getX() const {
	return mX;
}

template<class T>
void Rect<T>::setY( T y ) {
	mY = y;
}

template<class T>
T Rect<T>::getY() const {
	return mY;
}

template<class T>
void Rect<T>::setW( T w ) {
	mW = w;
}

template<class T>
T Rect<T>::getW() const {
	return mW;
}

template<class T>
void Rect<T>::setH( T h ) {
	mH = h;
}

template<class T>
T Rect<T>::getH() const {
	return mH;
}

#endif
