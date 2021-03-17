/* Copyright (C) 1999 Lucent Technologies */
/* Excerpted with slight modifications from 'The Practice of Programming' */
/* by Brian W. Kernighan and Rob Pike */


#include "Csv.h"

using namespace std;

// getline: get one line
void Csv::setline(string& str)
{
	line = str;
	split();
}

// split: split line into fields
int Csv::split()
{
	string fld;
	int i, j;
	
	nfield = 0;
	if (line.length() == 0)
		return 0;
	i = 0;
	
	do {
		if (i < line.length() && line[i] == '"')
			j = advquoted(line, fld, ++i);	// skip quote
		else
			j = advplain(line, fld, i);
		if (nfield >= field.size())
			field.push_back(fld);
		else
			field[nfield] = fld;
		nfield++;
		i = j + 1;
	} while (j < line.length());
	
	return nfield;
}

// advquoted: quoted field; return index of next separator
int Csv::advquoted(const string& s, string& fld, int i)
{
	int j;
	
	fld = "";
	for (j = i; j < s.length(); j++) {
		if (s[j] == '"' && s[++j] != '"') {
			int k = s.find_first_of(fieldsep, j);
			if (k > s.length())	// no separator found
				k = s.length();
			for (k -= j; k-- > 0; )
				fld += s[j++];
			break;
		}
		fld += s[j];
	}
	return j;
}

// advplain: unquoted field; return index of next separator
int Csv::advplain(const string& s, string& fld, int i)
{
	int j;
	
	j = s.find_first_of(fieldsep, i); // look for separator
	if (j > s.length())               // none found
		j = s.length();
	fld = string(s, i, j-i);
	return j;
}


// getfield: return n-th field
string Csv::getfield(int n)
{
	if (n < 0 || n >= nfield)
		return "";
	else
		return field[n];
}

