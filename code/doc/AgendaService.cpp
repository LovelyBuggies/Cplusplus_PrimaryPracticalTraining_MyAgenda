
//
// 2017 SYSU Software Engineering Training Project -- Agenda
//
// Service.cpp
// C++
//
// Created by Nino on 2017/06.
// Copyright (c) Nino 2017. All rights reserved.
//


#include "AgendaService.hpp"
#include "User.hpp"
#include "Meeting.hpp"
#include "Date.hpp"
#include <list>
#include <string>
#include <vector>

using namespace std;


/**
* constructor
*/
AgendaService::AgendaService() { startAgenda(); }


/**
* destructor
*/
AgendaService::~AgendaService() { quitAgenda(); }


/**
* start Agenda service and connect to storage
*/
void AgendaService::startAgenda(void) { m_storage = Storage::getInstance(); }


/**
* quit Agenda service
*/
void AgendaService::quitAgenda(void) { m_storage->sync(); }

/**
* check if the username match password
* @param userName the username want to login
* @param password the password user enter
* @return if success, true will be returned
*/
bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
  std::list<User> users;
  users = m_storage->queryUser([&userName, &password](const User &member) {
    if(member.getName() == userName && member.getPassword() == password)
      		return true;
    return false;
  });
  return (users.empty())? false : true;
}


/**
* regist a user
* @param userName new user's username
* @param password new user's password
* @param email new user's email
* @param phone new user's phone
* @return if success, true will be returned
*/
  bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                    				const std::string &email, const std::string &phone) {
  	std::list<User> users;
  	users = m_storage->queryUser([&userName](const User &member) {
    if(member.getName() == userName)
      return true;
    else 
      return false;
  	});
  	if(!users.empty())
  		return false;
  	User temp(userName, password, email, phone);
  	m_storage->createUser(temp);
  	return true;
}


/**
* delete a user
* @param userName user's username
* @param password user's password
* @return if success, true will be returned
*/
bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
  	int index;

  	// delete the user
  	index = m_storage->deleteUser([&userName, &password](const User &member) {
      if(member.getName() == userName && member.getPassword() == password)
        return true;
      else
        return false;
    });
    if(index == 0) return false;

  	// delete the meetings whose sponsor is userName or only one participator
  	m_storage->deleteMeeting([&userName](const Meeting &member) {
    if(member.getSponsor() == userName)
    	return true;
    if(member.isParticipator(userName) && member.getParticipator().size() == 1)
    	return true;
    else 
      return false;
  	});
    
    // remove participator in the meeting
    m_storage->updateMeeting([&userName](const Meeting &member) {
    if(member.isParticipator(userName) && member.getParticipator().size() > 1)
      return true;
    else 
      return false;
    },[&userName](Meeting &member) {
    member.removeParticipator(userName); 
  });
  	
  	return true;
}


/**
* list all users from storage
* @return a user list result
*/
std::list<User> AgendaService::listAllUsers(void) const {
	std::list<User> users;
	users = m_storage->queryUser([](const User& member){ return true; });
	return users;
}


