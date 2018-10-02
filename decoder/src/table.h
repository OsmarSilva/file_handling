/*
 * table.h
 *
 *  Created on: Oct 1, 2018
 *      Author: icts
 */

#ifndef TABLE_H_
#define TABLE_H_

#include <list>
#include <map>
#include <string>
#include <iostream>
#include <iterator>

using namespace std;

/*definitions of tables*/
#define APT_TABLE 	0x0A
#define MPT_TABLE 	0x0B
#define ITN_TABLE 	0x0C
#define ATB_TABLE 	0x0D
#define TBA_TABLE 	0x0E
#define TAP_TABLE 	0x0F

class Table
{
public:
  Table();

  void setId(unsigned char id);

  unsigned char getId();

  void setLenght(int lenght);

  int getLenght();

  void setBody(list<unsigned char> table_body);

  list<unsigned char> getBody();

  list<map<string, unsigned char>> getExtraInfo();

  string getName();

  //attributes
  unsigned int id;
  int length_table;
  list<unsigned char> body;

};




#endif /* TABLE_H_ */
