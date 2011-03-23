#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <algorithm>

#include "KeyMagicKeyboard.h"

bool sortRule (RuleInfo * r1, RuleInfo * r2) {

	int s1 = r1->getLHSSwitchCount();
	int s2 = r2->getLHSSwitchCount();
	if (s1 != s2) {
		return s1 < s2;
	}

	int v1 = r1->getLHSVKCount();
	int v2 = r2->getLHSVKCount();
	if (v1 != v2) {
		return v1 < v2;
	}

	int m1 = r1->getMatchLength();
	int m2 = r2->getMatchLength();
	if (m1 != m2) {
		return m1 < m2;
	}

	return false;
}

KeyMagicKeyboard::KeyMagicKeyboard() :
	m_logger(KeyMagicLogger::getInstance()),
	m_verbose(0)
{
}

KeyMagicKeyboard::~KeyMagicKeyboard() {
	deleteRules();
	deleteInfos();
}

void KeyMagicKeyboard::deleteRules() {
	for (RuleList::iterator i = m_rules.begin(); i != m_rules.end(); i++) {
		delete *i;
	}
	m_rules.clear();
}

void KeyMagicKeyboard::deleteInfos() {
	for (InfoList::iterator i = m_infos.begin(); i != m_infos.end(); i++) {
		delete[] i->second.data;
	}
	m_infos.clear();
}
#if defined (_WIN32) || defined (_WIN64)
//TODO: use ICU converter instead of WideCharToMultiByte to be availabled on all platforms
bool KeyMagicKeyboard::loadKeyboardFile(const WCHAR * wcPath) {
	char szPath[MAX_PATH];
	int len = WideCharToMultiByte(CP_ACP, 0, wcPath, wcslen(wcPath), szPath, MAX_PATH, 0, 0);
	if (len) {
		szPath[len] = '\0';
		return loadKeyboardFile(szPath);
	}
	return false;
}

InfoList * KeyMagicKeyboard::getInfosFromKeyboardFile(const WCHAR * file) {
	char szFile[MAX_PATH];
	int len = WideCharToMultiByte(CP_ACP, 0, file, wcslen(file), szFile, MAX_PATH, 0, 0);
	if (len) {
		szFile[len] = '\0';
		return getInfosFromKeyboardFile(szFile);
	}
	return NULL;
}
#endif

InfoList * KeyMagicKeyboard::getInfosFromKeyboardFile(const char * file) {
	FileHeader fh;
	FILE * hFile;

	InfoList * infos = new InfoList();

	hFile = fopen(file, "rb");
	if (!hFile){
		PERROR("Cannot open keyboard file : %s\n", file);
		return NULL;
	}

	if (ReadHeader(hFile, &fh) == false) {
		PERROR("Fail to load keyboard file : %s\n", file);
		fclose(hFile);
		return NULL;
	}

	for (int i = 0; i < fh.stringCount; i++)
	{
		short sLength;
		fread(&sLength, sizeof(short), 1, hFile);

		short * local_buf = new short[sLength+1];
		local_buf[sLength]='\0';
		fread(local_buf, sLength * sizeof(short), 1, hFile);

		delete[] local_buf;
	}

	for (int i = 0; i < fh.infoCount; i++)
	{
		int id;
		short sLength;

		fread(&id, sizeof(int), 1, hFile);
		fread(&sLength, sizeof(short), 1, hFile);

		char * local_buf = new char[sLength+1];
		local_buf[sLength]='\0';
		fread(local_buf, sLength * sizeof(char), 1, hFile);

		//infos[id].size = sLength;
		//infos[id].data = local_buf;
		infos->operator [](id).size = sLength;
		infos->operator [](id).data = local_buf;
	}
	
	fclose(hFile);

	return infos;
}

