/* 
 * This regex is intended for light weight normal string processing only.
 * And tested only for some basic thing for Burglish Systems.
 * No error checking and no input checking also.
 * NO WARRANTIES of course, use at your own risk! 
 *
 *	code - aTxIvG4001
 * 
 * support the following regex syntax (even some of syntaxes are only partial support)
 *
 * ^ $ => start of string, end of the string
 * [a-z] => square bracket
 * [^0-9] => reverse match
 * ? * + => repeat count
 * . => any character
 * (abcd) => group $0 - group $9
 *
 */

#include "regex.h"

//#include <stdlib.h> will need for malloc

/* pass regex pattern to constructor */
Regex::Regex(wchar_t* pat, bool global, bool greedy){
	this->pattern=pat;
	this->global=global;
	this->greedy=greedy;
	this->compile();
}

Regex::~Regex(){
	//free(this->buffer);
}

void Regex::compile(){
	/* if pattern is nothing, stop here 
	 * !!! need to revise this !!! */
	if(pattern==NULL) return;
	
	/* just the length of pattern to use later */
	int patlen=(int)wcslen(pattern);
	
	/* if blank regex pattern OR 
	 * pattern length is too long, stop here */
	if(patlen<1) return;
	
	/* indexes */
	int chIdx=0;int rgIdx=0;int grIdx=0;
	
	/* declare flags for [ .. ] and ( .. ) */
	bool sq=false;bool cr=false;bool eaten=true;
	
	/* initialize regex default data */
	error=true;
	r.length =0;
	r.mustEnd=false;
	r.mustStart=false;
	
	/* ^ */
	if(pattern[0]=='^') r.mustStart=true;
	
	/* $ */
	if(pattern[patlen-1]=='$') r.mustEnd=true;
	
	/* loop each character in regex pattern */
	for(int i=0;i<patlen;i++){
	
		int pat=pattern[i]; /* cache it for easiness */
		int chIdx2=chIdx-1; /* index of previous char, i am not sure this is really needed, just for temporary solution */

		/* initialize as exact match */
		r.ch[chIdx].repeatMin=1;
		r.ch[chIdx].repeatMax=1;
		
		/* initialize the invert flag to false */
		if(!sq)
			r.ch[chIdx].invert=false; 
		
		switch (pat){
			case '^' :
				/* if ^ char found in side square bracket, 
				 * it will set to true */
				r.ch[chIdx].invert=sq;
				continue;
			case '$' :
				continue;
			case '[':
				sq=true;
				rgIdx=0;
				continue;
			case ']':
				sq=false;
				r.ch[chIdx].length=rgIdx; 
				chIdx++; 
				continue;
			case '(':
				if(pattern[i+1]=='?'){
					eaten=false;
				}else{
					cr=true;
					grIdx++;
				}
				continue;
			case ')':
				cr=false;
				eaten=true;
				continue;
			case '?':
				r.ch[chIdx2].repeatMin=0;
				r.ch[chIdx2].repeatMax=1;
				continue;
			case '*':
				r.ch[chIdx2].repeatMin=0;
				r.ch[chIdx2].repeatMax=MAXREPEAT;/* kind of infinity */
				continue;
			case '+':
				r.ch[chIdx2].repeatMin=1;
				r.ch[chIdx2].repeatMax=MAXREPEAT;/* kind of infinity */
				continue;
		}
		
		/* within ( ..  ) brackets
		 * if inside cr assign group number, if not make it 0 */
		r.ch[chIdx].group=(cr && eaten)?grIdx:0;
		
		/* capture or not 
		 * if skip, dont capture it */
		r.ch[chIdx].eaten=eaten?true:false; 
		
		/* within [ .. ] brackets */
		if(sq){
			if(pattern[i+1]=='-'){
				r.ch[chIdx].range[rgIdx].start=pat;
				r.ch[chIdx].range[rgIdx].end=pattern[i+2];
				rgIdx++;i=i+2;
			}else{
				r.ch[chIdx].range[rgIdx].start=r.ch[chIdx].range[rgIdx].end=pat;
				rgIdx++;
			}
			continue;
		}
		
		/* if "." char, match any */
		if(pat=='.'){
			r.ch[chIdx].range[0].start=32;
			r.ch[chIdx].range[0].end=MAXCHARCODE;
		}else{
			/* exact match not in square brackets [..]  
			* but can be in ( .. ) :) */
			r.ch[chIdx].range[0].start=r.ch[chIdx].range[0].end=pat;
		}
		r.ch[chIdx].length=1;
		chIdx++;

	}/* main loop end */
	
	/* put char count in r.length */
	r.length=chIdx;

	/* if everything is fine, pass the regex compile :D */
	this->error=false;
}

