#ifndef AGENDAUI_H
#define AGENDAUI_H

#include <iostream>
#include <string>
#include "Date.hpp"
#include "History.hpp"
#include "AgendaService.hpp"

class AgendaUI {
 public:
  AgendaUI();
  void OperationLoop(void);

 private:
  /**
   * constructor
   */
  void startAgenda(void);

  /**
   * catch user's operation
   * @return the operation
   */
  std::string getOperation();

  /**
   * execute the operation
   * @return if the operationloop continue
   */
  bool executeOperation(std::string op);

  /**
   * user Login
   */
  void userLogIn(void);

  /**
   * user regist
   */
  void userRegister(void);

  /**
   * user logout
   */
  void userLogOut(void);

  /**
   * remind user meetings today
   */
  void remindMeetings(void);
  
  /**
   * quit the Agenda
   */
  void quitAgenda(void);

  /**
   * delete a user from storage
   */
  void deleteUser(void);

  /**
   * list all users from storage
   */
  void listAllUsers(void);

  /**
   * user create a meeting with someone else
   */
  void createMeeting(void);

  /**
   * sponsor add a participator to the meeting
   */
  void addMeetingParticipator(void);

  /**
   * sponsor add a participator to the meeting
   */
  void removeMeetingParticipator(void);

  /**
   * user quit from meeting
   */
  void quitMeeting(void);

  /**
   * list all meetings from storage
   */
  void listAllMeetings(void);

  /**
   * list all meetings that this user sponsored
   */
  void listAllSponsorMeetings(void);

  /**
   * list all meetings that this user take part in
   */
  void listAllParticipateMeetings(void);

  /**
   * search meetings by title from storage
   */
  void queryMeetingByTitle(void);

  /**
   * search meetings by timeinterval from storage
   */
  void queryMeetingByTimeInterval(void);

  /**
   * delete meetings by title from storage
   */
  void deleteMeetingByTitle(void);

  /**
   * delete all meetings that this user sponsored
   */
  void deleteAllMeetings(void);

  /**
   * show the meetings in the screen
   */
  void printMeetings(const std::list<Meeting> &meetings);

  /**
   *show history
   */
  void printHistory(void);

/**
   *show history by operation
   */
  void printOperationHistory(void);

  // dates
  std::string m_userName;
  std::string m_userPassword;
  AgendaService m_agendaService;
  std::shared_ptr<History> m_history;
};

#endif
