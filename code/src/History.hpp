#ifndef HISTORY_H
#define HISTORY_H

#include <functional>
#include <list>
#include <string>
#include <memory>


using namespace std;
class Operation
{
public:
  Operation() {
    m_oper = "";
    m_time = "";
  }
  Operation(string op, string tm) : m_oper(op), m_time(tm) {}
  ~Operation() {}
  string getOp() { return m_oper; }
  string getTime() { return m_time; }
private:
  string m_oper;
  string m_time;
};


class History {
public:
  /**
  *   default constructor
  */
  History();

  /**
  *   read file content into memory
  *   @return if success, true will be returned
  */
  bool readFile(void);

  /**
  *   write file content from memory
  *   @return if success, true will be returned
  */
  bool writeFile(void);

 public:

  /**
  *   destructor
  */
  ~History();

  // CRUD for History & Meeting
  // using C++11 Function Template and Lambda Expressions

  /**
  * create a History
  * @param a History object
  */
  void addHistory(const Operation &t_history);

  /**
  * sync with the file
  */
  bool sync(void);

  /**
  * get Instance of storage
  * @return the pointer of the instance
  */
  static std::shared_ptr<History> Instance(void);

  static std::shared_ptr<History> m_instance;
  std::list<Operation> m_operationList;
  bool m_dirty;
};

#endif
