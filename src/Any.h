#pragma once
#ifndef _H_ANY_H_
#define _H_ANY_H_


template<typename T>
using StorageType = typename decay<typename remove_reference<T>::type>::type;

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
		//virtual string ToString() = 0;
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

		const std::type_info& Type() const override
		{
			return typeid(T);
		}

		//string ToString() override
		//{
		//	return string(Type().name()) + ": " + to_string(_value);
		//}

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

	template<typename T>
	operator T()
	{
		return As<T>();
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
	bool Is() const
	{
		return Type() == typeid(T);
	}

	//template<typename T>
	//typename std::enable_if<std::is_same<T, bool>::value, bool>::type
	//As() const
	//{
	//	AnyValue<bool>* value = dynamic_cast<AnyValue<bool>*>(_value_ptr);
	//	if (value == nullptr)
	//	{
	//		//throw std::bad_cast();
	//		return false;
	//	}

	//	return value->GetValue();
	//}

	//template<typename T>
	//typename std::enable_if<std::is_same<T, string>::value, string>::type
	//	As() const
	//{
	//	//auto& type_id = typeid(_value_ptr);
	//	//auto& target_type_id = typeid(AnyValue<string>*);

	//	//if (type_id != target_type_id)
	//	//{
	//	//	return string();
	//	//}

	//	Any::AnyValue<string>* value = static_cast<Any::AnyValue<string>*>(_value_ptr);
	//	if (value == nullptr)
	//	{
	//		//throw std::bad_cast();
	//		return string();
	//	}

	//	return value->GetValue();
	//}

	//template<typename T>
	//typename std::enable_if<std::is_same<T, double>::value, double>::type
	//	As() const
	//{
	//	AnyValue<double>* value = dynamic_cast<AnyValue<double>*>(_value_ptr);
	//	if (value == nullptr)
	//	{
	//		//throw std::bad_cast();
	//		return 0.0;
	//	}

	//	return value->GetValue();
	//}

	//template<typename T>
	//typename std::enable_if<std::is_same<T, ScriptFunction>::value, ScriptFunction>::type
	//	As() const
	//{
	//	AnyValue<ScriptFunction>* value = dynamic_cast<AnyValue<ScriptFunction>*>(_value_ptr);
	//	if (value == nullptr)
	//	{
	//		throw std::bad_cast();
	//	}

	//	return value->GetValue();
	//}

	template<typename T>
	StorageType<T>& As() const
	{
		if (IsNull() || Type() != typeid(T))
		{
			throw std::bad_cast();
		}

		auto* value = dynamic_cast<AnyValue<StorageType<T>>*>(_value_ptr);
		if (value == nullptr)
		{
			throw std::bad_cast();
		}

		return value->GetValue();
	}

	static Any& EmptyValue()
	{
		static Any empty_value;
		return empty_value;
	}

	bool IsNull() const
	{
		return _value_ptr == nullptr;
	}

	const std::type_info& Type() const
	{
		if (IsNull())
		{
			return typeid(nullptr);
		}

		return _value_ptr->Type();
	}

	//string toString() const
	//{
	//	if (IsNull())
	//	{
	//		return EMPTY_STRING;
	//	}

	//	return std::move(_value_ptr->ToString());
	//}

private:
	AnyValueBase* _value_ptr;
};

#endif
