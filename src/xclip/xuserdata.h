#pragma once

#ifndef _XUSERDATA_H_
#define _XUSERDATA_H_

/*

	Store associative array in a string, which can be strored in xclip file header...

	USAGE :	store values

	XUserData uData;
	uData.setVariable("modelName", "umalawi");
	userDataString = uData.buildString();

	USAGE :	retrieve values

	XUserData uData;
	uData.buildMap(userDataString);
	string modelNameVar = getVariable(" modelName");
*/
//#include "xnode.h"
#include "stringutils.h"

#include <map>


class XUserData
{
public:
	XUserData(){}
	~XUserData(){}
	/*
	void addVariable(string name, string value)
	{
		debugPrint("");
		//variables.insert(pair<string,string>("MODEL","umalawi"));
//		variables.insert(pair<string,string>(name, value));
		variables[name]=value;
	}
	*/
	std::string getVariable(std::string name)
	{
		//debugPrint("");
		if(variables.find(name) == variables.end())
			return "";
		return variables[name];
	}
	float getFloatVariable(std::string name)
	{
		float valueF;
		std::stringstream valueSs;
		valueSs << getVariable(name);
		//debugPrint( valueSs.str() );
		if( (valueSs >> valueF).fail() )
			debugPrint( "float conversion failed! " );
		else
			return valueF;
		return 0.0f;
	}
	int getIntVariable(std::string name)
	{
		int valueI;
		std::stringstream valueSs;
		valueSs << getVariable(name);
		//debugPrint( valueSs.str() );
		if( (valueSs >> valueI).fail() )
			debugPrint( "int conversion failed! " );
		else
			return valueI;
		return 0;
	}
	void setVariable(std::string name, std::string value)
	{
		variables[name]=value;
	}
	void setFloatVariable(std::string name, float value)
	{
		std::stringstream valueSs;
		valueSs << value;
		setVariable(name, valueSs.str() );
	}
	void setIntVariable(std::string name, int value)
	{
		std::stringstream valueSs;
		valueSs << value;
		setVariable(name, valueSs.str() );
	}
	std::string buildString()
	{
		std::stringstream ss;
		for(std::map<std::string, std::string>::iterator ii=variables.begin(); ii!=variables.end(); ++ii)
		{
			ss << (*ii).first << "=" << (*ii).second << std::endl;
		}
		std::string result=ss.str();
		//debugPrint("\n" + result);
		return result;
	}
	void buildMap(std::string str)
	{
		//debugPrint("\n" + str);
		if (str == "") {
			return;
		}
		std::stringstream ss;
		std::vector<std::string> tmpVec;
		string_utils::tokenize(str, tmpVec, "\n");
		//debugPrint(std::to_string(tmpVec.size()));
		for(int i=0; i<tmpVec.size(); i++)
		{
			std::vector<std::string> tmpVec2;
			string_utils::tokenize(tmpVec.at(i), tmpVec2, "=");
			if(tmpVec2.size()==2)
			{
				std::string name = tmpVec2.at(0);
				std::string value = tmpVec2.at(1);
				if( name.length()>0 && value.length()>0 )
					setVariable(name, value);
				else
					debugPrint("skipping empty name/value...");
			}else{
				debugPrint("user data format error ...");
			}
			//variables.insert(pair<string,string>(tmpVec.at(i), tmpVec.at(i+1)));
		}
	}
private:
	std::map<std::string, std::string> variables;
};

#endif
