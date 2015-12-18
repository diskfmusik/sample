#include <sstream>
#include <fstream>
#include <iostream>
#include "picojson.h"


// エラーが起きなければ 0 を返す
int load_json(const char* _fileName)
{
	using namespace std;

	stringstream ss;
	ifstream f;


	f.open(_fileName, ios::binary);
	if (!f.is_open())
	{
		return 1;
	}
	ss << f.rdbuf();
	f.close();


	picojson::value val;
	ss >> val;
	string err = picojson::get_last_error();
	if (!err.empty())
	{
		cerr << err << endl;
		return -1;
	}

	// "object" : { }
	// "array" : [ ]


	// l----------------------------------
	// オブジェクト
	picojson::object& obj = val.get<picojson::object>();

	// l--------------------------------------------
	// "joblist" : [ ]  <-  joblistという配列
	picojson::array& joblist = obj["joblist"].get<picojson::array>();

	for (picojson::array::iterator it = joblist.begin(); it != joblist.end(); it++)
	{
		picojson::object& job = it->get<picojson::object>();

		job["name"].get<string>();
		job["number"].get<double>();
		job["move"].get<double>();
		job["hp"].get<double>();
		job["hpMax"].get<double>();
		job["atk"].get<double>();
		job["magicAtk"].get<double>();
		job["skill"].get<double>();
		job["spd"].get<double>();
		job["def"].get<double>();
		job["magicDef"].get<double>();

	}

	return 0;
}


int main()
{
	if (load_json("test.json"))return 0;

	return 0;
}