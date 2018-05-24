#include "AgendaService.hpp"
#include "AgendaUI.hpp"
#include "History.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <list>
#define STANDARD_TIME 1900
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::list;
using std::ios;
using std::setw;
using std::setiosflags;
using std::resetiosflags;


/**
 * constructor
 */
AgendaUI::AgendaUI() {
    m_userName = "";
    m_userPassword = "";
    startAgenda();
}

/**
 * OperatrionLoop for Agenda
 */
void AgendaUI::OperationLoop(void) {
    for(int i = 0; i < 16; i++)
        cout << " ";
    cout << "Ω Ω Ω   iAgenda, make life get better!   Ω Ω Ω";
    cout << endl << endl << endl;
    while (true) {
        for (int i = 1; i <= 17; i++) cout << "* ";
        cout << " iAgenda ";
        for (int i = 1; i <= 17; i++) cout << "* ";
        cout << endl;
        cout << "Options :" << endl;
        if (m_userName == "") {
            cout << "l    - log in my iAgenda" << endl;
            cout << "r    - register my iAgenda account" << endl;
            cout << "q    - quit iAgenda" << endl;
            cout << "u    - list all iAgenda users" << endl;
        } 
        else {
            cout << "his   - show my history" << endl;
            cout << "hso   - show history by operation" << endl;
            cout << "out   - log out my iAgenda" << endl;
            cout << "dac   - delete my iAgenda account" << endl;
            cout << "crm   - create my meeting" << endl;
            cout << "alm   - list all of my meetings" << endl;
            cout << "spm   - list all of my meetings as a sponsor" << endl;
            cout << "adp   - add a participator" << endl;
            cout << "dtp   - delete a participator" << endl;
            cout << "uqt   - user quit from the meeting" << endl;
            cout << "ppm   - list all of my meetings as a participator" << endl;
            cout << "qmt   - query my meeting by title" << endl;
            cout << "qmi   - query my meeting by time interval" << endl;
            cout << "dmt   - delete my meeting by title" << endl;
            cout << "dam   - delete all of my meetings" << endl;
        }
        for (int i = 1; i <= 39; i++) cout << "* ";
        cout << endl;
        cout << endl;
        if (m_userName == "")
            cout << "iAgenda:/ Dashboard$ ";
        else
            cout << "iAgenda:Users " << m_userName << " $ ";
        if (!executeOperation(getOperation())) break;
    }
}

/**
 * Agenda start function
 */
void AgendaUI::startAgenda(void) { 
    system("clear"); 
    m_history = History::Instance();
}

/**
 * catch user's operation
 * @return the operation
 */
std::string AgendaUI::getOperation() {
    string s;
    cin >> s;
    return s;
}

/**
 * search meetings by timeinterval from storage
 */
void AgendaUI::remindMeetings(void) {
    time_t t;
    time(&t);
    struct tm *now = localtime(&t);
    Date current(STANDARD_TIME + now->tm_year, now->tm_mon + 1, 
                now->tm_mday, now->tm_hour, now->tm_min);
    Date future(STANDARD_TIME + now->tm_year, now->tm_mon + 1, now->tm_mday + 1, 0, 0);
    cout << endl << "[Current Time] " << ctime(&t);
    if(now->tm_hour > 22 || now->tm_hour < 4)
        cout << "Very late now, have a sleep zz..." << endl;
    if((now->tm_hour > 11 && now->tm_hour < 13) || (now->tm_hour > 17 && now->tm_hour < 19))
        cout << "Umm... I am hungry..." << endl;
    if(now->tm_hour > 8 && now->tm_hour < 11)
        cout << "Hope you have nice day!" << endl;
    if(now->tm_hour > 15 && now->tm_hour < 17)
        cout << "What a sunny day, let us to do some exercise." << endl;
    string sTime, eTime;
    sTime = Date::dateToString(current);
    eTime = Date::dateToString(future);
    list<Meeting> l(m_agendaService.meetingQuery(m_userName, sTime, eTime));
    cout << endl;
    if (l.empty()) {
        cout << "No meetings recently, have a relax." << endl;
    } 
    else {
        cout << "Today you have " <<  l.size() << " meetings:" << endl << endl;
        printMeetings(l);
    }
}

/**
 * execute the operation
 * @return if the operationloop continue
 */
