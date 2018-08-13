#pragma once
#ifndef _H_ANY_H_
#define _H_ANY_H_
#include "ExceptionDefind.h"
#include "Define.h"
#include "Common.h"


class Any
{
private:
	class AnyValueBase
	{
	public:
		AnyValueBase() = default;
		virtual ~AnyValueBase() = default;
		virtual AnyValueBase* Clone() = 0;
		virtual const std::type_info& Type() const = 0;
		virtual Boolean IsBool() const = 0;
		virtual Boolean IsNumber() const = 0;
		virtual Boolean IsString() const = 0;
		virtual Boolean IsList() const = 0;
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

		~AnyValue() = default;

		AnyValueBase* Clone() override
		{
			return new AnyValue<T>(_value);
		}

		T& GetValue()
		{
			return _value;
		}

		Boolean IsBool() const override
		{
			return is_bool<T>::value;
		}

		Boolean IsNumber() const override
		{
			return is_number<T>::value;
		}

		Boolean IsString() const override
		{
			return is_string<T>::value;
		}

		Boolean IsList() const override
		{
			return is_list<T>::value;
		}

		const std::type_info& Type() const override
		{
			return typeid(T);
		}

	private:
		T _value;
	};

public:
	template<typename T, typename std::enable_if<!std::is_same<StorageType<T>, Any>::value && !std::is_same<T, AnyValueBase>::value, int>::type = 0>
	Any(T&& value)
		:_value_ptr(new AnyValue<StorageType<T>>(std::forward<T>(value)))
	{
	}

	template<typename T>
	Any(T* value)
		:Any()
	{
		if (value != nullptr)
		{
			_value_ptr = new AnyValue<StorageType<T>*>(value);
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

	Any(std::nullptr_t)
		:Any()
	{
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
	const std::type_info& Type() const
	{
		if (IsNull())
		{
			return typeid(nullptr);
		}

		return _value_ptr->Type();
	}

	static Any& EmptyValue()
	{
		static Any empty_value;
		return empty_value;
	}

	Boolean IsNull() const
	{
		return _value_ptr == nullptr;
	}

	Boolean IsBool() const
	{
		return IsNull() ? false : _value_ptr->IsBool();
	}

	Boolean IsNumber() const
	{
		return IsNull() ? false : _value_ptr->IsNumber();
	}

	Boolean IsString() const
	{
		return IsNull() ? false : _value_ptr->IsString();
	}

	Boolean IsList() const
	{
		return IsNull() ? false : _value_ptr->IsList();
	}

	AnyValueBase* CloneValue() const
	{
		if (IsNull())
		{
			return nullptr;
		}

		return _value_ptr->Clone();
	}

	template<typename T>
	Boolean Is() const
	{
		return Type() == typeid(T);
	}

	template<typename T>
	StorageType<T>& As() const
	{
		auto* value = dynamic_cast<AnyValue<StorageType<T>>*>(_value_ptr);
		if (value == nullptr)
		{
			throw ::bad_any_cast(Type().name());
		}

		return value->GetValue();
	}

	template<typename T>
	operator T()
	{
		return As<StorageType<T>>();
	}

	String ToString() const
	{
		if (IsBool())
		{
			return As<Boolean>() ? "true" : "false";
		}
		if (IsNumber())
		{
			return to_string(As<Number>());
		}
		if (IsString())
		{
			return As<String>();
		}
		if (IsList())
		{
			String result;
			const auto& list = As<List>();
			for (const auto& item : list)
			{
				result += item.ToString();
			}

			return result;
		}

		return EMPTY_STRING;
	}

public:
	template<typename T>
	Any& operator=(const T& value)
	{
		*this = Any(value);
		return *this;
	}

	Any& operator=(std::nullptr_t)
	{
		delete _value_ptr;

		_value_ptr = nullptr;

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

	bool operator==(const Any& rhs) const
	{
		if (IsNull() && rhs.IsNull())
		{
			return true;
		}
		if (IsBool() && rhs.IsBool())
		{
			return As<Boolean>() == rhs.As<Boolean>();
		}
		if (IsNumber() && rhs.IsNumber())
		{
			return As<Number>() == rhs.As<Number>();
		}
		if (IsString() && rhs.IsString())
		{
			return As<String>() == rhs.As<String>();
		}
		if (IsList() && rhs.IsList())
		{
			return As<List>() == rhs.As<List>();
		}

		throw no_operator("==");
	}

	bool operator!=(const Any& rhs) const
	{
		return !(*this == rhs);
	}

	bool operator>(const Any& rhs) const
	{
		return !(*this < rhs);
	}

	bool operator<(const Any& rhs) const
	{
		if (IsNumber() && rhs.IsNumber())
		{
			return As<Number>() < rhs.As<Number>();
		}
		if (IsString() && rhs.IsString())
		{
			return As<String>() < rhs.As<String>();
		}

		throw no_operator("<");
	}

	bool operator>=(const Any& rhs) const
	{
		return (*this == rhs) || (*this > rhs);
	}

	bool operator<=(const Any& rhs) const
	{
		return (*this == rhs) || (*this < rhs);
	}

	//Any operator+(const Any& rhs) const
	//{
	//	if (IsNumber() && rhs.IsNumber())
	//	{
	//		return As<Number>() + rhs.As<Number>();
	//	}
	//	if (IsString() && rhs.IsString())
	//	{
	//		return As<String>() + rhs.As<String>();
	//	}
	//	if (IsList() && rhs.IsList())
	//	{
	//		List result_list;

	//		for (const Any& item : As<List>())
	//		{
	//			result_list.push_back(item);
	//		}

	//		for (const Any& item : rhs.As<List>())
	//		{
	//			result_list.push_back(item);
	//		}

	//		return result_list;
	//	}

	//	throw no_operator();
	//}

	//Any operator-(const Any& rhs) const
	//{
	//	if (IsNumber() && rhs.IsNumber())
	//	{
	//		return As<Number>() - rhs.As<Number>();
	//	}

	//	throw no_operator();
	//}

	//Any operator*(const Any& rhs) const
	//{
	//	if (IsNumber() && rhs.IsNumber())
	//	{
	//		return As<Number>() * rhs.As<Number>();
	//	}

	//	throw no_operator();
	//}

	//Any operator/(const Any& rhs) const
	//{
	//	if (IsNumber() && rhs.IsNumber())
	//	{
	//		return As<Number>() / rhs.As<Number>();
	//	}

	//	throw no_operator();
	//}

private:
	AnyValueBase* _value_ptr;
};

#endif
