//
// Created by Katsu Matsuda on 2020/04/21.
//

#ifndef _OBJ_HPP_
#define _OBJ_HPP_

#include <string>
#include <memory>

namespace xylo::vm
{
enum ObjType
{
	INT,
	FLOAT,
	STRING,
};

struct Obj
{
 private:
	union ObjValue
	{
		int64_t ival;

		double dval;

		std::unique_ptr<std::string> str;

		ObjValue()
			: ival(0)
		{
		}

		~ObjValue()
		{

		}
	};

	ObjType type = INT;

	ObjValue value;

	inline void Release()
	{
		if (type == STRING)
			value.str.reset();
	}

 public:
	Obj()
	{
		Set((int64_t)0);
	}

	Obj(const Obj& obj)
	{
		type = obj.type;
		switch (obj.type)
		{
		case INT:
			Set(obj.value.ival);
			break;
		case FLOAT:
			Set(obj.value.dval);
			break;
		case STRING:
			Set(*obj.value.str);
			break;
		}
	}

	explicit Obj(int64_t ival)
	{
		Set(ival);
	}

	explicit Obj(double dval)
	{
		Set(dval);
	}

	explicit Obj(std::string str)
	{
		Set(std::move(str));
	}

	inline void Set(int64_t ival)
	{
		Release();
		type = INT;
		value.ival = ival;
	}

	inline void Set(double dval)
	{
		Release();
		type = FLOAT;
		value.dval = dval;
	}

	inline void Set(std::string str)
	{
		Release();
		type = STRING;
		value.str = std::make_unique<std::string>(std::move(str));
	}

	inline ObjType GetType()
	{
		return type;
	}

	inline int64_t GetInt()
	{
		switch (type)
		{
		case INT:
			return value.ival;
		case FLOAT:
			return (int64_t)value.dval;
		case STRING:
			return strtoll(value.str->c_str(), nullptr, 10);
		}
	}

	inline double GetFloat()
	{
		switch (type)
		{
		case INT:
			return (double)value.ival;
		case FLOAT:
			return value.dval;
		case STRING:
			return strtod(value.str->c_str(), nullptr);
		}
	}

	inline std::string GetString()
	{
		switch (type)
		{
		case INT:
			return std::to_string(value.ival);
		case FLOAT:
			return std::to_string(value.dval);
		case STRING:
			return *value.str;
		}
	}

	bool operator<(const Obj& obj) const
	{
		switch (type)
		{
		case INT:
			return std::tie(type, value.ival) < std::tie(obj.type, obj.value.ival);
		case FLOAT:
			return std::tie(type, value.dval) < std::tie(obj.type, obj.value.dval);
		case STRING:
			return std::tie(type, value.str) < std::tie(obj.type, obj.value.str);
		}
	}

	Obj& operator=(Obj obj)
	{
		Release();
		type = obj.type;
		switch (type)
		{
		case INT:
			value.ival = obj.GetInt();
			break;
		case FLOAT:
			value.dval = obj.GetFloat();
			break;
		case STRING:
			value.str = std::make_unique<std::string>(obj.GetString());
			break;
		}

		return *this;
	}
};
}

#endif //_OBJ_HPP_