bool AgendaUI::executeOperation(std::string op) {
    for (int i = 0; i < op.length(); i++) 
        op[i] = tolower(op[i]);
    bool flag = true;
    if (m_userName == "") flag = false;
    if (op == "l" && !flag) {
        userLogIn();
        return true;
    }
    if (op == "r" && !flag) {
        userRegister();
        return true;
    }
    if (op == "dac" && flag) {
        deleteUser();
        return true;
    }
    if (op == "u" && !flag) {
        listAllUsers();
        return true;
    }
    if (op == "his" && flag) {
        printHistory();
        return true;
    }
    if (op == "hso" && flag) {
        printOperationHistory();
        return true;
    }
    if (op == "crm" && flag) {
        createMeeting();
        return true;
    }
    if (op == "alm" && flag) {
        listAllMeetings();
        return true;
    }
    if (op == "spm" && flag) {
        listAllSponsorMeetings();
        return true;
    }
    if (op == "ppm" && flag) {
        listAllParticipateMeetings();
        return true;
    }
    if (op == "qmt" && flag) {
        queryMeetingByTitle();
        return true;
    }
    if (op == "qmi" && flag) {
        queryMeetingByTimeInterval();
        return true;
    }
    if (op == "dmt" && flag) {
        deleteMeetingByTitle();
        return true;
    }
    if (op == "dam" && flag) {
        deleteAllMeetings();
        return true;
    }
    if (op == "out" && flag) {
        userLogOut();
        return true;
    }
    if (op == "q" && !flag) {
        m_agendaService.quitAgenda();
        return false;
    }
    if (op == "adp" && flag) {
        addMeetingParticipator();
        return false;
    }
    if (op == "dtp" && flag) {
        removeMeetingParticipator();
        return false;
    }
    if (op == "uqt" && flag) {
        quitMeeting();
        return false;
    }
    cout << endl;
    cout << "Please enter correct command!" << endl;
    cout << endl;
    return true;
}

/**
* sponsor add a participator to the meeting
*/
void AgendaUI::addMeetingParticipator(void) {
    cout << endl;
    string title, participator;
    cout << "[add participator] [title] [luckydog]" << endl;
    cout << "[add participator] ";
    cin >> title >> participator;
    if(m_agendaService.addMeetingParticipator(m_userName, title, participator)) {
        cout << "[add participator] succeed!" << endl;
        cout << participator << ", welcome to " << title << " !";
        time_t t;
        time(&t);
        string s = "\nAdd ";
        s = s + participator;
        s = s + " in ";
        s = s + title;
        s = s + " at "; 
        Operation temp(s, ctime(&t));
        m_history->addHistory(temp);
    }
    else {
        cout << "[addMeetingParticipator] fail!" << endl;
    }
}

/**
* sponsor add a participator to the meeting
*/
void AgendaUI::removeMeetingParticipator(void) {
    cout << endl;
    string title, participator;
    cout << "[remove participator] [title] [poor guy]" << endl;
    cout << "[remove participator] ";
    cin >> title >> participator;
    if(m_agendaService.removeMeetingParticipator(m_userName, title, participator)) {
        cout << "[remove participator] succeed!" << endl;
        cout << "You have removed " << participator << " ." << endl;
        time_t t;
        time(&t);
        string s = "\nRemove ";
        s = s + participator;
        s = s + " in ";
        s = s + title;
        s = s + " at "; 
        Operation temp(s, ctime(&t));
        m_history->addHistory(temp);
    }
    else {
        cout << "[removeMeetingParticipator] fail!" << endl;
    }
}

/**
* user quit from meeting
*/
void AgendaUI::quitMeeting(void) {
    cout << endl;
    string title;
    cout << "[quit meeting] [title]" << endl;
    cout << "[quit meeting] ";
    cin >>title;
    cin >> title;
    if(m_agendaService.quitMeeting(m_userName, title)) {
        cout << "[quit meeting] succeed!" << endl;
        cout << "You have left " << title << " ." << endl;
        time_t t;
        time(&t);
        string s = "\nQuit ";
        s = s + title;
        s = s + " at "; 
        Operation temp(s, ctime(&t));
        m_history->addHistory(temp);
    }
    else {
        cout << "[quit meeting] fail!" << endl;
    }
}

/**
 * user Login
 */
void AgendaUI::userLogIn(void) {
    cout << endl;
    cout << "[log in] [username] [password]" << endl;
    cout << "[log in] ";
    cin >> m_userName >> m_userPassword;
    if (m_agendaService.userLogIn(m_userName, m_userPassword)) {
        cout << "[log in] succeed!" << endl;
        remindMeetings();
        time_t t;
        time(&t);
        Operation temp("\nLog in at ", ctime(&t));
        m_history->addHistory(temp);
    } 
    else {
        cout << "[log in] Password error or user doesn't exist" << endl;
        m_userName = "";
        m_userPassword = "";
    }
    cout << endl;
}

/**
 * user regist
 */