bool KeyMagicKeyboard::ReadHeader(FILE * hFile, FileHeader * fh) {
	
	fread(fh, sizeof(FileHeader), 1, hFile);

	if (fh->magicCode[0] != 'K' || fh->magicCode[1] != 'M' || fh->magicCode[2] != 'K' || fh->magicCode[3] != 'L') {
		PERROR("Not KeyMagic keyboard file.\n");
		return false;
	}

	if (fh->majorVersion > MAJOR_VERSION) {
		PERROR("Can't load newer version of keyboard file.\n");
		return false;
	}

	if (fh->minorVersion > MINOR_VERSION) {
		PERROR("Can't load newer version of keyboard file.\n");
		return false;
	}

	// if older version
	if (fh->majorVersion == 1 && fh->minorVersion == 3) {
		FileHeader_1_3 fh13;
		fseek(hFile, 0, SEEK_SET);
		// read the header back
		fread(&fh13, sizeof(FileHeader_1_3), 1, hFile);
		// backward compability
		fh->stringCount = fh13.stringCount;
		fh->ruleCount = fh13.ruleCount;
		fh->layoutOptions = fh13.layoutOptions;
		fh->infoCount = 0; // 1.3 don't have infos, so set it to 0
	}

	return true;
}

bool KeyMagicKeyboard::loadKeyboardFile(const char * szPath) {
	FileHeader fh;
	FILE * hFile;

	BinaryRuleList rules;
	BinaryStringList strings;

	hFile = fopen(szPath, "rb");
	if (!hFile){
		PERROR("Cannot open keyboard file : %s\n", szPath);
		return false;
	}

	if (ReadHeader(hFile, &fh) == false) {
		PERROR("Fail to load keyboard file : %s\n", szPath);
		fclose(hFile);
		return false;
	}

	/*fread(&fh, sizeof(FileHeader), 1, hFile);

	if (fh.magicCode[0] != 'K' || fh.magicCode[1] != 'M' || fh.magicCode[2] != 'K' || fh.magicCode[3] != 'L') {
		LOG("Not KeyMagic keyboard file;%s", szPath);
		return false;
	}

	if (fh.majorVersion > MAJOR_VERSION) {
		LOG("Can't load newer version of keyboard file;%s", szPath);
		return false;
	}

	if (fh.minorVersion > MINOR_VERSION) {
		LOG("Can't load newer version of keyboard file;%s", szPath);
		return false;
	}

	// if older version
	if (fh.majorVersion == 1 && fh.minorVersion == 3) {
		FileHeader_1_3 fh13;
		fseek(hFile, 0, SEEK_SET);
		// read the header back
		fread(&fh13, sizeof(FileHeader_1_3), 1, hFile);
		// backward compability
		fh.stringCount = fh13.stringCount;
		fh.ruleCount = fh13.ruleCount;
		fh.layoutOptions = fh13.layoutOptions;
		fh.infoCount = 0; // 1.3 don't have infos, so set it to 0
	}*/

	m_layoutOptions = fh.layoutOptions;
	
	if (m_verbose) {
		m_logger->log("autoBksp=%x\n", m_layoutOptions.autoBksp);
		m_logger->log("eat=%x\n", m_layoutOptions.eat);
		m_logger->log("posBased=%x\n", m_layoutOptions.posBased);
		m_logger->log("trackCaps=%x\n", m_layoutOptions.trackCaps);
	}
	
	m_strings.clear();
	deleteRules();
	deleteInfos();

	for (int i = 0; i < fh.stringCount; i++)
	{
		short sLength;
		fread(&sLength, sizeof(short), 1, hFile);

		short * local_buf = new short[sLength+1];
		local_buf[sLength]='\0';
		fread(local_buf, sLength * sizeof(short), 1, hFile);

		strings.push_back(local_buf);
	}

	for (int i = 0; i < fh.infoCount; i++)
	{
		int id;
		short sLength;

		fread(&id, sizeof(int), 1, hFile);
		fread(&sLength, sizeof(short), 1, hFile);

		char * local_buf = new char[sLength + 1];
		local_buf[sLength]='\0';
		fread(local_buf, sLength * sizeof(char), 1, hFile);

		m_infos[id].size = sLength;
		m_infos[id].data = local_buf;
	}

	for (int i = 0; i < fh.ruleCount; i++)
	{
		BinaryRule Rule;
		short sLength;

		fread(&sLength, sizeof(short), 1, hFile);
		short * ruleBinaryIn = new short[sLength+1];
		ruleBinaryIn[sLength]='\0';
		fread(ruleBinaryIn, sLength * sizeof(short), 1, hFile);

		fread(&sLength, sizeof(short), 1, hFile);
		short * ruleBinaryOut = new short[sLength+1];
		ruleBinaryOut[sLength]='\0';
		fread(ruleBinaryOut, sLength * sizeof(short), 1, hFile);
		
		Rule.strInRule = (short*)ruleBinaryIn;
		Rule.strOutRule = (short*)ruleBinaryOut;

		rules.push_back(Rule);
	}

	//Convert variable to pure string. Originally, variables are binary arrays containing inline index
	variablesToStrings(&strings, &m_strings);
	binaryRulesToManagedRules(&rules, &m_rules);
	std::sort(m_rules.begin(), m_rules.end(), sortRule);
	std::reverse(m_rules.begin(), m_rules.end());
	
	if (m_verbose) {
		m_logger->log("strings;%d==%d\n", strings.size(), m_strings.size());
		m_logger->log("rules;%d==%d\n", rules.size(), m_rules.size());
		m_logger->log("|----rules-start----|\n");
		for (RuleList::iterator i = m_rules.begin(); i != m_rules.end(); i++) {
			RuleInfo * rule = *i;
			std::string * s;
			
			if (i != m_rules.begin()) {
				m_logger->log(",\n");
			}
			
			m_logger->log("|---index=%d---|\n{\n", rule->getRuleIndex());
			s = rule->description();
			m_logger->log(s->c_str());
			m_logger->log("}");
			delete s;
		}
		m_logger->log("\n|----rules-end----|\n");
	}
	
	fclose(hFile);
	
	for (BinaryStringList::iterator i = strings.begin(); i != strings.end(); i++) {
		delete[] *i;
	}
	
	for (BinaryRuleList::iterator i = rules.begin(); i != rules.end(); i++) {
		delete[] i->strInRule;
		delete[] i->strOutRule;
	}

	return true;
}

