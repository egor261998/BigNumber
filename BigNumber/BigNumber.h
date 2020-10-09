#pragma once

#include <iostream>
#include <windows.h>
#include <vector>
#include <string>

class BigNumber
{
	std::vector<short> _data;

public:
	~BigNumber() {}

	//contructors
	BigNumber() {}

	BigNumber(BigNumber& number)
	{
		_data = number._data;
	}

	BigNumber(std::string str)
	{
		Convert(str);
	}

	BigNumber(DWORD number)
	{
		Convert(number);
	}

	BigNumber(const char* number) :BigNumber(std::string(number)) {}

	//operator +
	static friend BigNumber& operator+(BigNumber numberA, BigNumber numberB)
	{
		BigNumber* buffer = new BigNumber(numberB);

		for (size_t i = 0; i < numberA._data.size(); i++)
			if (i >= buffer->_data.size())
				buffer->_data.push_back(numberA._data[i]);
			else
				buffer->_data[i] += numberA._data[i];

		buffer->Correct();

		return *buffer;
	}

	//operator +=
	BigNumber& operator+=(BigNumber number)
	{
		*this = *this + number;

		return *this;
	}

	//operator ++
	BigNumber& operator++(int number)
	{
		number = 1;

		*this += number;

		return *this;
	}

	// operator -
	static friend BigNumber& operator-(BigNumber numberA, BigNumber numberB)
	{
		if (numberA < numberB)
			throw std::exception("Return negative number");

		BigNumber* buffer = new BigNumber();

		for (size_t i = 0; i < numberB._data.size(); i++)
		{
			auto sub = numberA._data[i] - numberB._data[i];

			if (sub < 0)
			{
				numberA.QueryTen(i);
				sub = numberA._data[i] - numberB._data[i];
			}

			buffer->_data.push_back(sub);
		}

		if (numberA > numberB)
			for (size_t i = numberB._data.size(); i < numberA._data.size(); i++)
				buffer->_data.push_back(numberA._data[i]);

		buffer->Correct();

		return *buffer;
	}

	//operator -=
	BigNumber& operator-=(BigNumber number)
	{
		*this = *this - number;

		return *this;
	}

	//operator *
	static friend BigNumber& operator*(BigNumber numberA, BigNumber numberB)
	{
		BigNumber* buffer = new BigNumber();

		for (size_t i = 0; i < numberA._data.size(); i++)
		{
			for (size_t j = 0; j < numberB._data.size(); j++)
			{
				size_t k = i + j;

				if (k >= buffer->_data.size())
					buffer->_data.push_back(numberA._data[i] * numberB._data[j]);
				else
					buffer->_data[k] += numberA._data[i] * numberB._data[j];
			}

			buffer->Correct();
		}

		return *buffer;
	}

	//operator *=
	BigNumber& operator*=(BigNumber number)
	{
		*this = *this * number;

		return *this;
	}

	//operator /
	static friend BigNumber& operator/(BigNumber numberA, BigNumber numberB)
	{
		BigNumber* buffer = new BigNumber();

		while (numberA >= numberB)
		{
			numberA -= numberB;
			(*buffer)++;
		}

		return *buffer;
	}

	//operator /=
	BigNumber& operator/=(BigNumber number)
	{
		*this = *this / number;

		return *this;
	}

	//operator ^
	static friend BigNumber& operator^(BigNumber numberA, BigNumber numberB)
	{
		BigNumber* buffer = new BigNumber(numberA);

		for (BigNumber i = 1 ; i < numberB; i++)
			*buffer *= numberA;

		return *buffer;
	}

	//operator ^=
	BigNumber& operator^=(BigNumber number)
	{
		*this = *this ^ number;

		return *this;
	}

	//operator %
	static friend BigNumber& operator%(BigNumber numberA, BigNumber numberB)
	{
		BigNumber* buffer = new BigNumber(numberA);

		while (*buffer >= numberB)
			*buffer -= numberB;

		return *buffer;
	}

