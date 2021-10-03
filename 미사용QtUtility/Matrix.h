#pragma once

#include <qstring.h>
#include <QtDebug>

enum FillDirection { FillLeft, FillRight, FillTop, FillBottom };
enum DiagDirection { DiagTopLeft, DiagTopRight, DiagBottomRight, DiagBottomLeft };

template <typename T>
class Matrix
{

public:

	Matrix()
	{
	}

	Matrix(int nByn) // Orthogonal Matrix NxN
	{
		setSize(nByn);
	}

	Matrix(int width, int height)
	{
		setSize(width, height);
	}

	~Matrix()
	{
		delete[] _data;
	}

	void setSize(int nByn)
	{
		setSize(nByn, nByn);
	}

	void setSize(int width, int height)
	{
		reset();

		_width = width;
		_height = height;

		_data = new T[width*height];
		memset(_data, 0, width*height * sizeof(T));
	}

	void setData(const T* data, int width, int height)
	{
		setSize(width, height);
		memcpy(_data, data, width*height * sizeof(T));
	}

	const T* data() const
	{
		return _data;
	}

	void reset()
	{
		if (_data) delete[] _data;
		
		_width = 0;
		_height = 0;

		resetStatistics();
	}

	void resetStatistics()
	{
		_min = NAN;
		_max = NAN;
		_mean = NAN;
		_var = NAN;
		_sum = NAN;
	}