/**
* create a meeting
* @param userName the sponsor's userName
* @param title the meeting's title
* @param participator the meeting's participator
* @param startData the meeting's start date
* @param endData the meeting's end date
* @return if success, true will be returned
*/
bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     				const std::string &startDate, const std::string &endDate,
                     				const std::vector<std::string> &participator) {
	// data invalid
	if(title == "" || userName == "") return false;

	// throw duplicate participators
	for(auto it = participator.begin(); it != participator.end(); ++it)
		for(auto i = participator.begin(); i != it; ++i)
			if((*it) == (*i)) 
				return false;

	// throw date error
	Date stime = Date::stringToDate(startDate);
	Date etime = Date::stringToDate(endDate);

  if(stime.getYear() == 0 && stime.getMonth() == 0 && stime.getDay() == 0
      && stime.getHour() == 0 && stime.getMinute() == 0) return false;
  if(etime.getYear() == 0 && etime.getMonth() == 0 && etime.getDay() == 0
      && etime.getHour() == 0 && etime.getMinute() == 0) return false;

  
	if(!Date::isValid(stime) || !Date::isValid(etime) || stime >= etime)
		return false;


	std::list<User> users;
	std::list<Meeting> meetings;
	for(auto it = participator.begin(); it != participator.end(); it++) {

		// sponsor cannot be participator
    	if(*it == userName) 
    		return false;

    	std::string parname = *it;
    	if(parname == "") 
    		return false;
    	// whether participator have account
    	users = m_storage->queryUser([&parname](const User &member) {
    		if(member.getName() == parname) 
    			return true;
    		else 
    			return false;
    	});
    	if(users.empty()) 
    		return false;

    	// whether the participators are ocupied
    	meetings = m_storage->queryMeeting([&parname, &stime, &etime](const Meeting &member) {
    		if(member.getSponsor() == parname || member.isParticipator(parname)) {
    			if(member.getStartDate() > etime || member.getEndDate() < stime) 
    				return false;
    			return true;
    		}
    		else 
    			return false;
    	});
    	if(!meetings.empty()) 
    		return false;
    }

    // whether sponser have account
    users = m_storage->queryUser([&userName](const User &member) {
    	if(member.getName() == userName)
    		return true;
    	else 
    		return false;
    });
    if(users.empty())
    	return false;

    // whether the sponser is ocupied
    meetings = m_storage->queryMeeting([&userName, &stime, &etime](const Meeting &member){
    	if(member.getSponsor() == userName || member.isParticipator(userName)) {
    		if(member.getStartDate() > etime || member.getEndDate() < stime)
    			return false;
    		else 
    			return true;
    	}
    	else 
    		return false;
    });
    if(!meetings.empty()) 
    	return false;
    
    // whether the title have existed
    meetings = m_storage->queryMeeting([&title](const Meeting &member) {
    	if(member.getTitle() == title)
    		return true;
    	else 
    		return false;
    });
    if(!meetings.empty())
    	return false;

    Meeting temp(userName, participator, stime, etime, title);
    m_storage->createMeeting(temp);
    return true;
}

/**
* add a participator to a meeting
* @param userName the sponsor's userName
* @param title the meeting's title
* @param participator the meeting's participator
* @return if success, true will be returned
*/
bool AgendaService::addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator) {
  std::list<User> users;
  std::list<Meeting> meetings;

  // whether the adding participator is ocupied
  Date stime = (meetings.begin())->getStartDate();
  Date etime = (meetings.begin())->getEndDate();
  meetings = m_storage->queryMeeting([&participator, &stime, &etime](const Meeting &member) {
    	if(member.getSponsor() == participator || member.isParticipator(participator)) {
    		if(member.getStartDate() > etime || member.getEndDate() < stime) 
    			return false;
    		return true;
    	}
    	else 
    		return false;
    });
  if(!meetings.empty()) 
    return false;

  int index;
  index = m_storage->updateMeeting([&userName, &title](const Meeting &member) {
    if(member.getTitle() == title && member.getSponsor() == userName)
      if(!member.isParticipator(userName))
        return true;
    return false;
  },[&userName](Meeting &member) {
    std::vector<std::string> p = member.getParticipator();
    p.push_back(userName);
    member.setParticipator(p);
  });
  if(index == 0)
  	return false;
  else
  	return true;
}

/**
* remove a participator from a meeting
* @param userName the sponsor's userName
* @param title the meeting's title
* @param participator the meeting's participator
* @return if success, true will be returned
*/
bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                              const std::string &title,
                                              const std::string &participator) {
  std::list<User> users;

  int index;
  index = m_storage->deleteMeeting([&userName, &participator, &title](const Meeting &member) {
    if(member.getTitle() == title && member.getSponsor() == userName)
      if(member.isParticipator(participator) && member.getParticipator().size() == 1)
        return true;
    return false;
  });

  index += m_storage->updateMeeting([&userName, &participator, &title](const Meeting &member) {
    if(member.getTitle() == title && member.getSponsor() == userName)
      if(member.isParticipator(participator) && member.getParticipator().size() > 1)
        return true;
    return false;
  },[&participator](Meeting &member) {
    member.removeParticipator(participator);
  });
  if(index == 0) 
  	return false;
  return true;
}