StringList * KeyMagicKeyboard::getStrings() {
	return &m_strings;
}

RuleList * KeyMagicKeyboard::getRules() {
	return &m_rules;
}

LayoutOptions * KeyMagicKeyboard::getLayoutOptions() {
	return &m_layoutOptions;
}

const InfoList& KeyMagicKeyboard::getInfoList() {
	return m_infos;
}

void KeyMagicKeyboard::variablesToStrings(BinaryStringList * variables, StringList * strings) {

	for (BinaryStringList::iterator i = variables->begin(); i != variables->end(); i++) {

		const short * binString = *i;
		KeyMagicString value;

		while(*binString) {
			switch(*binString) {
			case opVARIABLE:
				binString++;
				value += getVariableValue(*binString++, variables);
				break;
			default:
				value += *binString++;
			}
		}
		strings->push_back(value);
	}
}

void KeyMagicKeyboard::binaryRulesToManagedRules(BinaryRuleList * binRules, RuleList * rules) {
	for (BinaryRuleList::iterator i = binRules->begin(); i != binRules->end(); i++) {
		RuleInfo * managedMatchRule = new RuleInfo(i->strInRule, i->strOutRule, &m_strings);
		managedMatchRule->setIndex(i - binRules->begin());
		rules->push_back(managedMatchRule);
	}
}

KeyMagicString KeyMagicKeyboard::getVariableValue(int index, BinaryStringList * binStrings) {
	KeyMagicString value;

	index--;

	if (index > binStrings->size()) {
		return KeyMagicString();
	}

	const short * binRule = binStrings->at(index);

	while (*binRule) {
		if (*binRule == opVARIABLE) {
			binRule++;
			KeyMagicString value2 = getVariableValue(*binRule++, binStrings);
			value += value2;
		} else {
			value += *binRule++;
		}
	}

	return value;
}