void AgendaUI::userRegister(void) {
    cout << endl;
    cout << "[register] [username] [password] [email] [phone]" << endl;
    cout << "[register] ";
    string newUserName, newPassword, newEmail, newPhone;
    cin >> newUserName >> newPassword;
    cin >> newEmail >> newPhone;
    if (m_agendaService.userRegister(newUserName, newPassword, newEmail,
                                     newPhone)) {
        cout << "[register] succeed!" << endl;
    } 
    else {
        cout << "[register] This username has been registered!" << endl;
    }
    cout << endl;
}

/**
 * quit the Agenda
 */
void AgendaUI::quitAgenda(void) { 
    m_agendaService.quitAgenda();
    m_history->writeFile();
}

/**
 * user logout
 */
void AgendaUI::userLogOut(void) {
    m_userName = "";
    m_userPassword = "";
    cout << endl;
    cout << endl;
}

/**
 * delete a user from storage
 */
void AgendaUI::deleteUser(void) {
    cout << endl;
    if(m_agendaService.deleteUser(m_userName, m_userPassword)) {
        m_userName = "";
        m_userPassword = "";
        cout << "[delete agenda account] succeed!" << endl;
        cout << endl;
    }
    else {
        cout << "[delete agenda account] fail!" << endl;
        cout << endl;
    }
}

/**
 * list all users from storage
 */
void AgendaUI::listAllUsers(void) {
    cout << endl;
    cout << "[list all users] " << endl;
    cout << endl;
    cout << setiosflags(ios::left);
    cout << setw(20) << "name";
    cout << setw(30) << "email";
    cout << "phone" << endl;
    list<User> l;
    l = m_agendaService.listAllUsers();
    list<User>::iterator it = l.begin();
    while (it != l.end()) {
        cout << setw(20) << it->getName();
        cout << setw(30) << it->getEmail();
        cout << it->getPhone() << endl;
        it++;
    }
    cout << resetiosflags(ios::left);
    cout << endl;
}

/**
 * user create a meeting with someone else
 */
void AgendaUI::createMeeting(void) {
    cout << endl;
    cout << "[create meeting] [the number of participators]" << endl
         << "[create meeting] ";
    int num;
    cin >> num;
    while (num == 0) {
        cout << "[create meeting] the number of participators can't be zero" << endl;
        cout << "                 Do you want to try again?  ";
        char a;
        cin >> a;
        if(a == 'y' || a == 'Y') {
        cout << "[create meeting] [the number of participators]" << endl
        << "[create meeting] ";
        cin >> num;
            continue;
        }
        else return;
    }
    std::vector<std::string> l;
    string temp;
    for (int i = 1; i <= num; i++) {
        cout << "[create meeting] [please enter the participator " << i << " ]" << endl
             << "[create meeting] ";
        cin >> temp;
        l.push_back(temp);
    }
    cout << "[create meeting] [title] "
         << "[start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"
         << endl;
    cout << "[create meeting] ";
    string sTime, eTime, title;
    cin >> title >> sTime >> eTime;
    while(!m_agendaService.createMeeting(m_userName, title, sTime, eTime, l)) {
        cout << "[create meeting] error!" << endl;
        cout << "Do you want have another try?  ";
        char t;
        cin >> t;
        if(t == 'Y' || t == 'y') {
            cout << "\n[create meeting] [title] "
            << "[start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"
            << endl;
            cout << "[create meeting] ";
            cin >> title >> sTime >> eTime;
        }
        else return;
    }
    cout << endl;

    if(m_agendaService.createMeeting(m_userName, title, sTime, eTime, l)) {
        cout << "[create meeting] succeed!" << endl;
        time_t t;
        time(&t);
        string s;
        s = "\nCreated ";
        s = s + title;
        s = s + " at ";
        Operation temp(s, ctime(&t));
        m_history->addHistory(temp);
    } 
}
/**
 * list all meetings from storage
 */
void AgendaUI::listAllMeetings(void) {
    list<Meeting> l(m_agendaService.listAllMeetings(m_userName));
    // auto l = m_agendaService.listAllMeetings(m_userName);
    cout << endl;
    cout << "[list all meetings]" << endl;
    cout << endl;
    if (l.empty()) {
        cout << "None" << endl;
        cout << endl;
    } else {
        printMeetings(l);
    }
}

/**
 * list all meetings that this user sponsored
 */
void AgendaUI::listAllSponsorMeetings(void) {
    cout << endl;
    cout << "[list all sponsor meetings]" << endl;
    list<Meeting> l(m_agendaService.listAllSponsorMeetings(m_userName));
    cout << endl;
    if (l.empty()) {
        cout << "None" << endl;
        cout << endl;
    } else {
        printMeetings(l);
    }
}

/**
 * list all meetings that this user take part in
 */