	//operator %=
	BigNumber& operator%=(BigNumber number)
	{
		*this = *this % number;

		return *this;
	}

	//operator =
	BigNumber& operator=(std::string str)
	{
		Convert(str);
		return *this;
	}

	BigNumber& operator=(BigNumber number)
	{
		this->_data = number._data;
		return *this;
	}

	BigNumber& operator=(DWORD number)
	{
		Convert(number);
		return *this;
	}

	//operator ==
	static friend bool operator==(BigNumber numberA, BigNumber numberB)
	{
		if (numberA._data.size() != numberB._data.size())
			return false;

		for (size_t i = 0; i < numberA._data.size(); i++)
			if (numberA._data[i] != numberB._data[i])
				return false;

		return true;
	}

	//operator !=
	static friend bool operator!=(BigNumber numberA, BigNumber numberB)
	{
		return !(numberA == numberB);
	}

	//operator <
	static friend bool operator<(BigNumber numberA, BigNumber numberB)
	{
		if (numberA._data.size() < numberB._data.size())
			return true;

		if (numberA._data.size() > numberB._data.size())
			return false;

		size_t size = numberA._data.size();
		for (size_t i = 0; i != size; i++)
			if (numberA._data[size - i - 1] < numberB._data[size - i - 1])
				return true;
			else if (numberA._data[size - i - 1] > numberB._data[size - i - 1])
				return false;

		return false;
	}

	//operator <=
	static friend bool operator<=(BigNumber numberA, BigNumber numberB)
	{
		return (numberA < numberB || numberA == numberB);
	}

	//operator >
	static friend bool operator>(BigNumber numberA, BigNumber numberB)
	{
		if (numberA._data.size() > numberB._data.size())
			return true;

		if (numberA._data.size() < numberB._data.size())
			return false;

		size_t size = numberA._data.size();
		for (size_t i = 0; i != size; i++)
			if (numberA._data[size - i - 1] > numberB._data[size - i - 1])
				return true;
			else if (numberA._data[size - i - 1] < numberB._data[size - i - 1])
				return false;

		return false;
	}

	//operator >=
	static friend bool operator>=(BigNumber numberA, BigNumber numberB)
	{
		return (numberA > numberB || numberA == numberB);
	}

	//stream <<
	static friend std::ostream& operator<<(std::ostream& os, BigNumber& bn)
	{
		for (auto it = bn._data.rbegin(); it != bn._data.rend(); ++it)
			os << (short)*it;

		return os;
	}

	//stream >>
	static friend std::istream& operator>>(std::istream& is, BigNumber& bn)
	{
		std::string str;
		is >> str;
		bn = str;

		return is;
	}

private:
	void Convert(DWORD& number)
	{
		_data.clear();

		size_t i = 0;

		while (number)
		{
			_data.push_back(number % 10);
			number /= 10;
			i++;
		}
	}

	void Convert(std::string& str)
	{
		for (size_t i = 0; i < str.length(); i++)
			if (str[i] >= '0' && str[i] <= '9')
				_data.insert(_data.begin(), str[i] - '0');
			else
				throw std::exception("Incorrect data");
	}

	void Correct()
	{
		for (size_t i = 0; i < _data.size(); i++)
		{
			if (_data[i] / 10)
			{
				if (i + 1 >= _data.size())
					_data.push_back(_data[i] / 10);
				else
					_data[i + 1] += _data[i] / 10;

				_data[i] %= 10;
			}
		}

		for (size_t i = _data.size() - 1; i > 0; i--)
		{
			if (!_data[i])
				_data.erase(_data.begin() + i);
			else
				break;
		}
	}

	void QueryTen(size_t i)
	{
		if (!_data[i + 1])
			QueryTen(i + 1);

		_data[i + 1]--;
		_data[i] += 10;
	};
};
