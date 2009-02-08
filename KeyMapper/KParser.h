#pragma once

#include <string>
using namespace std;

class KParser
{
public:
	KParser()
	{
		GetLinePos();
	}

	KParser(string sStringToParse)
	{
		sToParse = sStringToParse;
		GetLinePos();
	}

	KParser(CHAR cStringToParse)
	{
		sToParse = cStringToParse;
		GetLinePos();
	}

	size_t GetTotalLines()	{return sTotalLines;}

	string GetLine(size_t LineNumber)
	{
		return sToParse.substr(lines[LineNumber-1].iStartPos, lines[LineNumber-1].iEndPos-lines[LineNumber-1].iStartPos);
	}

	virtual ~KParser()	{ Empty(); }

	void Empty()	{ sToParse.~string(); }

private:
	VOID GetLinePos()
	{
		sTotalLines = CountLines();
		lines = new KLine[sTotalLines];
		
		int sStartPos;
		int sEndPos=0;

		for (int i=0; i < sTotalLines; i++){
			sStartPos = sEndPos;
			sEndPos = sToParse.find(";", sStartPos);
			if (sEndPos!=string::npos)
			{
				lines[i].iStartPos = sStartPos;
				lines[i].iEndPos = sEndPos;
				sEndPos++;
			}
		}
	}

	size_t CountLines()
	{
		int sStartPos;
		int sEndPos=0;
		size_t TotalLines=0;

		do {
			sStartPos = sEndPos;
			sEndPos = sToParse.find(";", sStartPos);
			TotalLines++;
			if (sEndPos!=string::npos)
				sEndPos++;
		} while (sEndPos!=string::npos);
		return --TotalLines;
	}

private:

	struct KLine{
		int iStartPos;
		int iEndPos;
	};

	string sToParse; // String, Entrie lines
	size_t sTotalLines; // Current line number
	KLine* lines;

};