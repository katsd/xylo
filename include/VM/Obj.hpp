//
// Created by Katsu Matsuda on 2020/04/21.
//

#ifndef _OBJ_HPP_
#define _OBJ_HPP_

#include <string>
#include <memory>
#include <cmath>

namespace xylo::vm
{
enum ObjType
{
	INST,
	INT,
	FLOAT,
	STRING,
};

struct Obj
{
 private:
	union ObjValue
	{
		Inst inst;

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
		case INST:
			Set(obj.value.inst);
			break;
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

	explicit Obj(Inst inst)
	{
		Set(inst);
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

	inline void Set(Inst inst)
	{
		Release();
		type = INST;
		value.inst = inst;
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

	[[nodiscard]] inline ObjType GetType() const
	{
		return type;
	}

	[[nodiscard]] inline Inst GetInst() const
	{
		if (type == ObjType::INST)
			return value.inst;

		return Inst::DO_NOTHING;
	}

	[[nodiscard]] inline int64_t GetInt() const
	{
		switch (type)
		{
		case INST:
			return 0;
		case INT:
			return value.ival;
		case FLOAT:
			return (int64_t)value.dval;
		case STRING:
			return strtoll(value.str->c_str(), nullptr, 10);
		}
	}

	[[nodiscard]] inline double GetFloat() const
	{
		switch (type)
		{
		case INST:
			return 0;
		case INT:
			return (double)value.ival;
		case FLOAT:
			return value.dval;
		case STRING:
			return strtod(value.str->c_str(), nullptr);
		}
	}

	[[nodiscard]] inline std::string GetString() const
	{
		switch (type)
		{
		case INST:
			return "";
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
		case INST:
			return std::tie(type, value.inst) < std::tie(obj.type, obj.value.inst);
		case INT:
			return std::tie(type, value.ival) < std::tie(obj.type, obj.value.ival);
		case FLOAT:
			return std::tie(type, value.dval) < std::tie(obj.type, obj.value.dval);
		case STRING:
			return std::tie(type, *value.str) < std::tie(obj.type, *obj.value.str);
		}
	}

	bool operator==(const Obj& obj) const
	{
		auto l_type = GetType();
		auto r_type = obj.GetType();

		if (l_type == r_type)
		{
			switch (l_type)
			{
			case INST:
				return GetInst() == obj.GetInst();
			case INT:
				return GetInt() == obj.GetInt();
			case FLOAT:
				return GetFloat() == obj.GetFloat();
			case STRING:
				return GetString() == obj.GetString();
			}
		}

		if (l_type == ObjType::STRING || r_type == ObjType::STRING)
			return GetString() == obj.GetString();

		return abs(GetFloat() - obj.GetFloat()) < 1e-15;
	}

	bool operator!=(const Obj& obj) const
	{
		return !(this == &obj);
	}

	Obj& operator=(Obj obj)
	{
		Release();
		type = obj.type;
		switch (type)
		{
		case INST:
			value.inst = obj.GetInst();
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
