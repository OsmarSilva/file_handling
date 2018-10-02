//============================================================================
// Name        : decode.cpp
// Author      : Osmar Silva
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <decoder.h>
#include <memory>
#include <list>
#include <table.h>
#include <string>
#include <map>

using namespace std;

#define HDR_BEG 	0x47	/*byte header begin*/
#define HDR_END   	0xDD	/*byte header end*/
#define TAB_BEG 	0x48	/*byte table begin*/
#define TAB_END		0xAA	/*byte table end*/
#define FIL_END		0xFF	/*byte end file*/

#define MAX_BUFFER 	256

#define MAX_HEADER_BODY 2

unsigned char header[4];
unsigned char table_id = 0;
int data_position = 0;

int count_bytes = 0;

typedef struct{
	int apt_tables_count;
	int mpt_tables_count;
	int itn_tables_count;
	int atb_tables_count;
	int tba_tables_count;
	int tap_tables_count;
} table_counters;

table_counters counters = {0};

list<unsigned char> table_body;

list<shared_ptr<Table>> tables;


void (*Action)(unsigned char data);

//prototypes
void stHeaderBegin(unsigned char data);
void stHeaderData(unsigned char data);
void stHeaderEnd(unsigned char data);
void stTableBegin(unsigned char data);
void stTableID(unsigned char data);
void stTableBody(unsigned char data);
void stTableEnd(unsigned char data);
void stFileEnd(unsigned char data);
void stError(unsigned char data);


void stHeaderBegin(unsigned char data)
{
	if (data == HDR_BEG)
	{
		header[data_position] = data;
		Action = stHeaderData;
	}
}

void stHeaderData(unsigned char data)
{
	data_position += 1;
	header[data_position] = data;

	if(data_position == MAX_HEADER_BODY)
	{
		data_position += 1;
		Action = stHeaderEnd;
	}
}

void stHeaderEnd(unsigned char data)
{
	if (data == HDR_END)
	{
		header[data_position] = data;
//		//TODO REMOVER CODIGO, USADO PARA SIMPLES CHECAGEM
//		for (int x =0; x < 4; x++)
//		{
//			cout << std::hex << (int)header[x] << endl;
//		}

		Action = stTableBegin;
	}
}

void stTableBegin(unsigned char data)
{
	if (data == TAB_BEG)
		Action = stTableID;

	//TODO PUT EXCEPTIONS
}

void stTableID(unsigned char data)
{
	table_id = data;

	switch (data) {
		case APT_TABLE:
			counters.apt_tables_count++;
			Action = stTableBody;
			break;
		case MPT_TABLE:
			counters.mpt_tables_count++;
			Action = stTableBody;
			break;
		case ITN_TABLE:
			counters.itn_tables_count;
			Action = stTableBody;
			break;
		case ATB_TABLE:
			counters.atb_tables_count++;
			Action = stTableBody;
			break;
		case TBA_TABLE:
			counters.tba_tables_count;
			Action = stTableBody;
			break;
		case TAP_TABLE:
			counters.tap_tables_count++;
			Action = stTableBody;
			break;
		default:
			//TODO add exception
			break;
	}
}

void stTableBody(unsigned char data)
{
	if (data == TAB_END)
		Action = stTableEnd;
	else{
		table_body.push_back(data);
		count_bytes++;
	}
}

void stTableEnd(unsigned char data)
{
	shared_ptr<Table> table = make_shared<Table>();
	table->setId(table_id);
	table->setLenght(count_bytes);
	table->setBody(table_body);

	tables.push_back(table);

	//reinicialize counters and informations about the table
	count_bytes = 0;
	table_body.clear();

	if (data == TAB_BEG)
		Action = stTableID;
	else if (data == FIL_END)
		Action = stFileEnd;
}

void stFileEnd(unsigned char data)
{
	if (data == FIL_END)
		Action = stHeaderBegin;
	else
		Action = stError;
}

void stError(unsigned char data)
{
	//TODO add error routine code
	cout << "Error State" << endl;
}

static vector<unsigned char> ReadBytesFromFile(char const *filename)
{
	//open a binary file
    ifstream file(filename, ios::binary | ios::ate);

    //get length of file
    ifstream::pos_type size = file.tellg();

    std::vector<unsigned char> result(size);

    file.seekg(0, ios::beg); // set the pointer to the start
    file.read((char*)&result[0], size);

    return result;
}

void showAllTableInfos()
{
	list<shared_ptr<Table>>::iterator it;
	it = tables.begin();

	cout << endl << endl;
	cout << "================ NUMBER OF TABLES ================" << endl;
	cout << "ATP TABLES: " << counters.apt_tables_count << endl;
	cout << "MPT TABLES: " << counters.mpt_tables_count << endl;
	cout << "ITN TABLES: " << counters.itn_tables_count << endl;
	cout << "ATB TABLES: " << counters.atb_tables_count << endl;
	cout << "TBA TABLES: " << counters.tba_tables_count << endl;
	cout << "TAP TABLES: " << counters.tap_tables_count << endl;

	cout << endl;

	cout << "================ INFO OF TABLES ================" << endl;
	for(; it != tables.end(); it++)
	{
		cout << "TABLE:  " << (*it)->getName() << endl;
		cout << "ID:     " << (int)(*it)->getId() << endl;
		cout << "LENGHT: " << (*it)->getLenght() << endl;

		list<unsigned char> body = (*it)->getBody();
		list<unsigned char>::iterator body_it;

		cout << "BODY    ";

		for(body_it = body.begin(); body_it != body.end(); body_it++)
		{
			cout << std::hex << (int)*body_it << " ";
		}
		cout << endl;

		list<map<string, unsigned char>> infos = (*it)->getExtraInfo();

		list<map<string, unsigned char>>::iterator list_it;
		map<string, unsigned char>::iterator map_it;

		list_it = infos.begin();
		map_it = (*list_it).begin();

		if (infos.size() > 0)
			for(; map_it != (*list_it).end(); map_it++)
			{
				cout << map_it->first << ": " << std::hex << (int) map_it->second << endl;
			}

		cout << endl;

	}

}

void handleData(char const *filename)
{
	vector<unsigned char> data = ReadBytesFromFile(filename);

	Action = stHeaderBegin;

	for (int i = 0; i < data.size(); ++i)
	{
		Action(data[i]);
	}

	showAllTableInfos();
}


int main()
{
	const char *filename = "teste.bin";
	int read;
	handleData(filename);

	return 0;
}