	template <typename K>
	void fromData(K* data, int width, int height)
	{
		setSize(width, height);
		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hi + w;
				_data[i] = (T)data[i];
			}
		}
	}

	Matrix& operator+=(const Matrix& other)
	{
		if (_data == 0 || _width != other._width || _height != other._height) return *this;

		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hi + w;
				_data[i] += other._data[i];
			}
		}

		resetStatistics();

		return *this;
	}

	Matrix& operator-=(const Matrix& other)
	{
		if (_data == 0 || _width != other._width || _height != other._height) return *this;

		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0;w  < _width; w++)
			{
				int i = hi + w;
				_data[i] -= other._data[i];
			}
		}
		resetStatistics();
		return *this;
	}

	Matrix& operator*=(const Matrix& other)
	{
		if (_data == 0 || _width != other._width || _height != other._height) return *this;

		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w  < _width; w++)
			{
				int i = hi + w;
				_data[i] *= other._data[i];
			}
		}
		resetStatistics();
		return *this;
	}

	// Asign Operator
	Matrix& operator=(const Matrix& other)
	{
		//setSize(other._width, other._height);
		setData(other._data, other._width, other._height);
		//memcpy(this, &other, _width*_height * sizeof(T));

		return *this;
	}

	Matrix& operator+=(double v)
	{
		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hi + w;
				_data[i] += v;
			}
		}
		resetStatistics();
		return *this;
	}

	Matrix& operator-=(double v)
	{
		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hi + w;
				_data[i] -= v;
			}
		}
		resetStatistics();
		return *this;
	}

	Matrix operator*(const Matrix& o)
	{
		Matrix r = *this;
		r *= o;
		return r;
	}

	Matrix& operator*=(double v)
	{
		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hi + w;
				_data[i] *= v;
			}
		}
		resetStatistics();
		return *this;
	}

	Matrix& operator/=(double v)
	{
		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hi + w;
				_data[i] /= v;
			}
		}
		resetStatistics();
		return *this;
	}

	Matrix& operator/=(const Matrix& v)
	{
		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hi + w;
				_data[i] /= v._data[i];
			}
		}
		resetStatistics();
		return *this;
	}

	Matrix& operator^(int n)
	{
		if (n == 0)
		{
			return *this;
		}

		T* d = new T[_width*_height];
		memset(d, 0, _width*_height * sizeof(T));
		int copySize = sizeof(T)*(_height - abs(n))*_width;

		if (n > 0) // Shift Up
		{
			memcpy(d, _data + (_width*n), copySize);
		}
		else // Shift Down
		{
			memcpy(d + (_width*(-n)), _data, copySize);
		}

		delete[] _data;
		_data = d;
		resetStatistics();

		return *this;
	}

	// Shift Left Operator
	Matrix& operator<<(int n)
	{
		if (n == 0)
		{
			return *this;
		}

		T* d = new T[_width*_height];
		memset(d, 0, _width*_height * sizeof(T));
		int lineSize = sizeof(T)*(_width - abs(n));

		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			if (n > 0) // Shift Left
			{
				memcpy(d + hi, _data + hi + n, lineSize);
			}
			else // Shift Right
			{
				memcpy(d + hi - n, _data + hi, lineSize);
			}
		}

		delete[] _data;
		_data = d;
		resetStatistics();
		return *this;
	}

	int width() const
	{
		return _width;
	}

	int height() const
	{
		return _height;
	}

	// return value of index, index = x + y*width
	T& operator[](int index) const
	{
		return _data[index];
	}

	// return value at (w, h) point
	T& value(int w, int h)
	{
		return _data[h*_width + w];
	}

	// op(operator) : equal('='), less('<'), greater('>'), lessequal('['), greaterequal(']')
	// change with value if m1[i] op m2[i]
	Matrix& conditionalReplace(const Matrix& m1, char op, const Matrix& m2, T value)
	{
		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hi + w;
				switch (op)
				{
				case '=':
					if (m1._data[i] == m2._data[i]) _data[i] = value;
					break;
				case '<':
					if (m1._data[i] < m2._data[i]) _data[i] = value;
					break;
				case '>':
					if (m1._data[i] > m2._data[i]) _data[i] = value;
					break;
				case '[':
					if (m1._data[i] <= m2._data[i]) _data[i] = value;
					break;
				case ']':
					if (m1._data[i] >= m2._data[i]) _data[i] = value;
					break;
				}
			}
		}

		resetStatistics();
		return *this;
	}

	// replace with replaceValue if this[i] op compareValue
	Matrix& conditionalReplace(char op, T compareValue, T replaceValue)
	{
		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hi + w;
				switch (op)
				{
				case '=':
					if (_data[i] == compareValue) _data[i] = replaceValue;
					break;
				case '<':
					if (_data[i] < compareValue) _data[i] = replaceValue;
					break;
				case '>':
					if (_data[i] > compareValue) _data[i] = replaceValue;
					break;
				case '[':
					if (_data[i] <= compareValue) _data[i] = replaceValue;
					break;
				case ']':
					if (_data[i] >= compareValue) _data[i] = replaceValue;
					break;
				}
			}
		}

		resetStatistics();

		return *this;
	}

	// Fill [[fromW, toW], [fromH, toH]] with value
	void fill( T value, int fromW, int toW, int fromH, int toH)
	{
		for (int h = fromH; h <= toH; h++)
		{
			int hi = h * _width;
			for (int w = fromW; w <= toW; w++)
			{
				int i = hi + w;
				_data[i] = value;
			}
		}
		resetStatistics();
	}
	// direction
	// 0:left, 1:right, 2:top, 3:bottom
	// Fill [first, boundary], [boundary, last] with value. Include boundary index.
	void fill(T value, int boundary, FillDirection direction)
	{
		if (direction == FillLeft) // left
		{
			fill(value, 0, boundary, 0, _height-1);
		}
		else if (direction == FillRight) // right
		{
			fill(value, boundary, _width-1, 0, _height-1);
		}
		else if (direction == FillTop) // top
		{
			fill(value, 0, _width-1, 0, boundary);
		}
		else
		{
			fill(value, 0, _width - 1, boundary, _height - 1);
		}
		resetStatistics();
	}

	// corner : 
	// 0:topleft,		1:topright		2:bottomright		3:botomleft		
	// 1, 1, 1, 1		1, 1, 1, 1		0, 0, 0, 1			1, 0, 0, 0		
	// 1, 1, 1, 0		0, 1, 1, 1		0, 0, 1, 1			1, 1, 0, 0		
	// 1, 1, 0, 0		0, 0, 1, 1		0, 1, 1, 1			1, 1, 1, 0		
	// 1, 0, 0, 0		0, 0, 0, 1		1, 1, 1, 1			1, 1, 1, 1		
	void diagnonal(T value, DiagDirection corner)
	{
		if (corner == DiagTopLeft) // Top Left
		{
			for (int h = 0; h < _height; h++)
			{
				int hi = h * _width;
				for (int w = 0; w < _width - h; w++)
				{
					int i = hi + w;
					_data[i] = value;
				}
			}
		}
		else if (corner == DiagTopRight) // topright
		{
			for (int h = 0; h < _height; h++)
			{
				int hi = h * _width;
				for (int w = h; w < _width; w++)
				{
					int i = hi + w;
					_data[i] = value;
				}
			}
		}
		else if (corner == DiagBottomRight) // bottomright
		{
			for (int h = 0; h < _height; h++)
			{
				int hi = h * _width;
				for (int w = _width-h-1; w < _width; w++)
				{
					int i = hi + w;
					_data[i] = value;
				}
			}
		}
		else
		{
			for (int h = 0; h < _height; h++)
			{
				int hi = h * _width;
				for (int w = 0; w < h + 1; w++)
				{
					int i = hi + w;
					_data[i] = value;
				}
			}
		}
		resetStatistics();
	}
	
	float min()
	{
		if (!_isnanf(_min)) return _min;
		_min = FLT_MAX;
		_max = 0;

		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hi + w;

				T v = _data[i];
				if (v > _max)  _max = v;
				if (v < _min)  _min = v;
			}
		}

		return _min;
	}

	float max()
	{
		if (!_isnanf(_max)) return _max;
		_min = FLT_MAX;
		_max = 0;

		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hi + w;

				T v = _data[i];
				if (v > _max)  _max = v;
				if (v < _min)  _min = v;
			}
		}

		return _max;
	}

	float mean() 
	{
		if (!_isnanf(_mean)) return _mean;

		_sum = 0;
		_min = FLT_MAX;
		_max = 0;

		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hi + w;

				T v = _data[i];
				_sum += v;
				if (v > _max)  _max = v;
				if (v < _min)  _min = v;
			}
		}
		_mean = _sum / (_width * _height);

		return _mean;
	}

	float variance()
	{
		if (!_isnanf(_var)) return _var;
		if (!_isnanf(_mean)) mean();

		float temp = 0.0;
		for (int h = 0; h < _height; h++)
		{
			int hh = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hh + w;
				float v = _data[i] - _mean;
				temp += v * v;
			}
		}
		_var = temp / (_width * _height - 1);
		return _var;
	}

	float sum()
	{
		if (!_isnanf(_sum)) return _sum;
		_sum = 0;

		for (int h = 0; h < _height; h++)
		{
			int hi = h * _width;
			for (int w = 0; w < _width; w++)
			{
				int i = hi + w;

				T v = _data[i];
				_sum += v;
			}
		}

		return _sum;
	}

	/// Debuging
	void print() const
	{
		QString str;
		for (int i = 0; i < _height; i++)
		{
			QStringList list;
			for (int j = 0; j < _width; j++)
			{
				list << QString("%1").arg((double)_data[i*_width + j], 0, 'f', 6);
			}

			str += list.join(", ");
			str += "\n";
		}

		qDebug().noquote() << str;
	}

private:
	int _width = 0;
	int _height = 0;
	float _min = NAN;
	float _max = NAN;
	float _mean = NAN;
	float _var = NAN;
	float _sum = NAN;

	T* _data = 0;
};