/* pattern matching for one regex char */
int Regex::check(int chIdx, wchar_t* inputStr, int strIdx){
	int matchCount=0;
	int repeatCount=0;
	
	while(repeatCount<r.ch[chIdx].repeatMax){
		
		bool match=false;
		
		/* if pattern match, it will set match to true */
		for(int j2=0;j2<r.ch[chIdx].length;j2++){
			if(inputStr[strIdx]>=r.ch[chIdx].range[j2].start && inputStr[strIdx]<=r.ch[chIdx].range[j2].end) {
				match=true;
				break;
			}
		}
		
		/* for  "*"  char */
		bool nonGreedyMatch=false;
		if(match && !this->greedy){
			if(r.ch[chIdx].range[0].end== MAXCHARCODE && (r.ch[chIdx].repeatMax-r.ch[chIdx].repeatMin)>0) {
				if(chIdx+1<r.length){
					//search in next regex char
					for(int j2=0;j2<r.ch[chIdx+1].length;j2++){
						if(inputStr[strIdx]>=r.ch[chIdx+1].range[j2].start && inputStr[strIdx]<=r.ch[chIdx+1].range[j2].end) {
							nonGreedyMatch=true;
							break;
						}
					}		
				}
			}
		}
		
		if(nonGreedyMatch)
			break;
		
		/* if normal match and no invert flag  => [abcd] OR
		 * if not match and with invert flag => [^abcd] */
		if(	(match && !r.ch[chIdx].invert) || 
			(!match && r.ch[chIdx].invert && r.ch[chIdx].repeatMin>0)){
			strIdx++;
			matchCount++;
			repeatCount++;
			
		/* if match with invert flag and with negligible char "?"
		 * eg., [^z]? => my regex engine refuse to match it, 
		 * though other engine return true :P */
		}else if(match && r.ch[chIdx].invert && r.ch[chIdx].repeatMin==0){
			matchCount=-1;
			break;
		
		
		/*}else if(!match && r.ch[chIdx].invert && strIdx==(int)wcslen(inputStr)-1){
			
			match=true;
			matchCount++;
			break;
		*/	
		
		/* if doesnot match above two, stop here
		* but need to revise this routine, coz something strange :P */
		}else{
			break;
		}
		
	}
	
	/* return how many matches, or 0 or -1 */
	return matchCount;
}

/* regex test */
bool Regex::test(wchar_t*str){
	return this->exec(str);
}

