#pragma once
#include<iostream>
#include<vector>
#include<functional>
#include"Shape.h"


class Parser {
	std::string fileName; // file with rules
	std::vector<std::string> readLines(); // read lines from file
	std::vector<std::string> splitString(std::string str, char delimeter, char delimeter2); 
	Type stringToType(std::string str);
	std::function<std::vector<Shape>(Shape)> stringToRule(std::string string); 
public:
	Parser(std::string rulesFile);
	std::vector<std::function<std::vector<Shape>(Shape)>> parseRules(); 
};