/**
* quit from a meeting
* @param userName the current userName. need to be the participator (a sponsor can not quit his/her meeting)
* @param title the meeting's title
* @return if success, true will be returned
*/
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
  int index;

  // userName is sponsor
  index = m_storage->deleteMeeting([&userName, &title](const Meeting &member){
    if(member.getSponsor() == userName || member.getTitle() == title)
        return true;
    else
      return false;
  });
  if(index != 0) return false;

  // userName is the only paticipator
  index = m_storage->deleteMeeting([&userName, &title](const Meeting &member){
  	if(member.isParticipator(userName) && member.getParticipator().size() == 1 && member.getTitle() == title)
  		return true;
  	else 
  		return false;
  });

  // userName is one of the participator
  index += m_storage->updateMeeting([&userName, &title](const Meeting &member){
  	if(member.isParticipator(userName) && member.getParticipator().size() > 1 && member.getTitle() == title)
  		return true;
  	else 
  		return false;
  }, [&userName](Meeting &member){
  	member.removeParticipator(userName);
  });
  if(index == 0)
  	return false;
  return true;
}


/**
* search a meeting by username and title
* @param userName as a sponsor OR a participator
* @param title the meeting's title
* @return a meeting list result
*/
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  				const std::string &title) const {
	std::list<Meeting> meetings;
	meetings = m_storage->queryMeeting([&userName, &title](const Meeting &member) {
		if(member.getSponsor() == userName || member.isParticipator(userName)) {
			if(member.getTitle() == title)
				return true;
			return false;
		}
		else
			return false;
	});
	return meetings;
}

 
/**
* search a meeting by username, time interval
* @param userName as a sponsor OR a participator
* @param startDate time interval's start date
* @param endDate time interval's end date
* @return a meeting list result
*/
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const {
	std::list<Meeting> meetings;
	Date stime, etime;
	stime = Date::stringToDate(startDate);
	etime = Date::stringToDate(endDate);
	if(stime.getYear() == 0 && stime.getMonth() == 0 && stime.getDay() == 0
		&& stime.getHour() == 0 && stime.getMinute() == 0)
		return meetings;
	if(etime.getYear() == 0 && etime.getMonth() == 0 && etime.getDay() == 0
		&& etime.getHour() == 0 && etime.getMinute() == 0)
		return meetings;
	if(!Date::isValid(stime) || !Date::isValid(etime) || stime >= etime) 
		return meetings;

	meetings = m_storage->queryMeeting([&userName, &stime, &etime](const Meeting &member) {
		if(member.getSponsor() == userName || member.isParticipator(userName)) {
			if(member.getStartDate() > etime || member.getEndDate() < stime)
				return false;
			return true;
		}
		else
			return false;
	});
	return meetings;
}


/**
* list all meetings the user take part in
* @param userName user's username
* @return a meeting list result
*/
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
	std::list<Meeting> meetings;
	meetings = m_storage->queryMeeting([&userName](const Meeting& member){
		if(userName == member.getSponsor() || member.isParticipator(userName))
			return true;
		else
			return false;
	});
	return meetings;
}

/**
* list all meetings the user sponsor
* @param userName user's username
* @return a meeting list result
*/
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
	std::list<Meeting> meetings;
	meetings = m_storage->queryMeeting([&userName](const Meeting& member){
		if(userName == member.getSponsor())
			return true;
		else
			return false;
	});
	return meetings;
}


/**
* list all meetings the user take part in and sponsor by other
* @param userName user's username
* @return a meeting list result
*/
std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string &userName) const {
	std::list<Meeting> meetings;
	meetings = m_storage->queryMeeting([&userName](const Meeting& member){
		if(member.isParticipator(userName))
			return true;
		else
			return false;
	});
	return meetings;
}


/**
* delete a meeting by title and its sponsor
* @param userName sponsor's username
* @param title meeting's title
* @return if success, true will be returned
*/
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
  	int ret = 0;
  	ret = m_storage->deleteMeeting([&userName, &title](const Meeting& member){
  		if(member.getSponsor() == userName && member.getTitle() == title)
  			return true;
  		else 
  			return false;
  	});
  	if(ret == 0)
  		return false;
  	else
  		return true;
}


/**
* delete all meetings by sponsor
* @param userName sponsor's username
* @return if success, true will be returned
*/
bool AgendaService::deleteAllMeetings(const std::string &userName) {
  	int ret = 0;
  	ret = m_storage->deleteMeeting([&userName](const Meeting& member){
  		if(member.getSponsor() == userName)
  			return true;
  		else 
  			return false;
  	});
  	if(ret == 0) 
  		return false;
  	else
  		return true;
}