bool Regex::exec(wchar_t*str){
	/* if compile doesnt succeeded, stop here */
	if(this->error) 
		return false;
	
	/* just the length of input string to use later */
	int strlen=(int)wcslen(str);
	
	/* if regex is nothing or source string is nothing or 
	 * regex length is too long , stop here */
	if(r.length<1 || strlen<1 || strlen>MAXSOURCESTRLEN)
		return false;
	
	/* if pattern have ^ and not match first character, 
	 * no need to continue anymore. */
	if(r.mustStart && !this->check(0,str, 0)) 
		return false;

	/* if pattern have $ and not match with last character, 
	 * no need to continue anymore. */
	if(r.mustEnd && !this->check(r.length-1,str, strlen-1)) 
		return false;
	
	/* set pointer to NULL 
	 * !!! I am not sure, I really need this, need to revise !!! */
	gr.pointer=NULL;
	
	bool match;
	int strIdx=0;
	int resIdx=0;

	do{
	
		/* The following initialize each matching */
		match=true;
		int regIdx=0;int grIdx=0;int matchLen=0;
		int skipLen=0;
		
		/* initialize group parameters */
		gr.res[resIdx].range[0].start=-1;	
		gr.res[resIdx].length=0;
				
		while(strIdx<strlen && regIdx<r.length){
				
			matchLen=this->check(regIdx,	str, strIdx);
			
			//special case, no regex engine support this, but important :P
			if(matchLen==0 && r.ch[regIdx].repeatMin==0){
				if(this->check(regIdx+1,str,strIdx)>0){
					regIdx++;
					continue;
				}
			}
			
			/* normal match */
			if(matchLen>0 ){ //|| (r.ch[regIdx].repeatMin==0 && !r.ch[regIdx].invert)
				
				/* group 0 start*/
				if(gr.res[resIdx].range[0].start==-1){
					gr.res[resIdx].range[0].start=strIdx;
				}

				 /* sub groups $1 $2 $3 start to catch here */
				if(grIdx<r.ch[regIdx].group){
					grIdx++;
					gr.res[resIdx].range[grIdx].end=gr.res[resIdx].range[grIdx].start=strIdx;
				}
				
				/* skip match string length */		
				strIdx+=matchLen;
				
				/* group 1-9 end catching here */
				if(grIdx==r.ch[regIdx].group && grIdx>0) {
					gr.res[resIdx].range[0].end=gr.res[resIdx].range[grIdx].end=strIdx-1;
				}
				
				/* next regex char */
				regIdx++;
			}
			
			/* if doesnot match, reset the regex ptr and 
			* start matching again from next char */
			if(matchLen==0){
				regIdx=0;
				grIdx=0;
				strIdx++;
			}
			
			/* for special case */
			if(matchLen==-1){
				regIdx--;
				grIdx=0;
				strIdx++;
			}
			
		}
		
		/* dummy_test(L"ab",L"a"); */
		if(regIdx!=r.length)
			match=false;
		
		/* dummy_test(L"^[a-z]$",L"zzabcdef"); */
		if(r.mustStart && r.mustEnd && strIdx!=strlen ){
			match=false;break;
		}
		
		if(match) {
			/* group 0 end now */	
			gr.res[resIdx].range[0].end=strIdx-1;
			
			/* last group end now, if not yet ended */
			if(r.ch[regIdx-1].group == grIdx) {
				gr.res[resIdx].range[regIdx].end=strIdx-1;
			}
			
			/* set sub groups counts here */
			gr.res[resIdx].length=grIdx;
				
			/* increment the matching groups counter */
			resIdx++;

		}
	
	/* if global flag is true, 
	 * loop until length of string */
	}while(strIdx<strlen && this->global);
	
	/* how many match */
	gr.length=resIdx;
	
	/* store input string address */
	gr.pointer=str;
	
	return resIdx>0;
}

/* regex substitution */
void Regex::sub(wchar_t*srcStr, wchar_t* replStr, wchar_t* destStr){

	/* check string is not compile yet, 
	* try to compile here */
	if(gr.pointer!=srcStr){
		if(!this->exec(srcStr)){
			/* if compile fail, no replacing will occur */
			destStr=srcStr;
			return;
		}
	}
	
	/* once .test with regex no more needed */
	int resIdx=0;
	
	wchar_t*ds=this->buffer;
	
	int srcIdx=0;
	
	do{
		/* if within range of "replace" source string */
		if(srcIdx>=gr.res[resIdx].range[0].start && srcIdx<=gr.res[resIdx].range[0].end ){
		
			wchar_t*rs=replStr;
		
			/* loop for replStr */
			while(*rs){
				
				/* if $1-$9 */
				if(*rs<10){
					
					/* error checking for group number */
					if(*rs<=gr.res[resIdx].length){
					
						/* append the source string in group to buffer */
						for(int i=gr.res[resIdx].range[(int)*rs].start;i<=gr.res[resIdx].range[(int)*rs].end;i++){
							*ds++=srcStr[i];
						}
						
					}else{
						/* !!! need to return syntax error !!! */
					}
					
				/* append a char in replStr to Buffer*/
				}else{
					*ds++=*rs;
				}
				
				/* next char in replStr */
				rs++;
				
			}; /* while loop */
			
			/* skip replace characters in source string */
			srcIdx+=gr.res[resIdx].range[0].end-gr.res[resIdx].range[0].start+1;
			
			resIdx++;
				
		/* if not in group range, just copy it */
		}else{
		
			*ds++=srcStr[srcIdx];
			srcIdx++;
		
		}
		
	}while(srcStr[srcIdx]);
	
	*ds=NULL;/* very important :P */
	
	wcscpy(destStr,this->buffer);
}

