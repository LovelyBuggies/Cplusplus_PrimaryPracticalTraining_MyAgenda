
//
// 2017 SYSU Software Engineering Training Project -- Agenda
//
// History.cpp
// C++
//
// Created by Nino on 2017/06.
// Copyright (c) Nino 2017. All rights reserved.
//


#include "History.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


std::shared_ptr<History> History::m_instance = nullptr;

/**
*   default constructor
*/
History::History() : m_dirty(0) { readFile(); }

/**
*   destructor
*/
History::~History() { writeFile(); }

/**
* get Instance of storage
* @return the pointer of the instance
*/
std::shared_ptr<History> History::Instance(void) {
  if(m_instance == nullptr)
    m_instance = std::shared_ptr<History>(new History);
  return m_instance;
}

/**
*   remove the quatations in the tokens
*   @param t_tokens the source tokens
*/
static void removeQuatations(std::vector<std::string> &t_tokens) {
  for(auto it = t_tokens.begin(); it != t_tokens.end(); ++it) {
    if((*it)[0] == '\"')
      it->erase(it->begin());
    if((*it)[it->length()-1] == '\"')
      it->erase(it->length()-1);
  }
}


/**
*   spl a string into list using the given delim
*   @param t_str the source string
*   @param t_delim the delim of the string
*   @return a vector of string
*/
static std::vector<std::string> spl_csv(const std::string &t_str, char t_delim) {
  std::stringstream ss(t_str);
  std::string item;
  std::vector<std::string> tokens;
  while(getline(ss, item, t_delim))
    tokens.push_back(item);
  removeQuatations(tokens);
  return tokens;
}


/**
*   read file content into memory
*   @return if success, true will be returned
*/
bool History::readFile(void) {
  std::ifstream his_ifs("tmp/history.csv", std::ios::in);
  if(!his_ifs.good())
    return false;
  std::string original_line;
  
  // handle history parser
  while(std::getline(his_ifs, original_line)) {
  	// remove the first and last character
    if(!original_line.length())
      continue;
    std::string filtted_line(original_line.begin()+1, 
    						original_line.begin()+original_line.length()-1);
    std::vector<std::string> elements = spl_csv(filtted_line, ',');
    if(elements.size() != 2)
      continue;
    m_operationList.push_back({elements[0], elements[1]});
  }
  his_ifs.close();
  return true;
}


/**
*   join factors to a string
*   @param t_factors the string factors
*   @return a csv string
*/
std::string fab_csv(std::list<std::string> t_elements) {
  std::string ret;
  bool first = true;
  for(std::string &each : t_elements) {
    if(!first) {
      ret += ',';
    }
    ret += '\"';
    ret += each;
    ret += '\"';
    first = false;
  }
  return ret;
}

/**
*   write file content from memory
*   @return if success, true will be returned
*/
bool History::writeFile(void) {
  if(!m_dirty)
    return false;
	// handle users readfile
  std::ofstream his_ofs("tmp/history.csv", std::ios::out);
  for(auto &each : m_operationList) {
    his_ofs << fab_csv({each.getOp(), each.getTime()}) << std::endl;
  }
  his_ofs.close();

  m_dirty = false;
  return true;
}



/**
* create a user
* @param a user object
*/
void History::addHistory(const Operation &t_history) {
  m_operationList.push_back(t_history);
  m_dirty = true;
  sync();
}

/**
* sync with the file
*/
bool History::sync(void) {
  if(m_dirty)
    return writeFile();
else
	return false;
}
