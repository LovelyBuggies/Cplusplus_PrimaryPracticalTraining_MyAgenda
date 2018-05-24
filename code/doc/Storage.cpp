
//
// 2017 SYSU Software Engineering Training Project -- Agenda
//
// Storage.cpp
// C++
//
// Created by Nino on 2017/06.
// Copyright (c) Nino 2017. All rights reserved.
//


#include "Storage.hpp"
#include "Path.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#define USER_ELEMENT 4
#define MEETING_ELEMENT 5

/**
*   instance of Storage
*/
std::shared_ptr<Storage> Storage::m_instance = nullptr;


/**
*   default constructor
*/
Storage::Storage() : m_dirty(0) { readFromFile(); }

/**
*   destructor
*/
Storage::~Storage() {
    writeToFile();
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
*   split a string into list using the given delim
*   @param t_str the source string
*   @param t_delim the delim of the string
*   @return a vector of string
*/
static std::vector<std::string> split_csv(const std::string &t_str, char t_delim) {
  std::stringstream ss(t_str);
  std::string item;
  std::vector<std::string> tokens;
  while(getline(ss, item, t_delim))
    tokens.push_back(item);
  removeQuatations(tokens);
  return tokens;
}


/**
*   convert the string to participator in a meeting 
*   @param the source string t_str
*   @return the vector string participatorList
*/
std::vector<std::string> participatorVector(std::string t_str) {
  std::stringstream ss(t_str);
  std::string item;
  std::vector<std::string> participatorList;
  while(getline(ss, item, '&')) {
  	participatorList.push_back(item);
  }
  return participatorList;
}


/**
*   read file content into memory
*   @return if success, true will be returned
*/
bool Storage::readFromFile(void) {
  std::ifstream users_ifs(Path::userPath, std::ios::in);
  if(!users_ifs.good())
    return false;
  std::string original_line;
  
  // handle users parser
  while(std::getline(users_ifs, original_line)) {
  	// remove the first and last character
    if(!original_line.length())
      continue;
    std::string filtted_line(original_line.begin()+1, 
    						original_line.begin()+original_line.length()-1);
    std::vector<std::string> elements = split_csv(filtted_line, ',');
    if(elements.size() != USER_ELEMENT)
      continue;
    m_userList.push_back({elements[0], elements[1], elements[2], elements[3]});
  }
  users_ifs.close();

  // handle meetings parser
  std::ifstream meetings_ifs(Path::userPath, std::ios::in);
  if(!meetings_ifs.good())
  	return false;
  while(std::getline(users_ifs, original_line)) {
    if(!original_line.length())
      continue;
    std::string filtted_line(original_line.begin()+1, 
    						original_line.begin()+original_line.length()-1);
    std::vector<std::string> elements = split_csv(filtted_line, ',');
    if(elements.size() != MEETING_ELEMENT)
      continue;
  	Meeting temp(elements[0], participatorVector(elements[1]), elements[2], elements[3], elements[4]);
    m_meetingList.push_back(temp);
  }
  meetings_ifs.close();
  return true;
}


/**
*   join factors to a string
*   @param t_factors the string factors
*   @return a csv string
*/
std::string fabricate_csv(std::list<std::string> t_elements) {
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
*   convert the participator in a meeting to string
*   @param the source participatorList
*   @return the formatted string of paticipators
*/
std::string participatorString(std::vector<std::string> participatorList) {
  std::string t_str = "";
  bool first = true;
  for(auto it = participatorList.begin(); it != participatorList.end(); ++it) {
    if(first)
      first = false;
    else
      t_str += '&';
    t_str += *it;
  }
  return t_str;
}


/**
*   write file content from memory
*   @return if success, true will be returned
*/
bool Storage::writeToFile(void) {
  if(!m_dirty)
    return false;
	// handle users readfile
  std::ofstream users_ofs(Path::userPath, std::ios::out);
  for(User &each : m_userList) {
    users_ofs << fabricate_csv({each.getName(), each.getPassword(),
                              each.getEmail(), each.getPhone()}) << std::endl;
  }
  users_ofs.close();

  // handle meetings readfile
  std::ofstream meetings_ofs(Path::meetingPath, std::ios::out);
  for(Meeting &each : m_meetingList) {
    std::string participators = participatorString(each.getParticipator());
    meetings_ofs << fabricate_csv({each.getSponsor(), participators,
                                  Date::dateToString(each.getStartDate()),
                                  Date::dateToString(each.getEndDate()),
                                  each.getTitle()}) << std::endl;
  }
  meetings_ofs.close();

  m_dirty = false;
  return true;
}


/**
* get Instance of storage
* @return the pointer of the instance
*/
std::shared_ptr<Storage> Storage::getInstance(void) {
  if(m_instance == nullptr)
    m_instance = std::shared_ptr<Storage>(new Storage);
  return m_instance;
}



/**
* create a user
* @param a user object
*/
void Storage::createUser(const User &t_user) {
  m_userList.push_back(t_user);
  m_dirty = true;
  sync();
}


/**
* query users
* @param a lambda function as the filter
* @return a list of fitted users
*/
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
  std::list<User> filtted_userList;
  for(auto &each : m_userList)
     if(filter(each))
      filtted_userList.push_back(each);
  return filtted_userList;
}


/**
* update users
* @param a lambda function as the filter
* @param a lambda function as the method to update the user
* @return the number of updated users
*/
int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher) {
  int index = 0;
  for(auto &each : m_userList)
    if(filter(each)) {
      switcher(each);
      index++;
    }
   if(index != 0) {
   	m_dirty = true;
   	sync();
   }
   return index;
}
  

/**
* delete users
* @param a lambda function as the filter
* @return the number of deleted users
*/
int Storage::deleteUser(std::function<bool(const User &)> filter) {
  int index = 0;
  for(auto it = m_userList.begin(); it != m_userList.end(); ++it) {
    if(filter(*it)) {
      it = m_userList.erase(it);
      index++;
      it--;
    }
  }
  if(index != 0) {
   	m_dirty = true;
   	sync();
   }
  return index;
}

  
/**
* create a meeting
* @param a meeting object
*/
void Storage::createMeeting(const Meeting &t_meeting) {
  m_meetingList.push_back(t_meeting);
  m_dirty = true;
  sync();
}


/**
* query meetings
* @param a lambda function as the filter
* @return a list of fitted meetings
*/
std::list<Meeting> Storage::queryMeeting(
      std::function<bool(const Meeting &)> filter) const {
  std::list<Meeting> filtted_meetingList;
  for(auto &each : m_meetingList)
    if(filter(each))
      filtted_meetingList.push_back(each);
  return filtted_meetingList;
}


/**
* update meetings
* @param a lambda function as the filter
* @param a lambda function as the method to update the meeting
* @return the number of updated meetings
*/
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher) {
  int index = 0;
  for(auto &each : m_meetingList)
    if(filter(each)) {
      switcher(each);
      index++;
    }
   if(index != 0) {
   	m_dirty = true;
   	sync();
   }
   return index;
}

/**
* delete meetings
* @param a lambda function as the filter
* @return the number of deleted meetings
*/
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
  int index = 0;
  for(auto it = m_meetingList.begin(); it != m_meetingList.end(); ++it)
    if(filter(*it)) {
      it = m_meetingList.erase(it);
      index++;
      it--;
    }
   if(index != 0) {
   	m_dirty = true;
   	sync();
   }
   return index;
}


/**
* sync with the file
*/
bool Storage::sync(void) {
  if(m_dirty)
    return writeToFile();
else
	return false;
}