void AgendaUI::listAllParticipateMeetings(void) {
    list<Meeting> l(m_agendaService.listAllParticipateMeetings(m_userName));
    // auto l = m_agendaService.listAllParticipateMeetings(m_userName);
    cout << endl;
    cout << "[list all participator meetings]" << endl;
    cout << endl;
    if (l.empty()) {
        cout << "None" << endl;
        cout << endl;
    } else {
        printMeetings(l);
    }
}

/**
 * search meetings by title from storage
 */
void AgendaUI::queryMeetingByTitle(void) {
    cout << endl;
    cout << "[query meeting] [title]:" << endl;
    string title;
    cout << "[query meeting] ";
    cin >> title;
    list<Meeting> l(m_agendaService.meetingQuery(m_userName, title));
    if (l.empty()) {
        cout << "None" << endl;
        cout << endl;
        return;
    } else {
        printMeetings(l);
    }
}

/**
 * search meetings by timeinterval from storage
 */
void AgendaUI::queryMeetingByTimeInterval(void) {
    cout << endl;
    cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] "
         << "[end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meetings] ";
    string sTime, eTime;
    cin >> sTime >> eTime;
    cout << "[query meetings]" << endl;
    list<Meeting> l(m_agendaService.meetingQuery(m_userName, sTime, eTime));
    cout << endl;
    if (l.empty()) {
        cout << "None" << endl;
        cout << endl;
    } 
    else {
        printMeetings(l);
    }
}

/**
 * delete meetings by title from storage
 */
void AgendaUI::deleteMeetingByTitle(void) {
    cout << endl;
    cout << "[delete meeting] [title]" << endl;
    cout << "[delete meeting] ";
    string title;
    cin >> title;
    if (m_agendaService.deleteMeeting(m_userName, title)) {
        cout << "[delete meeting] succeed!" << endl;
        time_t t;
        time(&t);
        string s;
        s = "\nDelete ";
        s = s + title;
        s = s + " at ";
        Operation temp(s, ctime(&t));
        m_history->addHistory(temp);
    } 
    else {
        cout << "[error] delete meeting fail!" << endl;
    }
    cout << endl;
}

/**
 * delete all meetings that this user sponsored
 */
void AgendaUI::deleteAllMeetings(void) {
    cout << endl;
    if (m_agendaService.deleteAllMeetings(m_userName)) {
        cout << "[delete all meeting] succeed!" << endl;
        time_t t;
        time(&t);
        Operation temp("Delete all meetings at ", ctime(&t));
        m_history->addHistory(temp);
    } 
    else {
        cout << "[error] delete all meeting fail!" << endl;
    }
    cout << endl;
}

/**
* convert the participatorlist to string
* @param the source participatorlist
* @return the result string
*/
std::string turnVectorToString(std::vector<std::string> participatorlist) {
    std::string result = "";
    bool isFirstItem = true;
    for (auto it = participatorlist.begin(); it != participatorlist.end();
         it++) {
        if (isFirstItem) {
            isFirstItem = false;
        } else {
            result += ',';
        }
        result += *it;
    }
    return result;
}

/**
 * show the meetings in the screen
 */
void AgendaUI::printMeetings(const std::list<Meeting> &meetings) {
    cout << setiosflags(ios::left);
    cout << setw(15) << "title";
    cout << setw(15) << "sponsor";
    cout << setw(18) << "start time";
    cout << setw(18) << "end time";
    cout << "participators";
    cout << endl;
    auto it = meetings.begin();
    while (it != meetings.end()) {
        cout << setw(15) << it->getTitle();
        cout << setw(15) << it->getSponsor();
        cout << setw(18) << Date::dateToString(it->getStartDate());
        cout << setw(18) << Date::dateToString(it->getEndDate());
        cout << turnVectorToString(it->getParticipator());
        cout << endl;
        it++;
    }
    cout << resetiosflags(ios::left);
    cout << endl;
}

void AgendaUI::printHistory() {
    cout << endl;
    cout << "[history] ";
    std::list<Operation> l = m_history->m_operationList;
    cout << endl;
    auto it = l.begin();
    while (it != l.end()) {
        cout << it->getOp();
        cout << it->getTime();
        it++;
    }
    cout << resetiosflags(ios::left);
    cout << endl;
}

/**
   *show history by operation
   */
  void AgendaUI::printOperationHistory(void) {
    cout << endl;
    cout << "[history] [operation]";
    std::list<Operation> l = m_history->m_operationList;
    cout << endl;
    cout << "\n[operation] ";
    string op;
    cin >> op;
    auto it = l.begin();
    while (it != l.end()) {
        int i;
        for(i = 0; i < (it->getOp()).length(); i++)
            if(isalpha( (it->getOp())[i] ))
                break;
        if(tolower((it->getOp())[i]) == op[0]) {
            cout << it->getOp();
            cout << it->getTime();
        }
        it++;
    }
    cout << resetiosflags(ios::left);
    cout << endl;
}