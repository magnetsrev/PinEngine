//#pragma once
//#ifndef GenerateEnumClassBitwiseOverloads
//#define GenerateEnumClassBitwiseOverloads(CLASSNAME) \
//CLASSNAME operator |(CLASSNAME lhs, CLASSNAME rhs)  \
//{ \
//return static_cast<CLASSNAME> ( \
//	static_cast<std::underlying_type<CLASSNAME>::type>(lhs) | \
//	static_cast<std::underlying_type<CLASSNAME>::type>(rhs) \
//	); \
//} \
//CLASSNAME operator &(CLASSNAME lhs, CLASSNAME rhs)  \
//{ \
//return static_cast<CLASSNAME> ( \
//	static_cast<std::underlying_type<CLASSNAME>::type>(lhs)& \
//	static_cast<std::underlying_type<CLASSNAME>::type>(rhs) \
//	); \
//} 
//#endif