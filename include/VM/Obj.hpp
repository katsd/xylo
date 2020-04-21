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
		uint64_t ival;

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
			value.str.release();
	}

 public:
	Obj()
	{
		Set((uint64_t)0);
	}

	explicit Obj(uint64_t ival)
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

	inline void Set(uint64_t ival)
	{
		Release();
		type = ObjType::INT;
		value.ival = ival;
	}

	inline void Set(double dval)
	{
		Release();
		type = ObjType::FLOAT;
		value.dval = dval;
	}

	inline void Set(std::string str)
	{
		Release();
		type = ObjType::STRING;
		value.str = std::make_unique<std::string>(std::move(str));
	}
};
}

#endif //_OBJ_HPP_
