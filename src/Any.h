#pragma once
#ifndef _H_ANY_H_
#define _H_ANY_H_

#include <algorithm>


class Any
{
private:
	class AnyValueBase
	{
	public:
		AnyValueBase() = default;
		virtual ~AnyValueBase() = default;
		virtual AnyValueBase* Clone() = 0;
	};

	template<typename T>
	class AnyValue : public AnyValueBase
	{
	public:
		template<typename U>
		AnyValue(U&& value)
			:_value(std::forward<U>(value))
		{}

		template<typename U>
		AnyValue(const U* value)
			:_value(const_cast<U*>(value))
		{
		}

		AnyValue(AnyValue* other)
		{
			if (other != nullptr)
			{
				_value = other->_value;
			}
		}

		~AnyValue() = default;

		AnyValueBase* Clone() override
		{
			return new AnyValue<T>(_value);
		}

		T GetValue()
		{
			return _value;
		}

	private:
		T _value;
	};

public:
	template<typename T>
	Any(T&& value)
		:_value_ptr(new AnyValue<T>(std::forward<T>(value)))
	{
	}

	template<typename T>
	Any(T* value)
		:Any()
	{
		if (value != nullptr)
		{
			_value_ptr = new AnyValue<T*>(value);
		}
	}

	Any(Any&& other) noexcept
		:Any()
	{
		*this = std::move(other);
	}

	Any(const Any& other)
		:_value_ptr(other.CloneValue())
	{
	}

	Any(const Any* other)
		:Any()
	{
		if (other != nullptr)
		{
			_value_ptr = other->CloneValue();
		}
	}

	Any()
		:_value_ptr(nullptr)
	{
	}

	~Any()
	{
		delete _value_ptr;
	}

public:
	template<typename T>
	Any& operator=(const T& value)
	{
		*this = Any(value);
		return *this;
	}

	Any& operator=(Any&& other) noexcept
	{
		if (_value_ptr != other._value_ptr)
		{
			delete _value_ptr;

			_value_ptr = other._value_ptr;
			other._value_ptr = nullptr;
		}

		return *this;
	}

	Any& operator=(const Any& other)
	{
		if (_value_ptr != other._value_ptr)
		{
			delete _value_ptr;

			_value_ptr = other._value_ptr;
		}

		return *this;
	}

	AnyValueBase* CloneValue() const
	{
		if (_value_ptr == nullptr)
		{
			return nullptr;
		}

		return _value_ptr->Clone();
	}

	template<typename T>
	T Value() const
	{
		auto* value = dynamic_cast<AnyValue<T>*>(_value_ptr);
		if (value == nullptr)
		{
			return 0;
		}

		return value->GetValue();
	}

	static const Any& EmptyValue()
	{
		static Any empty_value;
		return empty_value;
	}

private:
	AnyValueBase* _value_ptr;
};

#endif