/* * * * * * * * * * * * * * * * * * * * * * *
 * The following functions are for testing regex.
 * * * * * * * * * * * * * * * * * * * * * * */
 #ifdef _DEBUG

/* inline test */
bool dummy_test(wchar_t* pat, wchar_t*str){
	Regex re(pat);
	return re.test(str);
}

/* inline replace */
wchar_t* dummy_replace(wchar_t* pat, wchar_t*str, wchar_t* replStr, bool global=false){
	wchar_t ret[100];
	Regex re(pat);
	re.global=global;
	re.test(str);
	re.sub(str,replStr,ret);
	return ret;
}

#include "burmese.h"

void test_block(){
	/* can test with 10k or 100k loop :P */
	for(int i=0; i< 1;i++){
		int *****p;
		//wchar_t ttt[]=L"z123zgw";
		//Regex re(L"z([1])(23)z");
		//re.test(ttt);
		//re.sub(ttt,L"x\1y\2z",ttt); /* test for self replace, debugger will claim u, data is corrupted :P */

		/* !!! need (?!abcd) and (?:abcd) !!! */
		wchar_t dest[10];
		Burmese bur;
		bur.convertDigit(dest, L"123");
		bur.convertDigit(dest, L"a123a");
		
		/* isNumber */
		dummy_test(L"[^0-9]",L"abcd");
		dummy_test(L"[^0-9]",L"s"); 
		dummy_test(L"[^0-9]",L"123"); //unmatch
		dummy_test(L"[^0-9]",L"123a");
		dummy_test(L"[^0-9]",L"a123");
		dummy_test(L"[^0-9]",L"ab123asdf");
		
		dummy_replace(L"[^Z]?([a-z])",L"a",L"Z\1", true);
		
		dummy_replace(L"[^Z\u200B\u1039]?([\u1000-\u102A\u104C-\u104F])",L"\u1000\u1001\u1002\u1003",L"Z\1",true);
		
		dummy_replace(L"[^Z\u200B\u1039]?([\u1000-\u102A\u104C-\u104F])(?[^\u103A])",L"\u1000\u1001\u1002\u1003",L"Z\u200B\1",true);
		dummy_replace(L"[^Z\u200B\u1039]?([\u1000-\u102A\u104C-\u104F])",L"\u1019\u103E\u1010\u103A\u200B\u1015\u102F\u1036\u200B\u1010\u1004\u103A\u200B\u1016\u103C\u1004\u103A\u1037\u200B\u101D\u1004\u103A\u200B\u1015\u102B\u200B\20/\20\u1019\u103E\u1010\u103A\u200B\u1015\u102F\u1036\u200B\u1010\u1004\u103A\u200B\u1015\u103C\u102F\u200B\u101C\u102F\u1015\u103A\u200B\u1015\u102B\u200B",L"Z\u200B\1",true);
		
		dummy_test(L"a(.*)f",L"");
		dummy_test(L"",L"aaaa");
		
		/* replace all test */
		dummy_replace(L"[^z]?(ab)",L"XabXabXabXabX",L"z\1",true);
		dummy_replace(L"[^z]?(ab)",L"abababab",L"z\1", true);
		dummy_replace(L"[^z]?([a-z])",L"abababab",L"z\1", true);
		
		dummy_test(L"a(.*)f",L"abcdefabcdef");
		
		dummy_test(L"([\u100d\u100b\u1005\u103a\u100a\u103b\u107e])(.*)[\u102f]",L"\u103B\u1019\u102D\u102F");
		dummy_test(L"([\u100d\u100b\u1005\u103a\u100a\u103b\u107e])(.*)[\u102f]",L"\u103B\u1019\u102D\u102F\u1038");
		
		dummy_replace(L"([\u100d\u100b\u1005\u103a\u100a\u103b\u107e])(.*)[\u102f]",L"\u103B\u1019\u102D\u102F\u1038",L"\1\2\u1033");
		
		/* non greedy matching test */
		dummy_test(L"a(.*)f",L"abcdef");
		
		dummy_test(L"(.*)",L"abcdef");
		dummy_test(L"([a-z]*)",L"abcde1");
		dummy_test(L"([a-z]*)",L"1bcdef");
		
		dummy_test(L"([a-z])",L"abcdef");
		dummy_replace(L"([a-z])",L"abcdef",L"z\1");
		
		/* step by step replace */
		dummy_replace(L"[^z]?([abcd])",L"abcd",L"z\1");
		dummy_replace(L"[^z]?([abcd])",L"abcd",L"z\1",true);
		dummy_replace(L"[^z]?([abcd])",L"zabcd",L"z\1");
		dummy_replace(L"[^z]?([abcd])",L"zazbcd",L"z\1");
		dummy_replace(L"[^z]?([abcd])",L"zazbzcd",L"z\1");
		
		/* ? char test */
		dummy_test(L"ab?c",L"abc");
		dummy_test(L"ab?c",L"ac");
		dummy_test(L"ab?c",L"adc");//unmatch
		
		/* ^ char test */
		dummy_test(L"([abcd])([lmno])[efgh]",L"ame");
		dummy_test(L"([abcd])([^lmno])[efgh]",L"ame");//unmatch
		dummy_test(L"([abcd])([^lmno])[efgh]",L"aze");
		dummy_test(L"([^lmno])[efgh]al",L"zeal");
		dummy_test(L"([^lmno])[efgh]al",L"meal");//unmatch
		dummy_test(L"([^lmno])?[efgh]al",L"meal");//unmatch
		dummy_test(L"([^lmno])[efgh]al",L"meal");//unmatch
		dummy_test(L"([^lmno])?[efgh]al",L"zeal");
		
		/* the following is special case, need special routine, 
		 * coz its break some regex parsing rules, no other regex engine will provide this :P*/
		dummy_test(L"([^lmno])?[efgh]al",L"eal"); 
		
		dummy_test(L"([lmno])?[efgh]al",L"zeal");//logically unmatch, but all regex engine return match :P
		dummy_test(L"([lmno])?[efgh]al",L"meal");
		dummy_test(L"([lmno])?[efgh]al",L"eal");
		
		//unmatches
		dummy_test(L"([\u1001\u1002\u1004\u1012\u1015\u101D])\u102C",L"\u1015");
		dummy_test(L"([\u1001\u1002\u1004\u1012\u1015\u101D])\u102C",L"\u101D\u103D");
		dummy_test(L"([\u1001\u1002\u1004\u1012\u1015\u101D])\u102C",L"\u101D\u103D\u102C");
		dummy_test(L"([\u1001\u1002\u1004\u1012\u1015\u101D])\u102C\u1039",L"\u101D\u103D");
		dummy_test(L"(^[\u103B])([\u1001\u1002\u1004\u1012\u1015\u101D])\u102C",L"\u1001\u102C");
		
		//match
		dummy_test(L"(^[\u103B])([\u1001\u1002\u1004\u1012\u1015\u101D])\u102C",L"\u103B\u1001\u102C");
		dummy_test(L"(^[\u103B])?([\u1001\u1002\u1004\u1012\u1015\u101D])\u102C",L"\u1001\u102C");
		dummy_test(L"(^[\u103B])?([\u1001\u1002\u1004\u1012\u1015\u101D])\u102C",L"\u103B\u1001\u102C");
		
		dummy_test(L"(^[\u103B])?([\u1001\u1002\u1004\u1012\u1015\u101D])\u102C",L"\u1000\u1001\u102C");//unmatch
		
		//replace success
		dummy_replace(L"([ab])([cd])",L"ac",L"\2\1");
		dummy_replace(L"z([1])(23)z",L"z123zgw",L"x\1y\2z");
		dummy_replace(L"z([1])z(23)z(45)z",L"z1z23z45zgw",L"x\1y\2z");
		dummy_replace(L"z([1])(23)z",L"wgz123zgw",L"x\1y\2z");
		dummy_replace(L"z([1])(23)z",L"wgz123z",L"xy");
		dummy_replace(L"z([1])(23)z",L"wgz123z",L"xyxyxyxy");
		
		//re ordering test success
		dummy_replace(L"z([1])y(23)x(456)w(7890)v",L"wgz1y23x456w7890vgw",L"A\4B\3C\2D\1E");
		//result -> wgA7890B456C23D1Egw
		
		//success also
		dummy_replace(L"(\u1015\u102C)",L"\u1015\u102C",L"\u1015\u102B");
		
		//success as well
		dummy_replace(L"([\u1001\u1002\u1004\u1012\u1015\u101D])(\u102C)",L"\u1015\u102C",L"\1\u102B");
		dummy_replace(L"([\u1001\u1002\u1004\u1012\u1015\u101D])\u102C",L"\u1015\u102C",L"\1\u102B");
		
		//success too
		dummy_replace(L"([\u1001\u1002\u1004\u1012\u1015\u101D])\u102C",L"\u1000\u1000\u1015\u102C",L"\1\u102B");
		dummy_replace(L"([\u1001\u1002\u1004\u1012\u1015\u101D])\u102C",L"\u101D\u103D\u102C",L"\1\u102B");
		
		//of coz success :P
		dummy_replace(L"([abcd])hk",L"bhk",L"\1mz");
		dummy_replace(L"([\u1001\u1002\u1004\u1012\u1015\u101D])\u102B\u1039",L"\u1031\u1001\u102B\u1039",L"\1\u105A");
		dummy_replace(L"([\u1001\u1002\u1004\u1012\u1015\u101D])\u102C",L"\u1031\u1000\u103A\u102C",L"\1\u102B");
		
		//unmatched so return same thing
		dummy_replace(L"([\u1001\u1002\u1004\u1012\u1015\u101D])(\u102C)",L"\u1000\u102C",L"\1\u102B");
		
		//step by step test
		dummy_test(L"a",L"a");
		dummy_test(L"b",L"a");//false
		dummy_test(L"ab",L"abc");
		dummy_test(L"ab",L"a");//false
		dummy_test(L"[a-z]*",L"a");
		dummy_test(L"^([a-z])*1$",L"zzda1");
		dummy_test(L"[1-23](45)6(78)",L"145678");
		dummy_test(L"[1-23](45)",L"145");
		dummy_test(L"[1-23](45)",L"z145z");
		dummy_test(L"^[1-23](45)",L"z145z");//false
		dummy_test(L"[1-23](45)",L"z141z145z");
		
		//matches
		dummy_test(L"^[0-9]$",L"1");
		dummy_test(L"^[0-9]",L"123a0");
		dummy_test(L"[b-e]",L"abcdef");
		dummy_test(L"[a-z]$",L"1zzabcdef");
		dummy_test(L"^[a-z][a-z][a-z][a-z]$",L"zzab");
		dummy_test(L"^[a-z]*$",L"zzaba");
		dummy_test(L"^[a-z]+$",L"zzabasdlkfjslkd");
		dummy_test(L"^[a-z]+[0-9]*$",L"za123");
		
		//unmatches
		dummy_test(L"^[0-9]$",L"a");
		dummy_test(L"^[a-z]",L"123a0");
		dummy_test(L"[0-9]",L"abcdef");
		dummy_test(L"^[a-z]$",L"1zzabcdef");
		dummy_test(L"^[a-z]$",L"zzabcdef");
		dummy_test(L"^zz[a-x]+$",L"zzabasdlkfjslkdzd");
		dummy_test(L"^[a-z]+[0-9]*[a-z]$",L"zzabasdlk123");
	}

	/* u can set breakpoint in next line, to test the performance of my regex, like looping 10k times or 100k times :P */
}
#endif