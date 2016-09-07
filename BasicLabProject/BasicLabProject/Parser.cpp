#include <iterator>
#include <algorithm> 
#include <stack>
#include <string>
#include <fstream>
#include "Parser.h"

std::random_device rd;
std::mt19937 mt(rd());

Parser::Parser(std::string rulesFile) {
	this->fileName = rulesFile;
}

std::vector<std::string> Parser::readLines(std::string rulesFile) {
	std::vector<std::string> linesFromFile;
	std::ifstream dict_file(rulesFile);
	std::string line;
	while (std::getline(dict_file, line))
		linesFromFile.push_back(line);
	return linesFromFile;
}

std::vector<std::string> Parser::splitString(std::string str, char delimeter, char delimeter2) {
	std::vector<std::string> words;
	//split string
	unsigned int last_pos = 0;
	for (unsigned int i = 0; i < str.length(); i++)
		if (str[i] == delimeter || str[i] == delimeter2) {
			words.push_back(str.substr(last_pos, i - last_pos));
			last_pos = i + 1;
		}
	if (last_pos != str.length())
		words.push_back(str.substr(last_pos, str.length() - last_pos));
	return words;
}

Type Parser::stringToType(std::string str) {
	if (!str.compare("cube"))
		return CUBE;
	else if (!str.compare("cylinder"))
		return CYLINDER;
	else if (!str.compare("scope"))
		return SCOPE;
	else throw "Can't parse string to type.";
}

std::function<std::vector<Shape>(Shape)> Parser::stringToRule(std::string string){
	auto tokens = splitString(string, ' ', ' '); // oprations to be performed
	std::string sourceScope = tokens[0]; //shape the rule will be applied
	std::vector<Shape> *result = new std::vector<Shape>() ; //shapes being return to build tree
	std::stack<Shape>  *processing = new std::stack<Shape>(); //shape being processed
	std::function<void(Shape)> rule = [=](Shape x) { //initial rule
			(*processing).push(x);
	};

	for (int i = 2; i < tokens.size(); i++) { // i=2 skipping sourceScope and '-'
		
		if (!tokens[i].find("T")) { //Scope translation
			auto funs = parseArguments(tokens[i]);
		 	rule = [=](Shape x) {
				rule(x);
				Shape& currentShape = (*processing).top();
				currentShape.translate(Vector3D(funs[0](), funs[1](), funs[2]()));
			};
		}

		else if (!tokens[i].find("Subdiv")) {
			auto args = parseArguments(tokens[i]);
			auto parameters = splitString(splitString(tokens[i], '{', '}')[1], ',', ',');
			int axis = round(args[0]());
			rule = [=](Shape x) {
				rule(x);
				Shape& currentShape = (*processing).top();
				std::vector<float> ratios;
				for (int i = 1; i < args.size(); ratios.push_back(args[i++]())); //parsion ratios
				auto newShapes = currentShape.split(axis, ratios, parameters);
				std::copy(newShapes.begin(), newShapes.end(), std::back_inserter((*result))); //save new shape to result
			};
		}

		else if (!tokens[i].find("S")) { // Set new scope size
			 auto funs = parseArguments(tokens[i]);
			rule = [=](Shape x) {
				rule(x);
				Shape& currentShape = (*processing).top();
				currentShape.setSize(Vector3D(funs[0](), funs[1](), funs[2]()));
			};
		}

		else if (!tokens[i].find("[")) { // push current scope to the stack
			rule = [=](Shape x) {
				rule(x);
				(*processing).push(x);
				(*processing).push(Shape(x.getName(),x.getScopePosition(), x.getSize(), x.getType()));
			};
		}

		else if (!tokens[i].find("]")) { // // pop current scope from the stack
			rule = [=](Shape x) {
				rule(x);
				(*processing).pop();
			};
		}

		else if (!tokens[i].find("I")) { // Instantiate a figure to draw
			auto type = splitString(tokens[i], '(', ')')[1];
			rule = [=](Shape x) {
				rule(x);
				Shape& currentShape = (*processing).top();
				currentShape.setName(type);
				currentShape.setType(stringToType(type));
				(*result).push_back(currentShape);
			};
		}

		else if (!tokens[i].find("Repeat")) { 
			auto arguments = splitString(splitString(tokens[i], '(', ')')[1], ',', ',');
			auto parameters = splitString(splitString(tokens[i], '{', '}')[1], ',', ',');
			rule = [=](Shape x) {
				rule(x);
				Shape& currentShape = (*processing).top();
				auto newShapes = currentShape.repeat(std::stoi(arguments[0]), std::stoi(arguments[1]), parameters[0]);
				std::copy(newShapes.begin(), newShapes.end(), std::back_inserter((*result)));
			};
		}
	}
	return [=](Shape shape) {
		(*result).clear();
		if(sourceScope.compare(shape.getName())==0)
			rule(shape);
		return (*result);};
}

std::vector<std::function<float()>> Parser::parseArguments(std::string token)
{
	auto args = splitString(splitString(token, '(', ')')[1], ',', ',');
	std::vector<std::function<float()>> funs;
	for (int k = 0; k < args.size(); k++) {
		if (!args[k].find("rnd")) {
			auto rArgs = splitString(splitString(args[k], '<', '>')[1], '-', '-');
			std::uniform_real_distribution<double> dist(std::stof(rArgs[0]), std::stof(rArgs[1]));
			funs.push_back([=] { return dist(mt); });
		}
		else funs.push_back([=] { return std::stof(args[k]); });
	}
	return funs;
}

std::vector<std::function<std::vector<Shape>(Shape)>> Parser::parseRules() {
	std::vector<std::function<std::vector<Shape>(Shape)>> rules;
	auto lines = readLines(this->fileName);
	for each (auto line in lines)
		rules.push_back(stringToRule(line));
	return rules;
}
