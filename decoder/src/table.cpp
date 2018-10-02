/*
 * table.cpp
 *
 *  Created on: Oct 1, 2018
 *      Author: icts
 */

#include "table.h"

Table::Table()
: id(0), length_table(0), body(0)
{
}

void Table::setId(unsigned char uid)
{
	id = uid;
}

unsigned char Table::getId()
{
	return id;
}

void Table::setLenght(int lenght)
{
	length_table = lenght;
}

int Table::getLenght()
{
	return length_table;
}

void Table::setBody(list<unsigned char> table_body)
{
	body = table_body;
}

list<unsigned char> Table::getBody()
{
	return body;
}

list<map<string, unsigned char>> Table::getExtraInfo()
{
	map<string, unsigned char> valor;
	list<map<string, unsigned char>> lista;

	if (getId() == 0x0A){
		valor["Nome da Rede"] = *(next(body.begin()));
		valor["Numero de Programas"] = body.front();
		lista.push_back(valor);
	}
	else if (getId() == 0x0B)
	{
		valor["Numero do Programa"] = *(next(body.begin()));
		valor["Tipo Programa"] = body.front();
		lista.push_back(valor);
	}
	else if (getId() == 0x0E)
	{
		valor["Numero do Servico"] = *(next(body.begin()));
		valor["Numero da Rede"] = body.front();
		lista.push_back(valor);
	}

	return lista;
}

string Table::getName()
{
	string name;

	switch (id) {
		case APT_TABLE:
			name = "APT_TABLE";
			break;
		case MPT_TABLE:
			name = "MPT_TABLE";
			break;
		case ITN_TABLE:
			name = "ITN_TABLE";
			break;
		case ATB_TABLE:
			name = "ATB_TABLE";
			break;
		case TBA_TABLE:
			name = "TBA_TABLE";
			break;
		case TAP_TABLE:
			name = "TAP_TABLE";
			break;
		default:
			break;
	}

	return name;
}

